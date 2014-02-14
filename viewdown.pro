
QT += webkitwidgets widgets
TARGET = viewdown
HEADERS += src/mainwindow.h \
           src/hoedown/src/document.h \
           src/hoedown/src/html.h \
           src/hoedown/src/buffer.h \
           src/hoedown/src/autolink.h \
           src/hoedown/src/escape.h \
           src/hoedown/src/stack.h
SOURCES += src/main.cpp \
           src/mainwindow.cpp \
           src/hoedown/src/document.c \
           src/hoedown/src/html.c \
           src/hoedown/src/buffer.c \
           src/hoedown/src/autolink.c \
           src/hoedown/src/escape.c \
           src/hoedown/src/html_blocks.c \
           src/hoedown/src/stack.c

