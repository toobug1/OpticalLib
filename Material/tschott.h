#ifndef TSCHOTT_H
#define TSCHOTT_H

#include "tdielectricbase.h"

class TSchott : public TDielectricBase
{
public:
    TSchott();
    TSchott(double temp, const QString name);
    TSchott(double temp, const QString name, const double* dispCoeff, int n);
    TSchott(double temp, const QString name,
            double A0, double A1, double A2,
            double A3, double A4, double A5);
    ~TSchott();

    double get_refractive_index(double wavelen) const;

    int setDispCoeff(const double* tempCoeff, int size);

private:
    double m_A0;
    double m_A1;
    double m_A2;
    double m_A3;
    double m_A4;
    double m_A5;
};

#endif // TSCHOTT_H
