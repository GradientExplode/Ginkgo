#ifndef _gkg_dmri_odf_sa_aqbi_SolidAngleAnalyticalQBallOdf_h_
#define _gkg_dmri_odf_sa_aqbi_SolidAngleAnalyticalQBallOdf_h_


#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-io/TypeOf.h>


namespace gkg
{


template < class T > class Volume;
class HeaderedObject;
template < class T > class Vector3d;
class Matrix;


class SolidAngleAnalyticalQBallOdf : public OrientationDistributionFunction
{

  public:

    template < class S >
    SolidAngleAnalyticalQBallOdf( const OrientationSet* orientationSet,
                                  const Volume< float >& t2,
                                  const Volume< float >& dw,
                                  const Vector3d< S >& site,
                                  int32_t maximumSphericalHarmonicOrder,
                                  float regularizationLCurveFactor,
                                  float laplacianSharpening );

    template < class S >
    SolidAngleAnalyticalQBallOdf( const OrientationSet* orientationSet,
                                  const Volume< float >& t2,
                                  const Volume< float >& dw,
                                  const Vector3d< S >& site,
                                  const Matrix& reconstructionMatrix,
                                  const Matrix& Bplus );

    virtual ~SolidAngleAnalyticalQBallOdf();

    const Vector& getDwSphericalHarmonicsCoefficients() const;

  protected:

    void getReconstructionMatrix( const HeaderedObject& headeredObject,
                                  int32_t maximumSphericalHarmonicOrder,
                                  float regularizationLCurveFactor,
                                  float laplacianSharpening,
                                  Matrix& reconstructionMatrix,
                                  Matrix& Bplus );
    template < class S >
    void reconstruct( const OrientationSet* orientationSet,
                      const Volume< float >& t2,
                      const Volume< float >& dw,
                      const Vector3d< S >& site,
                      const Matrix& reconstructionMatrix,
                      const Matrix& Bplus );

    Vector _dwSphericalHarmonicsCoefficients;

};


}


///////////////////////////////////////////////////////////////////////////////
// creating type name for SolidAngleAnalyticalQBallOdf
///////////////////////////////////////////////////////////////////////////////

RegisterBaseType( gkg::SolidAngleAnalyticalQBallOdf,
                  gkg_SolidAngleAnalyticalQBallOdf );


#endif
