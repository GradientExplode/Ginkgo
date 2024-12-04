#ifndef _gkg_processing_algobase_LowerThanFunction_h_
#define _gkg_processing_algobase_LowerThanFunction_h_


#include <gkg-processing-algobase/TestFunction.h>


namespace gkg
{


template < class T >
class LowerThanFunction : public TestFunction< T >
{

  public:

    LowerThanFunction( const T& threshold );
    virtual ~LowerThanFunction();

    bool getState( const T& value ) const;

  private:

    T _threshold;

};


}


#endif
