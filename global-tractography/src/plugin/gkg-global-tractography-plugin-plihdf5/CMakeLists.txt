################################################################################
# HEADERS
################################################################################

set( HEADERS
PLIOdfSiteMapDiskFormat.h
PLIOdfTextureMapDiskFormat.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
PLIOdfSiteMapDiskFormat.cxx
PLIOdfTextureMapDiskFormat.cxx
)


################################################################################
# Compilation and link
################################################################################

# temporary add for HDF5
include_directories( ${HDF5_INCLUDE_DIRS} )

# temporary add for PLIHDF5
include_directories( ${PLIHDF5_INCLUDE_DIR} )

add_library( gkg-global-tractography-plugin-plihdf5 SHARED ${SOURCES} )
set_property( TARGET gkg-global-tractography-plugin-plihdf5 PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-global-tractography-plugin-plihdf5
                       gkg-global-tractography 
                       ${PLIHDF5_LIBRARIES} )


################################################################################
# Installation
################################################################################

install( FILES ${HEADERS}
         DESTINATION include/${PROJECT_NAME}/gkg-global-tractography-plugin-plihdf5
         COMPONENT devel )
install( TARGETS gkg-global-tractography-plugin-plihdf5
         DESTINATION lib
         COMPONENT runtime )
