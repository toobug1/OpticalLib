#ifndef TCURVEROC_H
#define TCURVEROC_H


class TCurveRoc
{
public:
    /** Set the radius of curvature */
    void set_roc(double roc);

    /** Get the radius of curvature */
    double get_roc() const;

protected:
    TCurveRoc(double roc);

    double m_roc;
};

#endif // TCURVEROC_H
