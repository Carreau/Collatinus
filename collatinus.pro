######################################################################
# Automatically generated by qmake (2.01a) mer. nov. 1 14:52:27 2006
# And modify manually
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS     += src/*.h 
FORMS       += src/*.ui
SOURCES     += src/*.cpp src/*.h src/*.cc
RESOURCES   += collatinus.qrc

CONFIG += release_binary

macx{
    #error("resources files won't be copied in application bundle");
    #CONFIG += x86 ppc
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4
    #QMAKE_MAC_SDK = /Developer/SDKs/MacOSX10.4u.sdk

    #QMAKE_POST_LINK=strip collatinus.app/Contents/MacOS/collatinus

    # install into app bundle
    data.path = build/Debug/collatinus.app/Contents/MacOS
    data.files =  lemmata.* expressions.fr config lucretia.txt INSTALL COPYING     INSTALLS += data
    INSTALLS += data
    ICON = collatinus.icns
    #misc.path = myapp.app/Contents
    #misc.files = mac/Info.plist mac/PkgInfo
    #INSTALLS += misc

    #doc.path = myapp.app/Contents/Resources/en.lproj
    #doc.files = docs/book/*.html
    #INSTALLS += doc
 
}
install.files = collatinus lemmata.* expressions.fr config lucretia.txt INSTALL COPYING 
install.path = /usr/share/collatinus 
documentation.path = /usr/share/collatinus/doc
documentation.files = doc/*
install.commands = $(SYMLINK) /usr/share/collatinus/collatinus /usr/bin/collatinus 
INSTALLS += install
INSTALLS += documentation
