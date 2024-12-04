################################################################################
# HEADERS
################################################################################

set( HEADERS
Anonymizer/AnonymizerCommand.h
Autoshim/AutoshimCommand.h
BrukerEpiReconstructor/BrukerEpiReconstructorCommand.h
BrukerGre2dReconstructor/BrukerGre2dReconstructorCommand.h
BrukerGre3dReconstructor/BrukerGre3dReconstructorCommand.h
BrukerPhaseMap3dReconstructor/BrukerPhaseMap3dReconstructorCommand.h
BrukerPhaseMapReconstructor/BrukerPhaseMapReconstructorCommand.h
BrukerRaw2GisConverter/BrukerRaw2GisConverterCommand.h
BrukerRawDump/BrukerRawDumpCommand.h
BrukerReco2GisConverter/BrukerReco2GisConverterCommand.h
BrukerRecoDump/BrukerRecoDumpCommand.h
CoilCovarianceMatrix/CoilCovarianceMatrixCommand.h
CoilInhomogeneityMap/CoilInhomogeneityMapCommand.h
CoilSensitivityMap/CoilSensitivityMapCommand.h
DiskFormatConverter/DiskFormatConverterCommand.h
DwBalancedSSFPPhaseOffsetCombiner/DwBalancedSSFPPhaseOffsetCombinerCommand.h
GehcB0MapReconstructor/GehcB0MapReconstructorCommand.h
GehcB1MapReconstructor/GehcB1MapReconstructorCommand.h
GehcEpiBoldReconstruction/GehcEpiBoldReconstructionCommand.h
GehcEpiDwReconstructor/GehcEpiDwReconstructorCommand.h
GehcEpiReconstructor/GehcEpiReconstructorCommand.h
GehcEviReconstructor/GehcEviReconstructorCommand.h
GehcGre2dReconstructor/GehcGre2dReconstructorCommand.h
GehcGradientWarpingCorrection/GehcGradientWarpingCorrectionCommand.h
GehcSignaGenesis22GisConverter/GehcSignaGenesis22GisConverterCommand.h
GehcSignaGenesis2Dump/GehcSignaGenesis2DumpCommand.h
GehcSignaGenesis2GisConverter/GehcSignaGenesis2GisConverterCommand.h
GehcSignaGenesisDump/GehcSignaGenesisDumpCommand.h
GehcSignaPFile2GisConverter/GehcSignaPFile2GisConverterCommand.h
GehcSignaPFileDump/GehcSignaPFileDumpCommand.h
SenseReconstructor/SenseReconstructorCommand.h
SmsRaw2GisConverter/SmsRaw2GisConverterCommand.h
SmsRawDump/SmsRawDumpCommand.h
)



if ( ISMRMRD_FOUND )

set( HEADERS
${HEADERS}
IsmrmrdRaw2GisConverter/IsmrmrdRaw2GisConverterCommand.h
)

endif ( ISMRMRD_FOUND )



################################################################################
# SOURCES
################################################################################

set( SOURCES
Anonymizer/AnonymizerCommand.cxx
Autoshim/AutoshimCommand.cxx
BrukerEpiReconstructor/BrukerEpiReconstructorCommand.cxx
BrukerGre2dReconstructor/BrukerGre2dReconstructorCommand.cxx
BrukerGre3dReconstructor/BrukerGre3dReconstructorCommand.cxx
BrukerPhaseMap3dReconstructor/BrukerPhaseMap3dReconstructorCommand.cxx
BrukerPhaseMapReconstructor/BrukerPhaseMapReconstructorCommand.cxx
BrukerRaw2GisConverter/BrukerRaw2GisConverterCommand.cxx
BrukerRawDump/BrukerRawDumpCommand.cxx
BrukerReco2GisConverter/BrukerReco2GisConverterCommand.cxx
BrukerRecoDump/BrukerRecoDumpCommand.cxx
CoilCovarianceMatrix/CoilCovarianceMatrixCommand.cxx
CoilInhomogeneityMap/CoilInhomogeneityMapCommand.cxx
CoilSensitivityMap/CoilSensitivityMapCommand.cxx
DiskFormatConverter/DiskFormatConverterCommand.cxx
DwBalancedSSFPPhaseOffsetCombiner/DwBalancedSSFPPhaseOffsetCombinerCommand.cxx
GehcB0MapReconstructor/GehcB0MapReconstructorCommand.cxx
GehcB1MapReconstructor/GehcB1MapReconstructorCommand.cxx
GehcEpiBoldReconstruction/GehcEpiBoldReconstructionCommand.cxx
GehcEpiDwReconstructor/GehcEpiDwReconstructorCommand.cxx
GehcEpiReconstructor/GehcEpiReconstructorCommand.cxx
GehcEviReconstructor/GehcEviReconstructorCommand.cxx
GehcGradientWarpingCorrection/GehcGradientWarpingCorrectionCommand.cxx
GehcGre2dReconstructor/GehcGre2dReconstructorCommand.cxx
GehcSignaGenesis22GisConverter/GehcSignaGenesis22GisConverterCommand.cxx
GehcSignaGenesis2Dump/GehcSignaGenesis2DumpCommand.cxx
GehcSignaGenesis2GisConverter/GehcSignaGenesis2GisConverterCommand.cxx
GehcSignaGenesisDump/GehcSignaGenesisDumpCommand.cxx
GehcSignaPFile2GisConverter/GehcSignaPFile2GisConverterCommand.cxx
GehcSignaPFileDump/GehcSignaPFileDumpCommand.cxx
SenseReconstructor/SenseReconstructorCommand.cxx
SmsRaw2GisConverter/SmsRaw2GisConverterCommand.cxx
SmsRawDump/SmsRawDumpCommand.cxx
)



if ( ISMRMRD_FOUND )

set( SOURCES
${SOURCES}
IsmrmrdRaw2GisConverter/IsmrmrdRaw2GisConverterCommand.cxx
)

endif ( ISMRMRD_FOUND )



################################################################################
# Compilation and link
################################################################################

add_library( gkg-mri-reconstruction-plugin-functors SHARED ${SOURCES} )
set_property( TARGET gkg-mri-reconstruction-plugin-functors PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-mri-reconstruction-plugin-functors gkg-mri-reconstruction )


################################################################################
# Installation
################################################################################

get_directories( HDR_DIR_LIST HEADERS )
foreach( d ${HDR_DIR_LIST} )
  install( DIRECTORY ${d}
           DESTINATION include/${PROJECT_NAME}/gkg-mri-reconstruction-plugin-functors
           COMPONENT devel
           FILES_MATCHING PATTERN "*.h" )
endforeach()
install( TARGETS gkg-mri-reconstruction-plugin-functors
         DESTINATION lib
         COMPONENT runtime )
