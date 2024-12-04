################################################################################
# HEADERS
################################################################################

set( HEADERS
gkg-dartel-bundle/DartelBundleTransformation.h
gkg-dartel-core/DartelCompositionContext.h
gkg-dartel-core/DartelExponentiation.h
gkg-dartel-core/DartelTransformation.h
gkg-dartel-mesh/DartelMeshTransformation.h
gkg-dartel-volume/DartelVolumeTransformation.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
gkg-dartel-bundle/DartelBundleTransformation.cxx
gkg-dartel-core/DartelCompositionContext.cxx
gkg-dartel-core/DartelExponentiation.cxx
gkg-dartel-core/DartelTransformation.cxx
gkg-dartel-mesh/DartelMeshTransformation.cxx
gkg-dartel-volume/DartelVolumeTransformation.cxx
)


################################################################################
# Compilation and link
################################################################################

add_library( gkg-dartel SHARED ${SOURCES} )
set_property( TARGET gkg-dartel PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-dartel gkg-dmri )


################################################################################
# Installation
################################################################################

get_directories( HDR_DIR_LIST HEADERS )
foreach( d ${HDR_DIR_LIST} )
  install( DIRECTORY ${d}
           DESTINATION include/${PROJECT_NAME}
           COMPONENT devel
           FILES_MATCHING PATTERN "*.h" )
endforeach()
install( TARGETS gkg-dartel DESTINATION lib COMPONENT runtime )
