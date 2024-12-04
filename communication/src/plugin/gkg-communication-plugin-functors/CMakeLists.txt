################################################################################
# HEADERS
################################################################################

set( HEADERS
CudaDeviceList/CudaDeviceListCommand.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
CudaDeviceList/CudaDeviceListCommand.cxx
)


################################################################################
# Compilation and link
################################################################################

add_library( gkg-communication-plugin-functors SHARED ${SOURCES} )
set_property( TARGET gkg-communication-plugin-functors 
              PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )

if ( CUDA_FOUND )

  target_link_libraries( gkg-communication-plugin-functors 
                         gkg-communication ${CUDA_LIBRARIES} )

else ( CUDA_FOUND )

  target_link_libraries( gkg-communication-plugin-functors gkg-communication )

endif ( CUDA_FOUND )


################################################################################
# Installation
################################################################################

get_directories( HDR_DIR_LIST HEADERS )
foreach( d ${HDR_DIR_LIST} )
  install( DIRECTORY ${d}
           DESTINATION include/${PROJECT_NAME}/gkg-communication-plugin-functors
           COMPONENT devel
           FILES_MATCHING PATTERN "*.h" )
endforeach()
install( TARGETS gkg-communication-plugin-functors
         DESTINATION lib
         COMPONENT runtime )
