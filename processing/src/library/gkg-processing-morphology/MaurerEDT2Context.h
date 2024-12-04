#ifndef _gkg_processing_morphology_MaurerEDT2Context_h_
#define _gkg_processing_morphology_MaurerEDT2Context_h_


#include <gkg-communication-thread/LoopContext.h>


namespace gkg
{


template < class T > class Volume;
template < class T > class TestFunction;


template < class IN, class OUT >
class MaurerEDT2Context : public LoopContext< int32_t >
{

  public:

    MaurerEDT2Context( const Volume< IN >& in,
                       const TestFunction< IN >& testFunction,
                       const OUT sqMaxDistance,
                       Volume< OUT >& out );

    void doIt( int32_t startIndex, int32_t count );

  private:

    const Volume< IN >& _in;
    const TestFunction< IN >& _testFunction;
    const OUT _sqMaxDistance;
    Volume< OUT >& _out;

    int32_t _sizeX;
    int32_t _sizeY;
    Vector3d< double > _resolution;

};


}


#endif
