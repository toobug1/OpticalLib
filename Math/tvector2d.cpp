#include "tvector2d.h"

#include <QtCore/qdebug.h>
#include <QtCore/qvariant.h>
#include <QtCore/qmath.h>



/*!
    \class TVector2D
    \brief The TVector2D class represents a vector or vertex in 2D space.
    \since 4.6
    \ingroup painting
    \ingroup painting-3D

    The TVector2D class can also be used to represent vertices in 2D space.
    We therefore do not need to provide a separate vertex class.

    \bold{Note:} By design values in the TVector2D instance are stored as \c float.
    This means that on platforms where the \c qreal arguments to TVector2D
    functions are represented by \c double values, it is possible to
    lose precision.

    \sa TVector3D, TVector4D, QQuaternion
*/


TVector2D::TVector2D(const TVector3D& vector)
{
    xp = vector.xp;
    yp = vector.yp;
}



/*!
    Constructs a vector with x and y coordinates from a 3D \a vector.
    The z and w coordinates of \a vector are dropped.

    \sa toVector4D()
*/
TVector2D::TVector2D(const TVector4D& vector)
{
    xp = vector.xp;
    yp = vector.yp;
}

/*!
    Returns the length of the vector from the origin.

    \sa lengthSquared(), normalized()
*/
qreal TVector2D::length() const
{
    return qSqrt(xp * xp + yp * yp);
}

/*!
    Returns the squared length of the vector from the origin.
    This is equivalent to the dot product of the vector with itself.

    \sa length(), dotProduct()
*/
qreal TVector2D::lengthSquared() const
{
    return xp * xp + yp * yp;
}

/*!
    Returns the normalized unit vector form of this vector.

    If this vector is null, then a null vector is returned.  If the length
    of the vector is very close to 1, then the vector will be returned as-is.
    Otherwise the normalized form of the vector of length 1 will be returned.

    \sa length(), normalize()
*/
TVector2D TVector2D::normalized() const
{
    // Need some extra precision if the length is very small.
    double len = double(xp) * double(xp) +
                 double(yp) * double(yp);
    if (qFuzzyIsNull(len - 1.0f))
        return *this;
    else if (!qFuzzyIsNull(len))
        return *this / qSqrt(len);
    else
        return TVector2D();
}

/*!
    Normalizes the currect vector in place.  Nothing happens if this
    vector is a null vector or the length of the vector is very close to 1.

    \sa length(), normalized()
*/
void TVector2D::normalize()
{
    // Need some extra precision if the length is very small.
    double len = double(xp) * double(xp) +
                 double(yp) * double(yp);
    if (qFuzzyIsNull(len - 1.0f) || qFuzzyIsNull(len))
        return;

    len = qSqrt(len);

    xp /= len;
    yp /= len;
}


/*!
    Returns the dot product of \a v1 and \a v2.
*/
qreal TVector2D::dotProduct(const TVector2D& v1, const TVector2D& v2)
{
    return v1.xp * v2.xp + v1.yp * v2.yp;
}

/*!
    Returns the 3D form of this 2D vector, with the z coordinate set to zero.

    \sa toVector4D(), toPoint()
*/
TVector3D TVector2D::toVector3D() const
{
    return TVector3D(xp, yp, 0.0f, 1);
}


/*!
    Returns the 4D form of this 2D vector, with the z and w coordinates set to zero.

    \sa toVector3D(), toPoint()
*/
TVector4D TVector2D::toVector4D() const
{
    return TVector4D(xp, yp, 0.0f, 0.0f, 1);
}



/*!
    Returns the 2D vector as a QVariant.
*/
TVector2D::operator QVariant() const
{
    return QVariant(QVariant::Vector2D, this);
}

QDebug operator<<(QDebug dbg, const TVector2D &vector)
{
    dbg.nospace() << "TVector2D(" << vector.x() << ", " << vector.y() << ')';
    return dbg.space();
}


/*!
    \fn QDataStream &operator<<(QDataStream &stream, const TVector2D &vector)
    \relates TVector2D

    Writes the given \a vector to the given \a stream and returns a
    reference to the stream.

    \sa {Serializing Qt Data Types}
*/

QDataStream &operator<<(QDataStream &stream, const TVector2D &vector)
{
    stream << double(vector.x()) << double(vector.y());
    return stream;
}

/*!
    \fn QDataStream &operator>>(QDataStream &stream, TVector2D &vector)
    \relates TVector2D

    Reads a 2D vector from the given \a stream into the given \a vector
    and returns a reference to the stream.

    \sa {Serializing Qt Data Types}
*/

QDataStream &operator>>(QDataStream &stream, TVector2D &vector)
{
    double x, y;
    stream >> x;
    stream >> y;
    vector.setX(qreal(x));
    vector.setY(qreal(y));
    return stream;
}



