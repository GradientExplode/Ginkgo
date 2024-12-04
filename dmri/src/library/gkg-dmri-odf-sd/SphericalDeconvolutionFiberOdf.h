#ifndef _gkg_dmri_odf_sd_SphericalDeconvolutionFiberOdf_h_
#define _gkg_dmri_odf_sd_SphericalDeconvolutionFiberOdf_h_


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


class SphericalDeconvolutionFiberOdf : public SHFiberOdf
{

  public:

    template < class S >
    SphericalDeconvolutionFiberOdf( const OrientationSet* orientationSet,
                                    const Volume< float >& t2,
                                    const Volume< float >& dw,
                                    const Vector3d< S >& site,
                                    const DiffusionKernel& kernel,
                                    int32_t maximumSphericalHarmonicOrder,
                                    const Vector& filteringCoefficients,
                                    bool constrainedSphericalDeconvolution,
                                    bool superResolution );

    template < class S >
    SphericalDeconvolutionFiberOdf( const OrientationSet* orientationSet,
                                    const Volume< float >& t2,
                                    const Volume< float >& dw,
                                    const Vector3d< S >& site,
                                    const Matrix& reconstructionMatrix,
                                    const Matrix& Bplus );

    template < class S >
    SphericalDeconvolutionFiberOdf( const OrientationSet* orientationSet,
                                    const Volume< float >& t2,
                                    const Volume< float >& dw,
                                    const Vector3d< S >& site,
                                    const Matrix& reconstructionMatrix,
                                    const Matrix& B,
                                    const Matrix& Bplus,
                                    const Matrix& regularizationMatrix,
                                    const Matrix& convolutionMatrix,
                                    bool superResolution );

    virtual ~SphericalDeconvolutionFiberOdf();

    const Vector& getDwSphericalHarmonicsCoefficients() const;

  protected:

    void getReconstructionMatrix( const HeaderedObject& headeredObject,
                                  const DiffusionKernel& diffusionKernel,
                                  int32_t maximumSphericalHarmonicOrder,
                                  const Vector& filteringCoefficients,
                                  double bValue,
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
// creating type name for SphericalDeconvolutionFiberOdf 
///////////////////////////////////////////////////////////////////////////////

RegisterBaseType( gkg::SphericalDeconvolutionFiberOdf,
                  gkg_SphericalDeconvolutionFiberOdf );


#endif

