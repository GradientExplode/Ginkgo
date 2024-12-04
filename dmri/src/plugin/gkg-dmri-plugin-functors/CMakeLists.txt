################################################################################
# HEADERS
################################################################################

set( HEADERS
AddRicianNoise/AddRicianNoiseCommand.h
B0InhomogeneityCorrection/B0InhomogeneityCorrectionCommand.h
Dwi90DegreeCrossingPhantomSimulator/Dwi90DegreeCrossingPhantomSimulatorCommand.h
DwiBundleMapAlphaShapeMesher/DwiBundleMapAlphaShapeMesherCommand.h
DwiBundleMapConvexHull/DwiBundleMapConvexHullCommand.h
DwiBundleMapDensityMask/DwiBundleMapDensityMaskCommand.h
DwiBundleMapHierarchy/DwiBundleMapHierarchyCommand.h
DwiBundleMapSectionMask/DwiBundleMapSectionMaskCommand.h
DwiBundleMapToObjAndMtlConverter/DwiBundleMapToObjAndMtlConverterCommand.h
DwiBundleMapToPointCloudConverter/DwiBundleMapToPointCloudConverterCommand.h
DwiBundleMapTransform3d/DwiBundleMapTransform3dCommand.h
DwiBundleMeasure/DwiBundleMeasureCommand.h
DwiBundleOperator/DwiBundleOperatorCommand.h
DwiBValue/DwiBValueCommand.h
DwiChangeRGBBackgroundColor/DwiChangeRGBBackgroundColorCommand.h
DwiConnectivityMatrix/DwiConnectivityMatrixCommand.h
DwiDiffusionKernel/DwiDiffusionKernelCommand.h
DwiDiffusoidField/DwiDiffusoidFieldCommand.h
DwiDirectionFieldDiscrepancy/DwiDirectionFieldDiscrepancyCommand.h
DwiDiskFormatConverter/DwiDiskFormatConverterCommand.h
DwiEapField/DwiEapFieldCommand.h
DwiEddyCurrent/DwiEddyCurrentCommand.h
DwiFiberFilter/DwiFiberFilterCommand.h
DwiFiberMeasure/DwiFiberMeasureCommand.h
DwiGaussianSimulator/DiffusionGaussianModel.h
DwiGaussianSimulator/DiffusionGaussianModel1.h
DwiGaussianSimulator/DiffusionGaussianModel2.h
DwiGaussianSimulator/DiffusionGaussianModel3.h
DwiGaussianSimulator/DwiGaussianSimulatorCommand.h
DwiGeometricMean/DwiGeometricMeanCommand.h
DwiGeometrySimulator/DwiGeometrySimulatorCommand.h
DwiGFAHistogram/DwiGFAHistogramCommand.h
DwiMakeWholeOrientationSet/DwiMakeWholeOrientationSetCommand.h
DwiMicrostructureField/DwiMicrostructureFieldCommand.h
DwiMotionCorrection/DwiMotionCorrectionCommand.h
DwiMultipleShellQSpaceSamplingSplitter/DwiMultipleShellQSpaceSamplingSplitterCommand.h
DwiMultipleSubjectTrackDensityImaging/DwiMultipleSubjectTrackDensityImagingCommand.h
DwiNodePaving/DwiNodePavingCommand.h
DwiNoiseFilter/DwiNoiseFilterCommand.h
DwiNonCentralChiFitAbacus/DwiNonCentralChiFitAbacusCommand.h
DwiOdfField/DwiOdfFieldCommand.h
DwiOdfFieldFunctor/DwiOdfFieldFunctorCommand.h
DwiOdfLMSDifference/DwiOdfLMSDifferenceCommand.h
DwiOutlierFilter/DwiOutlierFilterCommand.h
DwiParallelNoiseFilter/DwiParallelNoiseFilterCommand.h
DwiPdfField/DwiPdfFieldCommand.h
DwiPhantomSNRMeasure/DwiPhantomSNRMeasureCommand.h
DwiPostMortemBlockToWholeImageComposer/DwiPostMortemBlockToWholeImageComposerCommand.h
DwiQSpaceDomainFinder/DwiQSpaceDomainFinderCommand.h
DwiQSpaceResampler/DwiQSpaceResamplerCommand.h
DwiQSpaceSampling/DwiQSpaceSamplingCommand.h
DwiQSpaceSamplingRendering/DwiQSpaceSamplingRenderingCommand.h
DwiResampling3d/DwiResampling3dCommand.h
#DwiRgbMapFromDirectionMaps/DwiRgbMapFromDirectionMapsCommand.h
DwiRicianFitAbacus/DwiRicianFitAbacusCommand.h
DwiSingleShellNoiseFilter/DwiSingleShellNoiseFilterCommand.h
DwiSingleSubjectTrackDensityImaging/DwiSingleSubjectTrackDensityImagingCommand.h
DwiSiteAndTextureToVolumeConverter/DwiSiteAndTextureToVolumeConverterCommand.h
DwiSubVolume/DwiSubVolumeCommand.h
DwiSyntheticCorpusCallosumMidSagittalPlane/DwiSyntheticCorpusCallosumMidSagittalPlaneCommand.h
DwiTensorField/DwiTensorFieldCommand.h
DwiTensorNoiseContamination/DwiTensorNoiseContaminationCommand.h
DwiTractography/DwiTractographyCommand.h
DwiTractographyMask/DwiTractographyMaskCommand.h
DwiTwoPoolModelField/DwiTwoPoolModelFieldCommand.h
MeshMapSlicer/MeshMapSlicerCommand.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
AddRicianNoise/AddRicianNoiseCommand.cxx
B0InhomogeneityCorrection/B0InhomogeneityCorrectionCommand.cxx
Dwi90DegreeCrossingPhantomSimulator/Dwi90DegreeCrossingPhantomSimulatorCommand.cxx
DwiBundleMapAlphaShapeMesher/DwiBundleMapAlphaShapeMesherCommand.cxx
DwiBundleMapConvexHull/DwiBundleMapConvexHullCommand.cxx
DwiBundleMapDensityMask/DwiBundleMapDensityMaskCommand.cxx
DwiBundleMapHierarchy/DwiBundleMapHierarchyCommand.cxx
DwiBundleMapSectionMask/DwiBundleMapSectionMaskCommand.cxx
DwiBundleMapToObjAndMtlConverter/DwiBundleMapToObjAndMtlConverterCommand.cxx
DwiBundleMapToPointCloudConverter/DwiBundleMapToPointCloudConverterCommand.cxx
DwiBundleMapTransform3d/DwiBundleMapTransform3dCommand.cxx
DwiBundleMeasure/DwiBundleMeasureCommand.cxx
DwiBundleOperator/DwiBundleOperatorCommand.cxx
DwiBValue/DwiBValueCommand.cxx
DwiChangeRGBBackgroundColor/DwiChangeRGBBackgroundColorCommand.cxx
DwiConnectivityMatrix/DwiConnectivityMatrixCommand.cxx
DwiDiffusionKernel/DwiDiffusionKernelCommand.cxx
DwiDiffusoidField/DwiDiffusoidFieldCommand.cxx
DwiDirectionFieldDiscrepancy/DwiDirectionFieldDiscrepancyCommand.cxx
DwiDiskFormatConverter/DwiDiskFormatConverterCommand.cxx
DwiEapField/DwiEapFieldCommand.cxx
DwiEddyCurrent/DwiEddyCurrentCommand.cxx
DwiFiberFilter/DwiFiberFilterCommand.cxx
DwiFiberMeasure/DwiFiberMeasureCommand.cxx
DwiGaussianSimulator/DiffusionGaussianModel.cxx
DwiGaussianSimulator/DiffusionGaussianModel1.cxx
DwiGaussianSimulator/DiffusionGaussianModel2.cxx
DwiGaussianSimulator/DiffusionGaussianModel3.cxx
DwiGaussianSimulator/DwiGaussianSimulatorCommand.cxx
DwiGeometricMean/DwiGeometricMeanCommand.cxx
DwiGeometrySimulator/DwiGeometrySimulatorCommand.cxx
DwiGFAHistogram/DwiGFAHistogramCommand.cxx
DwiMakeWholeOrientationSet/DwiMakeWholeOrientationSetCommand.cxx
DwiMicrostructureField/DwiMicrostructureFieldCommand.cxx
DwiMotionCorrection/DwiMotionCorrectionCommand.cxx
DwiMultipleShellQSpaceSamplingSplitter/DwiMultipleShellQSpaceSamplingSplitterCommand.cxx
DwiMultipleSubjectTrackDensityImaging/DwiMultipleSubjectTrackDensityImagingCommand.cxx
DwiNodePaving/DwiNodePavingCommand.cxx
DwiNoiseFilter/DwiNoiseFilterCommand.cxx
DwiNonCentralChiFitAbacus/DwiNonCentralChiFitAbacusCommand.cxx
DwiOdfField/DwiOdfFieldCommand.cxx
DwiOdfFieldFunctor/DwiOdfFieldFunctorCommand.cxx
DwiOdfLMSDifference/DwiOdfLMSDifferenceCommand.cxx
DwiOutlierFilter/DwiOutlierFilterCommand.cxx
DwiParallelNoiseFilter/DwiParallelNoiseFilterCommand.cxx
DwiPdfField/DwiPdfFieldCommand.cxx
DwiPhantomSNRMeasure/DwiPhantomSNRMeasureCommand.cxx
DwiPostMortemBlockToWholeImageComposer/DwiPostMortemBlockToWholeImageComposerCommand.cxx
DwiQSpaceDomainFinder/DwiQSpaceDomainFinderCommand.cxx
DwiQSpaceResampler/DwiQSpaceResamplerCommand.cxx
DwiQSpaceSampling/DwiQSpaceSamplingCommand.cxx
DwiQSpaceSamplingRendering/DwiQSpaceSamplingRenderingCommand.cxx
DwiResampling3d/DwiResampling3dCommand.cxx
#DwiRgbMapFromDirectionMaps/DwiRgbMapFromDirectionMapsCommand.cxx
DwiRicianFitAbacus/DwiRicianFitAbacusCommand.cxx
DwiSingleShellNoiseFilter/DwiSingleShellNoiseFilterCommand.cxx
DwiSingleSubjectTrackDensityImaging/DwiSingleSubjectTrackDensityImagingCommand.cxx
DwiSiteAndTextureToVolumeConverter/DwiSiteAndTextureToVolumeConverterCommand.cxx
DwiSubVolume/DwiSubVolumeCommand.cxx
DwiSyntheticCorpusCallosumMidSagittalPlane/DwiSyntheticCorpusCallosumMidSagittalPlaneCommand.cxx
DwiTensorField/DwiTensorFieldCommand.cxx
DwiTensorNoiseContamination/DwiTensorNoiseContaminationCommand.cxx
DwiTractography/DwiTractographyCommand.cxx
DwiTractographyMask/DwiTractographyMaskCommand.cxx
DwiTwoPoolModelField/DwiTwoPoolModelFieldCommand.cxx
MeshMapSlicer/MeshMapSlicerCommand.cxx
)


################################################################################
# Compilation and link
################################################################################

# temporary add for HDF5
include_directories( ${HDF5_INCLUDE_DIRS} )

add_library( gkg-dmri-plugin-functors SHARED ${SOURCES} )
set_property( TARGET gkg-dmri-plugin-functors PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-dmri-plugin-functors gkg-dmri
                       ${HDF5_LIBRARIES} )


################################################################################
# Installation
################################################################################

get_directories( HDR_DIR_LIST HEADERS )
foreach( d ${HDR_DIR_LIST} )
  install( DIRECTORY ${d}
           DESTINATION include/${PROJECT_NAME}/gkg-dmri-plugin-functors
           COMPONENT devel
           FILES_MATCHING PATTERN "*.h" )
endforeach()
install( TARGETS gkg-dmri-plugin-functors
         DESTINATION lib
         COMPONENT runtime )
