#ifndef _gkg_processing_algobase_BetweenOrEqualToFunction_h_
#define _gkg_processing_algobase_BetweenOrEqualToFunction_h_


#include <gkg-processing-algobase/TestFunction.h>


namespace gkg
{


template < class T >
class BetweenOrEqualToFunction : public TestFunction< T >
{

  public:

    BetweenOrEqualToFunction( const T& lowerThreshold,
                              const T& upperThreshold );
    virtual ~BetweenOrEqualToFunction();

    bool getState( const T& value ) const;

  private:

    T _lowerThreshold;
    T _upperThreshold;

};


}


#endif
