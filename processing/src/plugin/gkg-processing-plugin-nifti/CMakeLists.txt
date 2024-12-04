################################################################################
# HEADERS
################################################################################

set( HEADERS
NiftiAnalyzer.h
NiftiDiskFormat.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
NiftiAnalyzer.cxx
NiftiDiskFormat.cxx
)


################################################################################
# Compilation and link
################################################################################

include_directories( ${PIO_INCLUDE_DIR}
                     ${PIO_INCLUDE_DIR}/pio/nifti/niftilib 
                     ${PIO_INCLUDE_DIR}/pio/nifti/znzlib )

add_library( gkg-processing-plugin-nifti SHARED ${SOURCES} )
set_property( TARGET gkg-processing-plugin-nifti
              PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-processing-plugin-nifti gkg-processing
                       ${PIO_LIBRARIES} )


################################################################################
# Installation
################################################################################

install( FILES ${HEADERS}
         DESTINATION include/${PROJECT_NAME}/gkg-processing-plugin-nifti
         COMPONENT devel )
install( TARGETS gkg-processing-plugin-nifti DESTINATION lib COMPONENT runtime )
