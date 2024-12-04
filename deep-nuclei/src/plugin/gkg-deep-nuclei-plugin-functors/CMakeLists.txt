################################################################################
# HEADERS
################################################################################

set( HEADERS
DeepNucleiSegmentation/DeepNucleiSegmentationCommand.h
MaxOccurrenceAtlas/MaxOccurrenceAtlasCommand.h
MidSagittalPlane/MidSagittalPlaneCommand.h
MidSagittalSurface/MidSagittalSurfaceCommand.h
OccurrenceAtlas/OccurrenceAtlasCommand.h
ProbabilisticAtlas/ProbabilisticAtlasCommand.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
DeepNucleiSegmentation/DeepNucleiSegmentationCommand.cxx
MaxOccurrenceAtlas/MaxOccurrenceAtlasCommand.cxx
MidSagittalPlane/MidSagittalPlaneCommand.cxx
MidSagittalSurface/MidSagittalSurfaceCommand.cxx
OccurrenceAtlas/OccurrenceAtlasCommand.cxx
ProbabilisticAtlas/ProbabilisticAtlasCommand.cxx
)


################################################################################
# Compilation and link
################################################################################

add_library( gkg-deep-nuclei-plugin-functors SHARED ${SOURCES} )
set_property( TARGET gkg-deep-nuclei-plugin-functors PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-deep-nuclei-plugin-functors gkg-deep-nuclei )


################################################################################
# Installation
################################################################################

get_directories( HDR_DIR_LIST HEADERS )
foreach( d ${HDR_DIR_LIST} )
  install( DIRECTORY ${d}
           DESTINATION include/${PROJECT_NAME}/gkg-deep-nuclei-plugin-functors
           COMPONENT devel
           FILES_MATCHING PATTERN "*.h" )
endforeach()
install( TARGETS gkg-deep-nuclei-plugin-functors
         DESTINATION lib
         COMPONENT runtime )
