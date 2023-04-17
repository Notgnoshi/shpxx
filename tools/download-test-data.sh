#!/bin/bash
set -o errexit
set -o pipefail
set -o nounset
set -o noclobber

REPO_DIR=$(git rev-parse --show-toplevel)

mkdir -p "$REPO_DIR/data/shape_eg_data/"
curl http://dl.maptools.org/dl/shapelib/shape_eg_data.zip --output "$REPO_DIR/data/shape_eg_data.zip"
unzip -d "$REPO_DIR/data/shape_eg_data/" "$REPO_DIR/data/shape_eg_data.zip"
