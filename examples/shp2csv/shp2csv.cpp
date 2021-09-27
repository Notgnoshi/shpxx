#include <boost/program_options.hpp>
#include <shapefil.h>

#include <fstream>
#include <iostream>
#include <string>

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
            //! @todo Make this an optional positional argument
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

int main(int argc, const char* argv[])
{
    const auto options = options_t::parse_args(argc, argv);

    const SHPHandle shapefile = SHPOpen(options.input_filename.c_str(), "rb");

    SHPClose(shapefile);
    return 0;
}
