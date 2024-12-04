#ifndef _gkg_processing_extrapolation_Extrapolator_h_
#define _gkg_processing_extrapolation_Extrapolator_h_


namespace gkg
{


template < class T > class Volume;
template < class T > class TestFunction;


template < class IN, class MASK, class OUT >
class Extrapolator
{

  public:

    virtual ~Extrapolator();

    virtual void extrapolate( const Volume< IN >& input,
                              const Volume< MASK >& restrictedMask,
                              const Volume< MASK >& enlargedMask,
                              const TestFunction< MASK >& testFunction,
                              Volume< OUT >& ouput,
                              bool verbose ) const = 0;

  protected:

    Extrapolator();

};


}


#endif
