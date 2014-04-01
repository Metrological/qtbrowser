TEMPLATE    = app
TARGET      = qtbrowser

SOURCES    += qtbrowser.cpp

!contains(DEFINES, QT_OPENGL_LIB) {
warning("QT_OPENGL_LIB is not defined, qtbrowser might work but supported features and performance are limited")
}

contains(QT_MAJOR_VERSION, 4) {

contains(DEFINES, QT_NO_OPENSSL):!contains(DEFINES, QT_OPENSSL) {
error("QT_NO_OPENSSL, qtbrowser requires openssl support")
}

    QT += \
	webkit

contains(DEFINES, QT_OPENGL_LIB) {
    QT += \ 
	opengl
}

}

contains(QT_MAJOR_VERSION, 5) {

contains(DEFINES, QT_NO_SSL):!contains(DEFINES, QT_SSL) {
error("QT_NO_OPENSSL, qtbrowser requires openssl support")
}

    QT += webkitwidgets
}
