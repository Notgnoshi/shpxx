#include <boost/program_options.hpp>
#include <shapefil.h>

#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

bool g_verbose = false;

struct options_t
{
  private:
    std::unique_ptr<std::ofstream> m_output_file;

  public:
    //! @brief The filename for the input .shp file.
    std::string input_filename;
    //! @brief The output stream.
    std::ostream& output = std::cout;
    //! @brief Whether to log extra information to stderr.
    bool verbose = true;

    options_t(std::string input_filename, const std::string& output_filename, bool verbose) :
        m_output_file((output_filename.empty() || output_filename == "-")
                          ? nullptr
                          : new std::ofstream(output_filename)),
        input_filename(std::move(input_filename)),
        output(m_output_file ? *m_output_file : std::cout),
        verbose(verbose)
    {
    }

    options_t() = default;
    static options_t parse_args(int argc, const char* argv[])
    {
        namespace po = boost::program_options;

        po::options_description desc("Convert shapefiles to CSV");
        // clang-format off
        desc.add_options()
            ("help,h",                                                      "Show this help message and exit")
            ("verbose,v",                                                   "Log extra information to stderr")
            //! @todo Make this a required positional argument
            ("input,i",     po::value<std::string>(),                       "Path to .SHP file, with or without .SHP extension")
            ("output,o",    po::value<std::string>()->default_value("-"),   "CSV file to output. Defaults to stdout")
        ;
        // clang-format on

        po::variables_map vm;
        try
        {
            po::store(po::parse_command_line(argc, argv, desc), vm);
            po::notify(vm);
        } catch (const po::error& e)
        {
            std::cerr << "Failed to parse commandline options: " << e.what() << std::endl;
            std::cerr << desc;
            std::exit(1);
        }

        if (vm.count("help") >= 1)
        {
            std::cout << desc;
            std::exit(0);
        }

        if (vm.count("input") <= 0)
        {
            std::cerr << "Input shapefile required!" << std::endl;
            std::cerr << desc;
            std::exit(2);
        }

        const bool verbose = vm.count("verbose") > 0;
        options_t options(vm["input"].as<std::string>(), vm["output"].as<std::string>(), verbose);
        return options;
    }
};

enum class shape_type
{
    null,

    point,
    linestring,
    polygon,
    multipoint,

    point_m,
    linestring_m,
    polygon_m,
    multipoint_m,

    point_z,
    linestring_z,
    polygon_z,
    multipoint_z,

    multipatch,

    error,
};

enum class part_type
{
    tri_strip = 0,
    tri_fan = 1,
    outer_ring = 2,
    inner_ring = 3,
    first_ring = 4,
    ring = 5,

    error,
};

part_type int2partt(int type)
{
    switch (type)
    {
    case SHPP_TRISTRIP:
        return part_type::tri_strip;
    case SHPP_TRIFAN:
        return part_type::tri_fan;
    case SHPP_OUTERRING:
        return part_type::outer_ring;
    case SHPP_INNERRING:
        return part_type::inner_ring;
    case SHPP_FIRSTRING:
        return part_type::first_ring;
    case SHPP_RING:
        return part_type::ring;
    default:
        return part_type::error;
    }
}

std::string partt2str(part_type type)
{
    switch (type)
    {
    case part_type::tri_strip:
        return "TRI_STRIP";
    case part_type::tri_fan:
        return "TRI_FAN";
    case part_type::outer_ring:
        return "OUTER_RING";
    case part_type::inner_ring:
        return "INNER_RING";
    case part_type::first_ring:
        return "FIRST_RING";
    case part_type::ring:
        return "RING";
    default:
        return "ERROR";
    }
}

shape_type int2shapet(int shp_type)
{
    switch (shp_type)
    {
    case SHPT_NULL:
        return shape_type::null;
    case SHPT_POINT:
        return shape_type::point;
    case SHPT_ARC:
        return shape_type::linestring;
    case SHPT_POLYGON:
        return shape_type::polygon;
    case SHPT_MULTIPOINT:
        return shape_type::multipoint;
    case SHPT_POINTM:
        return shape_type::point_m;
    case SHPT_ARCM:
        return shape_type::linestring_m;
    case SHPT_POLYGONM:
        return shape_type::polygon_m;
    case SHPT_MULTIPOINTM:
        return shape_type::multipoint_m;
    case SHPT_POINTZ:
        return shape_type::point_z;
    case SHPT_ARCZ:
        return shape_type::linestring_z;
    case SHPT_POLYGONZ:
        return shape_type::polygon_z;
    case SHPT_MULTIPOINTZ:
        return shape_type::multipoint_z;
    case SHPT_MULTIPATCH:
        return shape_type::multipatch;
    default:
        return shape_type::error;
    }
}

std::string shapet2str(shape_type type)
{
    switch (type)
    {
    case shape_type::null:
        return "null";
    case shape_type::point:
        return "point";
    case shape_type::linestring:
        return "linestring";
    case shape_type::polygon:
        return "polygon";
    case shape_type::multipoint:
        return "multipoint";
    case shape_type::point_m:
        return "point m";
    case shape_type::linestring_m:
        return "linestring m";
    case shape_type::polygon_m:
        return "polygon m";
    case shape_type::multipoint_m:
        return "multipoint m";
    case shape_type::point_z:
        return "point z";
    case shape_type::linestring_z:
        return "linestring z";
    case shape_type::polygon_z:
        return "polygon z";
    case shape_type::multipoint_z:
        return "multipoin z";
    case shape_type::multipatch:
        return "multipatch";
    default:
        return "error";
    }
}

void output_wkt_point(std::ostream& out, const SHPObject* point, bool has_z, bool has_m)
{
    const int parts = point->nParts;
    const int shape_vertices = point->nVertices;
    if (g_verbose)
    {
        std::cerr << "point " << point->nShapeId << " has " << parts << " parts and "
                  << shape_vertices << " vertices\n";
    }

    if (parts > 1)
        out << "MULTI";
    out << "POINT";
    if (has_z)
        out << " Z";
    if (has_m)
        out << " M";
    out << " ";

    if (parts > 1)
        out << "(";

    //! @todo part==0 case for the rest of the geometry types.
    const auto format_point = [&](int i) {
        const int part_start = (parts == 0) ? 0 : point->panPartStart[i];
        const int next_part =
            (parts == 0) ? 1 : ((i == parts - 1) ? shape_vertices : point->panPartStart[i + 1]);
        const int part_vertices = next_part - part_start;

        if (g_verbose)
        {
            std::cerr << "\tpart " << i;
            if (parts > 0)
            {
                const int type_ = point->panPartType[i];
                const part_type type = int2partt(type_);
                std::cerr << " of type " << partt2str(type) << " (" << type_ << ")";
            }
            std::cerr << " starts at " << part_start << " and goes to " << next_part << " with "
                      << part_vertices << " vertices\n";
        }

        out << "(" << point->padfX[i] << " " << point->padfY[i];
        if (has_z)
            out << " " << point->padfZ[i];
        if (has_m)
            out << " " << point->padfM[i];
        out << ")";
    };

    format_point(0);
    for (int p = 1; p < parts; ++p)
    {
        out << ", ";
        format_point(p);
    }

    if (parts > 1)
        out << ")";
}

void output_wkt_cs(std::ostream& out,
                   double* x,
                   double* y,
                   double* z,
                   double* m,
                   bool has_z,
                   bool has_m,
                   int vertices)
{
    const auto format_vertex = [&](int i) {
        out << x[i] << " " << y[i];
        if (has_z)
            out << " " << z[i];
        if (has_m)
            out << " " << m[i];
    };

    out << "(";
    if (vertices >= 0)
    {
        format_vertex(0);
    }
    for (int v = 1; v < vertices; ++v)
    {
        out << ", ";
        format_vertex(v);
    }
    out << ")";
}

void output_wkt_arc(std::ostream& out, const SHPObject* arc, bool has_z, bool has_m)
{
    const int parts = arc->nParts;
    const int shape_vertices = arc->nVertices;
    if (g_verbose)
    {
        std::cerr << "arc " << arc->nShapeId << " has " << parts << " parts and " << shape_vertices
                  << " vertices\n";
    }

    if (parts > 1)
        out << "MULTI";
    out << "LINESTRING";
    if (has_z)
        out << " Z";
    if (has_m)
        out << " M";
    out << " ";
    if (parts > 1)
        out << "(";

    const auto format_part = [&](int i) {
        const int part_start = (parts == 0) ? 0 : arc->panPartStart[i];
        const int next_part =
            (parts == 0) ? 1 : ((i == parts - 1) ? shape_vertices : arc->panPartStart[i + 1]);
        const int part_vertices = next_part - part_start;

        if (g_verbose)
        {
            std::cerr << "\tpart " << i;
            if (parts > 0)
            {
                const int type_ = arc->panPartType[i];
                const part_type type = int2partt(type_);
                std::cerr << " of type " << partt2str(type) << " (" << type_ << ")";
            }
            std::cerr << " starts at " << part_start << " and goes to " << next_part << " with "
                      << part_vertices << " vertices\n";
        }
        output_wkt_cs(out,
                      arc->padfX + part_start,
                      arc->padfY + part_start,
                      arc->padfZ + part_start,
                      arc->padfM + part_start,
                      has_z,
                      has_m,
                      part_vertices);
    };

    format_part(0);
    for (int p = 1; p < parts; ++p)
    {
        out << ", ";
        format_part(p);
    }

    if (parts > 1)
        out << ")";
}

void output_wkt_polygon(std::ostream& out, const SHPObject* polygon, bool has_z, bool has_m)
{
    const int parts = polygon->nParts;
    const int shape_vertices = polygon->nVertices;
    if (g_verbose)
    {
        std::cerr << "polygon " << polygon->nShapeId << " has " << parts << " parts and "
                  << shape_vertices << " vertices\n";
    }

    if (parts > 1)
        out << "MULTI";
    out << "POLYGON";
    if (has_z)
        out << " Z";
    if (has_m)
        out << " M";
    out << " ";

    if (parts > 1)
        out << "(";

    const auto format_part = [&](int i) {
        const int part_start = (parts == 0) ? 0 : polygon->panPartStart[i];
        const int next_part =
            (parts == 0) ? 1 : ((i == parts - 1) ? shape_vertices : polygon->panPartStart[i + 1]);
        const int part_vertices = next_part - part_start;

        if (g_verbose)
        {
            std::cerr << "\tpart " << i;
            if (parts > 0)
            {
                const int type_ = polygon->panPartType[i];
                const part_type type = int2partt(type_);
                std::cerr << " of type " << partt2str(type) << " (" << type_ << ")";
            }
            std::cerr << " starts at " << part_start << " and goes to " << next_part << " with "
                      << part_vertices << " vertices\n";
        }
        output_wkt_cs(out,
                      polygon->padfX + part_start,
                      polygon->padfY + part_start,
                      polygon->padfZ + part_start,
                      polygon->padfM + part_start,
                      has_z,
                      has_m,
                      part_vertices);
    };

    format_part(0);
    for (int p = 1; p < parts; ++p)
    {
        out << ", ";
        format_part(p);
    }

    if (parts > 1)
        out << ")";
}

void output_wkt_multipatch(std::ostream& out, const SHPObject* multipatch, bool has_z, bool has_m)
{
    const int parts = multipatch->nParts;
    const int shape_vertices = multipatch->nVertices;
    if (parts <= 0)
    {
        std::cerr << "multipatch requires multiple parts\n";
        return;
    }

    //! @todo ???
    has_z = true;

    if (g_verbose)
    {
        std::cerr << "multipatch " << multipatch->nShapeId << " has " << parts << " parts and "
                  << shape_vertices << " vertices\n";
    }

    out << "MULTIPATCH";
    if (has_z)
        out << " Z";
    if (has_m)
        out << " M";
    out << " (";

    // A _real_ shp2wkt tool would take the patches and turn them into a polygonal mesh of triangles
    // or simply a single multipolygon. For example, the following
    //
    //     MULTIPATCH (
    //         TRI_FAN (5 4 10, 0 0 5, 10 0 5, 10 8 5, 0 8 5, 0 0 5),
    //         TRI_STRIP (10 0 5, 10 0 0, 10 8 5, 10 8 0, 0 8 5, 0 8 0, 0 0 5, 0 0 0),
    //         OUTER_RING (0 0 0, 0 0 5, 10 0 5, 10 0 0, 6 0 0, 6 0 3, 4 0 3, 4 0 0, 0 0 0),
    //         INNER_RING (1 0 2, 3 0 2, 3 0 4, 1 0 4, 1 0 2),
    //         INNER_RING (7 0 2, 9 0 2, 9 0 4, 7 0 4, 7 0 2)
    //     )
    //
    // is equivalent to
    //
    //     MULTIPOLYGON Z (
    //         ((5 4 10, 0  0 5, 10 0 5, 5 4 10)),
    //         ((5 4 10, 10 0 5, 10 8 5, 5 4 10)),
    //         ((5 4 10, 10 8 5, 0  8 5, 5 4 10)),
    //         ((5 4 10, 0  8 5, 0  0 5, 5 4 10)),
    //
    //         ((10 0 5, 10 0 0, 10 8 5, 10 0 5)),
    //         ((10 0 0, 10 8 5, 10 8 0, 10 0 0)),
    //         ((10 8 5, 10 8 0, 0  8 5, 10 8 5)),
    //         ((10 8 0, 0  8 5, 0  8 0, 10 8 0)),
    //         ((0  8 5, 0  8 0, 0  0 5, 0  8 5)),
    //         ((0  8 0, 0  0 5, 0  0 0, 0  8 0)),
    //
    //         (
    //             (0 0 0, 0 0 5, 10 0 5, 10 0 0, 6 0 0, 6 0 3, 4 0 3, 4 0 0, 0 0 0),
    //             (1 0 2, 3 0 2, 3 0 4, 1 0 4, 1 0 2),
    //             (7 0 2, 9 0 2, 9 0 4, 7 0 4, 7 0 2)
    //         )
    //     )
    const auto format_patch = [&](int i) {
        const int part_start = multipatch->panPartStart[i];
        const int next_part = (i == parts - 1) ? shape_vertices : multipatch->panPartStart[i + 1];
        const int part_vertices = next_part - part_start;
        const int type_ = multipatch->panPartType[i];
        const part_type type = int2partt(type_);

        if (g_verbose)
        {
            std::cerr << "\tpart " << i << " of type " << partt2str(type) << " (" << type_ << ")"
                      << " starts at " << part_start << " and goes to " << next_part << " with "
                      << part_vertices << " vertices\n";
        }
        out << partt2str(type) << " ";
        output_wkt_cs(out,
                      multipatch->padfX + part_start,
                      multipatch->padfY + part_start,
                      multipatch->padfZ + part_start,
                      multipatch->padfM + part_start,
                      has_z,
                      has_m,
                      part_vertices);
    };

    format_patch(0);
    for (int p = 1; p < parts; ++p)
    {
        out << ", ";
        format_patch(p);
    }

    out << ")";
}

void output_wkt_shape(std::ostream& out, const SHPObject* shape, bool has_z, bool has_m)
{
    const shape_type type = int2shapet(shape->nSHPType);
    out << "\"";
    switch (type)
    {
    case shape_type::null:
        out << "NULL";
        break;
    case shape_type::point:
    case shape_type::point_m:
    case shape_type::point_z:
    case shape_type::multipoint:
    case shape_type::multipoint_m:
    case shape_type::multipoint_z:
        output_wkt_point(out, shape, has_z, has_m);
        break;
    case shape_type::linestring:
    case shape_type::linestring_m:
    case shape_type::linestring_z:
        output_wkt_arc(out, shape, has_z, has_m);
        break;
    case shape_type::polygon:
    case shape_type::polygon_m:
    case shape_type::polygon_z:
        output_wkt_polygon(out, shape, has_z, has_m);
        break;
    case shape_type::multipatch:
        output_wkt_multipatch(out, shape, has_z, has_m);
        break;
    default:
        out << "ERROR";
        break;
    }
    out << "\"";
}

void output_csv_header(std::ostream& out)
{
    out << "id,type,parts,vertices"
        << ",min_x,max_x,min_y,max_y,min_z,max_z,min_m,max_m"
        //! @todo attribute columns
        << ",geometry\n";
}

void output_csv_object(std::ostream& out, const SHPObject* object, bool has_z, bool has_m)
{
    out << object->nShapeId << "," << object->nSHPType << "," << object->nParts << ","
        << object->nVertices << "," << object->dfXMin << "," << object->dfXMax << ","
        << object->dfYMin << "," << object->dfYMax << "," << object->dfZMin << "," << object->dfZMax
        << "," << object->dfMMin << "," << object->dfMMax << ",";
    //! @todo attributes
    //! @todo escape quotes and newlines in string attributes
    output_wkt_shape(out, object, has_z, has_m);
    out << "\n";
}

int main(int argc, const char* argv[])
{
    const auto options = options_t::parse_args(argc, argv);
    g_verbose = options.verbose;
    options.output << std::setprecision(12);

    const SHPHandle shapefile = SHPOpen(options.input_filename.c_str(), "rb");
    if (shapefile == nullptr)
    {
        std::cerr << "Failed to read shapefile from '" << options.input_filename << "'\n";
        return 1;
    }

    int num_features = 0;
    int shptype = 0;
    std::array<double, 4> min_bound = {0};
    std::array<double, 4> max_bound = {0};

    //! @todo DBF
    SHPGetInfo(shapefile, &num_features, &shptype, min_bound.data(), max_bound.data());
    const shape_type type = int2shapet(shptype);
    if (type == shape_type::error)
    {
        std::cerr << "Unrecognized shape type " << shptype << "\n";
        return 1;
    }

    if (g_verbose)
    {
        // clang-format off
        std::cerr << "features: " << num_features << "\n"
                  << "shape type: " << shapet2str(type) << " (" << shptype << ")\n"
                  << "min bounds: " << min_bound[0] << ", " << min_bound[1] << ", " << min_bound[2] << ", " << min_bound[3] << "\n"
                  << "max bounds: " << max_bound[0] << ", " << max_bound[1] << ", " << max_bound[2] << ", " << max_bound[3] << "\n"
        ;
        // clang-format on
    }

    bool has_z = false;
    bool has_m = false;

    switch (type)
    {
    case shape_type::point_m:
    case shape_type::multipoint_m:
    case shape_type::linestring_m:
    case shape_type::polygon_m:
        has_m = true;
        break;

    case shape_type::point_z:
    case shape_type::linestring_z:
    case shape_type::polygon_z:
    case shape_type::multipoint_z:
        // the measure may, or may not be included in the Z types.
        has_m = min_bound[3] != max_bound[3];
        has_z = true;
        break;
    }

    output_csv_header(options.output);

    for (int feature = 0; feature < num_features; ++feature)
    {
        SHPObject* object = SHPReadObject(shapefile, feature);
        if (object == nullptr)
        {
            std::cerr << "Failed to read feature " << feature << " from shapefile\n";
            return 1;
        }
        //! @todo attributes
        output_csv_object(options.output, object, has_z, has_m);
        SHPDestroyObject(object);
    }

    SHPClose(shapefile);
    return 0;
}
