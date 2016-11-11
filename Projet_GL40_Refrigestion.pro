#-------------------------------------------------
#
# Project created by QtCreator 2016-05-20T14:22:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

TARGET = Projet_GL40_Refrigestion
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ingredient.cpp \
    recette.cpp \
    fenetreajoutingredient.cpp \
    fenetreajoutrecette.cpp \
    vignetteingredient.cpp \
    vignetterecette.cpp \
    gestiondefichiers.cpp \
    fenetredetailrecette.cpp

HEADERS  += mainwindow.h \
    ingredient.h \
    recette.h \
    fenetreajoutingredient.h \
    fenetreajoutrecette.h \
    vignetteingredient.h \
    vignetterecette.h \
    gestiondefichiers.h \
    fenetredetailrecette.h

FORMS    += mainwindow.ui \
    fenetreajoutingredient.ui \
    fenetreajoutrecette.ui \
    vignetteingredient.ui \
    vignetterecette.ui \
    fenetredetailrecette.ui

DISTFILES +=

RESOURCES += \
    ressources.qrc
