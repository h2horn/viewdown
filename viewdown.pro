
QT += webkitwidgets widgets printsupport
TARGET = viewdown
LIBS += -lhoedown
HEADERS += src/mainwindow.h \

SOURCES += src/main.cpp \
           src/mainwindow.cpp \

RESOURCES = viewdown.qrc
RC_ICONS = viewdown.ico

