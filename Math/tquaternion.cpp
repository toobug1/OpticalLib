#include "tquaternion.h"

#include <QtCore/qmath.h>
#include <QtCore/qvariant.h>
#include <QtCore/qdebug.h>


/*!
    \class TQuaternion
    \brief The TQuaternion class represents a quaternion consisting of a vector and scalar.
    \since 4.6
    \ingroup painting-3D

    Quaternions are used to represent rotations in 3D space, and
    consist of a 3D rotation axis specified by the x, y, and z
    coordinates, and a scalar representing the rotation angle.
*/

qreal TQuaternion::length() const
{
    return qSqrt(xp * xp + yp * yp + zp * zp + wp * wp);
}

/*!
    Returns the squared length of the quaternion.

    \sa length()
*/
qreal TQuaternion::lengthSquared() const
{
    return xp * xp + yp * yp + zp * zp + wp * wp;
}

/*!
    Returns the normalized unit form of this quaternion.

    If this quaternion is null, then a null quaternion is returned.
    If the length of the quaternion is very close to 1, then the quaternion
    will be returned as-is.  Otherwise the normalized form of the
    quaternion of length 1 will be returned.

    \sa length(), normalize()
*/
TQuaternion TQuaternion::normalized() const
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
        return TQuaternion(0.0f, 0.0f, 0.0f, 0.0f);
}

/*!
    Normalizes the currect quaternion in place.  Nothing happens if this
    is a null quaternion or the length of the quaternion is very close to 1.

    \sa length(), normalized()
*/
void TQuaternion::normalize()
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
    Rotates \a vector with this quaternion to produce a new vector
    in 3D space.  The following code:

    \code
    TVector3D result = q.rotatedVector(vector);
    \endcode

    is equivalent to the following:

    \code
    TVector3D result = (q * TQuaternion(0, vector) * q.conjugate()).vector();
    \endcode
*/
TVector3D TQuaternion::rotatedVector(const TVector3D& vector) const
{
    return (*this * TQuaternion(0, vector) * conjugate()).vector();
}


/*!
    Creates a normalized quaternion that corresponds to rotating through
    \a angle degrees about the specified 3D \a axis.
*/
TQuaternion TQuaternion::fromAxisAndAngle(const TVector3D& axis, qreal angle)
{
    // Algorithm from:
    // http://www.j3d.org/matrix_faq/matrfaq_latest.html#Q56
    // We normalize the result just in case the values are close
    // to zero, as suggested in the above FAQ.
    qreal a = (angle / 2.0f) * M_PI / 180.0f;
    qreal s = qSin(a);
    qreal c = qCos(a);
    TVector3D ax = axis.normalized();
    return TQuaternion(c, ax.x() * s, ax.y() * s, ax.z() * s).normalized();
}


/*!
    Creates a normalized quaternion that corresponds to rotating through
    \a angle degrees about the 3D axis (\a x, \a y, \a z).
*/
TQuaternion TQuaternion::fromAxisAndAngle
        (qreal x, qreal y, qreal z, qreal angle)
{
    qreal length = qSqrt(x * x + y * y + z * z);
    if (!qFuzzyIsNull(length - 1.0f) && !qFuzzyIsNull(length)) {
        x /= length;
        y /= length;
        z /= length;
    }
    qreal a = (angle / 2.0f) * M_PI / 180.0f;
    qreal s = qSin(a);
    qreal c = qCos(a);
    return TQuaternion(c, x * s, y * s, z * s).normalized();
}

/*!
    Interpolates along the shortest spherical path between the
    rotational positions \a q1 and \a q2.  The value \a t should
    be between 0 and 1, indicating the spherical distance to travel
    between \a q1 and \a q2.

    If \a t is less than or equal to 0, then \a q1 will be returned.
    If \a t is greater than or equal to 1, then \a q2 will be returned.

    \sa nlerp()
*/
TQuaternion TQuaternion::slerp
    (const TQuaternion& q1, const TQuaternion& q2, qreal t)
{
    // Handle the easy cases first.
    if (t <= 0.0f)
        return q1;
    else if (t >= 1.0f)
        return q2;

    // Determine the angle between the two quaternions.
    TQuaternion q2b;
    qreal dot;
    dot = q1.xp * q2.xp + q1.yp * q2.yp + q1.zp * q2.zp + q1.wp * q2.wp;
    if (dot >= 0.0f) {
        q2b = q2;
    } else {
        q2b = -q2;
        dot = -dot;
    }

    // Get the scale factors.  If they are too small,
    // then revert to simple linear interpolation.
    qreal factor1 = 1.0f - t;
    qreal factor2 = t;
    if ((1.0f - dot) > 0.0000001) {
        qreal angle = qreal(qAcos(dot));
        qreal sinOfAngle = qreal(qSin(angle));
        if (sinOfAngle > 0.0000001) {
            factor1 = qreal(qSin((1.0f - t) * angle)) / sinOfAngle;
            factor2 = qreal(qSin(t * angle)) / sinOfAngle;
        }
    }

    // Construct the result quaternion.
    return q1 * factor1 + q2b * factor2;
}

/*!
    Interpolates along the shortest linear path between the rotational
    positions \a q1 and \a q2.  The value \a t should be between 0 and 1,
    indicating the distance to travel between \a q1 and \a q2.
    The result will be normalized().

    If \a t is less than or equal to 0, then \a q1 will be returned.
    If \a t is greater than or equal to 1, then \a q2 will be returned.

    The nlerp() function is typically faster than slerp() and will
    give approximate results to spherical interpolation that are
    good enough for some applications.

    \sa slerp()
*/
TQuaternion TQuaternion::nlerp
    (const TQuaternion& q1, const TQuaternion& q2, qreal t)
{
    // Handle the easy cases first.
    if (t <= 0.0f)
        return q1;
    else if (t >= 1.0f)
        return q2;

    // Determine the angle between the two quaternions.
    TQuaternion q2b;
    qreal dot;
    dot = q1.xp * q2.xp + q1.yp * q2.yp + q1.zp * q2.zp + q1.wp * q2.wp;
    if (dot >= 0.0f)
        q2b = q2;
    else
        q2b = -q2;

    // Perform the linear interpolation.
    return (q1 * (1.0f - t) + q2b * t).normalized();
}

/*!
    Returns the quaternion as a QVariant.
*/
TQuaternion::operator QVariant() const
{
    return QVariant(QVariant::Quaternion, this);
}


QDebug operator<<(QDebug dbg, const TQuaternion &q)
{
    dbg.nospace() << "TQuaternion(scalar:" << q.scalar()
        << ", vector:(" << q.x() << ", "
        << q.y() << ", " << q.z() << "))";
    return dbg.space();
}

/*!
    \fn QDataStream &operator<<(QDataStream &stream, const TQuaternion &quaternion)
    \relates TQuaternion

    Writes the given \a quaternion to the given \a stream and returns a
    reference to the stream.

    \sa {Serializing Qt Data Types}
*/

QDataStream &operator<<(QDataStream &stream, const TQuaternion &quaternion)
{
    stream << double(quaternion.scalar()) << double(quaternion.x())
           << double(quaternion.y()) << double(quaternion.z());
    return stream;
}

/*!
    \fn QDataStream &operator>>(QDataStream &stream, TQuaternion &quaternion)
    \relates TQuaternion

    Reads a quaternion from the given \a stream into the given \a quaternion
    and returns a reference to the stream.

    \sa {Serializing Qt Data Types}
*/

QDataStream &operator>>(QDataStream &stream, TQuaternion &quaternion)
{
    double scalar, x, y, z;
    stream >> scalar;
    stream >> x;
    stream >> y;
    stream >> z;
    quaternion.setScalar(qreal(scalar));
    quaternion.setX(qreal(x));
    quaternion.setY(qreal(y));
    quaternion.setZ(qreal(z));
    return stream;
}
