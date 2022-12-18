#!/bin/sh
#
# ToDo - Add some error trapping

BUILD_DIR="build"

# Remove build dir <- Probably don't really need this, but cleaner for now
if [ -d "${BUILD_DIR}" ]
then
  echo "---------- Removing existing build directory ------------"
  rm -rf "${BUILD_DIR}"
fi

# Construct the make files
echo "---------- cmake ------------"
cmake -B "${BUILD_DIR}" -S .

# Compile
echo "---------- make ------------"
make -C "${BUILD_DIR}"/