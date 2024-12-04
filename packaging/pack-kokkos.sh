#! /bin/bash

cd ${GIT_PATH}

if [ -d kokkos ];
then
  cd kokkos
  git checkout -- CMakeLists.txt cmake/kokkos_tribits.cmake
  git pull
  cd ..
else
  git clone https://github.com/kokkos/kokkos.git
fi

# Modification of the file kokkos/CMakeLists.txt

ed -s kokkos/CMakeLists.txt <<EOF
/PARENT_DIRECTORY
+1
i
set( HAS_PARENT TRUE )
.
$
a


# build a CPack driven debian installer package

find_program( LSB_RELEASE_EXEC lsb_release )
if( LSB_RELEASE_EXEC )
  execute_process( COMMAND \${LSB_RELEASE_EXEC} -is
                   OUTPUT_VARIABLE LSB_RELEASE_ID_SHORT
                   OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  message( "Build platform : \${LSB_RELEASE_ID_SHORT}" )
  if( LSB_RELEASE_ID_SHORT STREQUAL "Ubuntu" )
    execute_process( COMMAND bash -c "dpkg --print-architecture"
                     COMMAND tr -d '\n' OUTPUT_VARIABLE outArch )
  else( LSB_RELEASE_ID_SHORT STREQUAL "Ubuntu" )
    set( outArch "amd64" )
  endif( LSB_RELEASE_ID_SHORT STREQUAL "Ubuntu" )
else( LSB_RELEASE_EXEC )
  set( LSB_RELEASE_ID_SHORT "Generic" )
  set( outArch "amd64" )
endif( LSB_RELEASE_EXEC )

message( "Build architecture : \${outArch}" )

include( InstallRequiredSystemLibraries )

set( CPACK_PACKAGE_VERSION_MAJOR "\${\${PROJECT_NAME}_VERSION_MAJOR}" )
set( CPACK_PACKAGE_VERSION_MINOR "\${\${PROJECT_NAME}_VERSION_MINOR}" )
set( CPACK_PACKAGE_VERSION_PATCH "\${\${PROJECT_NAME}_VERSION_PATCH}" )

if ( LSB_RELEASE_ID_SHORT STREQUAL "Ubuntu" )
  set( CPACK_GENERATOR "DEB" )
  set( OS_NAME "" )
  set( CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
  set( CPACK_DEBIAN_PACKAGE_VERSION "\${CPACK_PACKAGE_VERSION}" )
  set( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "\${outArch}" )
  set( CPACK_DEBIAN_PACKAGE_NAME "\${PROJECT_NAME}" )
  set( CPACK_DEBIAN_PACKAGE_MAINTAINER "Fabrice Poupon <fabrice.poupon@cea.fr>" )
  set( CPACK_DEBIAN_PACKAGE_DESCRIPTION "Kokkos debian package" )
else( LSB_RELEASE_ID_SHORT STREQUAL "Ubuntu" )
  if ( LSB_RELEASE_ID_SHORT STREQUAL "CentOS" )
    set( CPACK_GENERATOR "RPM" )
    set( OS_NAME "" )
    set( CPACK_RPM_PACKAGE_RELOCATABLE ON )
    set( CPACK_RPM_PACKAGE_AUTOREQ 0 )
    set( CPACK_RPM_PACKAGE_VERSION "\${CPACK_PACKAGE_VERSION}" )
    set( CPACK_RPM_PACKAGE_NAME "\${PROJECT_NAME}" )
    set( CPACK_RPM_PACKAGE_VENDOR "Fabrice Poupon <fabrice.poupon@cea.fr>" )
    set( CPACK_RPM_PACKAGE_DESCRIPTION "Kokkos debian package" )
  else ( LSB_RELEASE_ID_SHORT STREQUAL "CentOS" )
    set( CPACK_GENERATOR "STGZ" )
    set( OS_NAME "_\${LSB_RELEASE_ID_SHORT}" )
  endif ( LSB_RELEASE_ID_SHORT STREQUAL "CentOS" )
endif( LSB_RELEASE_ID_SHORT STREQUAL "Ubuntu" )

set( CPACK_PACKAGE_FILE_NAME
     "\${PROJECT_NAME}_\${\${PROJECT_NAME}_VERSION}\${OS_NAME}_\${outArch}" )

include( CPack )
.
w
EOF

# Modification of the file kokkos/cmake/kokkos_tribits.cmake

ed -s kokkos/cmake/kokkos_tribits.cmake <<EOF
/CMAKE_INSTALL_INCLUDEDIR
s/CMAKE_INSTALL_INCLUDEDIR/KOKKOS_HEADER_DIR/
.
w
EOF

# Compilation and creation of the Debian package

[ -d build-kokkos ] && rm -rf build-kokkos
mkdir build-kokkos
cd build-kokkos
cmake -DCMAKE_BUILD_TYPE=Release -DKokkos_ENABLE_OPENMP=ON \
      -DKokkos_ARCH_HSW=ON -DCMAKE_CXX_FLAGS=-fPIC ../kokkos
make -j$(nproc --all) package

if [ ! -d ${HOME}/Distributions/${SYS_OS}/gkg-1.0/Kokkos ];
then
  mkdir -p ${HOME}/Distributions/${SYS_OS}/gkg-1.0/Kokkos
fi

mv *.deb ${HOME}/Distributions/${SYS_OS}/gkg-1.0/Kokkos/
cd ..

