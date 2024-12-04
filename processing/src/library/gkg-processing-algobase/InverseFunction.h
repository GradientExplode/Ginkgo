#ifndef _gkg_processing_algobase_InverseFunction_h_
#define _gkg_processing_algobase_InverseFunction_h_


#include <gkg-processing-algobase/TestFunction.h>


namespace gkg
{


template < class T >
class InverseFunction : public TestFunction< T >
{

  public:

    InverseFunction( const TestFunction< T >& testFunction );
    virtual ~InverseFunction();

    bool getState( const T& value ) const;

  private:

    const TestFunction< T >& _testFunction;

};


}


#endif
