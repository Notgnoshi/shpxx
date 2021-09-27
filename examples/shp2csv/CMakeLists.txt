add_executable(shp2csv shp2csv.cpp)

find_package(Boost REQUIRED COMPONENTS program_options)
third_party_target_link_libraries(
    shp2csv
    PRIVATE
    Boost::program_options
    shapelib::shp
)
