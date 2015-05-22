#ifndef TPOLYNOMIAL_H
#define TPOLYNOMIAL_H

#include <vector>

#include "trotational.h"


class TPolynomial : public TRotational
{
public:
    /** Create a new zero polynomial */
    TPolynomial();
    /** Create a new polynomial with given order and
      coefficients. Same syntax as set() function. */
    TPolynomial(unsigned int first_term, unsigned int last_term, ...);

    /** Set polynomial order and coefficients.
   @param first_term order of first (lowest) term.
   @param last_term order of last (highest) term.
   @param ... list of coefficient starting at lowest term
  */
    void set(unsigned int first_term, unsigned int last_term, ...);

    /** Set even polynomial order and coefficients.
   @param first_term order of first (lowest) term, must be even.
   @param last_term order of last (highest) term, must be even.
   @param ... list of even coefficient starting at lowest term
  */
    void set_even(unsigned int first_term, unsigned int last_term, ...);

    /** Set odd polynomial order and coefficients.
   @param first_term order of first (lowest) term, must be odd.
   @param last_term order of last (highest) term, must be odd.
   @param ... list of odd coefficient starting at lowest term
  */
    void set_odd(unsigned int first_term, unsigned int last_term, ...);

    /** Adjust coefficient of a single term. Adujst polynomial
      length as needed, setting additional coefficients to 0.  */
    void set_term_factor(unsigned int n, double c);

    /** Set order of first (lowest) term. Polynomial may be
      truncated or extended with 0 coefficients. */

    void set_first_term(unsigned int n);
    /** Set order of last (highest) term. Polynomial may be
      truncated or extended with 0 coefficients.*/
    void set_last_term(unsigned int n);

    double sagitta(double r) const;
    double derivative(double r) const;

private:
    unsigned int _first_term, _last_term;
    std::vector<double> _coeff;
};

#endif // TPOLYNOMIAL_H
