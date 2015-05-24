//#include "tfoucault.h"

//void TFoucault::set_moving_source(double offset)
//{
//    _updated = false;
//    _moving_source = true;
//    _offset = offset;
//    clear();
//}

//void TFoucault::set_fixed_source(double source_to_surface)
//{
//    _updated = false;
//    _moving_source = false;
//    _offset = source_to_surface;
//    clear();
//}

//void TFoucault::set_radius(double radius)
//{
//    _updated = false;
//    _radius = radius;
//}

//double TFoucault::get_radius() const
//{
//    return _radius;
//}

//void TFoucault::set_ode_stepsize(double step)
//{
//    _updated = false;
//    _ode_step = step;
//}

//unsigned int TFoucault::get_zones_count() const
//{
//    return _reading.get_count();
//}

//void TFoucault::set_knife_offset(unsigned int zone_number, double  knife_offset)
//{
//    _reading.get_y_value(zone_number) = knife_offset;
//}

//const std::pair<double, double> TFoucault::get_reading(unsigned int index) const
//{
//    std::pair<double, double> r;

//    r.first = _reading.get_x_value(index);
//    r.second = _reading.get_y_value(index) - m_roc;


//    return r;
//}

//void TFoucault::init()
//{
//    _moving_source = true;
//    _offset = 0;
//    _radius = 0;
//    _ode_step = 1;
//    _updated = false;

////    _reading.set_interpolation(Data::Cubic);
////    _sagitta.set_interpolation(Data::CubicDeriv);

//    gsl_st = gsl_odeiv_step_alloc(gsl_odeiv_step_rkf45, 1);
//    gsl_sys.function = gsl_func;
//    gsl_sys.jacobian = NULL;
//    gsl_sys.dimension = 1;
//    gsl_sys.params = this;
//}

//TFoucault::TFoucault(double roc)
//    : TCurveRoc(roc),
//        _reading(),
//        _sagitta()
//{
//    init();
//}

//TFoucault::~TFoucault()
//{
//    gsl_odeiv_step_free(gsl_st);
//}

//void TFoucault::fit(const TRotational &c)
//{
//    _offset = 0;
//    _moving_source = true;

//    for (unsigned int j = 0; /*j < _reading.get_count()*/; j++)
//    {
//        double zn = _reading.get_x_value(j);

//        _reading.get_y_value(j) = c.sagitta(zn) + zn / c.derivative(zn);

//    }
//}

//void TFoucault::add_reading(double zone_radius, double knife_offset)
//{
//    _updated = false;

//    if (_radius < zone_radius * 1.1)
//        _radius = zone_radius * 1.1;

//    _reading.add_data(zone_radius, m_roc + knife_offset);
//}

//unsigned int TFoucault::add_uniform_zones(double hole_radius, unsigned int count)
//{
//    assert(hole_radius < _radius);
//    assert(count > 0);

//    double step = (_radius - hole_radius) / (double)count;

//    //  _reading.clear(); // TODO

//    for (unsigned int i = 0; i < count; i++)
//    {
//        _reading.add_data(hole_radius + step / 2.0, m_roc);
//        hole_radius += step;
//    }

//    return count;
//}

//unsigned int TFoucault::add_couder_zones(double hole_radius,
//                                         unsigned int count,
//                                         std::vector<double> *edge)
//{
//    assert(hole_radius < _radius);

//    // see http://www.atmsite.org/contrib/Carlin/couder/

//    if (count == 0)
//    {
//        static const double k = 37e-6;

//        count = (unsigned int)ceil(((1.0 - hole_radius / _radius) *
//                                    Math::square(_radius * 2.0)) /
//                                   (16.0 * sqrt(Math::square(m_roc) * m_roc * k)));
//    }

//    double out;
//    double in = hole_radius;

//    _reading.clear();

//    for (unsigned int i = 1; i < count + 1; i++)
//    {
//        double zr = (double)i / (double)count;
//        double hr = hole_radius / _radius;    // hole ratio
//        double sr = 1.0 - hr; // surface ratio

//        out = _radius * sqrt(sr * zr + hr);

//        if (edge)
//            edge->push_back(in);

//        _reading.add_data(sqrt(Math::square((out + in) / 2.0) +
//                               (Math::square((out - in) / 2.0) / 2.0)), m_roc);

//        in = out;
//    }

//    if (edge)
//        edge->push_back(in);

//    return count;
//}

//void TFoucault::clear()
//{
//    _updated = false;

//    _reading.clear();
//    _sagitta.clear();
//}

//double TFoucault::sagitta(double r) const
//{
//    if (!_updated)
//        const_cast<TFoucault *>(this)->update();

//    return _sagitta.interpolate(r);
//}

//double TFoucault::derivative(double r) const
//{
//    if (!_updated)
//        const_cast<TFoucault *>(this)->update();

//    return _sagitta.interpolate(r, 1);
//}

//void TFoucault::update()
//{
//    _sagitta.clear();

//    double t = 0;
//    double y = 0.0, yerr;
//    double dydt_in = 0.0, dydt_out;

//    // differential equation integration
//    while (1)
//    {
//        _sagitta.add_data(t, y, dydt_in);

//        if (t >= _radius)
//            break;

//        if (gsl_odeiv_step_apply(gsl_st, t, _ode_step, &y, &yerr, &dydt_in, &dydt_out, &gsl_sys))
//            throw Error("Foucault curve integration error");

//        dydt_in = dydt_out;
//        t += _ode_step;
//    }

//    _updated = true;
//}

//int TFoucault::gsl_func(double y, const double x_[],
//                        double d[], void *params)
//{
//    TFoucault *c = static_cast<TFoucault *>(params);
//    double f = c->_reading.interpolate(y);
//    double x = x_[0];
//    double s = c->_moving_source ? f + c->_offset : c->_offset;

//#if 0
//    // moving source only
//    d[0] = y / (f - x);
//#endif
//    double ds = sqrt(Math::square(s - x) + Math::square(y));
//    double df = sqrt(Math::square(f - x) + Math::square(y));

//    d[0] = ((df + ds) * y) / (df * (s - x) + ds * (f - x));

//    return 0;
//}

