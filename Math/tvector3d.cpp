#include "tvector3d.h"

#include <QtCore/qmath.h>
#include <QtCore/qvariant.h>
#include <QtCore/qdebug.h>



/*!
    \class TVector3D
    \brief The TVector3D class represents a vector or vertex in 3D space.
    \since 4.6
    \ingroup painting-3D

    Vectors are one of the main building blocks of 3D representation and
    drawing.  They consist of three coordinates, traditionally called
    x, y, and z.

    The TVector3D class can also be used to represent vertices in 3D space.
    We therefore do not need to provide a separate vertex class.

    \bold{Note:} By design values in the TVector3D instance are stored as \c float.
    This means that on platforms where the \c qreal arguments to TVector3D
    functions are represented by \c double values, it is possible to
    lose precision.

    \sa TVector2D, TVector4D, QQuaternion
*/

/*!
    Constructs a 3D vector from the specified 2D \a vector.  The z
    coordinate is set to zero.

    \sa toVector2D()
*/
TVector3D::TVector3D(const TVector2D& vector)
{
    xp = vector.xp;
    yp = vector.yp;
    zp = 0.0f;
}

/*!
    Constructs a 3D vector from the specified 2D \a vector.  The z
    coordinate is set to \a zpos.

    \sa toVector2D()
*/
TVector3D::TVector3D(const TVector2D& vector, qreal zpos)
{
    xp = vector.xp;
    yp = vector.yp;
    zp = zpos;
}



/*!
    Constructs a 3D vector from the specified 4D \a vector.  The w
    coordinate is dropped.

    \sa toVector4D()
*/
TVector3D::TVector3D(const TVector4D& vector)
{
    xp = vector.xp;
    yp = vector.yp;
    zp = vector.zp;
}





/*!
    \fn qreal TVector3D::x() const

    Returns the x coordinate of this point.

    \sa setX(), y(), z()
*/

/*!
    \fn qreal TVector3D::y() const

    Returns the y coordinate of this point.

    \sa setY(), x(), z()
*/

/*!
    \fn qreal TVector3D::z() const

    Returns the z coordinate of this point.

    \sa setZ(), x(), y()
*/

/*!
    \fn void TVector3D::setX(qreal x)

    Sets the x coordinate of this point to the given \a x coordinate.

    \sa x(), setY(), setZ()
*/


/*!
    Normalizes the currect vector in place.  Nothing happens if this
    vector is a null vector or the length of the vector is very close to 1.

    \sa length(), normalized()
*/

/*!
    Returns the normalized unit vector form of this vector.

    If this vector is null, then a null vector is returned.  If the length
    of the vector is very close to 1, then the vector will be returned as-is.
    Otherwise the normalized form of the vector of length 1 will be returned.

    \sa length(), normalize()
*/
TVector3D TVector3D::normalized() const
{
    // Need some extra precision if the length is very small.
    double len = double(xp) * double(xp) +
                 double(yp) * double(yp) +
                 double(zp) * double(zp);
    if (qFuzzyIsNull(len - 1.0f))
        return *this;
    else if (!qFuzzyIsNull(len))
        return *this / qSqrt(len);
    else
        return TVector3D();
}


void TVector3D::normalize()
{
    // Need some extra precision if the length is very small.
    double len = double(xp) * double(xp) +
                 double(yp) * double(yp) +
                 double(zp) * double(zp);
    if (qFuzzyIsNull(len - 1.0f) || qFuzzyIsNull(len))
        return;

    len = qSqrt(len);

    xp /= len;
    yp /= len;
    zp /= len;
}




/*!
    Returns the dot product of \a v1 and \a v2.
*/
qreal TVector3D::dotProduct(const TVector3D& v1, const TVector3D& v2)
{
    return v1.xp * v2.xp + v1.yp * v2.yp + v1.zp * v2.zp;
}

/*!
    Returns the cross-product of vectors \a v1 and \a v2, which corresponds
    to the normal vector of a plane defined by \a v1 and \a v2.

    \sa normal()
*/
TVector3D TVector3D::crossProduct(const TVector3D& v1, const TVector3D& v2)
{
    return TVector3D(v1.yp * v2.zp - v1.zp * v2.yp,
                    v1.zp * v2.xp - v1.xp * v2.zp,
                    v1.xp * v2.yp - v1.yp * v2.xp, 1);
}

/*!
    Returns the normal vector of a plane defined by vectors \a v1 and \a v2,
    normalized to be a unit vector.

    Use crossProduct() to compute the cross-product of \a v1 and \a v2 if you
    do not need the result to be normalized to a unit vector.

    \sa crossProduct(), distanceToPlane()
*/
TVector3D TVector3D::normal(const TVector3D& v1, const TVector3D& v2)
{
    return crossProduct(v1, v2).normalized();
}

/*!
    \overload

    Returns the normal vector of a plane defined by vectors
    \a v2 - \a v1 and \a v3 - \a v1, normalized to be a unit vector.

    Use crossProduct() to compute the cross-product of \a v2 - \a v1 and
    \a v3 - \a v1 if you do not need the result to be normalized to a
    unit vector.

    \sa crossProduct(), distanceToPlane()
*/
TVector3D TVector3D::normal
        (const TVector3D& v1, const TVector3D& v2, const TVector3D& v3)
{
    return crossProduct((v2 - v1), (v3 - v1)).normalized();
}

/*!
    Returns the distance from this vertex to a plane defined by
    the vertex \a plane and a \a normal unit vector.  The \a normal
    parameter is assumed to have been normalized to a unit vector.

    The return value will be negative if the vertex is below the plane,
    or zero if it is on the plane.

    \sa normal(), distanceToLine()
*/
qreal TVector3D::distanceToPlane
        (const TVector3D& plane, const TVector3D& normal) const
{
    return dotProduct(*this - plane, normal);
}

/*!
    \overload

    Returns the distance from this vertex a plane defined by
    the vertices \a plane1, \a plane2 and \a plane3.

    The return value will be negative if the vertex is below the plane,
    or zero if it is on the plane.

    The two vectors that define the plane are \a plane2 - \a plane1
    and \a plane3 - \a plane1.

    \sa normal(), distanceToLine()
*/
qreal TVector3D::distanceToPlane
    (const TVector3D& plane1, const TVector3D& plane2, const TVector3D& plane3) const
{
    TVector3D n = normal(plane2 - plane1, plane3 - plane1);
    return dotProduct(*this - plane1, n);
}

/*!
    Returns the distance that this vertex is from a line defined
    by \a point and the unit vector \a direction.

    If \a direction is a null vector, then it does not define a line.
    In that case, the distance from \a point to this vertex is returned.

    \sa distanceToPlane()
*/
qreal TVector3D::distanceToLine
        (const TVector3D& point, const TVector3D& direction) const
{
    if (direction.isNull())
        return (*this - point).length();
    TVector3D p = point + dotProduct(*this - point, direction) * direction;
    return (*this - p).length();
}


/*!
    Returns the 2D vector form of this 3D vector, dropping the z coordinate.

    \sa toVector4D(), toPoint()
*/
TVector2D TVector3D::toVector2D() const
{
    return TVector2D(xp, yp, 1);
}

/*!
    Returns the 4D form of this 3D vector, with the w coordinate set to zero.

    \sa toVector2D(), toPoint()
*/
TVector4D TVector3D::toVector4D() const
{
    return TVector4D(xp, yp, zp, 0.0f, 1);
}

/*!
    Returns the 3D vector as a QVariant.
*/
TVector3D::operator QVariant() const
{
    return QVariant(QVariant::Vector3D, this);
}

/*!
    Returns the length of the vector from the origin.

    \sa lengthSquared(), normalized()
*/
qreal TVector3D::length() const
{
    return qSqrt(xp * xp + yp * yp + zp * zp);
}

/*!
    Returns the squared length of the vector from the origin.
    This is equivalent to the dot product of the vector with itself.

    \sa length(), dotProduct()
*/
qreal TVector3D::lengthSquared() const
{
    return xp * xp + yp * yp + zp * zp;
}


QDebug operator<<(QDebug dbg, const TVector3D &vector)
{
    dbg.nospace() << "TVector3D("
        << vector.x() << ", " << vector.y() << ", " << vector.z() << ')';
    return dbg.space();
}

/*!
    \fn QDataStream &operator<<(QDataStream &stream, const TVector3D &vector)
    \relates TVector3D

    Writes the given \a vector to the given \a stream and returns a
    reference to the stream.

    \sa {Serializing Qt Data Types}
*/

QDataStream &operator<<(QDataStream &stream, const TVector3D &vector)
{
    stream << double(vector.x()) << double(vector.y())
           << double(vector.z());
    return stream;
}

/*!
    \fn QDataStream &operator>>(QDataStream &stream, TVector3D &vector)
    \relates TVector3D

    Reads a 3D vector from the given \a stream into the given \a vector
    and returns a reference to the stream.

    \sa {Serializing Qt Data Types}
*/

QDataStream &operator>>(QDataStream &stream, TVector3D &vector)
{
    double x, y, z;
    stream >> x;
    stream >> y;
    stream >> z;
    vector.setX(qreal(x));
    vector.setY(qreal(y));
    vector.setZ(qreal(z));
    return stream;
}

TVector2D TVector3D::project_xy() const  //LQ add
{
    TVector2D v;

    v.xp = x();
    v.yp = y();

    return v;
}


