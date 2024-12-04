#ifndef _gkg_processing_algobase_OutsideOrEqualToFunction_h_
#define _gkg_processing_algobase_OutsideOrEqualToFunction_h_


#include <gkg-processing-algobase/TestFunction.h>


namespace gkg
{


template < class T >
class OutsideOrEqualToFunction : public TestFunction< T >
{

  public:

    OutsideOrEqualToFunction( const T& lowerThreshold,
                              const T& upperThreshold );
    virtual ~OutsideOrEqualToFunction();

    bool getState( const T& value ) const;

  private:

    T _lowerThreshold;
    T _upperThreshold;

};


}


#endif
