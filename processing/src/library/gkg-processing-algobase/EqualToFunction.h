#ifndef _gkg_processing_algobase_EqualToFunction_h_
#define _gkg_processing_algobase_EqualToFunction_h_


#include <gkg-processing-algobase/TestFunction.h>


namespace gkg
{


template < class T >
class EqualToFunction : public TestFunction< T >
{

  public:

    EqualToFunction( const T& threshold );
    virtual ~EqualToFunction();

    bool getState( const T& value ) const;

  private:

    T _threshold;

};


}


#endif
