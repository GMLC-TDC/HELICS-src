#!/bin/bash
# This uses bash variable substitution in a few places
# 1. replacing x86 with Win32 (setting the Python version uses x86)
# 2. getting the cmake directory for running cpack with an absolute path (chocolatey has an unfortunately named alias)

echo "Building shared library with ${BUILD_GEN} for ${BUILD_ARCH}"
choco install -y swig

# Fix cpack command (interferes with chocolatey)
cpack_dir="$(command -v cmake)"
cpack_dir="${cpack_dir%/cmake}"

# Install Boost
BOOST_ROOT="/c/boost"
BOOST_URL="https://sourceforge.net/projects/boost/files/boost/1.74.0/boost_1_74_0.tar.bz2/download"
cd $(mktemp -d)
curl --location --output "download.tar.bz2" "$BOOST_URL"
tar xfvj "download.tar.bz2"
mkdir -p "$BOOST_ROOT"
cp -r boost_*/* "$BOOST_ROOT"
export BOOST_ROOT

# Build
mkdir build && cd build || exit
cmake -G "${BUILD_GEN}" -A "${BUILD_ARCH/x86/Win32}" -DCMAKE_BUILD_TYPE=Release -DHELICS_ENABLE_PACKAGE_BUILD=ON -DSTATIC_STANDARD_LIB=static -DHELICS_USE_ZMQ_STATIC_LIBRARY=ON -DHELICS_BUILD_APP_EXECUTABLES=OFF -DHELICS_BUILD_APP_LIBRARY=OFF -DHELICS_BUILD_EXAMPLES=OFF -DBUILD_TESTING=OFF ..
cmake --build . --config Release
cpack_dir="$(command -v cmake)"
cpack_dir="${cpack_dir%/cmake}"
"${cpack_dir}/cpack" -G "TGZ" -C Release -B "$(pwd)/../artifact"
cd ../artifact || exit
rm -rf _CPack_Packages
ARCHIVE_FILE="$(ls Helics-*)"
mv "$ARCHIVE_FILE" "${ARCHIVE_FILE/Helics-/Helics-shared-}"
