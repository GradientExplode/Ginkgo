################################################################################
# HEADERS
################################################################################

set( HEADERS
gkg-mri-reconstruction-core/FullKSpaceReconstructor2d.h
gkg-mri-reconstruction-core/FullKSpaceReconstructor3d.h
gkg-mri-reconstruction-core/HalfKSpaceReconstructor2d.h
gkg-mri-reconstruction-core/HalfKSpaceReconstructor3d.h
gkg-mri-reconstruction-core/Reconstructor.h
gkg-mri-reconstruction-correction/AhnPhaseCorrection.h
gkg-mri-reconstruction-correction/AhnPhaseCorrection3d.h
gkg-mri-reconstruction-correction/B0InhomogeneityCorrection.h
gkg-mri-reconstruction-correction/DcOffsetCorrection.h
gkg-mri-reconstruction-correction/EchoPlanarPhaseCorrection.h
gkg-mri-reconstruction-correction/EchoVolumarPhaseCorrection.h
gkg-mri-reconstruction-correction/GehcSignaGradientCoil.h
gkg-mri-reconstruction-correction/GradientCoil.h
gkg-mri-reconstruction-correction/GradientWarping2dCorrection.h
gkg-mri-reconstruction-correction/ModulationTransferFunction.h
gkg-mri-reconstruction-correction/ReadOutGradient.h
gkg-mri-reconstruction-correction/RegridingCorrection.h
gkg-mri-reconstruction-correction/TrapezoidROGradient.h
gkg-mri-reconstruction-io/BrukerRawAnalyzer.h
gkg-mri-reconstruction-io/BrukerRawDiskFormat.h
gkg-mri-reconstruction-io/BrukerRecoAnalyzer.h
gkg-mri-reconstruction-io/BrukerRecoDiskFormat.h
gkg-mri-reconstruction-io/ForceHardLinkWithMriReconstructionLibrary.h
gkg-mri-reconstruction-io/GehcSignaGenesisAnalyzer.h
gkg-mri-reconstruction-io/GehcSignaGenesis2Analyzer.h
gkg-mri-reconstruction-io/GehcSignaGenesisDiskFormat.h
gkg-mri-reconstruction-io/GehcSignaGenesis2DiskFormat.h
gkg-mri-reconstruction-io/GehcSignaGenesisHeader.h
gkg-mri-reconstruction-io/GehcSignaGenesis2Header.h
gkg-mri-reconstruction-io/GehcSignaPFileAnalyzer.h
gkg-mri-reconstruction-io/GehcSignaPFileDiskFormat.h
gkg-mri-reconstruction-io/GehcSignaPFileHeader.h
gkg-mri-reconstruction-io/SmsRawAnalyzer.h
gkg-mri-reconstruction-io/SmsRawDiskFormat.h
gkg-mri-reconstruction-io/SmsRawHeader.h
gkg-mri-reconstruction-parallel/B1ReferenceFunctor.h
gkg-mri-reconstruction-parallel/B1SensitivityFunctor.h
gkg-mri-reconstruction-parallel/NoiseCovarianceFunctor.h
gkg-mri-reconstruction-parallel/ParallelReconstructor.h
gkg-mri-reconstruction-parallel/ReductionStrategy.h
gkg-mri-reconstruction-parallel/SenseReconstructor.h
gkg-mri-reconstruction-signal/HalfFourierTransform3d.h
)



if ( ISMRMRD_FOUND )

set( HEADERS
${HEADERS}
gkg-mri-reconstruction-io/IsmrmrdRawAnalyzer.h
gkg-mri-reconstruction-io/IsmrmrdRawDiskFormat.h
gkg-mri-reconstruction-io/IsmrmrdRecoAnalyzer.h
gkg-mri-reconstruction-io/IsmrmrdRecoDiskFormat.h
)

endif ( ISMRMRD_FOUND )



################################################################################
# SOURCES
################################################################################

set( SOURCES
gkg-mri-reconstruction-core/FullKSpaceReconstructor2d.cxx
gkg-mri-reconstruction-core/FullKSpaceReconstructor3d.cxx
gkg-mri-reconstruction-core/HalfKSpaceReconstructor2d.cxx
gkg-mri-reconstruction-core/HalfKSpaceReconstructor3d.cxx
gkg-mri-reconstruction-core/Reconstructor.cxx
gkg-mri-reconstruction-correction/AhnPhaseCorrection.cxx
gkg-mri-reconstruction-correction/AhnPhaseCorrection3d.cxx
gkg-mri-reconstruction-correction/B0InhomogeneityCorrection.cxx
gkg-mri-reconstruction-correction/DcOffsetCorrection.cxx
gkg-mri-reconstruction-correction/EchoPlanarPhaseCorrection.cxx
gkg-mri-reconstruction-correction/EchoVolumarPhaseCorrection.cxx
gkg-mri-reconstruction-correction/GehcSignaGradientCoil.cxx
gkg-mri-reconstruction-correction/GradientCoil.cxx
gkg-mri-reconstruction-correction/GradientWarping2dCorrection.cxx
gkg-mri-reconstruction-correction/ModulationTransferFunction.cxx
gkg-mri-reconstruction-correction/ReadOutGradient.cxx
gkg-mri-reconstruction-correction/RegridingCorrection.cxx
gkg-mri-reconstruction-correction/TrapezoidROGradient.cxx
gkg-mri-reconstruction-io/BrukerRawAnalyzer.cxx
gkg-mri-reconstruction-io/BrukerRawDiskFormat.cxx
gkg-mri-reconstruction-io/BrukerRecoAnalyzer.cxx
gkg-mri-reconstruction-io/BrukerRecoDiskFormat.cxx
gkg-mri-reconstruction-io/ForceHardLinkWithMriReconstructionLibrary.cxx
gkg-mri-reconstruction-io/GehcSignaGenesisAnalyzer.cxx
gkg-mri-reconstruction-io/GehcSignaGenesis2Analyzer.cxx
gkg-mri-reconstruction-io/GehcSignaGenesisDiskFormat.cxx
gkg-mri-reconstruction-io/GehcSignaGenesis2DiskFormat.cxx
gkg-mri-reconstruction-io/GehcSignaPFileAnalyzer.cxx
gkg-mri-reconstruction-io/GehcSignaPFileDiskFormat.cxx
gkg-mri-reconstruction-io/SmsRawAnalyzer.cxx
gkg-mri-reconstruction-io/SmsRawDiskFormat.cxx
gkg-mri-reconstruction-io/SmsRawHeader.cxx
gkg-mri-reconstruction-parallel/B1ReferenceFunctor.cxx
gkg-mri-reconstruction-parallel/B1SensitivityFunctor.cxx
gkg-mri-reconstruction-parallel/NoiseCovarianceFunctor.cxx
gkg-mri-reconstruction-parallel/ParallelReconstructor.cxx
gkg-mri-reconstruction-parallel/ReductionStrategy.cxx
gkg-mri-reconstruction-parallel/SenseReconstructor.cxx
gkg-mri-reconstruction-signal/HalfFourierTransform3d.cxx
)



if ( ISMRMRD_FOUND )

set( SOURCES
${SOURCES}
gkg-mri-reconstruction-io/IsmrmrdRawAnalyzer.cxx
gkg-mri-reconstruction-io/IsmrmrdRawDiskFormat.cxx
gkg-mri-reconstruction-io/IsmrmrdRecoAnalyzer.cxx
gkg-mri-reconstruction-io/IsmrmrdRecoDiskFormat.cxx
)

endif ( ISMRMRD_FOUND )



################################################################################
# Compilation and link
################################################################################


if ( ISMRMRD_FOUND )

link_directories( ${ISMRMRD_LIBRARY_DIRS} )
include_directories( ${ISMRMRD_LIBRARY_DIRS} ${HDF5_INCLUDE_DIRS} ${MPI_INCLUDE_PATH} )

endif ( ISMRMRD_FOUND )

add_library( gkg-mri-reconstruction SHARED ${SOURCES} )
set_property( TARGET gkg-mri-reconstruction PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )

if ( ISMRMRD_FOUND )

target_link_libraries( gkg-mri-reconstruction gkg-processing ${ISMRMRD_LIBRARIES} ${HDF5_LIBRARIES} ${MPI_LIBRARIES} )

else ( ISMRMRD_FOUND )

target_link_libraries( gkg-mri-reconstruction gkg-processing )

endif ( ISMRMRD_FOUND )


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
install( TARGETS gkg-mri-reconstruction DESTINATION lib COMPONENT runtime )
