#ifndef TVECTOR2D_H
#define TVECTOR2D_H

#include <QtCore/qpoint.h>
#include <QtCore/qmetatype.h>

#include "tvector3d.h"
#include "tvector4d.h"

class TVector3D;
class TVector4D;

class QVariant;

class TVector2D
{
public:
    TVector2D();
    TVector2D(qreal xpos, qreal ypos);
    explicit TVector2D(const QPoint& point);
    explicit TVector2D(const QPointF& point);
    explicit TVector2D(const TVector3D& vector);
    explicit TVector2D(const TVector4D& vector);


    bool isNull() const;

    qreal x() const;
    qreal y() const;

    void setX(qreal x);
    void setY(qreal y);

    qreal length() const;
    qreal lengthSquared() const;

    TVector2D normalized() const;
    void normalize();

    TVector2D &operator+=(const TVector2D &vector);
    TVector2D &operator-=(const TVector2D &vector);
    TVector2D &operator*=(qreal factor);
    TVector2D &operator*=(const TVector2D &vector);
    TVector2D &operator/=(qreal divisor);

    static qreal dotProduct(const TVector2D& v1, const TVector2D& v2);

    friend inline bool operator==(const TVector2D &v1, const TVector2D &v2);
    friend inline bool operator!=(const TVector2D &v1, const TVector2D &v2);
    friend inline const TVector2D operator+(const TVector2D &v1, const TVector2D &v2);
    friend inline const TVector2D operator-(const TVector2D &v1, const TVector2D &v2);
    friend inline const TVector2D operator*(qreal factor, const TVector2D &vector);
    friend inline const TVector2D operator*(const TVector2D &vector, qreal factor);
    friend inline const TVector2D operator*(const TVector2D &v1, const TVector2D &v2);
    friend inline const TVector2D operator-(const TVector2D &vector);
    friend inline const TVector2D operator/(const TVector2D &vector, qreal divisor);

    friend inline bool qFuzzyCompare(const TVector2D& v1, const TVector2D& v2);


    TVector3D toVector3D() const;

    TVector4D toVector4D() const;


    QPoint toPoint() const;
    QPointF toPointF() const;

    operator QVariant() const;

private:
    float xp, yp;

    TVector2D(float xpos, float ypos, int dummy);

    friend class TVector3D;
    friend class TVector4D;
};

Q_DECLARE_TYPEINFO(TVector2D, Q_MOVABLE_TYPE);

inline TVector2D::TVector2D() : xp(0.0f), yp(0.0f) {}

inline TVector2D::TVector2D(float xpos, float ypos, int) : xp(xpos), yp(ypos) {}

inline TVector2D::TVector2D(qreal xpos, qreal ypos) : xp(xpos), yp(ypos) {}

inline TVector2D::TVector2D(const QPoint& point) : xp(point.x()), yp(point.y()) {}

inline TVector2D::TVector2D(const QPointF& point) : xp(point.x()), yp(point.y()) {}

inline bool TVector2D::isNull() const
{
    return qIsNull(xp) && qIsNull(yp);
}

inline qreal TVector2D::x() const { return qreal(xp); }
inline qreal TVector2D::y() const { return qreal(yp); }

inline void TVector2D::setX(qreal aX) { xp = aX; }
inline void TVector2D::setY(qreal aY) { yp = aY; }

inline TVector2D &TVector2D::operator+=(const TVector2D &vector)
{
    xp += vector.xp;
    yp += vector.yp;
    return *this;
}

inline TVector2D &TVector2D::operator-=(const TVector2D &vector)
{
    xp -= vector.xp;
    yp -= vector.yp;
    return *this;
}

inline TVector2D &TVector2D::operator*=(qreal factor)
{
    xp *= factor;
    yp *= factor;
    return *this;
}

inline TVector2D &TVector2D::operator*=(const TVector2D &vector)
{
    xp *= vector.xp;
    yp *= vector.yp;
    return *this;
}

inline TVector2D &TVector2D::operator/=(qreal divisor)
{
    xp /= divisor;
    yp /= divisor;
    return *this;
}

inline bool operator==(const TVector2D &v1, const TVector2D &v2)
{
    return v1.xp == v2.xp && v1.yp == v2.yp;
}

inline bool operator!=(const TVector2D &v1, const TVector2D &v2)
{
    return v1.xp != v2.xp || v1.yp != v2.yp;
}

inline const TVector2D operator+(const TVector2D &v1, const TVector2D &v2)
{
    return TVector2D(v1.xp + v2.xp, v1.yp + v2.yp, 1);
}

inline const TVector2D operator-(const TVector2D &v1, const TVector2D &v2)
{
    return TVector2D(v1.xp - v2.xp, v1.yp - v2.yp, 1);
}

inline const TVector2D operator*(qreal factor, const TVector2D &vector)
{
    return TVector2D(vector.xp * factor, vector.yp * factor, 1);
}

inline const TVector2D operator*(const TVector2D &vector, qreal factor)
{
    return TVector2D(vector.xp * factor, vector.yp * factor, 1);
}

inline const TVector2D operator*(const TVector2D &v1, const TVector2D &v2)
{
    return TVector2D(v1.xp * v2.xp, v1.yp * v2.yp, 1);
}

inline const TVector2D operator-(const TVector2D &vector)
{
    return TVector2D(-vector.xp, -vector.yp, 1);
}

inline const TVector2D operator/(const TVector2D &vector, qreal divisor)
{
    return TVector2D(vector.xp / divisor, vector.yp / divisor, 1);
}

inline bool qFuzzyCompare(const TVector2D& v1, const TVector2D& v2)
{
    return qFuzzyCompare(v1.xp, v2.xp) && qFuzzyCompare(v1.yp, v2.yp);
}

inline QPoint TVector2D::toPoint() const
{
    return QPoint(qRound(xp), qRound(yp));
}

inline QPointF TVector2D::toPointF() const
{
    return QPointF(qreal(xp), qreal(yp));
}


QDebug operator<<(QDebug dbg, const TVector2D &vector);

QDataStream &operator<<(QDataStream &, const TVector2D &);
QDataStream &operator>>(QDataStream &, TVector2D &);


#endif // TVECTOR2D_H
