QT += core gui widgets

TARGET = huffman_compression
TEMPLATE = app

CONFIG += c++11

# Source files
SOURCES += \
    src/main.cpp \
    src/gui/mainwindow.cpp \
    src/core/huffman/huffman.c \
    src/core/huffman/huffman_wrapper.cpp

# Header files
HEADERS += \
    src/gui/mainwindow.h \
    src/core/huffman/huffman.h \
    src/core/huffman/huffman_wrapper.h

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
