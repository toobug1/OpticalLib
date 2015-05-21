#ifndef TTVector_H
#define TTVector_H

#include <ostream>

#include "TMatrix"

/**
   @short TVector base class
   @header Goptical/Math/TVector
   @module {Core}
   @internal

   This class is the base class for N dimensions TVector.
 */
template <int N, typename T = double> struct TVectorBase
{
    template <int, typename> friend class TVectorBase;

    /** Set the whole TVector to the specified value */
    inline void set(T value);
    /** Get value at specified index */
    inline T operator[](int n) const;
    /** Get reference to value at specified index */
    inline T & operator[](int n);
    /** Add two TVectors */
    inline TVectorBase operator+(const TVectorBase &v) const;
    /** Subtract two TVectors */
    inline TVectorBase operator-(const TVectorBase &v) const;
    /** Get negated TVector */
    inline TVectorBase operator-() const;
    /** Negate TVector */
    inline TVectorBase & neg();
    /** Add a TVector */
    inline const TVectorBase & operator+=(const TVectorBase &v);
    /** Sutract a TVector */
    inline const TVectorBase & operator-=(const TVectorBase &v);
    /** TVector dot product */
    inline T operator*(const TVectorBase &v) const;
    /** TVector values multiply */
    inline TVectorBase mul(const TVectorBase &v) const;
    /** Scale TVector */
    inline TVectorBase operator*(T scale) const;
    /** Scale TVector */
    inline TVectorBase operator/(T scale) const;
    /** TVector division */
    inline TVectorBase operator/(const TVectorBase &v) const;
    /** Multiply by a TVector */
    inline const TVectorBase & operator*=(T scale);
    /** Divide by a TVector */
    inline const TVectorBase & operator/=(T scale);
    /** Compute TVector length */
    inline T len() const;
    /** Normalize TVector length @see normalized */
    inline const TVectorBase & normalize();
    /** Get normalized TVector @see normalize */
    inline TVectorBase normalized() const;
    /** Adjust TVector length */
    inline TVectorBase magnitude(T newlen) const;
    /** Select components from two TVectors. Components which have
      their corresponding bit set in the mask are extracted from
      this TVector and other components are taken from passed
      TVector. */
    template <int M>
    inline TVectorBase<M, T> select(unsigned int bitmask, const TVectorBase<M, T> &v) const;
    /** Multiply TVector with matrix. See TVector class for
      matrix/TVector multiplication. */
    inline TVectorBase operator*(const TSquareMatrix<N> &m);
    /** compare two TVectors for equality */
    inline bool operator==(const TVectorBase &m) const;
    /** compare two almost equal TVectors */
    inline bool close_to(const TVectorBase &m, T error = 1e-8);

protected:
    T _val[N];
};

/**
   @short N dimension TVector class
   @header Goptical/Math/TVector
   @module {Core}

   This class is the general purpose N dimensions TVector class.
 */
template <int N, typename T> struct TVector : public TVectorBase<N, T>
{
    inline TVector();
    inline TVector(const TVectorBase<N, T> &v);
    /** Create a 2d TVector with same value for all components */
    inline TVector(T v);
};

/**
   @short 2d TVector class
   @header Goptical/Math/TVector
   @module {Core}
   @alias TVector2

   This class implements 2d TVectors.

   The @ref Math namespace contains some commonly used constant TVector objects.
 */
template <typename T> struct TVector<2, T> : public TVectorBase<2, T>
{
    typedef TVectorBase<2, T> base;
//    typedef delegate<void (const Math::TVector2 &)> put_delegate_t;

    inline TVector();

    inline TVector(const TVectorBase<2, T> &v);

    template <int M>
    inline TVector(const TVectorBase<M, T> &v, T p);

    /** Create a 2d TVector with same value for all components */
    inline TVector(T v);

    /** Create a 2d TVector from x and y z values */
    inline TVector(T x, T y);

    /** Create a 2d TVector and initialize from specified components of an other TVector */
    template <int N>
    inline TVector(const TVectorBase<N, T> &v, unsigned int c0, unsigned int c1);

    inline T cross_product(const TVector<2, T> &v) const;

    /** Get reference to TVector x value */
    inline T & x();
    /** Get reference to TVector y value */
    inline T & y();
    /** Get TVector x value */
    inline T x() const;
    /** Get TVector y value */
    inline T y() const;
};

/**
   @short 3d TVector class
   @header Goptical/Math/TVector
   @module {Core}
   @alias TVector3

   This class implements 3d TVectors.

   The @ref Math namespace contains some commonly used constant TVector objects.
 */
template <typename T> struct TVector<3, T> : public TVectorBase<3, T>
{
    typedef TVectorBase<3, T> base;
//    typedef delegate<void (const TVector<3, T> &)> put_delegate_t;

    inline TVector();

    /** Copy constructor */
    inline TVector(const TVectorBase<3, T> &v);

    /** Create a 3d TVector from 2d TVector and given z value. */
    template <int M>
    inline TVector(const TVectorBase<M, T> &v, T p);

    /** Create a 2d TVector with same value for all components */
    inline TVector(T v);
    /** Create a 3d TVector from x, y and z values */
    inline TVector(T x, T y, T z);
    /** Get a 2d TVector formed x and y values. */
    inline TVector<2, T> project_xy() const;
    /** Get a 2d TVector formed z and y values. */
    inline TVector<2, T> project_zy() const;
    /** Compute TVectors cross product */
    inline TVector cross_product(const TVector<3, T> &v) const;

    /** Get reference to TVector x value */
    inline T & x();
    /** Get reference to TVector y value */
    inline T & y();
    /** Get reference to TVector z value */
    inline T & z();

    /** Get TVector x value */
    inline T x() const;
    /** Get TVector y value */
    inline T y() const;
    /** Get TVector z value */
    inline T z() const;
};

template <int N, typename T>
std::ostream & operator<<(std::ostream &o, const TVectorBase<N, T> &v);

typedef TVector<2, double> TVector2;
typedef TVector<3, double> TVector3;

/** @multiple @showvalue Commonly used TVector constant */
static const TVector2 TVector2_0 = TVector2(0.0, 0.0);
static const TVector2 TVector2_1 = TVector2(1.0, 1.0);

static const TVector2 TVector2_01 = TVector2(0.0, 1.0);
static const TVector2 TVector2_10 = TVector2(1.0, 0.0);

static const TVector3 TVector3_0 = TVector3(0.0, 0.0, 0.0);
static const TVector3 TVector3_1 = TVector3(1.0, 1.0, 1.0);

static const TVector3 TVector3_001 = TVector3(0.0, 0.0, 1.0);
static const TVector3 TVector3_010 = TVector3(0.0, 1.0, 0.0);
static const TVector3 TVector3_100 = TVector3(1.0, 0.0, 0.0);
/** */

template <int N, typename T>
void TVectorBase<N, T>::set(T value)
{
    for (unsigned int i = 0; i < N; i++)
        _val[i] = value;
}

template <int N, typename T>
T & TVectorBase<N, T>::operator[](int n)
{
    return _val[n];
}

template <int N, typename T>
T TVectorBase<N, T>::operator[](int n) const
{
    return _val[n];
}

template <int N, typename T>
TVectorBase<N, T> TVectorBase<N, T>::operator+(const TVectorBase<N, T> &v) const
{
    TVectorBase<N, T> r;

    for (unsigned int i = 0; i < N; i++)
        r._val[i] = _val[i] + v._val[i];

    return r;
}

template <int N, typename T>
TVectorBase<N, T> TVectorBase<N, T>::operator-(const TVectorBase<N, T> &v) const
{
    TVectorBase<N, T> r;

    for (unsigned int i = 0; i < N; i++)
        r._val[i] = _val[i] - v._val[i];

    return r;
}

template <int N, typename T>
TVectorBase<N, T> TVectorBase<N, T>::operator-() const
{
    TVectorBase<N, T> r;

    for (unsigned int i = 0; i < N; i++)
        r._val[i] = -_val[i];

    return r;
}

template <int N, typename T>
TVectorBase<N, T> & TVectorBase<N, T>::neg()
{
    for (unsigned int i = 0; i < N; i++)
        _val[i] = -_val[i];

    return *this;
}

template <int N, typename T>
const TVectorBase<N, T> & TVectorBase<N, T>::operator+=(const TVectorBase<N, T> &v)
{
    for (unsigned int i = 0; i < N; i++)
        _val[i] += v._val[i];

    return *this;
}

template <int N, typename T>
const TVectorBase<N, T> & TVectorBase<N, T>::operator-=(const TVectorBase<N, T> &v)
{
    for (unsigned int i = 0; i < N; i++)
        _val[i] -= v._val[i];

    return *this;
}

template <int N, typename T>
T TVectorBase<N, T>::operator*(const TVectorBase<N, T> &v) const
{
    T r = 0;

    for (unsigned int i = 0; i < N; i++)
        r += _val[i] * v._val[i];

    return r;
}

template <int N, typename T>
TVectorBase<N, T> TVectorBase<N, T>::mul(const TVectorBase &v) const
{
    TVectorBase<N, T> r;

    for (unsigned int i = 0; i < N; i++)
        r._val[i] = _val[i] * v._val[i];

    return r;
}

template <int N, typename T>
TVectorBase<N, T> TVectorBase<N, T>::operator/(const TVectorBase &v) const
{
    TVectorBase<N, T> r;

    for (unsigned int i = 0; i < N; i++)
        r._val[i] = _val[i] / v._val[i];

    return r;
}

template <int N, typename T>
TVectorBase<N, T> TVectorBase<N, T>::operator*(T scale) const
{
    TVectorBase<N, T> r;

    for (unsigned int i = 0; i < N; i++)
        r._val[i] = scale * _val[i];

    return r;
}

template <int N, typename T>
TVectorBase<N, T> TVectorBase<N, T>::operator/(T scale) const
{
    TVectorBase<N, T> r;

    for (unsigned int i = 0; i < N; i++)
        r._val[i] = _val[i] / scale;

    return r;
}

template <int N, typename T>
const TVectorBase<N, T> & TVectorBase<N, T>::operator*=(T scale)
{
    for (unsigned int i = 0; i < N; i++)
        _val[i] *= scale;

    return *this;
}

template <int N, typename T>
const TVectorBase<N, T> & TVectorBase<N, T>::operator/=(T scale)
{
    for (unsigned int i = 0; i < N; i++)
        _val[i] /= scale;

    return *this;
}

template <int N, typename T>
T TVectorBase<N, T>::len() const
{
    T r = 0;

    for (unsigned int i = 0; i < N; i++)
        r += square(_val[i]);

    return sqrt(r);
}

template <int N, typename T>
const TVectorBase<N, T> & TVectorBase<N, T>::normalize()
{
    return *this /= len();
}

template <int N, typename T>
TVectorBase<N, T> TVectorBase<N, T>::normalized() const
{
    TVectorBase<N, T> r = *this / len();
    return r;
}

template <int N, typename T>
TVectorBase<N, T> TVectorBase<N, T>::magnitude(T newlen) const
{
    return *this * (newlen / len());
}

template <int N, typename T>
template <int M>
TVectorBase<M, T> TVectorBase<N, T>::select(unsigned int mask, const TVectorBase<M, T> &v) const
{
    TVectorBase<M, T> r;

    for (unsigned int i = 0; i < M; i++)
        r._val[i] = (mask & (1 << i)) ? _val[i] : v._val[i];

    return r;
}

template <int N, typename T>
TVectorBase<N, T> TVectorBase<N, T>::operator*(const TSquareMatrix<N> &m)
{
    TVectorBase<N, T> r;

    for (int j = 0; j < N; j++)
    {
        T s = 0;

        for (int k = 0; k < N; k++)
            s += _val[k][j] * m._val[k];

        r[j] = s;
    }

    return r;
}

template <int N, typename T>
inline bool TVectorBase<N, T>::operator==(const TVectorBase &m) const
{
    for (int j = 0; j < N; j++)
        if (_val[j] != m._val[j])
            return false;

    return true;
}

template <int N, typename T>
bool TVectorBase<N, T>::close_to(const TVectorBase &m, T error)
{
    for (int j = 0; j < N; j++)
        if (fabs(_val[j] - m._val[j]) > error)
            return false;

    return true;
}

template <int N, typename T>
std::ostream & operator<<(std::ostream &o, const TVectorBase<N, T> &v)
{
    o << "[";

    for (unsigned int i = 0; i < N; i++)
    {
        o << v[i];
        if (i + 1 < N)
            o << ", ";
    }

    o << "]";

    return o;
}

// **********************************************************************

template <int N, typename T>
TVector<N, T>::TVector()
{
}

template <int N, typename T>
TVector<N, T>::TVector(T value)
{
    for (unsigned int i = 0; i < N; i++)
        TVectorBase<N, T>::_val[i] = value;
}

template <int N, typename T>
TVector<N, T>::TVector(const TVectorBase<N, T> &v)
    : TVectorBase<N, T>(v)
{
}

// **********************************************************************

template <typename T>
TVector<3, T>::TVector()
{
}

template <typename T>
TVector<3, T>::TVector(const TVectorBase<3, T> &v)
    : TVectorBase<3, T>(v)
{
}

template <typename T>
template <int M>
TVector<3, T>::TVector(const TVectorBase<M, T> &v, T p)
{
    int i;
    for (i = 0; i < std::min(3, M); i++)
        base::_val[i] = v[i];
    for (; i < 3; i++)
        base::_val[i] = p;
}

template <typename T>
TVector<3, T>::TVector(T value)
{
    base::_val[0] = base::_val[1] = base::_val[2] = value;
}

template <typename T>
TVector<3, T>::TVector(T x, T y, T z)
{
    base::_val[0] = x;
    base::_val[1] = y;
    base::_val[2] = z;
}

template <typename T>
TVector<2, T> TVector<3, T>::project_xy() const
{
    TVector<2, T> v;

    v[0] = x();
    v[1] = y();

    return v;
}

template <typename T>
TVector<2, T> TVector<3, T>::project_zy() const
{
    TVector<2, T> v;

    v[0] = z();
    v[1] = y();

    return v;
}

template <typename T>
TVector<3, T> TVector<3, T>::cross_product(const TVector<3, T> &v) const
{
    return TVector<3, T>(y() * v.z() - z() * v.y(),
                        z() * v.x() - x() * v.z(),
                        x() * v.y() - y() * v.x());
}

template <typename T>
T & TVector<3, T>::x()
{
    return base::_val[0];
}

template <typename T>
T & TVector<3, T>::y()
{
    return base::_val[1];
}

template <typename T>
T & TVector<3, T>::z()
{
    return base::_val[2];
}

template <typename T>
T TVector<3, T>::x() const
{
    return base::_val[0];
}

template <typename T>
T TVector<3, T>::y() const
{
    return base::_val[1];
}

template <typename T>
T TVector<3, T>::z() const
{
    return base::_val[2];
}

// **********************************************************************

template <typename T>
TVector<2, T>::TVector()
{
}

template <typename T>
TVector<2, T>::TVector(const TVectorBase<2, T> &v)
    : TVectorBase<2, T>(v)
{
}

template <typename T>
template <int M>
TVector<2, T>::TVector(const TVectorBase<M, T> &v, T p)
{
    int i;
    for (i = 0; i < std::min(2, M); i++)
        base::_val[i] = v[i];
    for (; i < 2; i++)
        base::_val[i] = p;
}

template <typename T>
TVector<2, T>::TVector(T value)
{
    base::_val[0] = base::_val[1] = value;
}

template <typename T>
TVector<2, T>::TVector(T x, T y)
{
    base::_val[0] = x;
    base::_val[1] = y;
}

template <typename T>
template <int N>
TVector<2, T>::TVector(const TVectorBase<N, T> &v, unsigned int a, unsigned int b)
{
    base::_val[0] = v[a];
    base::_val[1] = v[b];
}

template <typename T>
T TVector<2, T>::cross_product(const TVector<2, T> &v) const
{
    return base::_val[0] * v.base::_val[1] - base::_val[1] * v.base::_val[0];
}

template <typename T>
T & TVector<2, T>::x()
{
    return base::_val[0];
}

template <typename T>
T & TVector<2, T>::y()
{
    return base::_val[1];
}

template <typename T>
T TVector<2, T>::x() const
{
    return base::_val[0];
}

template <typename T>
T TVector<2, T>::y() const
{
    return base::_val[1];
}

#endif
