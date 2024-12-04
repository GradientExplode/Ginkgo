#ifndef _gkg_processing_morphology_MaurerEDT3Context_h_
#define _gkg_processing_morphology_MaurerEDT3Context_h_


#include <gkg-communication-thread/LoopContext.h>


namespace gkg
{


template < class T > class Volume;


template < class T >
class MaurerEDT3Context : public LoopContext< int32_t >
{

  public:

    MaurerEDT3Context( const T sqMaxDistance,
                       Volume< T >& out );

    void doIt( int32_t startIndex, int32_t count );

  private:

    const T _sqMaxDistance;
    Volume< T >& _out;

    int32_t _sizeX;
    int32_t _sizeZ;
    Vector3d< double > _resolution;

};


}


#endif
