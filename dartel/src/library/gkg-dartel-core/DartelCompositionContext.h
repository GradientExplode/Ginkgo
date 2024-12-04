#ifndef _gkg_dartel_core_DartelCompositionContext_h_
#define _gkg_dartel_core_DartelCompositionContext_h_


#include <gkg-communication-thread/LoopContext.h>


namespace gkg
{


template < class T >
class DartelCompositionContext : public gkg::LoopContext< int32_t >
{

  public:

    DartelCompositionContext( int32_t sX, int32_t sY, int32_t sZ,
                              T *pB, T *pA, T *pC );

    void doIt( int32_t startIndex, int32_t count );

  private:

    int32_t _sizeX;
    int32_t _sizeY;
    int32_t _sizeZ;
    int32_t _m;
    T* _A;
    T* _B;
    T* _C;

};


}


#endif
