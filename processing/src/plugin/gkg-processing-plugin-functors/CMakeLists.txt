################################################################################
# HEADERS
################################################################################

set( HEADERS
AddBorder2Volume/AddBorder2VolumeCommand.h
AddInformationToVolumeHeader/AddInformationToVolumeHeaderCommand.h
AddRoiStatisticsToProbabilisticAtlas/AddRoiStatisticsToProbabilisticAtlasCommand.h
AffineTransform3d/AffineTransform3dCommand.h
AffineTransform3dSplitter/AffineTransform3dSplitter.h
AntipodallySymmetricUniformOrientationSet/AntipodallySymmetricUniformOrientationSetCommand.h
BiasCorrection/BiasCorrectionCommand.h
Binarizer/BinarizerCommand.h
BrainVISAVectorFieldConverter/BrainVISAVectorFieldConverterCommand.h
Brownian/BrownianCommand.h
BrukerPhaseMapConverter/BrukerPhaseMapConverterCommand.h
Cat/CatCommand.h
CheckFloatingVolume/CheckFloatingVolumeCommand.h
Combiner/CombinerCommand.h
ComplexSplitter/ComplexSplitterCommand.h
ConnectedComponents/ConnectedComponentsCommand.h
ConvexHull/ConvexHullCommand.h
Convolution3d/Convolution3dCommand.h
DataBaseClassifier/DataBaseClassifierCommand.h
DericheGaussianFilter3d/DericheGaussianFilter3dCommand.h
Dicom2GisConverter/Dicom2GisConverterCommand.h
Dicom2NiftiConverter/Dicom2NiftiConverterCommand.h
DicomAnalyzer/DicomAnalyzer.h
DicomAnalyzer/DicomAnalyzerCommand.h
DicomAnalyzer/DicomSender.h
DicomProcessingUnit/DicomProcessingUnit.h
DicomProcessingUnit/DicomProcessingUnitCommand.h
DicomProcessingUnit/DpuCommandId.h
DicomProcessingUnit/DpuRegisterCallback.h
DicomProcessingUnit/DpuService.h
DicomProcessingUnit/DpuUnregisterCallback.h
DicomStorageUnit/DicomStorageUnitCommand.h
DicomStorageUnit/DsuAnalyzer.h
DicomStorageUnit/DsuSender.h
DiffeomorphismVolumeToMesh/DiffeomorphismVolumeToMeshCommand.h
DistanceMapper/DistanceMapperCommand.h
DoubleVolumeResolution/DoubleVolumeResolutionCommand.h
DwiQBallEquator/DwiQBallEquatorCommand.h
DwiQBallEquator2Curve/DwiQBallEquator2CurveCommand.h
DwiSO3Interpolator/DwiSO3InterpolatorCommand.h
Elea/EleaCommand.h
EnlargeVolume/EnlargeVolumeCommand.h
Filter3d/Filter3dCommand.h
FIRFilter3d/FIRFilter3dCommand.h
FistaDenoiserDeblurrer/FistaDenoiserDeblurrerCommand.h
Flipper/FlipperCommand.h
GehcPhaseMapConverter/GehcPhaseMapConverterCommand.h
GehcSignaDisplayWave/GehcSignaDisplayWaveCommand.h
GetMask/GetMaskCommand.h
GetSliceMask/GetSliceMaskCommand.h
GetTransform3dFromVolumeHeader/GetTransform3dFromVolumeHeaderCommand.h
Gis2NiftiConverter/Gis2NiftiConverterCommand.h
HistogramAnalyzer/HistogramAnalyzerCommand.h
HoleFilling2d/HoleFilling2dCommand.h
IntensityNormalization/IntensityNormalizationCommand.h
KMeansSegmentation/KMeansSegmentationCommand.h
MannWhitneyWilcoxonUTest/MannWhitneyWilcoxonUTestCommand.h
Mask2HemisphereMask/Mask2HemisphereMaskCommand.h
MergeVolumes/MergeVolumesCommand.h
MergeZoomedDwData/MergeZoomedDwDataCommand.h
MeshMapSurfaceComputer/MeshMapSurfaceComputerCommand.h
MeshMapToObjAndMtlConverter/MeshMapToObjAndMtlConverterCommand.h
MeshMapTransform3d/MeshMapTransform3dCommand.h
MeshSelectorUsingFileSize/MeshSelectorUsingFileSizeCommand.h
Minf2SomaConverter/Minf2SomaConverterCommand.h
MorphologicalOperation/MorphologicalOperationCommand.h
Mosaicer/MosaicerCommand.h
MotionCorrection/MotionCorrectionCommand.h
Nifti2GisConverter/Nifti2GisConverterCommand.h
NonLocalMeansFilter/NonLocalMeansFilterCommand.h
#NonLocalMeansInPainter/NonLocalMeansInPainterCommand.h
Normalization3d/Normalization3dCommand.h
NormalizedRootMeanSquareDifference/NormalizedRootMeanSquareDifferenceCommand.h
OrientationSetEvaluator/OrientationSetEvaluatorCommand.h
PhaseUnwrapper/PhaseUnwrapperCommand.h
PhilipsPhaseMapConverter/PhilipsPhaseMapConverterCommand.h
PostMortemBlockToWholeImageComposer/PostMortemBlockToWholeImageComposerCommand.h
PottsRegularizer/PottsRegularizerCommand.h
RandomIndexGenerator/RandomIndexGeneratorCommand.h
RegionGrowingSegmentation/RegionGrowingSegmentationCommand.h
Registration3d/Registration3dCommand.h
Resampling3d/Resampling3dCommand.h
RescalerWithSlopeAndIntercept/RescalerWithSlopeAndInterceptCommand.h
RgbIntensityMapper/RgbIntensityMapperCommand.h
RoiAverageSignalToVolume/RoiAverageSignalToVolumeCommand.h
RoiBasedTextureSplitter/RoiBasedTextureSplitterCommand.h
RoiStatistics/RoiStatisticsCommand.h
SiemensDeInterleave/SiemensDeInterleaveCommand.h
SiemensDemosaicer/SiemensDemosaicerCommand.h
SiemensDwiOrientationFile/SiemensDwiOrientationFileCommand.h
SiemensPhaseMapConverter/SiemensPhaseMapConverterCommand.h
SlrRf/SlrRfCommand.h
SNR/SNRCommand.h
SphericalHarmonicMesher/SphericalHarmonicMesherCommand.h
SplitDwData/SplitDwDataCommand.h
StatisticAnalyzer/StatisticAnalyzerCommand.h
SubVolume/SubVolumeCommand.h
SubVolumeEraser/SubVolumeEraserCommand.h
SupportVectorMachinePredicter/SupportVectorMachinePredicterCommand.h
SupportVectorMachinePredicterForVolume/SupportVectorMachinePredicterForVolumeCommand.h
SupportVectorMachineTrainer/SupportVectorMachineTrainerCommand.h
T2ToT1ContrastTransformer/T2ToT1ContrastTransformerCommand.h
TextureMapAverager/TextureMapAveragerCommand.h
TextureMapBinarizer/TextureMapBinarizerCommand.h
TextureMapFillRoiValues/TextureMapFillRoiValuesCommand.h
TextureMapMasker/TextureMapMaskerCommand.h
TextureMapSelector/TextureMapSelectorCommand.h
TextureMapStatisticAnalyzer/TextureMapStatisticAnalyzerCommand.h
TextureMapThresholder/TextureMapThresholderCommand.h
Thresholder/ThresholderCommand.h
TopologyClassifier/TopologyClassifierCommand.h
Transform3dComposer/Transform3dComposerCommand.h
Transform3dInverter/Transform3dInverterCommand.h
TwoTexturesCorrelationCoefficient/TwoTexturesCorrelationCoefficientCommand.h
UniformOrientationSet/UniformOrientationSetCommand.h
UnwrappedPhaseMap/UnwrappedPhaseMapCommand.h
VoiStatistics/VoiStatisticsCommand.h
VolumeAverager/VolumeAveragerCommand.h
VolumeBetweenTwoMeshesIntersection/VolumeBetweenTwoMeshesIntersectionCommand.h
VolumeBook/VolumeBookCommand.h
VolumeInformation/VolumeInformationCommand.h
VolumeRootMeanSquareDifference/VolumeRootMeanSquareDifferenceCommand.h
VolumeToMeshIntersection/VolumeToMeshIntersectionCommand.h
VolumeToSlabSplitter/VolumeToSlabSplitterCommand.h
Voronoi/VoronoiCommand.h
)


################################################################################
# SOURCES
################################################################################

set( SOURCES
AddBorder2Volume/AddBorder2VolumeCommand.cxx
AddInformationToVolumeHeader/AddInformationToVolumeHeaderCommand.cxx
AddRoiStatisticsToProbabilisticAtlas/AddRoiStatisticsToProbabilisticAtlasCommand.cxx
AffineTransform3d/AffineTransform3dCommand.cxx
AffineTransform3dSplitter/AffineTransform3dSplitterCommand.cxx
AntipodallySymmetricUniformOrientationSet/AntipodallySymmetricUniformOrientationSetCommand.cxx
BiasCorrection/BiasCorrectionCommand.cxx
Binarizer/BinarizerCommand.cxx
BrainVISAVectorFieldConverter/BrainVISAVectorFieldConverterCommand.cxx
Brownian/BrownianCommand.cxx
BrukerPhaseMapConverter/BrukerPhaseMapConverterCommand.cxx
Cat/CatCommand.cxx
CheckFloatingVolume/CheckFloatingVolumeCommand.cxx
Combiner/CombinerCommand.cxx
ComplexSplitter/ComplexSplitterCommand.cxx
ConnectedComponents/ConnectedComponentsCommand.cxx
ConvexHull/ConvexHullCommand.cxx
Convolution3d/Convolution3dCommand.cxx
DataBaseClassifier/DataBaseClassifierCommand.cxx
DericheGaussianFilter3d/DericheGaussianFilter3dCommand.cxx
Dicom2GisConverter/Dicom2GisConverterCommand.cxx
Dicom2NiftiConverter/Dicom2NiftiConverterCommand.cxx
DicomAnalyzer/DicomAnalyzer.cxx
DicomAnalyzer/DicomAnalyzerCommand.cxx
DicomAnalyzer/DicomSender.cxx
DicomProcessingUnit/DicomProcessingUnit.cxx
DicomProcessingUnit/DicomProcessingUnitCommand.cxx
DicomProcessingUnit/DpuRegisterCallback.cxx
DicomProcessingUnit/DpuService.cxx
DicomProcessingUnit/DpuUnregisterCallback.cxx
DicomStorageUnit/DicomStorageUnitCommand.cxx
DicomStorageUnit/DsuAnalyzer.cxx
DicomStorageUnit/DsuSender.cxx
DiffeomorphismVolumeToMesh/DiffeomorphismVolumeToMeshCommand.cxx
DistanceMapper/DistanceMapperCommand.cxx
DoubleVolumeResolution/DoubleVolumeResolutionCommand.cxx
DwiQBallEquator/DwiQBallEquatorCommand.cxx
DwiQBallEquator2Curve/DwiQBallEquator2CurveCommand.cxx
DwiSO3Interpolator/DwiSO3InterpolatorCommand.cxx
Elea/EleaCommand.cxx
EnlargeVolume/EnlargeVolumeCommand.cxx
Filter3d/Filter3dCommand.cxx
FIRFilter3d/FIRFilter3dCommand.cxx
FistaDenoiserDeblurrer/FistaDenoiserDeblurrerCommand.cxx
Flipper/FlipperCommand.cxx
GehcPhaseMapConverter/GehcPhaseMapConverterCommand.cxx
GehcSignaDisplayWave/GehcSignaDisplayWaveCommand.cxx
GetMask/GetMaskCommand.cxx
GetSliceMask/GetSliceMaskCommand.cxx
GetTransform3dFromVolumeHeader/GetTransform3dFromVolumeHeaderCommand.cxx
Gis2NiftiConverter/Gis2NiftiConverterCommand.cxx
HistogramAnalyzer/HistogramAnalyzerCommand.cxx
HoleFilling2d/HoleFilling2dCommand.cxx
IntensityNormalization/IntensityNormalizationCommand.cxx
KMeansSegmentation/KMeansSegmentationCommand.cxx
MannWhitneyWilcoxonUTest/MannWhitneyWilcoxonUTestCommand.cxx
Mask2HemisphereMask/Mask2HemisphereMaskCommand.cxx
MergeVolumes/MergeVolumesCommand.cxx
MergeZoomedDwData/MergeZoomedDwDataCommand.cxx
MeshMapSurfaceComputer/MeshMapSurfaceComputerCommand.cxx
MeshMapToObjAndMtlConverter/MeshMapToObjAndMtlConverterCommand.cxx
MeshMapTransform3d/MeshMapTransform3dCommand.cxx
MeshSelectorUsingFileSize/MeshSelectorUsingFileSizeCommand.cxx
Minf2SomaConverter/Minf2SomaConverterCommand.cxx
MorphologicalOperation/MorphologicalOperationCommand.cxx
Mosaicer/MosaicerCommand.cxx
MotionCorrection/MotionCorrectionCommand.cxx
Nifti2GisConverter/Nifti2GisConverterCommand.cxx
NonLocalMeansFilter/NonLocalMeansFilterCommand.cxx
#NonLocalMeansInPainter/NonLocalMeansInPainterCommand.cxx
Normalization3d/Normalization3dCommand.cxx
NormalizedRootMeanSquareDifference/NormalizedRootMeanSquareDifferenceCommand.cxx
OrientationSetEvaluator/OrientationSetEvaluatorCommand.cxx
PhaseUnwrapper/PhaseUnwrapperCommand.cxx
PhilipsPhaseMapConverter/PhilipsPhaseMapConverterCommand.cxx
PostMortemBlockToWholeImageComposer/PostMortemBlockToWholeImageComposerCommand.cxx
PottsRegularizer/PottsRegularizerCommand.cxx
RandomIndexGenerator/RandomIndexGeneratorCommand.cxx
RegionGrowingSegmentation/RegionGrowingSegmentationCommand.cxx
Registration3d/Registration3dCommand.cxx
Resampling3d/Resampling3dCommand.cxx
RescalerWithSlopeAndIntercept/RescalerWithSlopeAndInterceptCommand.cxx
RgbIntensityMapper/RgbIntensityMapperCommand.cxx
RoiAverageSignalToVolume/RoiAverageSignalToVolumeCommand.cxx
RoiBasedTextureSplitter/RoiBasedTextureSplitterCommand.cxx
RoiStatistics/RoiStatisticsCommand.cxx
SiemensDeInterleave/SiemensDeInterleaveCommand.cxx
SiemensDemosaicer/SiemensDemosaicerCommand.cxx
SiemensDwiOrientationFile/SiemensDwiOrientationFileCommand.cxx
SiemensPhaseMapConverter/SiemensPhaseMapConverterCommand.cxx
SlrRf/SlrRfCommand.cxx
SNR/SNRCommand.cxx
SphericalHarmonicMesher/SphericalHarmonicMesherCommand.cxx
SplitDwData/SplitDwDataCommand.cxx
StatisticAnalyzer/StatisticAnalyzerCommand.cxx
SubVolume/SubVolumeCommand.cxx
SubVolumeEraser/SubVolumeEraserCommand.cxx
SupportVectorMachinePredicter/SupportVectorMachinePredicterCommand.cxx
SupportVectorMachinePredicterForVolume/SupportVectorMachinePredicterForVolumeCommand.cxx
SupportVectorMachineTrainer/SupportVectorMachineTrainerCommand.cxx
T2ToT1ContrastTransformer/T2ToT1ContrastTransformerCommand.cxx
TextureMapAverager/TextureMapAveragerCommand.cxx
TextureMapBinarizer/TextureMapBinarizerCommand.cxx
TextureMapFillRoiValues/TextureMapFillRoiValuesCommand.cxx
TextureMapMasker/TextureMapMaskerCommand.cxx
TextureMapSelector/TextureMapSelectorCommand.cxx
TextureMapStatisticAnalyzer/TextureMapStatisticAnalyzerCommand.cxx
TextureMapThresholder/TextureMapThresholderCommand.cxx
Thresholder/ThresholderCommand.cxx
TopologyClassifier/TopologyClassifierCommand.cxx
Transform3dComposer/Transform3dComposerCommand.cxx
Transform3dInverter/Transform3dInverterCommand.cxx
TwoTexturesCorrelationCoefficient/TwoTexturesCorrelationCoefficientCommand.cxx
UniformOrientationSet/UniformOrientationSetCommand.cxx
UnwrappedPhaseMap/UnwrappedPhaseMapCommand.cxx
VoiStatistics/VoiStatisticsCommand.cxx
VolumeAverager/VolumeAveragerCommand.cxx
VolumeBetweenTwoMeshesIntersection/VolumeBetweenTwoMeshesIntersectionCommand.cxx
VolumeBook/VolumeBookCommand.cxx
VolumeInformation/VolumeInformationCommand.cxx
VolumeRootMeanSquareDifference/VolumeRootMeanSquareDifferenceCommand.cxx
VolumeToMeshIntersection/VolumeToMeshIntersectionCommand.cxx
VolumeToSlabSplitter/VolumeToSlabSplitterCommand.cxx
Voronoi/VoronoiCommand.cxx
)


################################################################################
# Compilation and link
################################################################################

add_library( gkg-processing-plugin-functors SHARED ${SOURCES} )
set_property( TARGET gkg-processing-plugin-functors 
              PROPERTY VERSION ${${PROJECT_NAME}_VERSION} )
target_link_libraries( gkg-processing-plugin-functors gkg-processing
                       ${JPEG_LIBRARIES} )


################################################################################
# Installation
################################################################################

get_directories( HDR_DIR_LIST HEADERS )
foreach( d ${HDR_DIR_LIST} )
  install( DIRECTORY ${d}
           DESTINATION include/${PROJECT_NAME}/gkg-processing-plugin-functors
           COMPONENT devel
           FILES_MATCHING PATTERN "*.h" )
endforeach()
install( TARGETS gkg-processing-plugin-functors
         DESTINATION lib
         COMPONENT runtime )
