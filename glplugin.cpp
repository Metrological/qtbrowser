#include "glplugin.h"
#include <QPainter>
#include <stdio.h>
#include <QGraphicsScene>
#include <QImage>

#define STRINGIFY(...) #__VA_ARGS__

static GLuint loadShader(GLenum type, const GLchar *shaderSrc)
{
    GLuint shader;
    GLint compiled;

    shader = glCreateShader(type);
    if (!shader)
        return 0;

    glShaderSource(shader, 1, &shaderSrc, 0);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

class ShaderProgram {
public:
    static ShaderProgram* instance() {
        if (!m_instance) {
            m_instance = new ShaderProgram();
        }
        return m_instance;
    }

    GLint program() { return m_program; }
    GLuint vertexAttr() { return m_vertexAttr; }

private:
    void initialize() {
        GLchar vShaderStr[] =
            STRINGIFY(
                attribute vec3 vertex;
                void main(void)
                {
                    gl_Position = vec4(vertex, 1.0);
                }
            );

        GLchar fShaderStr[] =
            STRINGIFY(
                void main(void)
                {
                    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
                }
            );

        GLuint vertexShader;
        GLuint fragmentShader;
        GLint linked;

        vertexShader = loadShader(GL_VERTEX_SHADER, vShaderStr);
        fragmentShader = loadShader(GL_FRAGMENT_SHADER, fShaderStr);
        if (!vertexShader || !fragmentShader)
            return;

        m_program = glCreateProgram();
        if (!m_program)
            return;

        glAttachShader(m_program, vertexShader);
        glAttachShader(m_program, fragmentShader);

        glLinkProgram(m_program);
        glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
        if (!linked) {
            glDeleteProgram(m_program);
            m_program = 0;
        }

        m_vertexAttr = glGetAttribLocation(m_program, "vertex");
    }

    ShaderProgram()
        : m_program(0)
        , m_vertexAttr(0)
    {
        initialize();
    }

    static ShaderProgram *m_instance;
    GLint m_program;
    GLuint m_vertexAttr;
};
ShaderProgram* ShaderProgram::m_instance = NULL;

GLPlugin::GLPlugin()
    : QGraphicsWidget(0)
    , m_context(0)
    , m_surface(0)
    , m_frame(0)
{
    // needed in order to get keyboard events
    setFlag(QGraphicsItem::ItemIsFocusable, true);
}

GLPlugin::~GLPlugin()
{
    delete m_context;
    delete m_surface;
}

void GLPlugin::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    // we don't really need our custon gl context, but it's safer
    // to use a new one to avoid messing with the rendering of the page content
    QOpenGLContext *prevContext = QOpenGLContext::currentContext();
    if (!m_context) {
        m_surface = new QOffscreenSurface();
        m_surface->create();
        m_context = new QOpenGLContext();
        m_context->setShareContext(prevContext);
        m_context->create();
    }
    m_context->makeCurrent(m_surface);

    GLuint fbo, tex;
    QSize size (geometry().toAlignedRect().width(), geometry().toAlignedRect().height());

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width(), size.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

    glViewport(0, 0, size.width(), size.height());

    glClearColor(0, 0, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    ShaderProgram *program = ShaderProgram::instance();
    glUseProgram(program->program());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    GLfloat afVertices[] = {
        -1, -1, 0,
        1, -1, 0,
        0,  1, 0
    };
    glVertexAttribPointer(program->vertexAttr(), 3, GL_FLOAT, GL_FALSE, 0, afVertices);
    glEnableVertexAttribArray(program->vertexAttr());
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(program->vertexAttr());

    // FIXME: avoid this CPU->GPU copy
    QImage img(size, QImage::Format_RGBA8888_Premultiplied);
    glReadPixels(0,0,size.width(), size.height(),GL_RGBA, GL_UNSIGNED_BYTE, img.bits());

    prevContext->makeCurrent(prevContext->surface());

    painter->drawImage(QPoint(0,0), img.mirrored());

    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &tex);
}

void GLPlugin::setNumber(int n)
{
    emit numberSet(n);
}

void GLPlugin::keyPressEvent(QKeyEvent* event)
{
    printf("press event\n");
}

void GLPlugin::keyReleaseEvent(QKeyEvent* event)
{
    printf("release event\n");
}

QVariant GLPlugin::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == QGraphicsItem::ItemSceneHasChanged && scene() && !m_frame) {
        // with the current qtwebkit code, when the item is added to the scene the
        // webFrame property has alredy been set
        m_frame = (QWebFrame*)property("webFrame").value<void*>();
        attachObject();
        connect(m_frame, SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(attachObject()));
    }

    // when the widget gets visible and inside a scene it grabs the keyboard to get keyboard events
    // the grab will be released automatically when the widget gets hidden or out of the scene
    if (change == QGraphicsItem::ItemVisibleHasChanged || change == QGraphicsItem::ItemSceneHasChanged) {
        if (isVisible() && scene())
            grabKeyboard();
    }

   return QGraphicsItem::itemChange(change, value);
}

void GLPlugin::attachObject()
{
    m_frame->addToJavaScriptWindowObject(QString("GLPlugin"), this);
}
