######################################################################
# Automatically generated by qmake (2.01a) mer. nov. 1 14:52:27 2006
# And modify manually
######################################################################

TEMPLATE = app
TARGET = collatinus
VERSION = "IX.4"
NVERSION = "9.4"
DEFINES += VERSION=\\\"$$VERSION\\\"
DEPENDPATH += .
INCLUDEPATH += .
TRANSLATIONS = collatinus_en.ts collatinus_la.ts collatinus_fr.ts

OBJECTS_DIR= obj/
MOC_DIR = moc/

# Input
HEADERS     += src/*.h
HEADERS     += src/external_code/*.hh
FORMS       += src/*.ui
SOURCES     += src/*.cpp src/*.cc src/external_code/*.cc
RESOURCES   += collatinus.qrc
QT          += svg
CONFIG += release_binary

macx{
    TARGET = Collatinus
    HEADERS     += MacOS/src/*.h
    SOURCES     += MacOS/src/*.cpp
    OBJECTIVE_SOURCES += MacOS/src/*.mm
	QMAKE_INFO_PLIST = MacOS/Info.plist

    #note mac os x, fair un $ qmake -spec macx-g++
    #CONFIG += x86 ppc
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.6
    ICON = MacOS/collatinus.icns
    #QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.4u.sdk

    #QMAKE_POST_LINK=strip Collatinus.app/Contents/MacOS/collatinus

    # install into app bundle
    # à changer en ressources
    ressources.path = Collatinus.app/Contents/Resources
    ressources.files =  MacOS/dsa_pub_collatinus.pem
    data.path = Collatinus.app/Contents/MacOS
    data.files =  ressources/*
    deploy.depends = install_documentation
    deploy.depends += install
	deploy.depends += plist
	plist.commands = @sed -e "s,@VERSION@,$$NVERSION,g" -i "''" Collatinus.app/Contents/Info.plist

    documentation.path = Collatinus.app/Contents/MacOS/doc/
    documentation.files = doc/*.html
    # ajouter un cible qui fait macdeploy Collatinus.app
    deploy.commands = macdeployqt Collatinus.app
    dmg.depends = deploy
	dmg.commands = ./MacOS/Collatinus.sh
    INSTALLS += documentation
    INSTALLS += data
    INSTALLS += ressources
    QMAKE_EXTRA_TARGETS += deploy
    QMAKE_EXTRA_TARGETS += plist
    QMAKE_EXTRA_TARGETS += dmg

    #Lets do some specific things for mac, especially for the update
    #Link cocoa framwork (gui framwork)
    #Sparkle (self update)
    LIBS += -framework Sparkle -framework AppKit
    QMAKE_POST_LINK = cp -fR /Library/Frameworks/Sparkle.framework Collatinus.app/Contents/Frameworks
}
unix:!macx{
    target.path = /usr/bin
    target.target = collatinus
    install.files = ressources/lemmata.* ressources/lucretia.txt ressources/expressions.fr collatinus
    install.path = /usr/share/collatinus9
    documentation.path = /usr/share/collatinus9/doc
    documentation.files = doc/*.html

    INSTALLS += target
    INSTALLS += install
    INSTALLS += documentation
}
