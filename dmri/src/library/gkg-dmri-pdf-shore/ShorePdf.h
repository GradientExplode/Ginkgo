#ifndef _gkg_dmri_pdf_shore_ShorePdf_h_
#define _gkg_dmri_pdf_shore_ShorePdf_h_


#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-dmri-pdf-dti/Tensor.h>
#include <gkg-dmri-ph-basis/PolarHarmonicsCache.h>
#include <gkg-dmri-qspace-sampling/MultipleShellQSpaceSampling.h>


namespace gkg
{


class MultipleDifferentShellQSpaceSampling;


class ShorePdf : public ProbabilityDensityFunction
{

  public:

    template < class S >
    ShorePdf(
           const std::map< float, OrientationSet >* outputMultipleShellSampling,
           const Volume< float >& t2,
	   const Volume< float >& dw,
	   const Vector3d< S >& site,
           int32_t maximumPolarHarmonicsOrder,
           float regularizationLCurveFactor,
           const Tensor& tensor );

    template < class S >
    ShorePdf( const Vector3d< int32_t >* outputCartesianGridSize,
              const Vector3d< double >* outputCartesianGridResolution,
              const Volume< float >& t2,
	      const Volume< float >& dw,
	      const Vector3d< S >& site,
              int32_t maximumPolarHarmonicsOrder,
              float regularizationLCurveFactor,
              const Tensor& tensor );

    template < class S >
    ShorePdf(
           const std::map< float, OrientationSet >* outputMultipleShellSampling,
           const Volume< float >& t2,
	   const Volume< float >& dw,
	   const Vector3d< S >& site,
           const MultipleShellQSpaceSampling& inputMultipleShellQSpaceSampling,
           int32_t maximumPolarHarmonicsOrder,
           float regularizationLCurveFactor,
           const Tensor& tensor );

    template < class S >
    ShorePdf(
            const Vector3d< int32_t >* outputCartesianGridSize,
            const Vector3d< double >* outputCartesianGridResolution,
            const Volume< float >& t2,
	    const Volume< float >& dw,
	    const Vector3d< S >& site,
            const MultipleShellQSpaceSampling& inputMultipleShellQSpaceSampling,
            int32_t maximumPolarHarmonicsOrder,
            float regularizationLCurveFactor,
            const Tensor& tensor );

    virtual ~ShorePdf();

  protected:
 
    MultipleShellQSpaceSampling* getInputMultipleShellQSpaceSampling( 
                                              const Volume< float >& dw ) const;
    template < class S >
    void reconstruct( const MultipleShellQSpaceSampling& 
                                               inputMultipleShellQSpaceSampling,
                      const Volume< float >& t2,
                      const Volume< float >& dw,
                      const Vector3d< S >& site,
                      int32_t maximumPolarHarmonicsOrder,
                      float regularizationLCurveFactor,
                      const Tensor& tensor );

    void getLaplaceBeltramiRegularizationMatrix(
                           NumericalAnalysisImplementationFactory* factory,
                           const PolarHarmonicsCache& polarHarmonicsCache,
                           const Vector3d< float >& scalings,
                           Matrix& L ) const;
    float getLaplaceS( NumericalAnalysisImplementationFactory* factory,
                       int32_t n, int32_t m ) const;
    float getLaplaceT( int32_t n, int32_t m ) const;
    float getLaplaceU( int32_t n, int32_t m ) const;

    static float _piToThePowerThreeAndAHalf;
    static float _piToThePowerOneAndAHalf;
    static float _piToThePowerOneHalf;

};


}


///////////////////////////////////////////////////////////////////////////////
// creating type name for ShorePdf
///////////////////////////////////////////////////////////////////////////////

RegisterBaseType( gkg::ShorePdf, gkg_ShorePdf );


#endif
