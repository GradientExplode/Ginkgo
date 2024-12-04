################################################################################
# HEADERS
################################################################################

set( HEADERS
gkg-global-tractography-node/NodeTopology.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
gkg-global-tractography-node/NodeTopology.cxx
)


################################################################################
# Compilation and link
################################################################################

include_directories( ${X11_INCLUDE_DIR} ${GKG_INCLUDE_DIR} )

add_library( gkg-global-tractography SHARED ${SOURCES} )
set_property( TARGET gkg-global-tractography PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-global-tractography gkg-dmri gkg-graphics-X11 )


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
install( TARGETS gkg-global-tractography DESTINATION lib COMPONENT runtime )
