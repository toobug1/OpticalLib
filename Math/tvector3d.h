#ifndef TVECTOR3D_H
#define TVECTOR3D_H


#include <QtCore/qpoint.h>
#include <QtCore/qmetatype.h>


#include "tvector2d.h"
#include "tvector4d.h"

class TVector2D;
class TVector4D;


class QMatrix4x4;


class TVector3D
{
public:
    TVector3D();
    TVector3D(qreal xpos, qreal ypos, qreal zpos);
    explicit TVector3D(const QPoint& point);
    explicit TVector3D(const QPointF& point);
    TVector3D(const TVector2D& vector);
    TVector3D(const TVector2D& vector, qreal zpos);
    explicit TVector3D(const TVector4D& vector);


    bool isNull() const;

    qreal x() const;
    qreal y() const;
    qreal z() const;

    void setX(qreal x);
    void setY(qreal y);
    void setZ(qreal z);

    qreal length() const;
    qreal lengthSquared() const;

    TVector3D normalized() const;
    void normalize();

    TVector3D &operator+=(const TVector3D &vector);
    TVector3D &operator-=(const TVector3D &vector);
    TVector3D &operator*=(qreal factor);
    TVector3D &operator*=(const TVector3D& vector);
    TVector3D &operator/=(qreal divisor);

    TVector3D operator/(const TVector3D& vec)
    {
        // LQ aa
        TVector3D r;

        r.xp = xp / vec.xp;
        r.yp = yp / vec.yp;
        r.zp = zp / vec.zp;

        return r;
    }

    static qreal dotProduct(const TVector3D& v1, const TVector3D& v2);
    static TVector3D crossProduct(const TVector3D& v1, const TVector3D& v2);
    static TVector3D normal(const TVector3D& v1, const TVector3D& v2);
    static TVector3D normal
        (const TVector3D& v1, const TVector3D& v2, const TVector3D& v3);

    qreal distanceToPlane(const TVector3D& plane, const TVector3D& normal) const;
    qreal distanceToPlane(const TVector3D& plane1, const TVector3D& plane2, const TVector3D& plane3) const;
    qreal distanceToLine(const TVector3D& point, const TVector3D& direction) const;

    friend inline bool operator==(const TVector3D &v1, const TVector3D &v2);
    friend inline bool operator!=(const TVector3D &v1, const TVector3D &v2);
    friend inline const TVector3D operator+(const TVector3D &v1, const TVector3D &v2);
    friend inline const TVector3D operator-(const TVector3D &v1, const TVector3D &v2);
    friend inline const TVector3D operator*(qreal factor, const TVector3D &vector);
    friend inline const TVector3D operator*(const TVector3D &vector, qreal factor);
    friend const TVector3D operator*(const TVector3D &v1, const TVector3D& v2);
    friend inline const TVector3D operator-(const TVector3D &vector);
    friend inline const TVector3D operator/(const TVector3D &vector, qreal divisor);

    friend inline bool qFuzzyCompare(const TVector3D& v1, const TVector3D& v2);


    TVector2D toVector2D() const;
    TVector4D toVector4D() const;

    QPoint toPoint() const;
    QPointF toPointF() const;

    operator QVariant() const;

    TVector2D project_xy() const;  //LQ add

private:
    float xp, yp, zp;

    TVector3D(float xpos, float ypos, float zpos, int dummy);

    friend class TVector2D;
    friend class TVector4D;

    friend TVector3D operator*(const TVector3D& vector, const QMatrix4x4& matrix);
    friend TVector3D operator*(const QMatrix4x4& matrix, const TVector3D& vector);

};

Q_DECLARE_TYPEINFO(TVector3D, Q_MOVABLE_TYPE);

inline TVector3D::TVector3D() : xp(0.0f), yp(0.0f), zp(0.0f) {}

inline TVector3D::TVector3D(qreal xpos, qreal ypos, qreal zpos) : xp(xpos), yp(ypos), zp(zpos) {}

inline TVector3D::TVector3D(float xpos, float ypos, float zpos, int) : xp(xpos), yp(ypos), zp(zpos) {}

inline TVector3D::TVector3D(const QPoint& point) : xp(point.x()), yp(point.y()), zp(0.0f) {}

inline TVector3D::TVector3D(const QPointF& point) : xp(point.x()), yp(point.y()), zp(0.0f) {}

inline bool TVector3D::isNull() const
{
    return qIsNull(xp) && qIsNull(yp) && qIsNull(zp);
}

inline qreal TVector3D::x() const { return qreal(xp); }
inline qreal TVector3D::y() const { return qreal(yp); }
inline qreal TVector3D::z() const { return qreal(zp); }

inline void TVector3D::setX(qreal aX) { xp = aX; }
inline void TVector3D::setY(qreal aY) { yp = aY; }
inline void TVector3D::setZ(qreal aZ) { zp = aZ; }

inline TVector3D &TVector3D::operator+=(const TVector3D &vector)
{
    xp += vector.xp;
    yp += vector.yp;
    zp += vector.zp;
    return *this;
}

inline TVector3D &TVector3D::operator-=(const TVector3D &vector)
{
    xp -= vector.xp;
    yp -= vector.yp;
    zp -= vector.zp;
    return *this;
}

inline TVector3D &TVector3D::operator*=(qreal factor)
{
    xp *= factor;
    yp *= factor;
    zp *= factor;
    return *this;
}

inline TVector3D &TVector3D::operator*=(const TVector3D& vector)
{
    xp *= vector.xp;
    yp *= vector.yp;
    zp *= vector.zp;
    return *this;
}

inline TVector3D &TVector3D::operator/=(qreal divisor)
{
    xp /= divisor;
    yp /= divisor;
    zp /= divisor;
    return *this;
}

inline bool operator==(const TVector3D &v1, const TVector3D &v2)
{
    return v1.xp == v2.xp && v1.yp == v2.yp && v1.zp == v2.zp;
}

inline bool operator!=(const TVector3D &v1, const TVector3D &v2)
{
    return v1.xp != v2.xp || v1.yp != v2.yp || v1.zp != v2.zp;
}

inline const TVector3D operator+(const TVector3D &v1, const TVector3D &v2)
{
    return TVector3D(v1.xp + v2.xp, v1.yp + v2.yp, v1.zp + v2.zp, 1);
}

inline const TVector3D operator-(const TVector3D &v1, const TVector3D &v2)
{
    return TVector3D(v1.xp - v2.xp, v1.yp - v2.yp, v1.zp - v2.zp, 1);
}

inline const TVector3D operator*(qreal factor, const TVector3D &vector)
{
    return TVector3D(vector.xp * factor, vector.yp * factor, vector.zp * factor, 1);
}

inline const TVector3D operator*(const TVector3D &vector, qreal factor)
{
    return TVector3D(vector.xp * factor, vector.yp * factor, vector.zp * factor, 1);
}

inline const TVector3D operator*(const TVector3D &v1, const TVector3D& v2)
{
    return TVector3D(v1.xp * v2.xp, v1.yp * v2.yp, v1.zp * v2.zp, 1);
}

inline const TVector3D operator-(const TVector3D &vector)
{
    return TVector3D(-vector.xp, -vector.yp, -vector.zp, 1);
}

inline const TVector3D operator/(const TVector3D &vector, qreal divisor)
{
    return TVector3D(vector.xp / divisor, vector.yp / divisor, vector.zp / divisor, 1);
}

inline bool qFuzzyCompare(const TVector3D& v1, const TVector3D& v2)
{
    return qFuzzyCompare(v1.xp, v2.xp) &&
           qFuzzyCompare(v1.yp, v2.yp) &&
           qFuzzyCompare(v1.zp, v2.zp);
}

inline QPoint TVector3D::toPoint() const
{
    return QPoint(qRound(xp), qRound(yp));
}

inline QPointF TVector3D::toPointF() const
{
    return QPointF(qreal(xp), qreal(yp));
}


QDebug operator<<(QDebug dbg, const TVector3D &vector);

QDataStream &operator<<(QDataStream &, const TVector3D &);
QDataStream &operator>>(QDataStream &, TVector3D &);

#endif // TVECTOR3D_H
