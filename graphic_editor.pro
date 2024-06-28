QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arc.cpp \
    circle.cpp \
    dialog_brush.cpp \
    dialog_pen.cpp \
    dialog_pixel.cpp \
    dialog_points.cpp \
    ellipse.cpp \
    line.cpp \
    main.cpp \
    mainwindow.cpp \
    my_brush.cpp \
    my_color.cpp \
    my_pen.cpp \
    my_widget.cpp \
    pixel.cpp \
    polygon.cpp \
    polyline.cpp \
    rectangle.cpp \
    tochka.cpp

HEADERS += \
    arc.h \
    circle.h \
    dialog_brush.h \
    dialog_pen.h \
    dialog_pixel.h \
    dialog_points.h \
    ellipse.h \
    line.h \
    mainwindow.h \
    my_brush.h \
    my_color.h \
    my_pen.h \
    my_widget.h \
    pixel.h \
    polygon.h \
    polyline.h \
    rectangle.h \
    tochka.h

FORMS += \
    dialog_brush.ui \
    dialog_pen.ui \
    dialog_pixel.ui \
    dialog_points.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
