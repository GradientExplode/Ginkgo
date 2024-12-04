################################################################################
# HEADERS
################################################################################

set( HEADERS
VtkBundleMapDiskFormat.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
VtkBundleMapDiskFormat.cxx
)


################################################################################
# Compilation and link
################################################################################

# required with vtk-7 but removed with vtk-9
#include( "${VTK_USE_FILE}" ) 

add_library( gkg-dmri-plugin-vtk SHARED ${SOURCES} )
set_property( TARGET gkg-dmri-plugin-vtk PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-dmri-plugin-vtk
                       gkg-dmri 
                       ${VTK_LIBRARIES} )


################################################################################
# Installation
################################################################################

install( FILES ${HEADERS}
         DESTINATION include/${PROJECT_NAME}/gkg-dmri-plugin-vtk
         COMPONENT devel )
install( TARGETS gkg-dmri-plugin-vtk
         DESTINATION lib
         COMPONENT runtime )
