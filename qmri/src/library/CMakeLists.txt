################################################################################
# HEADERS
################################################################################

set( HEADERS
gkg-qmri-t1/EKFFunctionT1.h
gkg-qmri-t1/UKFFunctionT1.h
gkg-qmri-t2/EKFFunctionT2.h
gkg-qmri-t2/UKFFunctionT2.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
gkg-qmri-t1/EKFFunctionT1.cxx
gkg-qmri-t1/UKFFunctionT1.cxx
gkg-qmri-t2/EKFFunctionT2.cxx
gkg-qmri-t2/UKFFunctionT2.cxx
)


################################################################################
# Compilation and link
################################################################################

add_library( gkg-qmri SHARED ${SOURCES} )
set_property( TARGET gkg-qmri PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-qmri gkg-processing )


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
install( TARGETS gkg-qmri DESTINATION lib COMPONENT runtime )
