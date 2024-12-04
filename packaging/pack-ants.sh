#! /bin/bash

cd ${GIT_PATH}

if [ -d ANTs ];
then
  cd ANTs
  git pull
  cd ..
else
  git clone https://github.com/ANTsX/ANTs.git
fi

# Compilation and creation of the Debian package

[ -d build-ANTs ] && rm -rf build-ANTs
mkdir build-ANTs
cd build-ANTs
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF -DRUN_LONG_TESTS=OFF \
      -DRUN_SHORT_TESTS=OFF -DCPACK_BINARY_DEB=ON -DCPACK_BINARY_STGZ=OFF \
      -DCPACK_BINARY_TGZ=OFF -DCPACK_BINARY_TZ=OFF \
      -DCMAKE_INSTALL_PREFIX=/usr ../ANTs
make -j$(nproc --all)
cmake -DCMAKE_BUILD_TYPE=Release -DANTS_SUPERBUILD=OFF -DBUILD_TESTING=OFF \
      -DRUN_LONG_TESTS=OFF -DRUN_SHORT_TESTS=OFF -DCPACK_BINARY_DEB=ON \
      -DCPACK_BINARY_STGZ=OFF -DCPACK_BINARY_TGZ=OFF -DCPACK_BINARY_TZ=OFF \
      -DCPACK_PACKAGE_CONTACT="Fabrice Poupon <fabrice.poupon@cea.fr>" \
      -DCMAKE_INSTALL_PREFIX=/usr ../ANTs
make -j$(nproc --all) package

if [ ! -d ${HOME}/Distributions/${SYS_OS}/gkg-1.0/ThirdPart ];
then
  mkdir -p ${HOME}/Distributions/${SYS_OS}/gkg-1.0/ThirdPart
fi

mv *.deb ${HOME}/Distributions/${SYS_OS}/gkg-1.0/ThirdPart/
cd ..

