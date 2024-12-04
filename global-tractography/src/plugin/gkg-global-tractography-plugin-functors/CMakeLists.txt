################################################################################
# HEADERS
################################################################################

set( HEADERS
MpiTest/MpiTestCommand.h
MpiCommand/MpiLaunchCommand.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
MpiTest/MpiTestCommand.cxx
MpiLaunch/MpiLaunchCommand.cxx
)

################################################################################
# Compilation and link
################################################################################

# temporary add for HDF5
include_directories( ${HDF5_INCLUDE_DIRS} )

add_library( gkg-global-tractography-plugin-functors SHARED ${SOURCES} )
set_property( TARGET gkg-global-tractography-plugin-functors PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-global-tractography-plugin-functors gkg-global-tractography gkg-processing
                       ${HDF5_LIBRARIES} )


################################################################################
# Installation
################################################################################

get_directories( HDR_DIR_LIST HEADERS )
foreach( d ${HDR_DIR_LIST} )
  install( DIRECTORY ${d}
           DESTINATION include/${PROJECT_NAME}/gkg-global-tractography-plugin-functors
           COMPONENT devel
           FILES_MATCHING PATTERN "*.h" )
endforeach()
install( TARGETS gkg-global-tractography-plugin-functors
         DESTINATION lib
         COMPONENT runtime )
