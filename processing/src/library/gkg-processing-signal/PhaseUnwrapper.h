#ifndef _gkg_processing_signal_PhaseUnwrapper_h_
#define _gkg_processing_signal_PhaseUnwrapper_h_


#include <gkg-core-cppext/StdInt.h>
#include <string>


namespace gkg
{


template < class T > class Volume;


template < class T >
class PhaseUnwrapper
{

  public:

    virtual ~PhaseUnwrapper();

    virtual std::string getName() const = 0;

    virtual void unwrap( Volume< T >& phase, 
                         const Volume< T >& weights, 
                         bool verbose ) const = 0;
    virtual void unwrap( Volume< T >& phase, 
                         bool verbose ) const = 0;

  protected:

    PhaseUnwrapper();

};


}


#endif
