#ifndef _gkg_processing_rfpulse_CayleyKleinParameters_h_
#define _gkg_processing_rfpulse_CayleyKleinParameters_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <complex>
#include <iostream>


namespace gkg
{


class CayleyKleinParameters
{

  public:

    CayleyKleinParameters( double phi, const Vector3d< double >& rotationAxis );
    CayleyKleinParameters( const std::complex< double >& alpha,
                           const std::complex< double >& beta );
    CayleyKleinParameters();
    CayleyKleinParameters( const CayleyKleinParameters& other );
    virtual ~CayleyKleinParameters();

    CayleyKleinParameters& operator = ( const CayleyKleinParameters& other );

    const std::complex< double >& getAlpha() const;
    const std::complex< double >& getBeta() const;

    CayleyKleinParameters cross( const CayleyKleinParameters& other ) const;

  protected:

    std::complex< double > _alpha;
    std::complex< double > _beta;

};


}


namespace std
{


ostream& operator<<( ostream& os,
                     const gkg::CayleyKleinParameters& thing );


}

#endif
