#ifndef _gkg_dmri_odf_dot_DotOdf_h_
#define _gkg_dmri_odf_dot_DotOdf_h_


#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-coordinates/Vector2d.h>
#include <gkg-core-io/TypeOf.h>


namespace gkg
{

template < class T > class Volume;
class HeaderedObject;
template < class T > class Vector3d;
class Matrix;


class DotOdf : public OrientationDistributionFunction
{

  public:

    template < class S >
    DotOdf( const OrientationSet* orientationSet,
            const Volume< float >& t2,
            const Volume< float >& dw,
            const Vector3d< S >& site,
	    int32_t maximumSphericalHarmonicOrder,
            float effectiveDiffusionTime,
            float R0,
            bool marginalODF );

    template < class S >
    DotOdf( const OrientationSet* orientationSet,
            const Volume< float >& t2,
            const Volume< float >& dw,
            const Vector3d< S >& site,
	    float effectiveDiffusionTime,
            float R0,
            bool marginalODF,
	    const Matrix& reconstructionMatrix,
	    const Matrix& Bplus );

    virtual ~DotOdf();

    const Vector& getDwSphericalHarmonicsCoefficients() const;
    const Vector& getAdcSphericalHarmonicsCoefficients() const;
    const Vector& getAdc() const;

  protected:

    void getReconstructionMatrix( 
			 const HeaderedObject& headeredObject,
			 int32_t maximumSphericalHarmonicOrder,
			 Matrix& reconstructionMatrix,
			 Matrix& Bplus  );

    template < class S >
    void reconstruct( const OrientationSet* orientationSet,
                      const Volume< float >& t2,
                      const Volume< float >& dw,
                      const Vector3d< S >& site,
	              float effectiveDiffusionTime,
                      float R0,
                      bool marginalODF,
                      const Matrix& reconstructionMatrix,
                      const Matrix& Bplus );


    void performDOT( const Matrix& reconstructionMatrix,
		     const Matrix& Bplus,
		     float effectiveDiffusionTime,
		     float R0,
		     bool marginalODF );

    void getIMatrix( int32_t maxSphericalHarmonicOrder,
		     int32_t orientationCount,
		     float effectiveDiffusionTime, float R0,
		     Matrix& IMatrix );
		     
    double dotIntegral( float adc, int32_t order,
			float effectiveDiffusionTime, float R0 );

    
    void performMarginalOdfComputation( const int32_t shCount,
					const int32_t maxOrder,
					const int32_t orientationCount,
					const Matrix& reconstructionMatrix,
					const Matrix& Bplus,
					float effectiveDiffusionTime,
					float R0 );


    Vector _dwSphericalHarmonicsCoefficients;
    Vector _adcSphericalHarmonicsCoefficients;
    Vector _adc;

};


}


///////////////////////////////////////////////////////////////////////////////
// creating type name for DotOdf
///////////////////////////////////////////////////////////////////////////////

RegisterBaseType( gkg::DotOdf, gkg_DotOdf );


#endif


