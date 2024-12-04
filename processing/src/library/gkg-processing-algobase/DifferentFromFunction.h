#ifndef _gkg_processing_algobase_DifferentFromFunction_h_
#define _gkg_processing_algobase_DifferentFromFunction_h_


#include <gkg-processing-algobase/TestFunction.h>


namespace gkg
{


template < class T >
class DifferentFromFunction : public TestFunction< T >
{

  public:

    DifferentFromFunction( const T& threshold );
    virtual ~DifferentFromFunction();

    bool getState( const T& value ) const;

  private:

    T _threshold;

};


}


#endif
