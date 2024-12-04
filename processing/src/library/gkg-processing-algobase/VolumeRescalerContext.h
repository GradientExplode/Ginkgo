#ifndef _gkg_processing_algobase_VolumeRescalerContext_h_
#define _gkg_processing_algobase_VolumeRescalerContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-algobase/Rescaler.h>


namespace gkg
{


template < class IN, class OUT >
class VolumeRescalerContext : public LoopContext< int32_t >
{

  public:

    VolumeRescalerContext( const Volume< IN >& in,
                           Volume< OUT >& out,
                           const Rescaler< IN, OUT >& itemRescaler );
			     
    void doIt( int32_t startIndex, int32_t count );

  private:

    const Volume< IN >& _in;
    Volume< OUT >& _out;
    const Rescaler< IN, OUT >& _itemRescaler;

};


}


#endif
