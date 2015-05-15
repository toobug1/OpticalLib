#ifndef TVECTOR4D_H
#define TVECTOR4D_H

#include <QtCore/qpoint.h>
#include <QtCore/qmetatype.h>

#include "tvector2d.h"
#include "tvector3d.h"

class TVector2D;
class TVector3D;

class QMatrix4x4;

class TVector4D
{
public:
    TVector4D();
    TVector4D(qreal xpos, qreal ypos, qreal zpos, qreal wpos);
    explicit TVector4D(const QPoint& point);
    explicit TVector4D(const QPointF& point);
    TVector4D(const TVector2D& vector);
    TVector4D(const TVector2D& vector, qreal zpos, qreal wpos);
    TVector4D(const TVector3D& vector);
    TVector4D(const TVector3D& vector, qreal wpos);

    bool isNull() const;

    qreal x() const;
    qreal y() const;
    qreal z() const;
    qreal w() const;

    void setX(qreal x);
    void setY(qreal y);
    void setZ(qreal z);
    void setW(qreal w);

    qreal length() const;
    qreal lengthSquared() const;

    TVector4D normalized() const;
    void normalize();

    TVector4D &operator+=(const TVector4D &vector);
    TVector4D &operator-=(const TVector4D &vector);
    TVector4D &operator*=(qreal factor);
    TVector4D &operator*=(const TVector4D &vector);
    TVector4D &operator/=(qreal divisor);

    static qreal dotProduct(const TVector4D& v1, const TVector4D& v2);

    friend inline bool operator==(const TVector4D &v1, const TVector4D &v2);
    friend inline bool operator!=(const TVector4D &v1, const TVector4D &v2);
    friend inline const TVector4D operator+(const TVector4D &v1, const TVector4D &v2);
    friend inline const TVector4D operator-(const TVector4D &v1, const TVector4D &v2);
    friend inline const TVector4D operator*(qreal factor, const TVector4D &vector);
    friend inline const TVector4D operator*(const TVector4D &vector, qreal factor);
    friend inline const TVector4D operator*(const TVector4D &v1, const TVector4D& v2);
    friend inline const TVector4D operator-(const TVector4D &vector);
    friend inline const TVector4D operator/(const TVector4D &vector, qreal divisor);

    friend inline bool qFuzzyCompare(const TVector4D& v1, const TVector4D& v2);

    TVector2D toVector2D() const;
    TVector2D toVector2DAffine() const;

    TVector3D toVector3D() const;
    TVector3D toVector3DAffine() const;


    QPoint toPoint() const;
    QPointF toPointF() const;

    operator QVariant() const;

private:
    float xp, yp, zp, wp;

    TVector4D(float xpos, float ypos, float zpos, float wpos, int dummy);

    friend class TVector2D;
    friend class TVector3D;

    friend TVector4D operator*(const TVector4D& vector, const QMatrix4x4& matrix);
    friend TVector4D operator*(const QMatrix4x4& matrix, const TVector4D& vector);
};

Q_DECLARE_TYPEINFO(TVector4D, Q_MOVABLE_TYPE);

inline TVector4D::TVector4D() : xp(0.0f), yp(0.0f), zp(0.0f), wp(0.0f) {}

inline TVector4D::TVector4D(qreal xpos, qreal ypos, qreal zpos, qreal wpos) : xp(xpos), yp(ypos), zp(zpos), wp(wpos) {}

inline TVector4D::TVector4D(float xpos, float ypos, float zpos, float wpos, int) : xp(xpos), yp(ypos), zp(zpos), wp(wpos) {}

inline TVector4D::TVector4D(const QPoint& point) : xp(point.x()), yp(point.y()), zp(0.0f), wp(0.0f) {}

inline TVector4D::TVector4D(const QPointF& point) : xp(point.x()), yp(point.y()), zp(0.0f), wp(0.0f) {}

inline bool TVector4D::isNull() const
{
    return qIsNull(xp) && qIsNull(yp) && qIsNull(zp) && qIsNull(wp);
}

inline qreal TVector4D::x() const { return qreal(xp); }
inline qreal TVector4D::y() const { return qreal(yp); }
inline qreal TVector4D::z() const { return qreal(zp); }
inline qreal TVector4D::w() const { return qreal(wp); }

inline void TVector4D::setX(qreal aX) { xp = aX; }
inline void TVector4D::setY(qreal aY) { yp = aY; }
inline void TVector4D::setZ(qreal aZ) { zp = aZ; }
inline void TVector4D::setW(qreal aW) { wp = aW; }

inline TVector4D &TVector4D::operator+=(const TVector4D &vector)
{
    xp += vector.xp;
    yp += vector.yp;
    zp += vector.zp;
    wp += vector.wp;
    return *this;
}

inline TVector4D &TVector4D::operator-=(const TVector4D &vector)
{
    xp -= vector.xp;
    yp -= vector.yp;
    zp -= vector.zp;
    wp -= vector.wp;
    return *this;
}

inline TVector4D &TVector4D::operator*=(qreal factor)
{
    xp *= factor;
    yp *= factor;
    zp *= factor;
    wp *= factor;
    return *this;
}

inline TVector4D &TVector4D::operator*=(const TVector4D &vector)
{
    xp *= vector.xp;
    yp *= vector.yp;
    zp *= vector.zp;
    wp *= vector.wp;
    return *this;
}

inline TVector4D &TVector4D::operator/=(qreal divisor)
{
    xp /= divisor;
    yp /= divisor;
    zp /= divisor;
    wp /= divisor;
    return *this;
}

inline bool operator==(const TVector4D &v1, const TVector4D &v2)
{
    return v1.xp == v2.xp && v1.yp == v2.yp && v1.zp == v2.zp && v1.wp == v2.wp;
}

inline bool operator!=(const TVector4D &v1, const TVector4D &v2)
{
    return v1.xp != v2.xp || v1.yp != v2.yp || v1.zp != v2.zp || v1.wp != v2.wp;
}

inline const TVector4D operator+(const TVector4D &v1, const TVector4D &v2)
{
    return TVector4D(v1.xp + v2.xp, v1.yp + v2.yp, v1.zp + v2.zp, v1.wp + v2.wp, 1);
}

inline const TVector4D operator-(const TVector4D &v1, const TVector4D &v2)
{
    return TVector4D(v1.xp - v2.xp, v1.yp - v2.yp, v1.zp - v2.zp, v1.wp - v2.wp, 1);
}

inline const TVector4D operator*(qreal factor, const TVector4D &vector)
{
    return TVector4D(vector.xp * factor, vector.yp * factor, vector.zp * factor, vector.wp * factor, 1);
}

inline const TVector4D operator*(const TVector4D &vector, qreal factor)
{
    return TVector4D(vector.xp * factor, vector.yp * factor, vector.zp * factor, vector.wp * factor, 1);
}

inline const TVector4D operator*(const TVector4D &v1, const TVector4D& v2)
{
    return TVector4D(v1.xp * v2.xp, v1.yp * v2.yp, v1.zp * v2.zp, v1.wp * v2.wp, 1);
}

inline const TVector4D operator-(const TVector4D &vector)
{
    return TVector4D(-vector.xp, -vector.yp, -vector.zp, -vector.wp, 1);
}

inline const TVector4D operator/(const TVector4D &vector, qreal divisor)
{
    return TVector4D(vector.xp / divisor, vector.yp / divisor, vector.zp / divisor, vector.wp / divisor, 1);
}

inline bool qFuzzyCompare(const TVector4D& v1, const TVector4D& v2)
{
    return qFuzzyCompare(v1.xp, v2.xp) &&
           qFuzzyCompare(v1.yp, v2.yp) &&
           qFuzzyCompare(v1.zp, v2.zp) &&
           qFuzzyCompare(v1.wp, v2.wp);
}

inline QPoint TVector4D::toPoint() const
{
    return QPoint(qRound(xp), qRound(yp));
}

inline QPointF TVector4D::toPointF() const
{
    return QPointF(qreal(xp), qreal(yp));
}


QDebug operator<<(QDebug dbg, const TVector4D &vector);


QDataStream &operator<<(QDataStream &, const TVector4D &);
QDataStream &operator>>(QDataStream &, TVector4D &);


#endif // TVECTOR4D_H
