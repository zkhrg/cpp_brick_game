QT       += core gui openglwidgets
SRC_FOLDER=src/

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $${SRC_FOLDER}controller.cpp \
    $${SRC_FOLDER}glview.cpp \
    $${SRC_FOLDER}backend_snake.cc \
    $${SRC_FOLDER}main.cpp \
    $${SRC_FOLDER}font_manager.cc \
    $${SRC_FOLDER}texture_manager.cc

HEADERS += \
    $${SRC_FOLDER}arcade_game_base.h \
    $${SRC_FOLDER}backend_snake.h \
    $${SRC_FOLDER}common_types.h \
    $${SRC_FOLDER}controller.h \
    $${SRC_FOLDER}glview.hpp \
    $${SRC_FOLDER}font_manager.h \
    $${SRC_FOLDER}texture_manager.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
