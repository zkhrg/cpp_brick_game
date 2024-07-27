QT       += core gui openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
PREFIX=../../

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $${PREFIX}controller/controller.cc \
    $${PREFIX}gui/desktop/views/main_view.cc \
    $${PREFIX}brick_game/snake/backend_snake.cc \
    $${PREFIX}gui/desktop/main.cc \
    $${PREFIX}gui/desktop/font_manager/font_manager.cc \
    $${PREFIX}gui/desktop/texture_manager/texture_manager.cc \
    $${PREFIX}brick_game/tetris/backend_tetris.cc

HEADERS += \
    $${PREFIX}brick_game/common/arcade_game_base.h \
    $${PREFIX}brick_game/snake/backend_snake.h \
    $${PREFIX}brick_game/common/common_types.h \
    $${PREFIX}controller/controller.h \
    $${PREFIX}gui/desktop/views/main_view.h \
    $${PREFIX}gui/desktop/font_manager/font_manager.h \
    $${PREFIX}gui/desktop/texture_manager/texture_manager.h \
    $${PREFIX}brick_game/tetris/backend_tetris.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    $${PREFIX}gui/desktop/resources.qrc
