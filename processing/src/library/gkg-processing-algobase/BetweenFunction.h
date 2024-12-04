#ifndef _gkg_processing_algobase_BetweenFunction_h_
#define _gkg_processing_algobase_BetweenFunction_h_


#include <gkg-processing-algobase/TestFunction.h>


namespace gkg
{


template < class T >
class BetweenFunction : public TestFunction< T >
{

  public:

    BetweenFunction( const T& lowerThreshold,
                     const T& upperThreshold );
    virtual ~BetweenFunction();

    bool getState( const T& value ) const;

  private:

    T _lowerThreshold;
    T _upperThreshold;

};


}


#endif
