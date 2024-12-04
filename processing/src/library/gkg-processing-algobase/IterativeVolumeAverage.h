#ifndef _gkg_processing_algobase_IterativeVolumeAverage_h_
#define _gkg_processing_algobase_IterativeVolumeAverage_h_


#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


template < class T >
class IterativeVolumeAverage
{

  public:

    IterativeVolumeAverage();
    IterativeVolumeAverage( const IterativeVolumeAverage< T >& other );
    IterativeVolumeAverage( Volume< T >& volume );
    virtual ~IterativeVolumeAverage();

    void addVolume( Volume< T >& volume );
    RCPointer< Volume< T > > getVolume();

  protected:

    RCPointer< Volume< T > > _volume;
    uint32_t _volumeCount;

};


}


#endif
