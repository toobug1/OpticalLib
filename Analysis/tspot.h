#ifndef TSPOT_H
#define TSPOT_H

#include <QSharedPointer>

#include "tpointimage.h"
#include "Data/tplot.h"
#include "Math/VectorPair"

class TRendererViewport;
class TRendererAxes;

class TSpot : public TPointImage
{
public:
    TSpot(TSystem &system);

    void invalidate();

    /** Get spot maximum radius */
    double get_max_radius();

    /** Get spot root mean square radius */
    double get_rms_radius();

    /** Get amount of light intensity in the whole spot */
    double get_total_intensity();

    /** Get spot centroid */
    const Math::Vector3 & get_centroid();

    /** Get spot window center */
    Math::Vector3 get_center();

    /** Get amount of light intensity which falls in given radius from spot center */
    double get_encircled_intensity(double radius);

    /** Get encircled energy plot */
    QSharedPointer<TPlot> get_encircled_intensity_plot(int zones = 100);

    /** Set radius used for diagram drawing and encircled
      plots. Updated with spot max radius on ray trace */
    void set_useful_radius(double radius);

    /** draw the rays intersection points only */
    void draw_spot(TRendererViewport &TRenderer);
    /** draw the spot diagram */
    void draw_diagram(TRendererViewport &TRenderer,
                      bool centroid_origin = true);

    /** Get a reference to axes object rendered with spot
      diagram. @ref Io::RendererAxes coordinates are updated on
      ray trace. */
    TRendererAxes & get_diagram_axes();

private:
    void process_trace();
    void process_analysis();

    Math::Vector3 _centroid;

    bool      _processed_analysis;
    double    _max_radius;
    double    _rms_radius;
    double    _tot_intensity;
    double    _useful_radius;

//    Io::RendererAxes _axes; TODO
};

#endif // TSPOT_H
