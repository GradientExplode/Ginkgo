#ifndef _gkg_processing_algobase_VolumeMinMaxFilterContext_h_
#define _gkg_processing_algobase_VolumeMinMaxFilterContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


template < class T >
class VolumeMinMaxFilterContext : public LoopContext< int32_t >
{

  public:

    VolumeMinMaxFilterContext( const Volume< T >& volume, 
                               T& minimum,
                               T& maximum );

    void doIt( int32_t startIndex, int32_t count );

  private:

    const T* _ptr0;
    T& _minimum;
    T& _maximum;

};


}


#endif
