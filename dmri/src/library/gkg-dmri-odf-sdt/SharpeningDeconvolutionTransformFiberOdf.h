#ifndef _gkg_dmri_odf_sdt_SharpeningDeconvolutionTransformFiberOdf_h_
#define _gkg_dmri_odf_sdt_SharpeningDeconvolutionTransformFiberOdf_h_


#include <gkg-dmri-odf/SHFiberOdf.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-io/TypeOf.h>


namespace gkg
{


template < class T > class Volume;
class HeaderedObject;
template < class T > class Vector3d;
class DiffusionKernel;
class Matrix;


class SharpeningDeconvolutionTransformFiberOdf : public SHFiberOdf
{

  public:

    template < class S >
    SharpeningDeconvolutionTransformFiberOdf(
                                      const OrientationSet* orientationSet,
                                      const Volume< float >& t2,
                                      const Volume< float >& dw,
                                      const Vector3d< S >& site,
                                      const DiffusionKernel& diffusionKernel,
                                      int32_t maximumSphericalHarmonicOrder,
                                      float regularizationLCurveFactor,
                                      bool constrainedSphericalDeconvolution,
                                      bool superResolution );

    template < class S >
    SharpeningDeconvolutionTransformFiberOdf(
                                      const OrientationSet* orientationSet,
                                      const Volume< float >& t2,
                                      const Volume< float >& dw,
                                      const Vector3d< S >& site,
                                      const Matrix& reconstructionMatrix,
                                      const Matrix& Bplus );

    template < class S >
    SharpeningDeconvolutionTransformFiberOdf(
                                      const OrientationSet* orientationSet,
                                      const Volume< float >& t2,
                                      const Volume< float >& dw,
                                      const Vector3d< S >& site,
                                      const Matrix& reconstructionMatrix,
                                      const Matrix& Bplus,
                                      const Matrix& B,
                                      const Matrix& regularizationMatrix,
                                      const Matrix& convolutionMatrix,
                                      bool superResolution );

    virtual ~SharpeningDeconvolutionTransformFiberOdf();

    const Vector& getDwSphericalHarmonicsCoefficients() const;

  protected:

    void getReconstructionMatrix( const HeaderedObject& headeredObject,
                                  const DiffusionKernel& diffusionKernel,
                                  int32_t maximumSphericalHarmonicOrder,
                                  float regularizationLCurveFactor,
                                  Matrix& reconstructionMatrix,
                                  Matrix& B,
                                  Matrix& Bplus );

    template < class S >
    bool reconstructUnregularized( const OrientationSet* orientationSet,
                                   const Volume< float >& t2,
                                   const Volume< float >& dw,
                                   const Vector3d< S >& site,
                                   const Matrix& reconstructionMatrix,
                                   const Matrix& Bplus,
                                   Vector& normalizedSignal );

    Vector _dwSphericalHarmonicsCoefficients;

};


}


///////////////////////////////////////////////////////////////////////////////
// creating type name for SharpeningDeconvolutionTransformFiberOdf
///////////////////////////////////////////////////////////////////////////////

RegisterBaseType( gkg::SharpeningDeconvolutionTransformFiberOdf, 
                  gkg_SharpeningDeconvolutionTransformFiberOdf );


#endif
