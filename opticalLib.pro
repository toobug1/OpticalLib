#-------------------------------------------------
#
# Project created by QtCreator 2015-05-14T08:06:59
#
#-------------------------------------------------

TARGET = opticalLib
TEMPLATE = lib
#CONFIG += staticlib


########

SOURCES += \
    Math/tvector2d.cpp \
    Math/tvector3d.cpp \
    Math/tvector4d.cpp \
    Math/tquaternion.cpp \
    Math/ttransform.cpp \
    Math/ttriangle.cpp \
    Material/tmaterialbase.cpp \
    Material/tair.cpp \
    Material/tdielectricbase.cpp \
    Material/tschott.cpp \
    Material/tglass.cpp

HEADERS += \
    Math/tvector2d.h \
    Math/tvector3d.h \
    Math/tvector4d.h \
    Math/tquaternion.h \
    Math/tmatrix.h \
    Math/tmatrix.hpp \
    Math/ttransform.h \
    Math/ttriangle.h \
    Material/tmaterialbase.h \
    Material/tair.h \
    Material/tdielectricbase.h \
    Material/tschott.h \
    Material/tglass.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

#LIBS += -lgsl \
#        -lgslcblas

QMAKE_CXXFLAGS +=  -Wno-unused-parameter

win32: LIBS += -lgsl

win32: LIBS += -lgslcblas

