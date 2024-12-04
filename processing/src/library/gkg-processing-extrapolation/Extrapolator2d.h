#ifndef _gkg_processing_extrapolation_Extrapolator2d_h_
#define _gkg_processing_extrapolation_Extrapolator2d_h_


#include <gkg-processing-extrapolation/Extrapolator.h>


namespace gkg
{


class ExtrapolatorFunction;


template < class IN, class MASK, class OUT >
class Extrapolator2d : public Extrapolator< IN, MASK, OUT >
{

  public:

    Extrapolator2d( int32_t degree,
                    const ExtrapolatorFunction& extrapolatorFunction );
    ~Extrapolator2d();

    void extrapolate( const Volume< IN >& input,
                      const Volume< MASK >& restrictedMask,
                      const Volume< MASK >& enlargedMask,
                      const TestFunction< MASK >& testFunction,
                      Volume< OUT >& ouput,
                      bool verbose = false ) const;

  private:

    int32_t _degree;
    const ExtrapolatorFunction& _extrapolatorFunction;

};


}


#endif
