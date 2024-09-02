QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    imagewidget.cpp \
    main.cpp \
    mainwindow.cpp \
    selectiondialog.cpp \
    templatematcher.cpp

HEADERS += \
    imagewidget.h \
    mainwindow.h \
    selectiondialog.h \
    templatematcher.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# OpenCV 源目录
OPENCV_DIR = C:/Qt/opencv/build

INCLUDEPATH += $$OPENCV_DIR/include
LIBS += -L$$OPENCV_DIR/x64/vc16/lib -lopencv_world4100
# LIBS += -L$$OPENCV_DIR/x64/vc16/lib -lopencv_world4100d

# 我不知道为什么不能这样，必须要单独写
# debug {
#     LIBS += -L$$OPENCV_DIR/x64/vc16/lib -lopencv_world4100d
# } else {
#     LIBS += -L$$OPENCV_DIR/x64/vc16/lib -lopencv_world4100
# }
