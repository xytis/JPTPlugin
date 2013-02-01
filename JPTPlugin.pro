#-------------------------------------------------
#
# Project created by QtCreator 2013-01-21T22:22:01
#
#-------------------------------------------------


TARGET      = JPTPlugin
TEMPLATE    = lib
CONFIG      += plugin
CONFIG      += release

DESTDIR = $$[QT_INSTALL_PLUGINS]/imageformats

SOURCES += jptplugin.cpp \
    jpthandler.cpp

HEADERS += jptplugin.h \
    jpthandler.h



#unix:!symbian {
#    maemo5 {
#        target.path = /opt/usr/lib
#    } else {
#        target.path = /usr/lib
#    }
#    INSTALLS += target
#}
