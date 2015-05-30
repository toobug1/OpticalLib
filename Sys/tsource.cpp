#include "tsource.h"


void TSource::set_material(const QSharedPointer<TGlass> &m)
{
    _mat = m;
}

void TSource::clear_spectrum()
{
    _spectrum.clear();
    _max_intensity = _min_intensity = 0.0;
}

void TSource::single_spectral_line(const TSpectralLine & l)
{
    _spectrum.clear();
    _spectrum.push_back(l);
}

void TSource::add_spectral_line(const TSpectralLine & l)
{
    _spectrum.push_back(l);
    _max_intensity = std::max(_max_intensity, l.get_intensity());
    _min_intensity = std::min(_min_intensity, l.get_intensity());
}

void TSource::set_spectral_line(const TSpectralLine & l, int index)
{
    _spectrum[index] = l;
    refresh_intensity_limits();
}

double TSource::get_max_intensity() const
{
    return _max_intensity;
}

double TSource::get_min_intensity() const
{
    return _min_intensity;
}

TSource::TSource(const Math::VectorPair3 &position)
    : TElement(position),
      _spectrum(),
      _mat()
{
    _max_intensity = _min_intensity = 1.0;
    _spectrum.push_back(TSpectralLine(550.0, 1.0));
}

void TSource::refresh_intensity_limits()
{
    if (_spectrum.empty())
        return;

    _min_intensity = _max_intensity = _spectrum[0].get_intensity();

    for (unsigned int i = 1; i < _spectrum.size(); i++)
    {
        _min_intensity = std::min(_min_intensity, _spectrum[i].get_intensity());
        _max_intensity = std::max(_max_intensity, _spectrum[i].get_intensity());
    }
}

void TSource::generate_rays_simple(TResult &result,
                                  const targets_t &entry) const
{
    throw Error("this light TSource is not designed generate light rays in simple ray trace mode");
}

void TSource::generate_rays_intensity(TResult &result,
                                     const targets_t &entry) const
{
    throw Error("this light TSource is not designed generate light rays in intensity ray trace mode");
}

void TSource::generate_rays_polarized(TResult &result,
                                     const targets_t &entry) const
{
    throw Error("this light TSource is not designed generate light rays in polarized ray trace mode");
}
