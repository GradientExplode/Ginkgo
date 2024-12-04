#ifndef _gkg_processing_algobase_GreaterThanFunction_h_
#define _gkg_processing_algobase_GreaterThanFunction_h_


#include <gkg-processing-algobase/TestFunction.h>


namespace gkg
{


template < class T >
class GreaterThanFunction : public TestFunction< T >
{

  public:

    GreaterThanFunction( const T& threshold );
    virtual ~GreaterThanFunction();

    bool getState( const T& value ) const;

  private:

    T _threshold;

};


}


#endif
