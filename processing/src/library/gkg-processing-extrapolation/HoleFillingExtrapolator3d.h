#ifndef _gkg_processing_extrapolation_HoleFillingExtrapolator3d_h_
#define _gkg_processing_extrapolation_HoleFillingExtrapolator3d_h_


#include <gkg-processing-extrapolation/Extrapolator.h>


namespace gkg
{


template < class IN, class MASK, class OUT >
class HoleFillingExtrapolator3d : public Extrapolator< IN, MASK, OUT >
{

  public:

    HoleFillingExtrapolator3d();
    ~HoleFillingExtrapolator3d();

    void extrapolate( const Volume< IN >& input,
                      const Volume< MASK >& restrictedMask,
                      const Volume< MASK >& enlargedMask,
                      const TestFunction< MASK >& testFunction,
                      Volume< OUT >& ouput,
                      bool verbose ) const;

};


}


#endif

