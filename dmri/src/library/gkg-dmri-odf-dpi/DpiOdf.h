#ifndef _gkg_dmri_odf_dpi_DpiOdf_h_
#define _gkg_dmri_odf_dpi_DpiOdf_h_

#include <gkg-dmri-pdf-dpi/DpiEap.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-io/TypeOf.h>

namespace gkg
{

template < class T > class Volume;
class HeaderedObject;
template < class T > class Vector3d;
class Matrix;

class DpiOdf : public DpiEap
{

  public:

  // for standard estimation on the acquired grid
    DpiOdf( const OrientationSet* orientationSet,
	    const Volume< float >& t2,
	    const Volume< float >& dw,
	    const Vector3d< int32_t >& site,
	    int32_t numberOfShells,
	    int32_t numberOfOrientationsPerShell,
	    int32_t maximumSphericalHarmonicOrder,
	    float R0min,
	    float R0max 
	    );

    // for partial voluming resampled data
    DpiOdf( const OrientationSet* orientationSet,
	    const Volume< float >& t2,
	    const Volume< float >& dw,
	    const Vector3d< float >& site,
	    int32_t numberOfShells,
	    int32_t numberOfOrientationsPerShell,
	    int32_t maximumSphericalHarmonicOrder,
	    float R0min,
	    float R0max );

    // for standard estimation on the acquired grid with reconstruction matrix
    DpiOdf( const OrientationSet* orientationSet,
	    const Volume< float >& t2,
	    const Volume< float >& dw,
	    const Vector3d< int32_t >& site,
	    const Matrix& reconstructionMatrix,
	    float R0min,
	    float R0max );

    // for partial voluming resampled data with reconstruction matrix
    DpiOdf( const OrientationSet* orientationSet,
	    const Volume< float >& t2,
	    const Volume< float >& dw,
	    const Vector3d< float >& site,
	    const Matrix& reconstructionMatrix,
	    float R0min,
	    float R0max );

    virtual ~DpiOdf();

    const Vector& getOdfSphericalHarmonicsCoefficients() const;

  protected:

    Vector _odfSphericalHarmonicsCoefficients;

};


}


///////////////////////////////////////////////////////////////////////////////
// creating type name for DpiOdf
///////////////////////////////////////////////////////////////////////////////
RegisterBaseType( gkg::DpiOdf, gkg_DpiOdf );


#endif
 

