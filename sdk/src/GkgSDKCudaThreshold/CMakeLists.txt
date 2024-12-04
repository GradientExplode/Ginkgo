################################################################################
# TARGET NAME
################################################################################

set( TARGET_NAME GkgSDKCudaThreshold )


################################################################################
# SOURCES
################################################################################

set( HEADERS
CudaThresholding.h
)

set( SOURCES
main.cxx
)

set( CUDA_SOURCES
CudaThresholding.cu
)


################################################################################
# Compilation and link
# SDK_LIBRARY_PATH: added so that to allow the integration of a further library
################################################################################

set( SDK_LIBRARY_PATH
     ${CMAKE_SOURCE_DIR}/src/library )

add_definitions( -DGKG_CUDA -D_FORCE_INLINES )

set( CUDA_NVCC_FLAGS --compiler-options 
                     -fno-strict-aliasing 
                     -use_fast_math
                     --optimize 2 )

cuda_include_directories( ${SDK_LIBRARY_PATH} ${GKG_INCLUDE_DIR}
                          ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${SDK_LIBRARY_PATH} ${GKG_INCLUDE_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR} ${CUDA_INCLUDE_DIRS} )
   
cuda_add_executable( ${TARGET_NAME} ${SOURCES} ${CUDA_SOURCES} )

target_link_libraries( ${TARGET_NAME} ${GKG_LIBRARIES} )


################################################################################
# Installation
################################################################################

install( TARGETS ${TARGET_NAME} DESTINATION bin )
