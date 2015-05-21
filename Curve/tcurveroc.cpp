#include "tcurveroc.h"

TCurveRoc::TCurveRoc(double roc)
  : m_roc(roc)
{
}

void TCurveRoc::set_roc(double roc)
{
  m_roc = roc;
}

double TCurveRoc::get_roc() const
{
  return m_roc;
}

