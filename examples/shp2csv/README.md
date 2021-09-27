# shp2csv

Convert a shapefile to a CSV file containing the attributes and WKT geometries.

```bash
mkdir data && pushd data
wget http://dl.maptools.org/dl/shapelib/shape_eg_data.zip
unzip shape_eg_data.zip
popd
shopt -s globstar
for shp in ./data/**/*.shp; do
    ./build/examples/shp2csv/shp2csv -i "$shp" -o "${shp%.shp}.csv";
done
```

This tool is provided as an example of using the shapelib library to read features and attributes from a shapefile.
