TEMPLATE    = app
TARGET      = qtbrowser

SOURCES    += qtbrowser.cpp

contains(QT_MAJOR_VERSION, 4) {
    QT += \
	webkit \
	opengl
}

contains(QT_MAJOR_VERSION, 5) {
    QT += webkitwidgets
}
