#include "ttransform.h"

TTransform2D::TTransform2D()
{

}


std::ostream & operator<<(std::ostream &os, const QTransform &t)
{
    os << "m11 m12 m13: " << t.m11() << " " << t.m12() << " " << t.m13() << std::endl;
    os << "m21 m22 m23: " << t.m21() << " " << t.m22() << " " << t.m23() << std::endl;
    os << "m31 m32 m13: " << t.m31() << " " << t.m32() << " " << t.m33() << std::endl;
    return os;
}


