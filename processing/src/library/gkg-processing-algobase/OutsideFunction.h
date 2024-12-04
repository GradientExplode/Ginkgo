#ifndef _gkg_processing_algobase_OutsideFunction_h_
#define _gkg_processing_algobase_OutsideFunction_h_


#include <gkg-processing-algobase/TestFunction.h>


namespace gkg
{


template < class T >
class OutsideFunction : public TestFunction< T >
{

  public:

    OutsideFunction( const T& lowerThreshold,
                     const T& upperThreshold );
    virtual ~OutsideFunction();

    bool getState( const T& value ) const;

  private:

    T _lowerThreshold;
    T _upperThreshold;

};


}


#endif
