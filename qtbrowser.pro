TEMPLATE    = app
TARGET      = qtbrowser

HEADERS     += \
               sslhandler.h \
               webpage.h \
               graphicswebview.h \
               graphicsview.h \
               webview.h \
               pluginfactory.h \
               glplugin.h \
               nrdplugin.h

SOURCES     += \
               sslhandler.cpp \
               webpage.cpp \
               graphicswebview.cpp \
               graphicsview.cpp \
               webview.cpp \
               pluginfactory.cpp \
               glplugin.cpp \
               nrdplugin.cpp \
               qtbrowser.cpp

contains(QT_CONFIG, opengl) {
   DEFINES += QT_BUILD_WITH_OPENGL
   message("Building with OpenGL support.")
} else {
   warning("OpenGL support is not available, qtbrowser might work but supported features and performance are limited.")
}

#contains(QT_CONFIG, opengles1) {}
#contains(QT_CONFIG, opengles2) {}
#contains(QT_CONFIG, egl) {}

LIBS += -lnetflix
LIBS += -lnrdtee
LIBS += -lmng
LIBS += -lwebp
LIBS += -lpng
LIBS += -ljpeg
LIBS += -lJavaScriptCore
LIBS += -lWTF
LIBS += -lfreetype
LIBS += -lGLESv2
LIBS += -lEGL
LIBS += -lnrdapp
LIBS += -lnrddpi
LIBS += -lpthread
LIBS += -ldl
LIBS += -lnrdapp
LIBS += -lmdxlib
LIBS += -lmdxlib_upnp
LIBS += -lmdxlib_ixml
LIBS += -lmdxlib_threadutil
LIBS += -lnrd
LIBS += -lnrddemux
LIBS += -lnrdase
LIBS += -lnrdnet_http
LIBS += -lnrdnet
LIBS += -lcares
LIBS += -lnrdase_heuristics
LIBS += -lcurl
LIBS += -lnrdbase
LIBS += -lssl
LIBS += -lcrypto
LIBS += -lexpat
LIBS += -lz

INCLUDEPATH += $(STAGING_DIR)/usr/include/interface/vcos/pthreads
INCLUDEPATH += $(STAGING_DIR)/usr/include/gibbon/nrdapp
INCLUDEPATH += $(STAGING_DIR)/usr/include/gibbon

contains(QT_CONFIG, openssl)|contains(QT_CONFIG, openssl-linked) {
   message("Building with OpenSSL support.")
} else {
   error("OpenSSL support is not available, qtbrowser requires OpenSSL.")
}

contains(QT_MAJOR_VERSION, 4) {

   contains(QT_CONFIG, webkit) {
      message("Module webkit found.")
   } else {
      error("No module webkit, qtbrowser requires module webkit.")
   }

   QT += \
         webkit

   contains(QT_CONFIG, opengl){
      QT += \ 
            opengl
   }

   message("Building for Qt4.")
}

contains(QT_MAJOR_VERSION, 5) {

   qtHaveModule(widgets)|qtHaveModule(webkitwidgets) {
      message("Module webkit/webkitwidgets found.")
   } else {
      error("No module webkit/webkitwidgets found, qtbrowser requires module webkitwidgets.")
   }

   QT += \
         webkitwidgets

   contains(QT_CONFIG, opengl){
      QT += \ 
            opengl
   }

   qtHaveModule(quick):qtHaveModule(qml) {
      message("Modules qml and quick found.")
 
     QT += \
         qml quick

     DEFINES += QT_BUILD_WITH_QML_API
   } else {
      warning("QtQuick/QML API might not be supported.")
   }

   message("Building for Qt5.")
}
