################################################################################
# HEADERS
################################################################################

set( HEADERS
gkg-graphics-video-avi/AviFile.h
gkg-graphics-video-avi/AviIndex.h
gkg-graphics-video-avi/AviMainHeader.h
gkg-graphics-video-avi/AviMemory.h
gkg-graphics-video-avi/AviStrategy.h
gkg-graphics-video-avi/AviStreamHeader.h
gkg-graphics-video-avi/AviWriter.h
gkg-graphics-video-avi/AviWriter_i.h
gkg-graphics-video-avi/Bitmap.h
gkg-graphics-video-avi/FourCC.h
gkg-graphics-video-avi/Rect.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
gkg-graphics-video-avi/AviFile.cxx
gkg-graphics-video-avi/AviMemory.cxx
gkg-graphics-video-avi/AviStrategy.cxx
)


################################################################################
# Compilation and link
################################################################################

add_library( gkg-graphics-video SHARED ${SOURCES} )
set_property( TARGET gkg-graphics-video PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-graphics-video gkg-processing )


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
install( TARGETS gkg-graphics-video DESTINATION lib COMPONENT runtime )
