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
    Material/tglass.cpp \
    Material/tsellmeier1.cpp \
    Material/therzberger.cpp \
    Material/tsellmeier2.cpp \
    Material/tconrady.cpp \
    Material/tsellmeier3.cpp \
    Material/thandbook1.cpp \
    Material/thandbook2.cpp \
    Material/tsellmeier4.cpp \
    Material/textended.cpp \
    Material/tsellmeier5.cpp \
    Curve/tcurvebase.cpp \
    Math/tvectorpair3d.cpp \
    Curve/tarray.cpp \
    Curve/tcomposer.cpp \
    Math/math_matrix.cc \
    Math/math_transform.cc \
    Curve/trotational.cpp \
    Curve/tcurveroc.cpp \
    Curve/tconicbase.cpp \
    Curve/tflat.cpp \
    Curve/tfoucault.cpp \
    Curve/tparabola.cpp \
    Curve/tpolynomial.cpp \
    Curve/tsphere.cpp \
    Curve/tspline.cpp \
    Curve/tzernike.cpp \
    Data/tset.cpp \
    Data/tset1d.cpp \
    Data/tsamplesetbase.cpp \
    Data/tdiscreteset.cpp \
    Data/tgrid.cpp \
    Data/tplot.cpp \
    Data/tplotdata.cpp \
    Shape/tshapebase.cpp \
    Shape/tdisk.cpp \
    Shape/tellipse.cpp \
    Shape/tellipticalring.cpp \
    Shape/tcomposershape.cpp \
    Shape/tinfinite.cpp \
    Shape/tpolygon.cpp \
    Shape/trectangle.cpp \
    Shape/tregularpolygon.cpp \
    Shape/tring.cpp \
    Analysis/tfocus.cpp \
    Analysis/tpointimage.cpp \
    Analysis/trayfan.cpp \
    Analysis/tspot.cpp \
    Light/tray.cpp \
    Light/tspectralline.cpp \
    Io/trgb.cpp \
    Sys/telement.cpp \
    Sys/tcontainer.cpp \
    Sys/tgroup.cpp \
    Sys/tsurface.cpp \
    Sys/timage.cpp \
    Sys/topticalsurface.cpp \
    Sys/tsource.cpp \
    Sys/tsourcepoint.cpp \
    Sys/tsourcerays.cpp \
    Sys/tstop.cpp \
    Sys/tlens.cpp \
    Sys/tsystem.cpp \
    Trace/tparams.cpp \
    Trace/tsequence.cpp \
    Trace/ttraceray.cpp \
    Io/trendererviewport.cpp \
    Io/trenderer.cpp \
    Trace/tresult.cpp \
    Io/trendereraxes.cpp \
    Trace/tdistribution.cpp \
    Trace/ttracer.cpp \
    Curve/tconic.cpp \
    Io/timport.cpp \
    Material/tcatalog.cpp \
    Io/timportzemax.cpp \
    Sys/tmirrorsurface.cpp \
    Material/tmirror.cpp \
    Io/trenderer2d.cpp \
    Io/trendererdxf.cpp

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
    Material/tglass.h \
    Material/tsellmeier1.h \
    Material/therzberger.h \
    Material/tsellmeier2.h \
    Material/tconrady.h \
    Material/tsellmeier3.h \
    Material/thandbook1.h \
    Material/thandbook2.h \
    Material/tsellmeier4.h \
    Material/textended.h \
    Material/tsellmeier5.h \
    Curve/tcurvebase.h \
    Math/tvectorpair3d.h \
    Curve/tarray.h \
    Curve/tcomposer.h \
    Math/tvector.h \
    common.hh \
    Math/VectorPair \
    Math/Matrix \
    Math/matrix.hh \
    Math/matrix.hxx \
    Math/Quaternion \
    Math/quaternion.hh \
    Math/quaternion.hxx \
    Math/Transform \
    Math/transform.hh \
    Math/transform.hxx \
    Math/Triangle \
    Math/triangle.hh \
    Math/triangle.hxx \
    Math/Vector \
    Math/vector.hh \
    Math/vector.hxx \
    Math/vector_pair.hh \
    Math/vector_pair.hxx \
    Error \
    error.hh \
    Curve/trotational.h \
    Curve/tcurveroc.h \
    Curve/tconicbase.h \
    Curve/tflat.h \
    Curve/tfoucault.h \
    Curve/tparabola.h \
    Curve/tpolynomial.h \
    Curve/tsphere.h \
    Curve/tspline.h \
    Curve/tzernike.h \
    Data/tset.h \
    Data/tset1d.h \
    Data/tsamplesetbase.h \
    Data/tsampleset.h \
    Data/tinterpolate1d.h \
    Data/tdiscreteset.h \
    Data/tgrid.h \
    vlarray \
    Data/tplot.h \
    Data/tplotdata.h \
    Shape/tshapebase.h \
    delegate \
    Trace/tdistribution.h \
    Trace/ttracer.h \
    Shape/tdisk.h \
    Shape/tellipse.h \
    Shape/tellipticalring.h \
    Shape/tcomposershape.h \
    Shape/tshaperound.h \
    Shape/tshaperound.h \
    Shape/tinfinite.h \
    Shape/tpolygon.h \
    Shape/trectangle.h \
    Shape/tregularpolygon.h \
    Shape/tring.h \
    Analysis/tfocus.h \
    Analysis/tpointimage.h \
    Analysis/trayfan.h \
    Analysis/tspot.h \
    Light/tray.h \
    Light/tspectralline.h \
    Io/trgb.h \
    Sys/telement.h \
    Sys/tcontainer.h \
    Sys/tgroup.h \
    Sys/tsurface.h \
    Sys/timage.h \
    vector_pool \
    Sys/topticalsurface.h \
    Sys/tsource.h \
    Sys/tsourcepoint.h \
    Sys/tsourcerays.h \
    Sys/tstop.h\
    Sys/tlens.h \
    Sys/tsystem.h \
    Trace/tparams.h \
    Trace/tsequence.h \
    Trace/ttraceray.h \
    Io/trendererviewport.h \
    Io/trenderer.h \
    Trace/tresult.h \
    Io/trendereraxes.h \
    fstring \
    ref \
    Curve/tconic.h \
    Io/timport.h \
    Material/tcatalog.h \
    Io/timportzemax.h \
    Sys/tmirrorsurface.h \
    Material/tmirror.h \
    Io/trenderer2d.h \
    Io/trendererdxf.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

QMAKE_CXXFLAGS +=  -Wno-unused-parameter

win32: LIBS += -lgsl

win32: LIBS += -lgslcblas


unix|win32: LIBS += -L$$PWD/dime/Release/release/ -ldime

INCLUDEPATH += $$PWD/dime/include
DEPENDPATH += $$PWD/dime/include

unix|win32: LIBS += -L$$PWD/lib/release -ldime

INCLUDEPATH += $$PWD/lib
DEPENDPATH += $$PWD/lib

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/release/dime.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/lib/release/libdime.a
