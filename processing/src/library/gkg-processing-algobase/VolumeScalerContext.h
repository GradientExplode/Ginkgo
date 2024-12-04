#ifndef _gkg_processing_algobase_VolumeScalerContext_h_
#define _gkg_processing_algobase_VolumeScalerContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-algobase/Scaler.h>


namespace gkg
{


template < class T >
class VolumeScalerContext : public LoopContext< int32_t >
{

  public:

    VolumeScalerContext( Volume< T >& in, const Scaler< T >& itemScaler );
			     
    void doIt( int32_t startIndex, int32_t count );

  private:

    Volume< T >& _in;
    const Scaler< T >& _itemScaler;

};


}


#endif
