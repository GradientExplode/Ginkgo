#ifndef _gkg_dmri_pdf_shore_ShorePdfCartesianField_h_
#define _gkg_dmri_pdf_shore_ShorePdfCartesianField_h_


#include <gkg-dmri-container/PdfCartesianField.h>
#include <gkg-dmri-pdf-dti/TensorCartesianField.h>
#include <gkg-dmri-qspace-sampling/MultipleSameShellQSpaceSampling.h>
#include <gkg-core-pattern/RCPointer.h>



namespace gkg
{


class ShorePdfCartesianField : public PdfCartesianField
{

  public:

    ShorePdfCartesianField(
           RCPointer< Volume< float > > t2,
           RCPointer< Volume< float > > dw,
           RCPointer< Volume< int16_t > > mask,
           const std::map< float, OrientationSet >* outputMultipleShellSampling,
           const std::vector< double >& specificScalarParameters,
           const std::vector< std::string >& specificStringParameters,
           bool verbose = false );
    ShorePdfCartesianField(
           RCPointer< Volume< float > > t2,
           RCPointer< Volume< float > > dw,
           RCPointer< Volume< int16_t > > mask,
           const BoundingBox< float >& boundingBox,
           const Vector3d< float >& superResolution,
           const std::map< float, OrientationSet >* outputMultipleShellSampling,
           const std::vector< double >& specificScalarParameters,
           const std::vector< std::string >& specificStringParameters,
           bool verbose = false );
    ShorePdfCartesianField(
           RCPointer< Volume< float > > t2,
           RCPointer< Volume< float > > dw,
           RCPointer< Volume< int16_t > > mask,
           const BoundingBox< int32_t >& boundingBox,
           const std::map< float, OrientationSet >* outputMultipleShellSampling,
           const std::vector< double >& specificScalarParameters,
           const std::vector< std::string >& specificStringParameters,
           bool verbose = false );

    ShorePdfCartesianField(
           RCPointer< Volume< float > > t2,
           RCPointer< Volume< float > > dw,
           RCPointer< Volume< int16_t > > mask,
           const Vector3d< int32_t >* outputCartesianGridSize,
           const Vector3d< double >* outputCartesianGridResolution,
           const std::vector< double >& specificScalarParameters,
           const std::vector< std::string >& specificStringParameters,
           bool verbose = false );
    ShorePdfCartesianField(
           RCPointer< Volume< float > > t2,
           RCPointer< Volume< float > > dw,
           RCPointer< Volume< int16_t > > mask,
           const BoundingBox< float >& boundingBox,
           const Vector3d< float >& superResolution,
           const Vector3d< int32_t >* outputCartesianGridSize,
           const Vector3d< double >* outputCartesianGridResolution,
           const std::vector< double >& specificScalarParameters,
           const std::vector< std::string >& specificStringParameters,
           bool verbose = false );
    ShorePdfCartesianField(
           RCPointer< Volume< float > > t2,
           RCPointer< Volume< float > > dw,
           RCPointer< Volume< int16_t > > mask,
           const BoundingBox< int32_t >& boundingBox,
           const Vector3d< int32_t >* outputCartesianGridSize,
           const Vector3d< double >* outputCartesianGridResolution,
           const std::vector< double >& specificScalarParameters,
           const std::vector< std::string >& specificStringParameters,
           bool verbose = false );

    virtual ~ShorePdfCartesianField();

    int32_t getMaximumPolarHarmonicsOrder() const;
    float getRegularizationLCurveFactor() const;

    static void checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters );

  protected:

    void initialize( 
                   const std::vector< double >& specificScalarParameters,
                   const std::vector< std::string >& specificStringParameters );

    //void calculatingOnGridTmp( const BoundingBox< int32_t >& boundingBox,
    //                           bool verbose );

    ProbabilityDensityFunction
      newItem( const Vector3d< int32_t >& originSite,
               const Vector3d< int32_t >& lutSite );
    ProbabilityDensityFunction
      newItem( const Vector3d< float >& originSite,
               const Vector3d< int32_t >& lutSite );

    int32_t _maximumPolarHarmonicsOrder;
    float _regularizationLCurveFactor;

    RCPointer< TensorCartesianField > _tensorCartesianField;
    RCPointer< MultipleShellQSpaceSampling > _inputMultipleShellQSpaceSampling;

};


}


#endif

