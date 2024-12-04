###############################################################################
# SETTINGS
###############################################################################

if ( CUDA_FOUND )
  set( CUDA_NVCC_FLAGS
       ${CUDA_NVCC_FLAGS};--compiler-options -fno-strict-aliasing -use_fast_math )
  set( CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};--optimize 2 )
  set( GKG_DEFINITIONS "${GKG_DEFINITIONS}" "-DGKG_CUDA" )
endif ( CUDA_FOUND )

add_definitions( ${GKG_DEFINITIONS} )


###############################################################################
# SETTING INCLUDE DIRS
###############################################################################

set( GKG_INCLUDE_DIRS 
     ${CMAKE_CURRENT_SOURCE_DIR}/src/plugin
     ${Boost_INCLUDE_DIRS} )

if( CUDA_FOUND )
  list( APPEND GKG_INCLUDE_DIRS ${CUDA_INCLUDE_DIRS} )
endif( CUDA_FOUND )

include_directories( ${COMPONENT_INCLUDE_DIRS} ${GKG_INCLUDE_DIRS} )


###############################################################################
# ADDING C++ SUB-DIRECTORIES TO BE PROCESSED
###############################################################################

add_subdirectory( src )
add_subdirectory( share )
add_subdirectory( python )
