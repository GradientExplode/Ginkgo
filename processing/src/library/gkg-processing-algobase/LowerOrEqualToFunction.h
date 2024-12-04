#ifndef _gkg_processing_algobase_LowerOrEqualToFunction_h_
#define _gkg_processing_algobase_LowerOrEqualToFunction_h_


#include <gkg-processing-algobase/TestFunction.h>


namespace gkg
{


template < class T >
class LowerOrEqualToFunction : public TestFunction< T >
{

  public:

    LowerOrEqualToFunction( const T& threshold );
    virtual ~LowerOrEqualToFunction();

    bool getState( const T& value ) const;

  private:

    T _threshold;

};


}


#endif
