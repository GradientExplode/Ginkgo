#ifndef _gkg_processing_algobase_VolumeMinMaxFilterWithMaskContext_h_
#define _gkg_processing_algobase_VolumeMinMaxFilterWithMaskContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-algobase/TestFunction.h>


namespace gkg
{


template < class T, class M >
class VolumeMinMaxFilterWithMaskContext : public LoopContext< int32_t >
{

  public:

    VolumeMinMaxFilterWithMaskContext( const Volume< T >& volume, 
                                       T& minimum,
                                       T& maximum );

    VolumeMinMaxFilterWithMaskContext( const Volume< T >& volume, 
                                       const Volume< M >& mask, 
                                       const TestFunction< M >& testFunction, 
                                       T& minimum,
                                       T& maximum );

    void doIt( int32_t startIndex, int32_t count );

  private:

    const T* _ptr0;
    const M* _m0;
    const TestFunction< M >* _testFunction;
    T& _minimum;
    T& _maximum;

};


}


#endif
