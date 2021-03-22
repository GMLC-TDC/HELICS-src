#!/bin/bash
# This uses bash variable substitution in a few places
# 1. replacing x86 with Win32 (setting the Python version uses x86)
# 2. getting the cmake directory for running cpack with an absolute path (chocolatey has an unfortunately named alias)
# 3. moving the generated installer with a rename to add msvcYYYY to the file name

echo "Building with ${BUILD_GEN} for ${BUILD_ARCH}"
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
cmake -G "${BUILD_GEN}" -A "${BUILD_ARCH/x86/Win32}" -DCMAKE_BUILD_TYPE=Debug -DHELICS_ENABLE_PACKAGE_BUILD=ON -DHELICS_BUILD_CXX_SHARED_LIB=ON -DHELICS_BUILD_EXAMPLES=OFF -DBUILD_TESTING=OFF -DHELICS_BUILD_APP_EXECUTABLES=OFF -DHELICS_BUILD_APP_LIBRARY=ON -DHELICS_DISABLE_C_SHARED_LIB=ON ..
cmake --build . --config Debug
echo "Packing Debug"
"${cpack_dir}/cpack" -G "ZIP" -C Debug -B "$(pwd)/../tmp_dir"

pushd ../tmp_dir || exit
rm -rf _CPack_Packages
ZIP_FILE="$(ls Helics-*.zip)"
7z x "$ZIP_FILE" -y
rm "$ZIP_FILE"
popd || exit

cmake -G "${BUILD_GEN}" -A "${BUILD_ARCH/x86/Win32}" -DCMAKE_BUILD_TYPE=Release -DHELICS_ENABLE_PACKAGE_BUILD=ON -DHELICS_BUILD_CXX_SHARED_LIB=ON -DHELICS_BUILD_EXAMPLES=OFF -DBUILD_TESTING=OFF -DHELICS_BUILD_APP_EXECUTABLES=OFF -DHELICS_BUILD_APP_LIBRARY=ON -DHELICS_DISABLE_C_SHARED_LIB=ON ..
cmake --build . --config Release
echo "Packing Release"
"${cpack_dir}/cpack" -G "ZIP" -C Release -B "$(pwd)/../tmp_dir"
cd ../tmp_dir || exit
rm -rf _CPack_Packages
ZIP_FILE="$(ls Helics-*.zip)"
7z x "$ZIP_FILE" -y
rm "$ZIP_FILE"
# shellcheck disable=SC2035
# using * instead of -- * or ./* because weird things with paths have happened before with 7z on Windows
# may be okay to change it with careful testing to make sure things don't break
7z a "$ZIP_FILE" -r *
mkdir ../artifact
mv "$ZIP_FILE" "../artifact/${ZIP_FILE/-win/-${MSVC_VER}-win}"
