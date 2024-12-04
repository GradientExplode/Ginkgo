#ifndef _gkg_processing_algobase_VolumeMaximumFilterContext_h_
#define _gkg_processing_algobase_VolumeMaximumFilterContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


template < class T >
class VolumeMaximumFilterContext : public LoopContext< int32_t >
{

  public:

    VolumeMaximumFilterContext( const Volume< T >& volume, T& maximum );

    void doIt( int32_t startIndex, int32_t count );

  private:

    const Volume< T >& _volume;
    T& _maximum;

};


}


#endif
