################################################################################
# HEADERS
################################################################################

set( HEADERS
DartelBundleCreateWarp/DartelBundleCreateWarpCommand.h
DartelCreateWarp/DartelCreateWarpCommand.h
DartelFlowFieldToVectorField/DartelFlowFieldToVectorFieldCommand.h
DartelMeshCreateWarp/DartelMeshCreateWarpCommand.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
DartelBundleCreateWarp/DartelBundleCreateWarpCommand.cxx
DartelCreateWarp/DartelCreateWarpCommand.cxx
DartelFlowFieldToVectorField/DartelFlowFieldToVectorFieldCommand.cxx
DartelMeshCreateWarp/DartelMeshCreateWarpCommand.cxx
)


################################################################################
# Compilation and link
################################################################################

add_library( gkg-dartel-plugin-functors SHARED ${SOURCES} )
set_property( TARGET gkg-dartel-plugin-functors PROPERTY VERSION ${${PROJECT_NAME}_VERSION}
 )
target_link_libraries( gkg-dartel-plugin-functors gkg-dartel )


################################################################################
# Installation
################################################################################

get_directories( HDR_DIR_LIST HEADERS )
foreach( d ${HDR_DIR_LIST} )
  install( DIRECTORY ${d}
           DESTINATION include/${PROJECT_NAME}/gkg-dartel-plugin-functors
           COMPONENT devel
           FILES_MATCHING PATTERN "*.h" )
endforeach()
install( TARGETS gkg-dartel-plugin-functors
         DESTINATION lib
         COMPONENT runtime )
