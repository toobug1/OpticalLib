#include "timportzemax.h"
#include "Error"

#include "Shape/tinfinite.h"
#include "Shape/tdisk.h"
#include "Shape/tring.h"
#include "Shape/trectangle.h"
#include "Shape/tellipse.h"
#include "Shape/tcomposershape.h"

#include "Curve/tflat.h"
#include "Curve/tsphere.h"
#include "Curve/tconic.h"
#include "Curve/tparabola.h"

#include "Sys/tsystem.h"
#include "Sys/timage.h"
#include "Sys/tsurface.h"
#include "Sys/topticalsurface.h"

#include "Material/tdielectricbase.h"
#include "Material/tcatalog.h"
#include "Material/tair.h"
#include "Material/tmirror.h"
#include "Material/tschott.h"
#include "Material/textended.h"
#include "Material/therzberger.h"
#include "Material/tconrady.h"
#include "Material/tsellmeier1.h"
#include "Material/tsellmeier2.h"
#include "Material/tsellmeier3.h"
#include "Material/tsellmeier4.h"
#include "Material/tsellmeier5.h"
#include "Material/thandbook1.h"
#include "Material/thandbook2.h"

#include "Math/Transform"

#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <QFileInfo>

#define ZMX_WARN(str) std::cerr << str << std::endl;

#define MAX_DISP_COEFF_COUNT 10

std::string TImportZemax::basename(const std::string &path)
{
    //    std::string str(path);

    //    size_t n = str.rfind(PATH_SEPARATOR);

    //    if (n != std::string::npos)
    //        str.erase(0, n + 1);

    //    n = str.rfind('.');

    //    if (n != std::string::npos)
    //        str.erase(n);

    QString strPath = QString::fromStdString(path);
    QFileInfo info(strPath);

    return info.fileName().toStdString();
}


////////////////////////////////////////////////////////////////////////
// Optical design import
////////////////////////////////////////////////////////////////////////

enum zemax_surface_e
{
    zs_none,
    zs_standard,
    zs_coordbrk
};

enum zemax_aperture_e
{
    za_none,
    za_circular,
    za_rectangular,
    za_elliptical,
};

enum zemax_glass_e
{
    zg_fixed = 0,
    zg_model = 1,
    zg_pickup = 2,
    zg_subst = 3,
    zg_offset = 4,
    zg_mirror,
    zg_air,
};

struct zemax_surface_s
{
    enum zemax_surface_e type;

    enum zemax_aperture_e ap_type;
    double ap_params[4];
    bool ap_obscuration;
    bool ap_decenter;

    enum zemax_glass_e gl_type;
    char gl_name[32];
    unsigned int gl_pickup;
    double gl_params[3];

    double roc;
    double coni;
    double thick;

    double params[13];

    inline zemax_surface_s()
        : type(zs_none),
          ap_type(za_none),
          ap_obscuration(false),
          ap_decenter(false),
          gl_type(zg_air),
          roc(0.0),
          coni(0.0),
          thick(0.0)
    {
        ap_params[0] = ap_params[1] = 0.0;
    }
};

QSharedPointer<TShapeBase>
TImportZemax::get_ap_shape(const struct zemax_surface_s &surf,
                           double unit_factor) const
{
    QSharedPointer<TShapeBase> shape;

    switch (surf.ap_type)
    {
    default:
        ZMX_WARN("unknown aperture shape");

    case za_none:
        shape.reset(new TInfinite);
        break;

    case za_circular:
        if (surf.ap_params[0] > 0.0)
            shape.reset(new TRing(surf.ap_params[1] * unit_factor,
                        surf.ap_params[0] * unit_factor));
        else
            shape.reset(new TDisk(surf.ap_params[1] * unit_factor));
        break;

    case za_elliptical:
        shape.reset(new TEllipse(surf.ap_params[0] * unit_factor,
                    surf.ap_params[1] * unit_factor));
        break;

    case za_rectangular:
        shape.reset(new TRectangle(surf.ap_params[0] * 2. * unit_factor,
                    surf.ap_params[1] * 2. * unit_factor));
        break;
    }

    if (surf.ap_decenter)
    {
        QSharedPointer<TComposerShape>  c(new TComposerShape);

        c->add_shape(shape).translate(Math::Vector2(surf.ap_params[2] * unit_factor,
                                      surf.ap_params[3] * unit_factor));

        return shape;
    }
    else
    {
        return shape;
    }
}

const TGlass& TImportZemax::get_glass(TSystem &sys, const struct zemax_surface_s &surf) const
{
    switch (surf.gl_type)
    {
    //      case zg_model:
    // FIXME use Abbe model

    case zg_air:
        return sys.get_environment();

    case zg_mirror:
        return mirror;

    case zg_fixed:
        GOPTICAL_FOREACH(c, _cat_list)
        {
            try {
                return (c->second->get_material(surf.gl_name));
            } catch (...) {
            }
        }
        throw Error("unable to find glass in loaded catalogs");

    default:
        throw Error("glass type not supported yet");
    }
}

QSharedPointer<TSystem> TImportZemax::import_design(const std::string &filename)
{
    std::ifstream file(filename.c_str());
    std::string line;

    double unit_factor = 1.0;
    std::vector<zemax_surface_s> surf_array;

    if (!file)
        throw Error("Unable to open file");

    QSharedPointer<TSystem> sys;

    while (std::getline(file, line))
    {
surf_end:

        if (line.size() < 4)
            continue;

        const char *buf = line.c_str();

#define ZMX_TYPE(a, b, c, d) ((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))

        unsigned int type = ZMX_TYPE(buf[0], buf[1], buf[2], buf[3]);
        buf += 4;

        ////////////////////////////////////////////////////
        // System units

        switch (type)
        {
        case ZMX_TYPE('U', 'N', 'I', 'T'): {

            char unit[6];

            if (sscanf(buf,
                       " %5s" // lens unit MM, CM, IN, METER
                       " %*s" // source prefix
                       " %*s" // source unit
                       " %*s" // analysis prefix
                       " %*s",        // analysis unit
                       unit) < 1)
                break;

            // strcasecmp ignore case, and return 0 when equaling

            if (!strcasecmp(unit, "mm"))
                unit_factor = 1.0;
            else if (!strcasecmp(unit, "cm"))
                unit_factor = 10.0;
            else if (!strcasecmp(unit, "in"))
                unit_factor = 25.4;
            else if (!strcasecmp(unit, "meter"))
                unit_factor = 1000.0;
            else
                ZMX_WARN("unknown unit token");

            break;
        }

            ////////////////////////////////////////////////////
            // System temperature and pressure

        case ZMX_TYPE('E', 'N', 'V', 'D'): {
            double temp, pressure;

            if (sscanf(buf,
                       " %lf" // system global temperature
                       " %lf", // system relative pressure
                       &temp, &pressure) < 2)
                break;

            QSharedPointer<TAir> env(&air);

            env->set_temperature(temp);
            env->set_pressure(pressure * 1.0/*Material::AirKohlrausch68::std_pressure*/);

            sys->set_environment(*env);

            break;
        }


            ////////////////////////////////////////////////////
            // Load glass catalog

        case ZMX_TYPE('G', 'C', 'A', 'T'): {
            char catname[32];
            int pos;

            while (sscanf(buf, " %31s%n", catname, &pos) >= 1)
            {
                import_catalog(catname);
                buf += pos;
            }

            break;
        }

            ////////////////////////////////////////////////////
            // Surface data

        case ZMX_TYPE('S', 'U', 'R', 'F'): {

            unsigned int id = std::atoi(buf);

            if (id <= surf_array.size())
                surf_array.resize(id + 1);

            zemax_surface_s &surface = surf_array[id];

            surface = zemax_surface_s();

            while (std::getline(file, line))
            {
                if (line.size() < 1 || line[0] != ' ')
                    goto surf_end;

                if (line.size() < 6)
                    continue;

                buf = line.c_str();
                type = ZMX_TYPE(buf[2], buf[3], buf[4], buf[5]);
                buf += 6;

                switch (type)
                {
                ////////////////////////////////////////////////////
                // Surface type

                case ZMX_TYPE('T', 'Y', 'P', 'E'): {

                    if (surface.type != zs_none)
                    {
                        ZMX_WARN("surface type already defined");
                        break;
                    }

                    char typestr[9];

                    if (sscanf(buf, " %8s", typestr) < 1)
                        break;

                    if      (!strcasecmp(typestr, "standard"))
                        surface.type = zs_standard;
                    else if (!strcasecmp(typestr, "coordbrk"))
                        surface.type = zs_coordbrk;
                    else
                        ZMX_WARN("unknown surface type token");

                    break;
                }

                    ////////////////////////////////////////////////////
                    // Surface curvature

                case ZMX_TYPE('C', 'U', 'R', 'V'): {

                    switch (surface.type)
                    {
                    case zs_standard: {
                        double curv = std::atof(buf);
                        surface.roc = curv == 0.0 ? 0.0 : 1.0 / curv;
                        break;
                    }

                    default:
                        break;
                    }

                    break;
                }

                    ////////////////////////////////////////////////////
                    // Surface conic constant

                case ZMX_TYPE('C', 'O', 'N', 'I'): {

                    surface.coni = std::atof(buf);

                    break;
                }

                    ////////////////////////////////////////////////////
                    // Surface param

                case ZMX_TYPE('P', 'A', 'R', 'M'): {

                    unsigned int param_id;
                    double param_val;

                    sscanf(buf, " %u %lf", &param_id, &param_val);

                    if (param_id < 13)
                        surface.params[param_id] = param_val;

                    break;
                }

                    ////////////////////////////////////////////////////
                    // Surface thickness

                case ZMX_TYPE('D', 'I', 'S', 'Z'): {

                    if (id > 0 && buf[2] == 'I') // infinity
                        ZMX_WARN("infinit thickness surface");

                    surface.thick = std::atof(buf);

                    break;
                }

                    ////////////////////////////////////////////////////
                    // Surface shape

                case ZMX_TYPE('D', 'I', 'A', 'M'): {

                    double d = std::atof(buf);

                    if (surface.ap_type == za_none && d > 0.0)
                    {
                        surface.ap_params[0] = 0.0;
                        surface.ap_params[1] = d;
                        surface.ap_type = za_circular;
                    }

                    break;
                }

                    ////////////////////////////////////////////////////
                    // Rectangular aperture

                case ZMX_TYPE('S', 'Q', 'O', 'B'):
                    surface.ap_obscuration = true;

                case ZMX_TYPE('S', 'Q', 'A', 'P'): {

                    surface.ap_type = za_rectangular;
                    sscanf(buf, " %lf %lf", &surface.ap_params[0], &surface.ap_params[1]);
                    break;
                }

                    ////////////////////////////////////////////////////
                    // Circular aperture

                case ZMX_TYPE('O', 'B', 'S', 'C'):
                    surface.ap_obscuration = true;

                case ZMX_TYPE('F', 'L', 'A', 'P'):
                case ZMX_TYPE('C', 'L', 'A', 'P'): {

                    surface.ap_type = za_circular;
                    sscanf(buf, " %lf %lf", &surface.ap_params[0], &surface.ap_params[1]);
                    break;
                }

                    ////////////////////////////////////////////////////
                    // Elliptical aperture

                case ZMX_TYPE('E', 'L', 'O', 'B'):
                    surface.ap_obscuration = true;

                case ZMX_TYPE('E', 'L', 'A', 'P'): {

                    surface.ap_type = za_elliptical;
                    sscanf(buf, " %lf %lf", &surface.ap_params[0], &surface.ap_params[1]);
                    break;
                }

                    ////////////////////////////////////////////////////
                    // Aperture decenter

                case ZMX_TYPE('O', 'B', 'D', 'C'): {

                    surface.ap_decenter = true;
                    sscanf(buf, " %lf %lf", &surface.ap_params[2], &surface.ap_params[3]);
                    break;
                }

                    ////////////////////////////////////////////////////
                    // Surface material

                case ZMX_TYPE('G', 'L', 'A', 'S'): {
                    unsigned int gl_type;
                    int pos;

                    if (sscanf(buf, " %31s %u %u%n", surface.gl_name, &gl_type, &surface.gl_pickup, &pos) < 3)
                        break;

                    surface.gl_type = (enum zemax_glass_e)gl_type;
                    buf += pos;

                    switch (surface.gl_type)
                    {
                    case zg_model:
                        sscanf(buf, " %lf %lf %lf %*i %*i %*f %*f",
                               &surface.gl_params[0],
                                &surface.gl_params[1],
                                &surface.gl_params[2]);
                        break;

                    case zg_offset:
                        sscanf(buf, " %*f %*f %*f %*i %*i %lf %lf",
                               &surface.gl_params[0],
                                &surface.gl_params[1]);
                        break;

                    case zg_fixed:
                        if (!strcasecmp(surface.gl_name, "mirror"))
                            surface.gl_type = zg_mirror;

                    default:
                        break;
                    }

                    break;
                }

                    ////////////////////////////////////////////////////

                } /* !switch */

            } /* !while */

            break;
        }

            ////////////////////////////////////////////////////

        } /* !switch */

    } /* !while */


#if 0
    // resolv "pickup" references

    for (unsigned int i = 0; i < surf_array.size(); i++)
    {
        zemax_surface_s &surface = surf_array[i];

        if (surface.gl_type == zg_pickup)
        {
            unsigned int id = surface.gl_pickup;

            if (id < surf_array.size())
            {
                surface.gl_type = surf_array[id].gl_type;
                strcpy(surface.gl_name, surf_array[id].gl_name);
                surface.gl_pickup = surf_array[id].gl_pickup; // FIXME ugly surf copy
                for (int j = 0; i < 3; i++)
                    surface.gl_params[j] = surf_array[id].gl_params[j];

                i = 0;
                continue;
            }
        }
    }
#endif

    Math::Transform<3> coord;
    coord.reset();

    TGlass last_mat = sys->get_environment();

    for (unsigned int i = 1; i < surf_array.size(); i++)
    {
        zemax_surface_s &surf = surf_array[i];

        QSharedPointer<TCurveBase> curve;

        switch (surf.type)
        {
        case zs_coordbrk: {
            bool order = surf.params[6] != 0.0;
            // FIXME
            if (!order)
                coord.apply_translation(Math::Vector3(surf.params[1], surf.params[2], 0.0) * unit_factor);
            coord.linear_rotation(-Math::Vector3(surf.params[3], surf.params[4], surf.params[5]));
            if (order)
                coord.apply_translation(Math::Vector3(surf.params[1], surf.params[2], 0.0) * unit_factor);
            continue;
        }

        case zs_none:
            ZMX_WARN("surface has unknown type");
            continue;

        case zs_standard:
            if (surf.roc == 0.0)
                curve.reset(&flat);
            else if (surf.coni == 0.0)
                curve.reset(new TSphere(unit_factor * surf.roc));
            else if (surf.coni == -1.0)
                curve.reset(new TParabola(unit_factor * surf.roc));
            else
                curve.reset(new TConic(unit_factor * surf.roc, surf.coni));
            break;
        }

        QSharedPointer<TShapeBase> shape = get_ap_shape(surf, unit_factor);

        QSharedPointer<TElement> element;

        if (i == surf_array.size() - 1)
        {
            element.reset(new TImage(Math::vector3_0, curve, shape));
        }
        else if (surf.gl_type == zg_air && surf_array[i-1].gl_type == zg_air)
        {

            QSharedPointer<TSurface> s(new TOpticalSurface(Math::vector3_0,
                                                           curve,
                                                           shape,
                                                           QSharedPointer<TGlass>(new TGlass()),
                                                           QSharedPointer<TGlass>(new TGlass())));
            s->set_enable_state(false);

            element = s;
        }
        else
        {
            TGlass m = get_glass(*sys, surf);
            QSharedPointer<TGlass> mat(&m);


            element.reset(new TOpticalSurface(Math::vector3_0,
                                              curve,
                                              shape,
                                              QSharedPointer<TGlass>(&last_mat),
                                              mat));

            if (surf.gl_type != zg_mirror)
                last_mat = *mat;
        }

        element->set_transform(coord);

        sys->add(element);

        coord.apply_translation(Math::Vector3(0.0, 0.0, unit_factor * surf.thick));
    }

    return sys;
}


////////////////////////////////////////////////////////////////////////
// Glass catalog import
////////////////////////////////////////////////////////////////////////

TCatalog& TImportZemax::import_catalog(const std::string &name)
{
    std::string filename(_cat_path);

    // FIXME ignore filename case

    filename += PATH_SEPARATOR;
    filename += name;
    filename += ".AGF";

    return import_catalog(filename, name);
}

TCatalog& TImportZemax::import_catalog_file(const std::string &filename)
{
    std::string name(basename(filename));

    return import_catalog(filename, name);
}

TCatalog& TImportZemax::import_catalog(const std::string &filename,
                                       const std::string &catname)
{
    std::ifstream file(filename.c_str());
    std::string line;

    if (!file)
        throw Error("Unable to open file");

    QSharedPointer<TCatalog> cat(new TCatalog(catname));

    // FIXME check already loaded catalog

    TDielectricBase* mat;

    while (std::getline(file, line))
    {
        int formula;

        if (line.size() < 2)
            continue;

        const char *buf = line.c_str();

        // actually ANSI takes 'char' type as 'int' in memory--LQ add
#define AGF_TYPE(a, b) ((a) + ((b) << 8))

        int type = AGF_TYPE(buf[0], buf[1]);

        switch(type)
        {

        ////////////////////////////////////////////////////
        // New Material line

        case (AGF_TYPE('N', 'M')): {
            char name[32];

            mat = 0;

            if (sscanf(buf, "NM %s %u", name, &formula) != 2)
                break;

            switch (formula)
            {
            case (1):       // Schott
                mat = new TSchott;
                break;
            case (10):      // Extended
                mat = new TExtended;
                break;
            case (3):       // Herzberger
                mat = new THerzberger;
                break;
            case (5):       // Conrady
                mat = new TConrady;
                break;
            case (7):       // Hanbook 1
                mat = new THandbook1;
                break;
            case (8):       // Hanbook 2
                mat = new THandbook2;
                break;
            case (4):       // Sellmeier 2
                mat = new TSellmeier2;
                break;
            case (2):       // Sellmeier 1
                mat = new TSellmeier1;
                break;
            case (6):       // Sellmeier 3
                mat = new TSellmeier3;
                break;
            case (9):       // Sellmeier 4
                mat = new TSellmeier4;
                break;
            case (11):      // Sellmeier 5
                mat = new TSellmeier5;
                break;
            }

            if (mat)
            {
//                mat->set_measurement_medium(Material::air);
                cat->add_material(name, QSharedPointer<TGlass>(new TGlass(*mat)));
            }

            break;
        } // case "NM"

            ////////////////////////////////////////////////////
            // Coefficient Data line

        case (AGF_TYPE('C', 'D')): {
            double dispCoeff[MAX_DISP_COEFF_COUNT];

            if (0 == mat)
                break;

            if (sscanf(buf, "CD %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
                       &dispCoeff[0],
                       &dispCoeff[1],
                       &dispCoeff[2],
                       &dispCoeff[3],
                       &dispCoeff[4],
                       &dispCoeff[5],
                       &dispCoeff[6],
                       &dispCoeff[7],
                       &dispCoeff[8],
                       &dispCoeff[9]) < 3)
                break;

            mat->setDispCoeff(dispCoeff, 10);
        }  // case "CD"

            ////////////////////////////////////////////////////
            // Thermal Data line

        case (AGF_TYPE('T', 'D')): {
            double d0, d1, d2, e0, e1, ltk, reftemp;

            if (0 == mat)
                break;

            if (sscanf(buf, "TD %lf %lf %lf %lf %lf %lf %lf",
                       &d0, &d1, &d2, &e0, &e1, &ltk, &reftemp) != 7)
                break;

            mat->setTempCoeff(d0, d1, d2, e0, e1, ltk);

//            // Zemax glasses are measured in air medium
//            ref<Material::AirKohlrausch68> air =
//                    ref<Material::AirKohlrausch68>::create();

            mat->set_temperature(reftemp);
//            mat->set_measurement_medium(*air);

            break;
        } // case "TD"

            ////////////////////////////////////////////////////
            // Internal Transmition line

        case (AGF_TYPE('I', 'T')): {
            double wl, it, th;

            if (0 == mat)
                break;

            if (sscanf(buf, "IT %lf %lf %lf", &wl, &it, &th) != 3)
                break;

            mat->insertAbsorbedCoeffi(wl * 1000.0, th, it);
            break;
        }

            ////////////////////////////////////////////////////
            // Extra Data line

        case (AGF_TYPE('E', 'D')): {
            double tce, density;

            if (0 == mat)
                break;

            if (sscanf(buf, "ED %lf %*f %lf", &tce, &density) != 2)
                break;

//            mat->set_thermal_expansion(tce * 1e-6);
//            mat->set_density(density);
            break;
        }

            ////////////////////////////////////////////////////
            // Limit Data line

        case (AGF_TYPE('L', 'D')): {
            double low, high;

            if (0 == mat)
                break;

            if (sscanf(buf, "LD %lf %lf", &low, &high) != 2)
                break;

            mat->setWave(low * 1000.0, high * 1000.0);
            break;
        }

        }

    }

    _cat_list.insert(cat_map_t::value_type(catname, cat));

    return *cat;
}

TCatalog& TImportZemax::get_catalog(const std::string &catalogname)
{
    cat_map_t::iterator i = _cat_list.find(catalogname);

    if (i == _cat_list.end())
        throw Error("no such catalog loaded");

    return *i->second;
}


////////////////////////////////////////////////////////////////////////
// Table glass import
////////////////////////////////////////////////////////////////////////

//TDielectricBase& TImportZemax::import_table_glass(const std::string &filename)
//{
//    std::ifstream file(filename.c_str());
//    std::string line;

//    if (!file)
//        throw Error("Unable to open file");

//    ref<Material::DispersionTable> mat = GOPTICAL_REFNEW(Material::DispersionTable);

//    while (std::getline(file, line))
//    {
//        double wl, index, trans, thick;
//        const char *buf = line.c_str();

//        switch (sscanf(buf, "%lf %lf %lf %lf", &wl, &index, &trans, &thick))
//        {
//        case 0: {
//            double density;

//            if (sscanf(buf, "DENSITY %lf", &density) == 1)
//                mat->set_density(density);
//            break;
//        }

//        case 4:
//            mat->set_internal_transmittance(wl * 1000.0, thick, trans);
//        case 2:
//        case 3:
//            mat->set_refractive_index(wl * 1000.0, index);
//        default:
//            break;
//        }
//    }

//    return mat;
//}

void TImportZemax::set_catalog_path(const std::string &path)
{
    _cat_path = path;
}



