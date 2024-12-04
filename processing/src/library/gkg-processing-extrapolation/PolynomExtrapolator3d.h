#ifndef _gkg_processing_extrapolation_PolynomExtrapolator3d_h_
#define _gkg_processing_extrapolation_PolynomExtrapolator3d_h_


#include <gkg-processing-extrapolation/Extrapolator.h>


namespace gkg
{


template < class IN, class MASK, class OUT >
class PolynomExtrapolator3d : public Extrapolator< IN, MASK, OUT >
{

  public:

    PolynomExtrapolator3d( int32_t degree );
    ~PolynomExtrapolator3d();

    void extrapolate( const Volume< IN >& input,
                      const Volume< MASK >& restrictedMask,
                      const Volume< MASK >& enlargedMask,
                      const TestFunction< MASK >& testFunction,
                      Volume< OUT >& ouput,
                      bool verbose = false ) const;

  private:

    int32_t _degree;

};


}


#endif

