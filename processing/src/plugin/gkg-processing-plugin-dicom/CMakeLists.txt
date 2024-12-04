################################################################################
# HEADERS
################################################################################

set( HEADERS
DcmtkDicomDumpImplementation.h
DcmtkDicomFileImplementation.h
DcmtkDicomImplementationFactory.h 
DcmtkDicomSCUImplementation.h 
DcmtkDicomSCPImplementation.h 
DicomAnalyzer.h
DicomDiskFormat.h
ForceHardLinkWithDicomPlugin.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
DcmtkDicomDumpImplementation.cxx
DcmtkDicomFileImplementation.cxx
DcmtkDicomImplementationFactory.cxx
DcmtkDicomSCUImplementation.cxx
DcmtkDicomSCPImplementation.cxx
DicomAnalyzer.cxx
DicomDiskFormat.cxx
ForceHardLinkWithDicomPlugin.cxx
)


################################################################################
# LIBRARIES
################################################################################

set( EXTRA_LIBRARIES
  ${PIO_LIBRARIES}
  ${DCMTK_LIBRARIES} 
  ${TIFF_LIBRARIES}
  ${PNG_LIBRARIES}
)

if ( JASPER_FOUND )
  list( APPEND EXTRA_LIBRARIES ${JASPER_LIBRARIES} )
endif ( JASPER_FOUND )


################################################################################
# Compilation and link
################################################################################

include_directories( ${PIO_INCLUDE_DIR} ${DCMTK_INCLUDE_DIR} )

add_definitions( -DHAVE_CONFIG_H )
add_library( gkg-processing-plugin-dicom SHARED ${SOURCES} )
set_property( TARGET gkg-processing-plugin-dicom
              PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-processing-plugin-dicom gkg-processing 
                       ${EXTRA_LIBRARIES} )


################################################################################
# Installation
################################################################################

install( FILES ${HEADERS}
         DESTINATION include/${PROJECT_NAME}/gkg-processing-plugin-dicom
         COMPONENT devel )
install( TARGETS gkg-processing-plugin-dicom
         DESTINATION lib
         COMPONENT runtime )
