cmake_minimum_required( VERSION 3.1 )
cmake_policy(SET CMP0074 NEW)


###############################################################################
# PROJECT
###############################################################################

project( gkg )

set( ${PROJECT_NAME}_VERSION_MAJOR 2 )
set( ${PROJECT_NAME}_VERSION_MINOR 0 )
set( ${PROJECT_NAME}_VERSION_PATCH 0 )
set( ${PROJECT_NAME}_VERSION
     "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}" )

# something has changed somewhere, the ELF format is not recognized by default
# any longer and makes install fail because of a rpath/relink step missing.
# The solution is to force ELF format
# see https://cmake.org/Bug/view.php?id=13934#c37157
set( CMAKE_EXECUTABLE_FORMAT "ELF" )

set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


###############################################################################
# OPTIONS
###############################################################################

option( GINKGO_BUILD_DARTEL "Build DARTEL component" OFF )
option( GINKGO_BUILD_REALTIME_MRI "Build realtime component" OFF )
option( GINKGO_USE_NUITKA "Compile python code with Nuitka" OFF )
option( GINKGO_USE_TORCH "Compile with Torch" OFF )
option( GINKGO_TORCH_CUDA_IMPLEMENTATION "Compile with CUDA version of Torch" OFF )
option( GINKGO_BUILD_NEW_GLOBAL_TRACTOGRAPHY "Compile new version of global tractography" OFF )


###############################################################################
# BUILD PLATFORM
###############################################################################

find_program( LSB_RELEASE_EXEC lsb_release )

if( LSB_RELEASE_EXEC )
  execute_process( COMMAND ${LSB_RELEASE_EXEC} -is 
                   OUTPUT_VARIABLE LSB_RELEASE_ID_SHORT
                   OUTPUT_STRIP_TRAILING_WHITESPACE )
  execute_process( COMMAND ${LSB_RELEASE_EXEC} -rs 
                   OUTPUT_VARIABLE LSB_RELEASE_NUMBER
                   OUTPUT_STRIP_TRAILING_WHITESPACE )
  message( STATUS "Build platform : ${LSB_RELEASE_ID_SHORT}-${LSB_RELEASE_NUMBER}" )
  if( LSB_RELEASE_ID_SHORT STREQUAL "Ubuntu" )
    execute_process( COMMAND bash -c "dpkg --print-architecture"
                     COMMAND tr -d '\n' OUTPUT_VARIABLE outArch )
    if( LSB_RELEASE_NUMBER VERSION_GREATER 17.10 )
      set( DESIRED_QT_VERSION 5 )
      message( STATUS "Desired Qt version : ${DESIRED_QT_VERSION}")
    endif( LSB_RELEASE_NUMBER VERSION_GREATER 17.10 )
  else( LSB_RELEASE_ID_SHORT STREQUAL "Ubuntu" )
    set( outArch "amd64" )
  endif( LSB_RELEASE_ID_SHORT STREQUAL "Ubuntu" )
else( LSB_RELEASE_EXEC )
  set( LSB_RELEASE_ID_SHORT "Generic" )
  set( outArch "amd64" )
endif( LSB_RELEASE_EXEC )

message( STATUS "Build architecture : ${outArch}" )


###############################################################################
# DEPENDENCIES
###############################################################################

set( CMAKE_MODULE_PATH
     /usr/share/pio/cmake/Modules
     ${CMAKE_SOURCE_DIR}/cmake/Modules
     ${CMAKE_MODULE_PATH} )

#     /usr/share/kokkos/cmake/Modules
#     /usr/lib/cmake/Kokkos

if ( GINKGO_USE_TORCH )

if ( GINKGO_TORCH_CUDA_IMPLEMENTATION )

set( CMAKE_PREFIX_PATH
     /usr/local/libtorch-gpu/share/cmake/Torch
     ${CMAKE_PREFIX_PATH} )

else( GINKGO_TORCH_CUDA_IMPLEMENTATION )

set( CMAKE_PREFIX_PATH
     /usr/local/libtorch-cpu/share/cmake/Torch
     ${CMAKE_PREFIX_PATH} )

endif( GINKGO_TORCH_CUDA_IMPLEMENTATION )

endif ( GINKGO_USE_TORCH )


if( EXISTS /opt/nvidia/hpc_sdk AND IS_DIRECTORY /opt/nvidia/hpc_sdk )
  message( STATUS "nVidia HPC SDK found" )
  find_program( UNAME_EXEC uname )
  if( UNAME_EXEC )
    execute_process( COMMAND ${UNAME_EXEC} -s 
                     OUTPUT_VARIABLE UNAME_KERNEL
                     OUTPUT_STRIP_TRAILING_WHITESPACE )
    execute_process( COMMAND ${UNAME_EXEC} -m 
                     OUTPUT_VARIABLE UNAME_MACHINE
                     OUTPUT_STRIP_TRAILING_WHITESPACE )
    set( nvArch "${UNAME_KERNEL}_${UNAME_MACHINE}" )
  else( UNAME_EXEC )
    set( nvArch "Generic" )
  endif( UNAME_EXEC )
  file( GLOB sdkDir /opt/nvidia/hpc_sdk/${nvArch}/[0-9]*.[0-9]* )
  set( cudaDir "${sdkDir}/cuda" )
  if( EXISTS ${cudaDir} AND IS_DIRECTORY ${cudaDir} )
    set( CUDA_TOOLKIT_ROOT_DIR ${cudaDir} )
  endif()
endif()

# Should be set to either "LEGACY" or "GLVND"
set( OpenGL_GL_PREFERENCE "LEGACY" )

find_package( Metis QUIET )
find_package( Threads REQUIRED )
find_package( Boost REQUIRED COMPONENTS log log_setup )
find_package( JPEG REQUIRED )

set( HDF5_PREFER_PARALLEL ON )
find_package( HDF5 REQUIRED COMPONENTS C HL )
include_directories( ${HDF5_INCLUDE_DIRS} )
if( NOT HDF5_IS_PARALLEL )
  message( FATAL_ERROR "HDF5 is not parallel" )
endif()

find_package( BLAS QUIET )
find_package( LAPACK QUIET )
find_package( GSL REQUIRED )
find_package( X11 QUIET )
find_package( CUDA REQUIRED )
find_package( PIO QUIET )
find_package( DCMTK QUIET )
find_package( SVM QUIET )
if( DESIRED_QT_VERSION EQUAL 5 )
  find_package( Qt5Core QUIET )
  find_package( Qt5Widgets QUIET )
  find_package( Qt5OpenGL QUIET )
  find_package( Qt5UiTools QUIET )
  if( Qt5Core_FOUND AND Qt5Widgets_FOUND AND Qt5OpenGL_FOUND AND Qt5UiTools_FOUND )
    set( QT_FOUND TRUE )
  endif( Qt5Core_FOUND AND Qt5Widgets_FOUND AND Qt5OpenGL_FOUND AND Qt5UiTools_FOUND )
else( DESIRED_QT_VERSION EQUAL 5 )
  find_package( Qt QUIET )
endif( DESIRED_QT_VERSION EQUAL 5 )
find_package( OpenGL QUIET )
find_package( Qwt QUIET )
find_package( Sigc++2 QUIET )
find_package( Brainvisa QUIET )
find_package( VTK QUIET )
find_package( MPI QUIET )
find_package( Wt QUIET )
find_package( OpenMP QUIET )
find_package( Kokkos REQUIRED )
find_package( OpenACC QUIET )
find_package( FFTW3 QUIET )
find_package( ISMRMRD QUIET )
if ( NOT CGAL_DATA_DIR )

  set( CGAL_DATA_DIR "/tmp" )

endif ( NOT CGAL_DATA_DIR )
find_package( CGAL QUIET )
find_package( PLIHDF5 QUIET )


if ( GINKGO_USE_TORCH )

set( CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE )
find_package( Torch QUIET )
set( CMAKE_FIND_PACKAGE_PREFER_CONFIG FALSE )

endif ( GINKGO_USE_TORCH )


if( ${CMAKE_VERSION} VERSION_LESS "3.12.0" )
  find_package( PythonInterp )
else()
  find_package( Python COMPONENTS Interpreter )
  set( PYTHON_EXECUTABLE ${Python_EXECUTABLE} )
endif()
message( STATUS "Python command : ${PYTHON_EXECUTABLE}" )

if ( CUDA_FOUND )
  message( STATUS "CUDA found")
endif ( CUDA_FOUND )


if ( GINKGO_USE_TORCH )

if ( TORCH_FOUND )
  message( STATUS "Torch found")
endif ( TORCH_FOUND )

endif ( GINKGO_USE_TORCH )


if ( OpenACC_C_FOUND OR OpenACC_CXX_FOUND )
  message( STATUS "OpenACC found")
endif ( OpenACC_C_FOUND OR OpenACC_CXX_FOUND )

unset( NUITKA CACHE )
if( GINKGO_USE_NUITKA )
  find_program( NUITKA nuitka )
  if( NUITKA )
    message( STATUS "nuitka found" )
  endif( NUITKA )
endif( GINKGO_USE_NUITKA )


set( CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE )
find_package( NLopt QUIET )
set( CMAKE_FIND_PACKAGE_PREFER_CONFIG FALSE )



###############################################################################
# DEFAULT INSTALL DIRECTORY
###############################################################################

set( CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" )


###############################################################################
# MACROS AND FUNCTIONS
###############################################################################

macro( GET_DIRECTORIES output_list input_list )
  set( args "${ARGN}" )
  set( exclude_list )
  list( FIND args EXCLUDE result )
  if( NOT result EQUAL -1 )
    list( REMOVE_AT args ${result} )
    list( LENGTH args len )
    while( result LESS len )
      list( GET args ${result} exclude )
      list( REMOVE_AT args ${result} )
      list( LENGTH args len )
      list( APPEND exclude_list ${exclude} )
    endwhile()
  endif()
  foreach( f ${${input_list}} )
    get_filename_component( dir_path ${f} PATH )
    string( FIND ${dir_path} "/" firstDir )
    if( firstDir GREATER 0 )
      string( SUBSTRING ${dir_path} 0 ${firstDir} dir_path )
    endif()
    list( APPEND dir_list ${dir_path} )
  endforeach()
  list( REMOVE_DUPLICATES dir_list )
  if( exclude_list )
    list( REMOVE_ITEM dir_list ${exclude_list} )
  endif()
  set( ${output_list} ${dir_list} )
endmacro()

macro( ADD_NUITKA output_file src_file )
  get_filename_component( PYFILE ${src_file} NAME_WE )
  set( args "${ARGN}" )
  list( FIND args OUTPUTNAME result )
  if( result EQUAL -1 )
    set( out_name ${PYFILE} )
  else()
    list( REMOVE_AT args ${result} )
    list( GET args ${result} out_name )
    list( REMOVE_AT args ${result} )
  endif()
  set( exclude_list )
  list( FIND args EXCLUDE result )
  if( NOT result EQUAL -1 )
    list( REMOVE_AT args ${result} )
    list( LENGTH args len )
    while( result LESS len )
      list( GET args ${result} exclude )
      list( REMOVE_AT args ${result} )
      list( LENGTH args len )
      list( APPEND exclude_list ${exclude} )
    endwhile()
  endif()
  set( depend_list )
  list( FIND args DEPENDS result )
  if( NOT result EQUAL -1 )
    list( REMOVE_AT args ${result} )
    list( LENGTH args len )
    while( result LESS len )
      list( GET args ${result} dep )
      list( REMOVE_AT args ${result} )
      list( LENGTH args len )
      list( APPEND depend_list ${dep} )
    endwhile()
  endif()
  set( ${output_file} ${out_name} )
  set( TARGET_EXE ${CMAKE_BINARY_DIR}/nuitka/${out_name} )
  set( TARGET_DIR ${CMAKE_BINARY_DIR}/nuitka/${PYFILE}.dir )
  set( RECURSE_TO "" )
  set( RECURSE_NOT_TO "" )
  file( MAKE_DIRECTORY ${TARGET_DIR} )
  file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/${src_file} DESTINATION ${TARGET_DIR} )
  if( depend_list )
    file( COPY ${depend_list} DESTINATION ${TARGET_DIR} )
    foreach( d ${depend_list} )
      get_filename_component( dep ${d} NAME_WE )
      string( APPEND RECURSE_TO "--recurse-to=${dep} " )
    endforeach()
  endif()
  if( exclude_list )
    foreach( e ${exclude_list} )
      get_filename_component( exclude ${e} NAME_WE )
      string( APPEND RECURSE_NOT_TO "--recurse-not-to=${exclude} " )
    endforeach()
  endif()
  if( WIN32 )
    separate_arguments( ARGUMENTS WINDOWS_COMMAND "${RECURSE_TO} ${RECURSE_NOT_TO}" )
  else( WIN32 )
    separate_arguments( ARGUMENTS UNIX_COMMAND "${RECURSE_TO} ${RECURSE_NOT_TO}" )
  endif( WIN32 )
  add_custom_command( OUTPUT ${TARGET_EXE}
                      COMMAND ${NUITKA}
                              ${ARGUMENTS}
  		              --output-dir=${TARGET_DIR}
  		              --remove-output
  		              ${TARGET_DIR}/${src_file}
                      COMMAND ${CMAKE_COMMAND} -E copy                    
                              ${TARGET_DIR}/${PYFILE}.bin
                              ${TARGET_EXE}
                      DEPENDS ${src_file} ${depend_list} )
  add_custom_target( ${PYFILE} ALL
                     DEPENDS ${TARGET_EXE}
                     VERBATIM )
endmacro()


###############################################################################
# COMPILATION FLAGS
###############################################################################

# -fpermissive added when using nvcc to remove error stemming from CGAL & boost
set( CMAKE_CXX_FLAGS
     "${CMAKE_CXX_FLAGS} -W -Wall -Wpointer-arith -Wformat -pipe ${OpenMP_CXX_FLAGS} -fpermissive" )


if ( MPI_FOUND )

set( CMAKE_CXX_FLAGS
     "${CMAKE_CXX_FLAGS} ${MPI_COMPILE_FLAGS}" )

endif ( MPI_FOUND )

if ( GINKGO_USE_TORCH )

if ( TORCH_FOUND )

set( CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}" )

endif ( TORCH_FOUND )

endif ( GINKGO_USE_TORCH )


###############################################################################
# SETTINGS
###############################################################################

if( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR 
    "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" OR 
    "${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel" )
  set( GKG_DEFINITIONS "-DGKG_RELEASE" )
else( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR  
      "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" OR 
      "${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel" )
  if( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
    set( GKG_DEFINITIONS "-DGKG_DEBUG" )
  else( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
    set( GKG_DEFINITIONS "-DGKG_DEFAULT" )
  endif( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
endif( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR 
       "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" OR 
       "${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel" )

set( GKG_DEFINITIONS "${GKG_DEFINITIONS}" 
     "-DGKG_VERSION=\"${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}\"" )
set( GKG_DEFINITIONS "${GKG_DEFINITIONS}"
     "-DGKG_VERSION_AND_PATCH=\"${${PROJECT_NAME}_VERSION}\"" )

add_definitions( -D_XOPEN_SOURCE=500 -D_POSIX_SOURCE
                 -D_POSIX_C_SOURCE=199506 -D_REENTRANT
                 -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS
                 -D__STDC_FORMAT_MACROS
                 -D_LARGEFILE_SOURCE -D_LARGE_FILES
                 -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
                 -D__USE_LARGEFILE64 -D__USE_FILE_OFFSET64
                 -D_GLIBCXX_USE_CXX11_ABI=1
                 ${GKG_DEFINITIONS} )

# ignore BOOST deprecated headers
add_definitions("-DBOOST_ALLOW_DEPRECATED_HEADERS")
add_definitions("-DBOOST_BIND_GLOBAL_PLACEHOLDERS")


###############################################################################
# PLUGINS
###############################################################################

if ( GSL_FOUND AND LAPACK_FOUND AND BLAS_FOUND )
  set( BUILD_GINKGO_GSL_LAPACK_PLUGIN TRUE )
endif ( GSL_FOUND AND LAPACK_FOUND AND BLAS_FOUND )

if( PIO_FOUND )
  set( BUILD_GINKGO_NIFTI_PLUGIN TRUE )
  if( DCMTK_FOUND )
    set( BUILD_GINKGO_DICOM_PLUGIN TRUE )
  endif( DCMTK_FOUND )
endif( PIO_FOUND  )

if ( SVM_FOUND )
  set( BUILD_GINKGO_SVM_PLUGIN TRUE )
endif ( SVM_FOUND )

if ( VTK_FOUND )
  set( BUILD_GINKGO_VTK_PLUGIN TRUE )
endif ( VTK_FOUND )

if ( CGAL_FOUND ) 

  find_package(Eigen3 3.1.0)
  include( CGAL_Eigen3_support )

  # deprecated with CGAL 5.4
  #if (EIGEN3_FOUND)
  #  include( ${EIGEN3_USE_FILE} )
  #endif()

  set( BUILD_GINKGO_CGAL_PLUGIN TRUE )

endif ( CGAL_FOUND )

if ( PLIHDF5_FOUND )
  set( BUILD_GINKGO_PLIHDF5_PLUGIN TRUE )
endif ( PLIHDF5_FOUND )


###############################################################################
# COMPONENTS
###############################################################################

set( COMPONENTS 
core
communication
processing
graphics
pygkg
mri-reconstruction
qmri
deep-nuclei
fmri
dmri
)

if( GINKGO_BUILD_DARTEL )
  list( APPEND COMPONENTS dartel )
endif( GINKGO_BUILD_DARTEL )


if ( Kokkos_FOUND )
  list( APPEND COMPONENTS simulation )
  set( BUILD_GINKGO_SIMULATION_PLUGIN TRUE )
endif( Kokkos_FOUND )


if( MPI_FOUND )
#  SET( CMAKE_CXX_COMPILER mpic++ )
#  SET( CMAKE_LINKER mpic++ )
  include_directories( ${MPI_INCLUDE_PATH} )
endif()

if( GINKGO_BUILD_NEW_GLOBAL_TRACTOGRAPHY AND MPI_FOUND AND GSL_FOUND )
  list( APPEND COMPONENTS global-tractography )
endif()


if ( Sigc++2_FOUND AND QT_FOUND AND QWT_FOUND AND OPENGL_FOUND AND BRAINVISA_FOUND )
  list( APPEND COMPONENTS anatomist-plugin )
endif( Sigc++2_FOUND AND QT_FOUND AND QWT_FOUND AND OPENGL_FOUND AND BRAINVISA_FOUND )



foreach( dir ${COMPONENTS} )
  list( APPEND COMPONENT_INCLUDE_DIRS 
        "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/src/library" )
  add_subdirectory( ${dir} )
endforeach()


###############################################################################
# INSTALLATION
###############################################################################

add_subdirectory( python )
add_subdirectory( packaging )

install( DIRECTORY cmake DESTINATION share/${PROJECT_NAME} COMPONENT devel )


###############################################################################
# DEBIAN OR RPM OR STGZ PACKAGING
###############################################################################

include( InstallRequiredSystemLibraries )

set( CPACK_RESOURCE_FILE_LICENSE "${CMAKE_BINARY_DIR}/packaging/COPYRIGHT" )
set( CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}" )
set( CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}" )
set( CPACK_PACKAGE_VERSION_PATCH "${${PROJECT_NAME}_VERSION_PATCH}" )

if ( LSB_RELEASE_ID_SHORT STREQUAL "Ubuntu" )
  set( CPACK_GENERATOR "DEB" )
  set( OS_NAME "" )
  set( CPACK_DEB_COMPONENT_INSTALL ON )
  #set( CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
  set( CPACK_DEBIAN_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}" )
  set( CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${outArch}" )
  set( CPACK_DEBIAN_PACKAGE_NAME "${PROJECT_NAME}" )
  set( CPACK_DEBIAN_PACKAGE_MAINTAINER "Cyril & Fabrice Poupon <cyril.poupon@cea.fr>" )
  set( CPACK_DEBIAN_PACKAGE_DESCRIPTION "MR image reconstruction and processing library" )
  set( CPACK_DEBIAN_DEVEL_PACKAGE_DEPENDS "${PROJECT_NAME}" )
  set( CPACK_DEBIAN_DEVEL_PACKAGE_NAME "${PROJECT_NAME}-dev" )
  set( CPACK_DEBIAN_RUNTIME_PACKAGE_NAME "${PROJECT_NAME}" )
else( LSB_RELEASE_ID_SHORT STREQUAL "Ubuntu" )
  if ( LSB_RELEASE_ID_SHORT STREQUAL "CentOS" )
    set( CPACK_GENERATOR "RPM" )
    set( OS_NAME "" )
    set( CPACK_RPM_COMPONENT_INSTALL ON )
    set( CPACK_RPM_PACKAGE_RELOCATABLE ON )
    set( CPACK_RPM_PACKAGE_AUTOREQ 0 )
    set( CPACK_RPM_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}" )
    set( CPACK_RPM_PACKAGE_NAME "${PROJECT_NAME}" )
    set( CPACK_RPM_PACKAGE_VENDOR "Cyril & Fabrice Poupon <cyril.poupon@cea.fr>" )
    set( CPACK_RPM_PACKAGE_DESCRIPTION "MR image reconstruction and processing library" )
    set( CPACK_RPM_DEVEL_PACKAGE_DEPENDS "${PROJECT_NAME}" )
    set( CPACK_RPM_DEVEL_PACKAGE_NAME "${PROJECT_NAME}-dev" )
    set( CPACK_RPM_RUNTIME_PACKAGE_NAME "${PROJECT_NAME}" )    
  else ( LSB_RELEASE_ID_SHORT STREQUAL "CentOS" )

    if ( LSB_RELEASE_ID_SHORT STREQUAL "Rocky" )
      set( CPACK_GENERATOR "RPM" )
      set( OS_NAME "" )
      set( CPACK_RPM_COMPONENT_INSTALL ON )
      set( CPACK_RPM_PACKAGE_RELOCATABLE ON )
      set( CPACK_RPM_PACKAGE_AUTOREQ 0 )
      set( CPACK_RPM_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}" )
      set( CPACK_RPM_PACKAGE_NAME "${PROJECT_NAME}" )
      set( CPACK_RPM_PACKAGE_VENDOR "Cyril & Fabrice Poupon <cyril.poupon@cea.fr>" )
      set( CPACK_RPM_PACKAGE_DESCRIPTION "MR image reconstruction and processing library" )
      set( CPACK_RPM_DEVEL_PACKAGE_DEPENDS "${PROJECT_NAME}" )
      set( CPACK_RPM_DEVEL_PACKAGE_NAME "${PROJECT_NAME}-dev" )
      set( CPACK_RPM_RUNTIME_PACKAGE_NAME "${PROJECT_NAME}" )    
    else ( LSB_RELEASE_ID_SHORT STREQUAL "Rocky" )
      set( CPACK_GENERATOR "STGZ" )
      set( OS_NAME "_${LSB_RELEASE_ID_SHORT}" )
    endif ( LSB_RELEASE_ID_SHORT STREQUAL "Rocky" )

  endif ( LSB_RELEASE_ID_SHORT STREQUAL "CentOS" )
endif( LSB_RELEASE_ID_SHORT STREQUAL "Ubuntu" )

set( CPACK_PACKAGE_FILE_NAME 
     "${PROJECT_NAME}_${${PROJECT_NAME}_VERSION}${OS_NAME}_${outArch}" )
set( CPACK_COMPONENTS_ALL devel runtime )

include( CPack )


# Usage 
#
# Configure CMake :
#
#   $ mkdir build-gkg
#   $ cd build-gkg
#   $ cmake -DCMAKE_BUILD_TYPE=Release ../gkg
#
# or, if you want to activate the build of the Dartel component
#
#   $ cmake -DCMAKE_BUILD_TYPE=Release -DGINKGO_BUILD_DARTEL=ON ../gkg
#
# Then, two options :
#
# 1) build and install in the build directory 
#
#   $ make install
#
# 2) build and create debian or rpm packages
#
#   $ make package
#
# To check the .deb content :
#
#   $ dpkg -c package.deb
#
# DEB package installation :
#
#   $ sudo dpkg -i package.deb
#   $ sudo apt-get -f install
#
# DEB package removal :
#
#   $ sudo dpkg --purge package_name
#
# SHELL package installation:
#
# $ sudo package.sh --prefix=/usr
# -> accept the license ? y
# -> include the subdirectory package ? n
#
# RPM package installation:
#
# $ rpm -i package.rpm
#
# RPM package relocated installation :
#
# $ rpm -i --prefix=/absolute/path package.rpm
#
