#ifndef _gkg_processing_algobase_VolumeThresholderContext_h_
#define _gkg_processing_algobase_VolumeThresholderContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-algobase/Thresholder.h>


namespace gkg
{


template < class T >
class VolumeThresholderContext : public LoopContext< int32_t >
{

  public:

    VolumeThresholderContext( const Volume< T >& in,
                              Volume< T >& out,
                              const Thresholder< T >& itemThresholder );
			     
    void doIt( int32_t startIndex, int32_t count );

  private:

    const Volume< T >& _in;
    Volume< T >& _out;
    const Thresholder< T >& _itemThresholder;

};


}


#endif
