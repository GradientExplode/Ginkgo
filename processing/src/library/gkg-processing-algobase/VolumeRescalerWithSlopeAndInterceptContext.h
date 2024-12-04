#ifndef _gkg_processing_algobase_VolumeRescalerWithSlopeAndInterceptContext_h_
#define _gkg_processing_algobase_VolumeRescalerWithSlopeAndInterceptContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-algobase/RescalerWithSlopeAndIntercept.h>


namespace gkg
{


template < class IN, class OUT >
class VolumeRescalerWithSlopeAndInterceptContext : public LoopContext< int32_t >
{

  public:

    VolumeRescalerWithSlopeAndInterceptContext(
                             const Volume< IN >& in,
                             Volume< OUT >& out,
                             const RescalerWithSlopeAndIntercept< IN, OUT >&
                                            itemRescalerWithSlopeAndIntercept );
			     
    void doIt( int32_t startIndex, int32_t count );

  private:

    const Volume< IN >& _in;
    Volume< OUT >& _out;
    const RescalerWithSlopeAndIntercept< IN, OUT >& 
      _itemRescalerWithSlopeAndIntercept;

};


}


#endif
