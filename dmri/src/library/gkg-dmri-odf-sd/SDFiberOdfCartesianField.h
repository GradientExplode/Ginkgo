#ifndef _gkg_dmri_odf_sd_SDFiberOdfCartesianField_h_
#define _gkg_dmri_odf_sd_SDFiberOdfCartesianField_h_


#include <gkg-dmri-odf-sd/SphericalDeconvolutionFiberOdf.h>
#include <gkg-dmri-container/OdfCartesianField.h>
#include <gkg-dmri-diffusion-kernel/DiffusionKernel.h>
#include <gkg-processing-numericalanalysis/Matrix.h>

namespace gkg
{


class SDFiberOdfCartesianField : public OdfCartesianField
{

  public:

    SDFiberOdfCartesianField(
                    RCPointer< Volume< float > > t2,
                    RCPointer< Volume< float > > dw,
                    RCPointer< Volume< int16_t > > mask,
                    const OrientationSet* outputOrientationSet,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose = false );
    SDFiberOdfCartesianField(
                    RCPointer< Volume< float > > t2,
                    RCPointer< Volume< float > > dw,
                    RCPointer< Volume< int16_t > > mask,
                    const BoundingBox< float >& boundingBox,
                    const Vector3d< float >& superResolution,
                    const OrientationSet* outputOrientationSet,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose = false );
    SDFiberOdfCartesianField(
                    RCPointer< Volume< float > > t2,
                    RCPointer< Volume< float > > dw,
                    RCPointer< Volume< int16_t > > mask,
                    const BoundingBox< int32_t >& boundingBox,
                    const OrientationSet* outputOrientationSet,
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters,
                    bool verbose = false );
    virtual ~SDFiberOdfCartesianField();

    const DiffusionKernel& getDiffusionKernel() const;
    int32_t getMaximumSphericalHarmonicOrder() const;
    const Vector& getFilteringCoefficients() const;
    bool getConstrainedSuperResolution() const;
    int32_t getSphericalHarmonicCount() const;

    const Matrix& getReconstructionMatrix() const;
    const Matrix& getB() const;
    const Matrix& getBplus() const;
    const Matrix& getRegularizationBMatrix() const;
    const Matrix& getConvolutionMatrix() const;

    static void checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters );

  protected:

    void getReconstructionMatrix( bool verbose );
    void getRegularizationMatrices( bool verbose );

    OrientationDistributionFunction
      newItem( const Vector3d< int32_t >& originSite,
               const Vector3d< int32_t >& lutSite );
    OrientationDistributionFunction
      newItem( const Vector3d< float >& originSite,
               const Vector3d< int32_t >& lutSite );

    DiffusionKernel _diffusionKernel;

    int32_t _maximumSphericalHarmonicOrder;
    Vector _filteringCoefficients;
    bool _constrainedSphericalDeconvolution;
    bool _superResolution;

    Matrix _reconstructionMatrix;
    Matrix _B;
    Matrix _Bplus;
    Matrix _regularizationBMatrix;
    Matrix _convolutionMatrix;

};


}


#endif




