#ifndef _gkg_dmri_odf_dsi_DsiOdf_h_
#define _gkg_dmri_odf_dsi_DsiOdf_h_


#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-io/TypeOf.h>


namespace gkg
{

template < class T > class Volume;
class HeaderedObject;
template < class T > class Vector3d;
class Matrix;

class DsiOdf : public OrientationDistributionFunction
{

  public:

    // for standard estimation on the acquired grid
    DsiOdf( const OrientationSet* inputOrientationSet,  
            const OrientationSet* outputOrientationSet,
            const Volume< float >& t2,
            const Volume< float >& dw,
            const Vector3d< int32_t >& site,
            bool filterOrNot,
            float R0min, float R0max, bool marginalODF );

    // for partial voluming resampled data
    DsiOdf( const OrientationSet* inputOrientationSet,  
            const OrientationSet* ouputOrientationSet,
            const Volume< float >& t2,
            const Volume< float >& dw,
            const Vector3d< float >& site,
            bool filterOrNot,
            float R0min, float R0max, bool marginalODF );

    // for standard estimation on the acquired grid with reconstruction matrix
    DsiOdf( const OrientationSet* inputOrientationSet,  
            const OrientationSet* outputOrientationSet,
            const Volume< float >& t2,
            const Volume< float >& dw,
            const Vector3d< int32_t >& site,
            const Volume< float >& filter,
            float R0min, float R0max, bool marginalODF );

    // for partial voluming resampled data with reconstruction matrix
    DsiOdf( const OrientationSet* inputOrientationSet,
            const OrientationSet* outputOrientationSet,
            const Volume< float >& t2,
            const Volume< float >& dw,
            const Vector3d< float >& site,
            const Volume< float >& filter,
            float R0min, float R0max, bool marginalODF  );

    virtual ~DsiOdf();

    float getAverageDwSignal();
    float getAverageNormalizedDwSignal();
    float getMeanSquaredDisplacement();

  protected:

    void prepareForDSI( bool filterOrNot,
                        Volume< float >& filter );

    void performDSI( const OrientationSet* inputOrientationSet,  
                     const Vector& normalizedSignal,
                     const Volume< float >& filter,
                     const float R0min, const float R0max,
                     const bool marginalODF );

    void interpolatePDF( const Volume< float >& pdf,
                         const float radius,
                         std::vector< float > & odfValues,
                         const bool marginalODF );

    float _averageDwSignal;
    float _averageNormalizedDwSignal;
    float _meanSquaredDisplacement;
    
};


}


///////////////////////////////////////////////////////////////////////////////
// creating type name for DsiOdf
///////////////////////////////////////////////////////////////////////////////
RegisterBaseType( gkg::DsiOdf, gkg_DsiOdf );


#endif
 
