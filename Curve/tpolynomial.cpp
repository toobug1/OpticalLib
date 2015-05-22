#include "tpolynomial.h"

#include <cstdarg>
#include <cassert>

TPolynomial::TPolynomial()
    : _first_term(0),
      _last_term(0),
      _coeff()
{
    _coeff.push_back(0);
}

TPolynomial::TPolynomial(unsigned int first_term, unsigned int last_term, ...)
    : _coeff()
{
    va_list ap;

    assert(last_term >= first_term/*last_term*/);
    _first_term = first_term;
    _last_term = last_term;
    _coeff.resize(_last_term + 1, 0.0);

    va_start(ap, last_term);

    for (unsigned int i = first_term; i <= last_term; i++)
        _coeff[i] = va_arg(ap, double);

    va_end(ap);
}

void TPolynomial::set(unsigned int first_term, unsigned int last_term, ...)
{
    va_list ap;

    assert(last_term >= first_term/*last_term*/);
    _first_term = first_term;
    _last_term = last_term;
    _coeff.clear();
    _coeff.resize(_last_term + 1, 0.0);

    va_start(ap, last_term);

    for (unsigned int i = first_term; i <= last_term; i++)
        _coeff[i] = va_arg(ap, double);

    va_end(ap);
}

void TPolynomial::set_even(unsigned int first_term, unsigned int last_term, ...)
{
    va_list ap;

    assert(first_term % 2 == 0);
    assert(last_term % 2 == 0);
    assert(last_term >= first_term/*last_term*/);
    _first_term = first_term;
    _last_term = last_term;
    _coeff.clear();
    _coeff.resize(_last_term + 1, 0.0);

    va_start(ap, last_term);

    for (unsigned int i = first_term; i <= last_term; i += 2)
        _coeff[i] = va_arg(ap, double);

    va_end(ap);
}

void TPolynomial::set_odd(unsigned int first_term, unsigned int last_term, ...)
{
    va_list ap;

    assert(first_term % 2 == 1);
    assert(last_term % 2 == 1);
    assert(last_term >= first_term/*last_term*/);
    _first_term = first_term;
    _last_term = last_term;
    _coeff.clear();
    _coeff.resize(_last_term + 1, 0.0);

    va_start(ap, last_term);

    for (unsigned int i = first_term; i <= last_term; i += 2)
        _coeff[i] = va_arg(ap, double);

    va_end(ap);
}

void TPolynomial::set_term_factor(unsigned int n, double c)
{
    if (_last_term < n)
    {
        _last_term = n;
        _coeff.resize(n + 1, 0.0);
    }
    else if (_first_term > n)
    {
        _first_term = n;
    }

    _coeff[n] = c;
}

void TPolynomial::set_last_term(unsigned int n)
{
    _last_term = n;

    if (_first_term > _last_term)
        _first_term = _last_term;

    _coeff.resize(_last_term + 1, 0.0);
}

void TPolynomial::set_first_term(unsigned int n)
{
    _first_term = n;

    if (_first_term > _last_term)
        _last_term = _first_term;

    _coeff.resize(_last_term + 1, 0.0);

    for (unsigned int i = 0; i < _first_term; i++)
        _coeff[i] = 0.0;
}

double TPolynomial::sagitta(double r) const
{
    double y = 0;
    int i;

    for (i = _last_term; i >= (int)_first_term; i--)
        y = y * r + _coeff[i];

    y *= pow(r, (double)(i + 1));

    return y;
}

double TPolynomial::derivative(double r) const
{
    double y = 0;
    int i;

    for (i = _last_term; i >= (int)_first_term; i--)
        y = y * r + (double)i * _coeff[i];

    y *= pow(r, (double)i);

    return y;
}

