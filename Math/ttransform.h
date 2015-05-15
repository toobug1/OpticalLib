#ifndef TTRANSFORM_H
#define TTRANSFORM_H

#include <ostream>

#include <QTransform>

class TTransform2D : public QTransform
{
public:
    TTransform2D();
    TTransform2D(qreal m11, qreal m12, qreal m21, qreal m22, qreal dx, qreal dy);
    TTransform2D(const QMatrix & matrix);

};

std::ostream & operator<<(std::ostream & os, const QTransform & t);

class TTransform3D
{
public:
    TTransform3D();

};

#endif // TTRANSFORM_H
