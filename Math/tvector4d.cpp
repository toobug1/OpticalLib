#include "tvector4d.h"

#include <QtCore/qdebug.h>
#include <QtCore/qvariant.h>
#include <QtCore/qmath.h>


/*!
    \class TVector4D
    \brief The TVector4D class represents a vector or vertex in 4D space.
    \since 4.6
    \ingroup painting-3D

    The TVector4D class can also be used to represent vertices in 4D space.
    We therefore do not need to provide a separate vertex class.

    \bold{Note:} By design values in the TVector4D instance are stored as \c float.
    This means that on platforms where the \c qreal arguments to TVector4D
    functions are represented by \c double values, it is possible to
    lose precision.

    \sa QQuaternion, TVector2D, TVector3D
*/


/*!
    Constructs a 4D vector from the specified 2D \a vector.  The z
    and w coordinates are set to zero.

    \sa toVector2D()
*/
TVector4D::TVector4D(const TVector2D& vector)
{
    xp = vector.xp;
    yp = vector.yp;
    zp = 0.0f;
    wp = 0.0f;
}

/*!
    Constructs a 4D vector from the specified 2D \a vector.  The z
    and w coordinates are set to \a zpos and \a wpos respectively.

    \sa toVector2D()
*/
TVector4D::TVector4D(const TVector2D& vector, qreal zpos, qreal wpos)
{
    xp = vector.xp;
    yp = vector.yp;
    zp = zpos;
    wp = wpos;
}

/*!
    Constructs a 4D vector from the specified 3D \a vector.  The w
    coordinate is set to zero.

    \sa toVector3D()
*/
TVector4D::TVector4D(const TVector3D& vector)
{
    xp = vector.xp;
    yp = vector.yp;
    zp = vector.zp;
    wp = 0.0f;
}

/*!
    Constructs a 4D vector from the specified 3D \a vector.  The w
    coordinate is set to \a wpos.

    \sa toVector3D()
*/
TVector4D::TVector4D(const TVector3D& vector, qreal wpos)
{
    xp = vector.xp;
    yp = vector.yp;
    zp = vector.zp;
    wp = wpos;
}


qreal TVector4D::length() const
{
    return qSqrt(xp * xp + yp * yp + zp * zp + wp * wp);
}

/*!
    Returns the squared length of the vector from the origin.
    This is equivalent to the dot product of the vector with itself.

    \sa length(), dotProduct()
*/
qreal TVector4D::lengthSquared() const
{
    return xp * xp + yp * yp + zp * zp + wp * wp;
}

/*!
    Returns the normalized unit vector form of this vector.

    If this vector is null, then a null vector is returned.  If the length
    of the vector is very close to 1, then the vector will be returned as-is.
    Otherwise the normalized form of the vector of length 1 will be returned.

    \sa length(), normalize()
*/
TVector4D TVector4D::normalized() const
{
    // Need some extra precision if the length is very small.
    double len = double(xp) * double(xp) +
                 double(yp) * double(yp) +
                 double(zp) * double(zp) +
                 double(wp) * double(wp);
    if (qFuzzyIsNull(len - 1.0f))
        return *this;
    else if (!qFuzzyIsNull(len))
        return *this / qSqrt(len);
    else
        return TVector4D();
}

/*!
    Normalizes the currect vector in place.  Nothing happens if this
    vector is a null vector or the length of the vector is very close to 1.

    \sa length(), normalized()
*/
void TVector4D::normalize()
{
    // Need some extra precision if the length is very small.
    double len = double(xp) * double(xp) +
                 double(yp) * double(yp) +
                 double(zp) * double(zp) +
                 double(wp) * double(wp);
    if (qFuzzyIsNull(len - 1.0f) || qFuzzyIsNull(len))
        return;

    len = qSqrt(len);

    xp /= len;
    yp /= len;
    zp /= len;
    wp /= len;
}


/*!
    Returns the dot product of \a v1 and \a v2.
*/
qreal TVector4D::dotProduct(const TVector4D& v1, const TVector4D& v2)
{
    return v1.xp * v2.xp + v1.yp * v2.yp + v1.zp * v2.zp + v1.wp * v2.wp;
}


/*!
    Returns the 2D vector form of this 4D vector, dropping the z and w coordinates.

    \sa toVector2DAffine(), toVector3D(), toPoint()
*/
TVector2D TVector4D::toVector2D() const
{
    return TVector2D(xp, yp, 1);
}

/*!
    Returns the 2D vector form of this 4D vector, dividing the x and y
    coordinates by the w coordinate and dropping the z coordinate.
    Returns a null vector if w is zero.

    \sa toVector2D(), toVector3DAffine(), toPoint()
*/
TVector2D TVector4D::toVector2DAffine() const
{
    if (qIsNull(wp))
        return TVector2D();
    return TVector2D(xp / wp, yp / wp, 1);
}



/*!
    Returns the 3D vector form of this 4D vector, dropping the w coordinate.

    \sa toVector3DAffine(), toVector2D(), toPoint()
*/
TVector3D TVector4D::toVector3D() const
{
    return TVector3D(xp, yp, zp, 1);
}

/*!
    Returns the 3D vector form of this 4D vector, dividing the x, y, and
    z coordinates by the w coordinate.  Returns a null vector if w is zero.

    \sa toVector3D(), toVector2DAffine(), toPoint()
*/
TVector3D TVector4D::toVector3DAffine() const
{
    if (qIsNull(wp))
        return TVector3D();
    return TVector3D(xp / wp, yp / wp, zp / wp, 1);
}


/*!
    Returns the 4D vector as a QVariant.
*/
TVector4D::operator QVariant() const
{
    return QVariant(QVariant::Vector4D, this);
}


QDebug operator<<(QDebug dbg, const TVector4D &vector)
{
    dbg.nospace() << "TVector4D("
        << vector.x() << ", " << vector.y() << ", "
        << vector.z() << ", " << vector.w() << ')';
    return dbg.space();
}





/*!
    \fn QDataStream &operator<<(QDataStream &stream, const TVector4D &vector)
    \relates TVector4D

    Writes the given \a vector to the given \a stream and returns a
    reference to the stream.

    \sa {Serializing Qt Data Types}
*/

QDataStream &operator<<(QDataStream &stream, const TVector4D &vector)
{
    stream << double(vector.x()) << double(vector.y())
           << double(vector.z()) << double(vector.w());
    return stream;
}

/*!
    \fn QDataStream &operator>>(QDataStream &stream, TVector4D &vector)
    \relates TVector4D

    Reads a 4D vector from the given \a stream into the given \a vector
    and returns a reference to the stream.

    \sa {Serializing Qt Data Types}
*/

QDataStream &operator>>(QDataStream &stream, TVector4D &vector)
{
    double x, y, z, w;
    stream >> x;
    stream >> y;
    stream >> z;
    stream >> w;
    vector.setX(qreal(x));
    vector.setY(qreal(y));
    vector.setZ(qreal(z));
    vector.setW(qreal(w));
    return stream;
}

