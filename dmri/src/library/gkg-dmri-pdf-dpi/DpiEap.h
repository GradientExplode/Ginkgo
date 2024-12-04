#ifndef _gkg_dmri_pdf_dpi_DpiEap_h_
#define _gkg_dmri_pdf_dpi_DpiEap_h_

#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-io/TypeOf.h>

namespace gkg
{

template < class T > class Volume;
class HeaderedObject;
template < class T > class Vector3d;
class Matrix;

class DpiEap : public OrientationDistributionFunction
{

  public:

  // for standard estimation on the acquired grid
    DpiEap( const OrientationSet* orientationSet,
	    const Volume< float >& t2,
	    const Volume< float >& dw,
	    const Vector3d< int32_t >& site,
	    int32_t numberOfShells,
	    int32_t numberOfOrientationsPerShell,
	    int32_t maximumSphericalHarmonicOrder );

    // for partial voluming resampled data
    DpiEap( const OrientationSet* orientationSet,
	    const Volume< float >& t2,
	    const Volume< float >& dw,
	    const Vector3d< float >& site,
	    int32_t numberOfShells,
	    int32_t numberOfOrientationsPerShell,
	    int32_t maximumSphericalHarmonicOrder );

    // for standard estimation on the acquired grid with reconstruction matrix
    DpiEap( const OrientationSet* orientationSet,
	    const Volume< float >& t2,
	    const Volume< float >& dw,
	    const Vector3d< int32_t >& site,
	    const std::vector< float >& qValues,
	    const Matrix& reconstructionMatrix );

    // for partial voluming resampled data with reconstruction matrix
    DpiEap( const OrientationSet* orientationSet,
	    const Volume< float >& t2,
	    const Volume< float >& dw,
	    const Vector3d< float >& site,
	    const std::vector< float >& qValues,
	    const Matrix& reconstructionMatrix );
    virtual ~DpiEap();

    const Vector& getDwSphericalHarmonicsCoefficients() const;
    float getAverageDwSignal();
    float getAverageNormalizedDwSignal();
    float getAnalyticalEapAtZero();
    float getAnalyticalMeanSquaredDisplacement();

  protected:

    void getReconstructionMatrix( const HeaderedObject& headeredObject,
				  std::vector< double > bValues,
				  int32_t numberOfShells,
				  int32_t numberOfOrientationsPerShell,
                                  int32_t maximumSphericalHarmonicOrder,
                                  Matrix& reconstructionMatrix );

    Vector               _dwSphericalHarmonicsCoefficients;
    float                _averageDwSignal;
    float                _averageNormalizedDwSignal;
    float                _averageT2;
    std::vector< float > _qValues;

};


}


///////////////////////////////////////////////////////////////////////////////
// creating type name for DpiEap
///////////////////////////////////////////////////////////////////////////////

RegisterBaseType( gkg::DpiEap, gkg_DpiEap );


#endif
 
