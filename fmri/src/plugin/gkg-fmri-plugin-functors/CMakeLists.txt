################################################################################
# HEADERS
################################################################################

set( HEADERS
BoldAnalysis/BoldAnalysisCommand.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
BoldAnalysis/BoldAnalysisCommand.cxx
)


################################################################################
# Compilation and link
################################################################################

add_library( gkg-fmri-plugin-functors SHARED ${SOURCES} )
set_property( TARGET gkg-fmri-plugin-functors PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-fmri-plugin-functors gkg-fmri )


################################################################################
# Installation
################################################################################

get_directories( HDR_DIR_LIST HEADERS )
foreach( d ${HDR_DIR_LIST} )
  install( DIRECTORY ${d}
           DESTINATION include/${PROJECT_NAME}/gkg-fmri-plugin-functors
           COMPONENT devel
           FILES_MATCHING PATTERN "*.h" )
endforeach()
install( TARGETS gkg-fmri-plugin-functors
         DESTINATION lib
         COMPONENT runtime )
