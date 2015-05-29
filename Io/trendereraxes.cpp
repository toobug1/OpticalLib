#include "trendereraxes.h"

const Math::Vector3 & TRendererAxes::get_position() const
{
    return _pos;
}

const Math::Vector3 & TRendererAxes::get_origin() const
{
    return _origin;
}

bool TRendererAxes::get_show_grid() const
{
    return _grid;
}

bool TRendererAxes::get_show_frame() const
{
    return _frame;
}

bool TRendererAxes::get_show_axes(unsigned int i) const
{
    return _axes[i]._axis;
}

bool TRendererAxes::get_show_tics(unsigned int i) const
{
    return _axes[i]._tics;
}

bool TRendererAxes::get_show_values(unsigned int i) const
{
    return _axes[i]._values;
}

const std::string & TRendererAxes::get_label(unsigned int i) const
{
    return _axes[i]._label;
}

TRendererAxes::Axis::Axis()
    : _axis(true),
      _tics(true),
      _values(true),
      _step_mode(step_base),
      _count(5),
      _step_base(10.0),
      _si_prefix(false),
      _pow10_scale(true),
      _pow10(0),
      _unit(),
      _label(),
      _range(0, 0)
{
}

TRendererAxes::TRendererAxes()
    : _axes(),
      _grid(false),
      _frame(true),
      _pos(Math::Vector3(0, 0, 0)),
      _origin(Math::Vector3(0, 0, 0))
{
}
int ffs(int x)
{
    int r = 0;
    if (!x)
        return 0;
    if (!(x & 0xffff)) {
        x >>= 16;
        r += 16;
    }
    if (!(x & 0xff)) {
        x >>= 8;
        r += 8;
    }
    if (!(x & 0xf)) {
        x >>= 4;
        r += 4;
    }
    if (!(x & 3)) {
        x >>= 2;
        r += 2;
    }
    if (!(x & 1)) {
        x >>= 1;
        r += 1;
    }
    return r;
}
void TRendererAxes::set_tics_step(double step, AxisMask a_)
{
    for (int i, a = a_; (i = ffs(a) - 1) >= 0; a &= ~(1 << i))
    {
        _axes[i]._step_base = step;
        _axes[i]._step_mode = step_interval;
    }
}

void TRendererAxes::set_tics_count(unsigned int count, AxisMask a_)
{
    for (int i, a = a_; (i = ffs(a) - 1) >= 0; a &= ~(1 << i))
    {
        _axes[i]._count = count;
        _axes[i]._step_mode = step_count;
    }
}

void TRendererAxes::set_tics_base(unsigned int min_count, double base, AxisMask a_)
{
    for (int i, a = a_; (i = ffs(a) - 1) >= 0; a &= ~(1 << i))
    {
        _axes[i]._count = min_count;
        _axes[i]._step_base = base;
        _axes[i]._step_mode = step_base;
    }
}

void TRendererAxes::set_origin(const Math::Vector3 &origin)
{
    _origin = origin;
}

void TRendererAxes::set_position(const Math::Vector3 &position)
{
    _pos = position;
}

void TRendererAxes::set_show_grid(bool show)
{
    _grid = show;
}

void TRendererAxes::set_show_frame(bool show)
{
    _frame = show;
}

void TRendererAxes::set_show_axes(bool show, AxisMask a_)
{
    for (int i, a = a_; (i = ffs(a) - 1) >= 0; a &= ~(1 << i))
        _axes[i]._axis = show;
}

void TRendererAxes::set_show_tics(bool show, AxisMask a_)
{
    for (int i, a = a_; (i = ffs(a) - 1) >= 0; a &= ~(1 << i))
    {
        _axes[i]._tics = show;
        _axes[i]._axis |= show;
    }
}

void TRendererAxes::set_show_values(bool show, AxisMask a_)
{
    for (int i, a = a_; (i = ffs(a) - 1) >= 0; a &= ~(1 << i))
    {
        _axes[i]._values = show;
        _axes[i]._tics |= show;
        _axes[i]._axis |= show;
    }
}

void TRendererAxes::set_label(const std::string &label,
                             AxisMask a_)
{
    for (int i, a = a_; (i = ffs(a) - 1) >= 0; a &= ~(1 << i))
        _axes[i]._label = label;
}

void TRendererAxes::set_unit(const std::string &unit, bool pow10_scale,
                            bool si_prefix, int pow10,
                            AxisMask a_)
{
    for (int i, a = a_; (i = ffs(a) - 1) >= 0; a &= ~(1 << i))
    {
        _axes[i]._si_prefix = si_prefix;
        _axes[i]._unit = unit;
        _axes[i]._pow10_scale = pow10_scale;
        _axes[i]._pow10 = pow10;
    }
}


void TRendererAxes::set_range(const Math::range_t &r,
                             AxisMask a_)
{
    for (int i, a = a_; (i = ffs(a) - 1) >= 0; a &= ~(1 << i))
        _axes[i]._range = r;
}

double TRendererAxes::get_tics_step(int index, const Math::range_t &r) const
{
    assert(index < 3);

    const Axis *a = _axes + index;
    double d = r.second - r.first;

    switch (a->_step_mode)
    {
    case step_interval:
        return d > 0 ? a->_step_base : -a->_step_base;

    case step_count:
        return d / (double)a->_count;

    case step_base: {
        if (d == 0.0)
            return 1;

        double da = fabs(d);
        double p = floor(log(da) / log(a->_step_base));
        double n = pow(a->_step_base, p);
        unsigned int f = 1;

        while ((unsigned int)(da / n * f) < a->_count)
        {
            if ((unsigned int)(da / n * f * 2) >= a->_count)
            {
                f *= 2;
                break;
            }
            else if ((unsigned int)(da / n * f * 5) >= a->_count)
            {
                f *= 5;
                break;
            }
            else
            {
                f *= 10;
            }
        }

        n /= f;

        return d > 0 ? n : -n;
    }

    default:
        abort();
    }
}


