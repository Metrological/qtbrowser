TEMPLATE    = app
TARGET      = qtbrowser

SOURCES     += qtbrowser.cpp

contains(QT_CONFIG, opengl) {
   DEFINES += QT_BUILD_WITH_OPENGL
   message("Building with OpenGL support.")
} else {
   warning("OpenGL support is not available, qtbrowser might work but supported features and performance are limited.")
}

#contains(QT_CONFIG, opengles1) {}
#contains(QT_CONFIG, opengles2) {}
#contains(QT_CONFIG, egl) {}

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

   message("Building for Qt5.")
}
