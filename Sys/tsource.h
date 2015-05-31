#ifndef TSOURCE_H
#define TSOURCE_H

#include <QSharedPointer>
#include "telement.h"
#include "Material/tglass.h"
#include "Light/tspectralline.h"

class TSource : public TElement
{
public:
    typedef std::vector<const TElement *> targets_t;

    /** Create a source at given position. */
    TSource(const Math::VectorPair3 &position);

    /** Set material where light rays are generated. system
      environment material is used by default. */
    void set_material(const QSharedPointer<TGlass> &m);

    /** Add a new wavelen for ray generation */
    void add_spectral_line(const TSpectralLine & l);

    /** Set a wavelen in list for ray generation */
    void set_spectral_line(const TSpectralLine & l,
                                  int index = 0);

    /** Clear ray wavelen list and set a single wavelen */
    void single_spectral_line(const TSpectralLine & l);

    /** Clear wavelen list */
    void clear_spectrum();

    /** Get maximal spectral line intensity */
    double get_max_intensity() const;

    /** Get minimal spectral line intensity */
    double get_min_intensity() const;

    /** Generate light rays from source */
    template <Trace::IntensityMode m>
    void generate_rays(TResult &result,
                       const targets_t &entry) const;

protected:

    /** This function generate light rays from source. Target entry
      surfaces may be used depending on source model. It must be
      reimplemented by subclasses. */
    virtual void generate_rays_simple(TResult &result,
                                      const targets_t &entry) const;

    /** This function process incoming light rays. It must be
      reimplemented by subclasses if the element can interact with
      light in intensity raytrace mode. */
    virtual void generate_rays_intensity(TResult &result,
                                         const targets_t &entry) const;

    /** This function process incoming light rays. It must be
      reimplemented by subclasses if the element can interact with
      light in polarized raytrace mode. */
    virtual void generate_rays_polarized(TResult &result,
                                         const targets_t &entry) const;

    void refresh_intensity_limits();

    std::vector<TSpectralLine>  _spectrum;
    double                      _min_intensity, _max_intensity;
    QSharedPointer<TGlass>      _mat;
};


template <Trace::IntensityMode m>
void TSource::generate_rays(TResult &result,
                            const targets_t &entry) const
{
  switch (m)
    {
    case Trace::SimpleTrace:
      return generate_rays_simple(result, entry);
    case Trace::IntensityTrace:
      return generate_rays_intensity(result, entry);
    case Trace::PolarizedTrace:
      return generate_rays_polarized(result, entry);
    }
}

#endif // TSOURCE_H
