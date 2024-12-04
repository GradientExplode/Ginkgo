#ifndef _gkg_processing_algobase_FilterWithMaskFunction_h_
#define _gkg_processing_algobase_FilterWithMaskFunction_h_


#include <gkg-processing-algobase/TestFunction.h>


namespace gkg
{


//
// class FilterWithMaskFunction< IN,
//                               MASK,
//                               MASK_ITEM,
//                               OUT >
//

template < class IN, class MASK, class MASK_ITEM, class OUT >
class FilterWithMaskFunction
{

  public:

    virtual ~FilterWithMaskFunction();

    virtual void filter( const IN& in,
                         const MASK& mask,
                         const TestFunction< MASK_ITEM >& testFunction,
                         OUT& out ) const = 0;

};


}


#endif




