#ifndef _gkg_dmri_odf_msmt_csd_MSMTCSDFiberOdfCartesianField_h_
#define _gkg_dmri_odf_msmt_csd_MSMTCSDFiberOdfCartesianField_h_


#include <gkg-dmri-odf-msmt-csd/MSMTConstrainedSphericalDeconvolutionFiberOdf.h>
#include <gkg-dmri-container/OdfCartesianField.h>
#include <gkg-dmri-diffusion-kernel/DiffusionKernel.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-pattern/RCPointer.h>
#include <vector>


namespace gkg
{


//
// specificScalarParameters[  0 ] -> tissue count
// specificScalarParameters[  1 ] -> CQP solver maximum iteration count
// specificScalarParameters[  2 ] -> CQP solver epsilon gap
// specificScalarParameters[  3 ] -> CQP solver epsilon residuals
// specificScalarParameters[  4 ] -> CQP solver epsilon infeasible
// specificScalarParameters[  5 ] -> tissue 1: kernel wanted voxel count
// specificScalarParameters[  6 ] -> tissue 1: tissue class 1st threshold
// specificScalarParameters[  7 ] -> tissue 1: tissue class 2nd threshold
// specificScalarParameters[  8 ] -> tissue 1: FA 1st threshold
// specificScalarParameters[  9 ] -> tissue 1: FA 2nd threshold
// specificScalarParameters[ 10 ] -> tissue 1: spherical harmonics order
// specificScalarParameters[ 11 ] -> tissue 2: ...
// specificScalarParameters[ 12 ] -> tissue 2: ...
// ...
//
// specificStringParameters[  0 ] -> tissue class probability map
// specificStringParameters[  1 ] -> tissue to diffusion 3D transform type (
//                                   none, linear, or linear_and_diffeomorphic)
// specificStringParameters[  2 ] -> tissue to diffusion 3D linear transform
// specificStringParameters[  3 ] -> tissue to diffusion direct 3D diffeomorph
//                                   transform
// specificStringParameters[  4 ] -> tissue to diffusion inverse 3D diffeomorph
//                                   transform
// specificStringParameters[  5 ] -> tissue 1: kernel type (symmetric_tensor)
// specificStringParameters[  6 ] -> tissue 1: tissue class threshdolding mode
// specificStringParameters[  7 ] -> tissue 1: FA threshdolding mode
// specificStringParameters[  8 ] -> tissue 1: FA sorting type (increasing or
//                                             decreasing)
// specificScalarParameters[  9 ] -> tissue 2: ...
// specificScalarParameters[ 10 ] -> tissue 2: ...
// specificScalarParameters[ 11 ] -> tissue 2: ...
// specificScalarParameters[ 12 ] -> tissue 2: ...
// ...
//



class MSMTCSDFiberOdfCartesianField : public OdfCartesianField
{

  public:

    MSMTCSDFiberOdfCartesianField(
                    RCPointer< Volume< float > > t2,
                    RCPointer< Volume< float > > dw,
                    RCPointer< Volume< int16_t > > mask,
                    const OrientationSet* outputOrientationSet,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose = false );
    MSMTCSDFiberOdfCartesianField(
                    RCPointer< Volume< float > > t2,
                    RCPointer< Volume< float > > dw,
                    RCPointer< Volume< int16_t > > mask,
                    const BoundingBox< float >& boundingBox,
                    const Vector3d< float >& superResolution,
                    const OrientationSet* outputOrientationSet,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose = false );
    MSMTCSDFiberOdfCartesianField(
                    RCPointer< Volume< float > > t2,
                    RCPointer< Volume< float > > dw,
                    RCPointer< Volume< int16_t > > mask,
                    const BoundingBox< int32_t >& boundingBox,
                    const OrientationSet* outputOrientationSet,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose = false );
    virtual ~MSMTCSDFiberOdfCartesianField();

    const DiffusionKernel& getDiffusionKernel( int32_t tissueIndex ) const;
    int32_t getTissueCount() const;
    int32_t getMaximumSphericalHarmonicOrder( int32_t tissueIndex ) const;
    int32_t getSphericalHarmonicCoefficientCount( int32_t tissueIndex ) const;
    int32_t getAllSphericalHarmonicCoefficientCount() const;

    static void checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters );

  protected:

    void prepare( const std::vector< double >& specificScalarParameters,
                  const std::vector< std::string >& specificStringParameters,
                  bool verbose );

    void getReconstructionMatrices( bool verbose );

    OrientationDistributionFunction
      newItem( const Vector3d< int32_t >& originSite,
               const Vector3d< int32_t >& lutSite );
    OrientationDistributionFunction
      newItem( const Vector3d< float >& originSite,
               const Vector3d< int32_t >& lutSite );


    int32_t _tissueCount;
    double _cqpSolverMaximumIterationCount;
    double _cqpSolverEpsilonGap;
    double _cqpSolverEpsilonResiduals;
    double _cqpSolverEpsilonInfeasible;

    std::vector< int32_t > _kernelWantedVoxelCounts;
    std::vector< std::string > _kernelTypes;

    std::vector< std::string > _fileNameTissueClassProbabilityMaps;
    std::vector< std::string > _tissueToDiffusion3DTransformTypes;
    std::vector< std::string > _fileNameTissueToDiffusion3DLinearTransforms;
    std::vector< std::string >
                             _fileNameTissueToDiffusionDirect3DDiffeoTransforms;
    std::vector< std::string > 
                            _fileNameTissueToDiffusionInverse3DDiffeoTransforms;
    std::vector< std::string > _tissueClassThresholdingModes;
    std::vector< float > _firstTissueClassThresholds;
    std::vector< float > _secondTissueClassThresholds;
    std::vector< std::string > _faThresholdingModes;
    std::vector< std::string > _faSortingTypes;
    std::vector< float > _firstFAThresholds;
    std::vector< float > _secondFAThresholds;
    std::vector< int32_t > _maximumSphericalHarmonicOrders;

    std::vector< RCPointer< DiffusionKernel > > _diffusionKernels;
    std::vector< int32_t > _shCoefficientCounts;
    int32_t _shellCount;
    std::vector< int32_t > _measurementCounts;

    Matrix _H;
    Matrix _minusCTransposed;
    Matrix _A;

};


}


#endif
