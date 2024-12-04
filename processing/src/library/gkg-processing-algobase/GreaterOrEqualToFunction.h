#ifndef _gkg_processing_algobase_GreaterOrEqualToFunction_h_
#define _gkg_processing_algobase_GreaterOrEqualToFunction_h_


#include <gkg-processing-algobase/TestFunction.h>


namespace gkg
{


template < class T >
class GreaterOrEqualToFunction : public TestFunction< T >
{

  public:

    GreaterOrEqualToFunction( const T& threshold );
    virtual ~GreaterOrEqualToFunction();

    bool getState( const T& value ) const;

  private:

    T _threshold;

};


}


#endif
