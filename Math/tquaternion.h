#ifndef TQUATERNION_H
#define TQUATERNION_H

#include "tvector3d.h"
#include "tvector4d.h"

class TQuaternion
{
public:
    TQuaternion();
    TQuaternion(qreal scalar, qreal xpos, qreal ypos, qreal zpos);
    TQuaternion(qreal scalar, const TVector3D& vector);

    explicit TQuaternion(const TVector4D& vector);


    bool isNull() const;
    bool isIdentity() const;


    TVector3D vector() const;
    void setVector(const TVector3D& vector);

    void setVector(qreal x, qreal y, qreal z);

    qreal x() const;
    qreal y() const;
    qreal z() const;
    qreal scalar() const;

    void setX(qreal x);
    void setY(qreal y);
    void setZ(qreal z);
    void setScalar(qreal scalar);

    qreal length() const;
    qreal lengthSquared() const;

    TQuaternion normalized() const;
    void normalize();

    TQuaternion conjugate() const;

    TVector3D rotatedVector(const TVector3D& vector) const;

    TQuaternion &operator+=(const TQuaternion &quaternion);
    TQuaternion &operator-=(const TQuaternion &quaternion);
    TQuaternion &operator*=(qreal factor);
    TQuaternion &operator*=(const TQuaternion &quaternion);
    TQuaternion &operator/=(qreal divisor);

    friend inline bool operator==(const TQuaternion &q1, const TQuaternion &q2);
    friend inline bool operator!=(const TQuaternion &q1, const TQuaternion &q2);
    friend inline const TQuaternion operator+(const TQuaternion &q1, const TQuaternion &q2);
    friend inline const TQuaternion operator-(const TQuaternion &q1, const TQuaternion &q2);
    friend inline const TQuaternion operator*(qreal factor, const TQuaternion &quaternion);
    friend inline const TQuaternion operator*(const TQuaternion &quaternion, qreal factor);
    friend inline const TQuaternion operator*(const TQuaternion &q1, const TQuaternion& q2);
    friend inline const TQuaternion operator-(const TQuaternion &quaternion);
    friend inline const TQuaternion operator/(const TQuaternion &quaternion, qreal divisor);

    friend inline bool qFuzzyCompare(const TQuaternion& q1, const TQuaternion& q2);


    TVector4D toVector4D() const;


    operator QVariant() const;

    static TQuaternion fromAxisAndAngle(const TVector3D& axis, qreal angle);

    static TQuaternion fromAxisAndAngle
            (qreal x, qreal y, qreal z, qreal angle);

    static TQuaternion slerp
        (const TQuaternion& q1, const TQuaternion& q2, qreal t);
    static TQuaternion nlerp
        (const TQuaternion& q1, const TQuaternion& q2, qreal t);

private:
    qreal wp, xp, yp, zp;
};

Q_DECLARE_TYPEINFO(TQuaternion, Q_MOVABLE_TYPE);

inline TQuaternion::TQuaternion() : wp(1.0f), xp(0.0f), yp(0.0f), zp(0.0f) {}

inline TQuaternion::TQuaternion(qreal aScalar, qreal xpos, qreal ypos, qreal zpos) : wp(aScalar), xp(xpos), yp(ypos), zp(zpos) {}


inline bool TQuaternion::isNull() const
{
    return qIsNull(xp) && qIsNull(yp) && qIsNull(zp) && qIsNull(wp);
}

inline bool TQuaternion::isIdentity() const
{
    return qIsNull(xp) && qIsNull(yp) && qIsNull(zp) && wp == 1.0f;
}

inline qreal TQuaternion::x() const { return qreal(xp); }
inline qreal TQuaternion::y() const { return qreal(yp); }
inline qreal TQuaternion::z() const { return qreal(zp); }
inline qreal TQuaternion::scalar() const { return qreal(wp); }

inline void TQuaternion::setX(qreal aX) { xp = aX; }
inline void TQuaternion::setY(qreal aY) { yp = aY; }
inline void TQuaternion::setZ(qreal aZ) { zp = aZ; }
inline void TQuaternion::setScalar(qreal aScalar) { wp = aScalar; }

inline TQuaternion TQuaternion::conjugate() const
{
    return TQuaternion(wp, -xp, -yp, -zp);
}

inline TQuaternion &TQuaternion::operator+=(const TQuaternion &quaternion)
{
    xp += quaternion.xp;
    yp += quaternion.yp;
    zp += quaternion.zp;
    wp += quaternion.wp;
    return *this;
}

inline TQuaternion &TQuaternion::operator-=(const TQuaternion &quaternion)
{
    xp -= quaternion.xp;
    yp -= quaternion.yp;
    zp -= quaternion.zp;
    wp -= quaternion.wp;
    return *this;
}

inline TQuaternion &TQuaternion::operator*=(qreal factor)
{
    xp *= factor;
    yp *= factor;
    zp *= factor;
    wp *= factor;
    return *this;
}

inline const TQuaternion operator*(const TQuaternion &q1, const TQuaternion& q2)
{
    qreal ww = (q1.zp + q1.xp) * (q2.xp + q2.yp);
    qreal yy = (q1.wp - q1.yp) * (q2.wp + q2.zp);
    qreal zz = (q1.wp + q1.yp) * (q2.wp - q2.zp);
    qreal xx = ww + yy + zz;
    qreal qq = 0.5 * (xx + (q1.zp - q1.xp) * (q2.xp - q2.yp));

    qreal w = qq - ww + (q1.zp - q1.yp) * (q2.yp - q2.zp);
    qreal x = qq - xx + (q1.xp + q1.wp) * (q2.xp + q2.wp);
    qreal y = qq - yy + (q1.wp - q1.xp) * (q2.yp + q2.zp);
    qreal z = qq - zz + (q1.zp + q1.yp) * (q2.wp - q2.xp);

    return TQuaternion(w, x, y, z);
}

inline TQuaternion &TQuaternion::operator*=(const TQuaternion &quaternion)
{
    *this = *this * quaternion;
    return *this;
}

inline TQuaternion &TQuaternion::operator/=(qreal divisor)
{
    xp /= divisor;
    yp /= divisor;
    zp /= divisor;
    wp /= divisor;
    return *this;
}

inline bool operator==(const TQuaternion &q1, const TQuaternion &q2)
{
    return q1.xp == q2.xp && q1.yp == q2.yp && q1.zp == q2.zp && q1.wp == q2.wp;
}

inline bool operator!=(const TQuaternion &q1, const TQuaternion &q2)
{
    return q1.xp != q2.xp || q1.yp != q2.yp || q1.zp != q2.zp || q1.wp != q2.wp;
}

inline const TQuaternion operator+(const TQuaternion &q1, const TQuaternion &q2)
{
    return TQuaternion(q1.wp + q2.wp, q1.xp + q2.xp, q1.yp + q2.yp, q1.zp + q2.zp);
}

inline const TQuaternion operator-(const TQuaternion &q1, const TQuaternion &q2)
{
    return TQuaternion(q1.wp - q2.wp, q1.xp - q2.xp, q1.yp - q2.yp, q1.zp - q2.zp);
}

inline const TQuaternion operator*(qreal factor, const TQuaternion &quaternion)
{
    return TQuaternion(quaternion.wp * factor, quaternion.xp * factor, quaternion.yp * factor, quaternion.zp * factor);
}

inline const TQuaternion operator*(const TQuaternion &quaternion, qreal factor)
{
    return TQuaternion(quaternion.wp * factor, quaternion.xp * factor, quaternion.yp * factor, quaternion.zp * factor);
}

inline const TQuaternion operator-(const TQuaternion &quaternion)
{
    return TQuaternion(-quaternion.wp, -quaternion.xp, -quaternion.yp, -quaternion.zp);
}

inline const TQuaternion operator/(const TQuaternion &quaternion, qreal divisor)
{
    return TQuaternion(quaternion.wp / divisor, quaternion.xp / divisor, quaternion.yp / divisor, quaternion.zp / divisor);
}

inline bool qFuzzyCompare(const TQuaternion& q1, const TQuaternion& q2)
{
    return qFuzzyCompare(q1.xp, q2.xp) &&
           qFuzzyCompare(q1.yp, q2.yp) &&
           qFuzzyCompare(q1.zp, q2.zp) &&
           qFuzzyCompare(q1.wp, q2.wp);
}


inline TQuaternion::TQuaternion(qreal aScalar, const TVector3D& aVector)
    : wp(aScalar), xp(aVector.x()), yp(aVector.y()), zp(aVector.z()) {}

inline void TQuaternion::setVector(const TVector3D& aVector)
{
    xp = aVector.x();
    yp = aVector.y();
    zp = aVector.z();
}

inline TVector3D TQuaternion::vector() const
{
    return TVector3D(xp, yp, zp);
}

#endif

inline void TQuaternion::setVector(qreal aX, qreal aY, qreal aZ)
{
    xp = aX;
    yp = aY;
    zp = aZ;
}

#ifndef QT_NO_VECTOR4D

inline TQuaternion::TQuaternion(const TVector4D& aVector)
    : wp(aVector.w()), xp(aVector.x()), yp(aVector.y()), zp(aVector.z()) {}

inline TVector4D TQuaternion::toVector4D() const
{
    return TVector4D(xp, yp, zp, wp);
}


QDebug operator<<(QDebug dbg, const TQuaternion &q);


QDataStream &operator<<(QDataStream &, const TQuaternion &);
QDataStream &operator>>(QDataStream &, TQuaternion &);


#endif // TQUATERNION_H
