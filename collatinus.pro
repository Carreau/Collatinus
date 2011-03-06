######################################################################
# Automatically generated by qmake (2.01a) mer. nov. 1 14:52:27 2006
# And modify manually
######################################################################

TEMPLATE = app
TARGET = collatinus
VERSION = "IX.2"
DEFINES += VERSION=\\\"$$VERSION\\\"
DEPENDPATH += .
INCLUDEPATH += .

OBJECTS_DIR= obj/
MOC_DIR = moc/

# Input
HEADERS     += src/*.h 
FORMS       += src/*.ui
SOURCES     += src/*.cpp src/*.cc
RESOURCES   += collatinus.qrc
QT          += svg
CONFIG += release_binary

macx{
    #note mac os x, fair un $ qmake -spec macx-g++
    #CONFIG += x86 ppc
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.6
    ICON = MacOS/collatinus.icns
    #QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.4u.sdk

    #QMAKE_POST_LINK=strip Collatinus.app/Contents/MacOS/collatinus

    # install into app bundle
    # à changer en ressources
    data.path = collatinus.app/Contents/MacOS
    data.files =  ressources/* 
    deploy.depends = install_documentation
    deploy.depends += install
    documentation.path = collatinus.app/Contents/MacOS/doc/
    documentation.files = doc/*.html
    # ajouter un cible qui fait macdeploy collatinus.app
    deploy.commands = macdeployqt collatinus.app;\
                         mv collatinus.app Collatinus.app;
    INSTALLS += documentation
    INSTALLS += data
    QMAKE_EXTRA_TARGETS += deploy
}
unix:!macx{
    target.path = /usr/bin
    target.target = collatinus
    install.files = ressources/lemmata.* ressources/lucretia.txt ressources/expressions.fr collatinus
    install.path = /usr/share/collatinus 
    documentation.path = /usr/share/collatinus/doc
    documentation.files = doc/*.html
    
    INSTALLS += target
    INSTALLS += install
    INSTALLS += documentation
}
