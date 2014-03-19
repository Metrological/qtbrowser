#include <QtGui>

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QApplication>
#else
#include <QtWidgets>
#endif

#include <QtOpenGL/QGLWidget>
#include <QGraphicsWebView>

#include <QWebSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWebSettings::globalSettings()->setAttribute(QWebSettings::AcceleratedCompositingEnabled, true);

    QGraphicsView g;
    g.setScene(new QGraphicsScene(&g));
    g.scene()->setItemIndexMethod(QGraphicsScene::NoIndex);

    g.setAttribute(Qt::WA_DeleteOnClose);
    g.setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);
    g.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    g.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    g.setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    g.setFrameStyle(QFrame::NoFrame);
    g.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    QGLWidget* glwidget = new QGLWidget(QGLFormat(QGL::NoSampleBuffers));

    //glwidget->setFocusPolicy(Qt::StrongFocus);
    //glwidget->setAttribute(Qt::WA_PaintOnScreen);
    //glwidget->setAttribute(Qt::WA_NoSystemBackground);
    //glwidget->setAutoBufferSwap(false);
    //glwidget->resize(1280, 720);

    g.setViewport(glwidget);

    g.showFullScreen();

    QGraphicsWebView view;

    view.load(QUrl((argc==1?"http://www.google.com" : argv[1])));

    view.setGeometry(QRect(0, 0, 1280, 720));

    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
    QWebSettings::globalSettings()->enablePersistentStorage("/tmp/qtbrowser");
    QWebSettings::globalSettings()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
    QWebSettings::globalSettings()->setOfflineWebApplicationCachePath("/tmp/qtbrowser");
    QWebSettings::globalSettings()->setOfflineWebApplicationCacheQuota(1024*1024*5);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);

    view.show();

    g.scene()->addItem(&view);

    a.exec();
}
