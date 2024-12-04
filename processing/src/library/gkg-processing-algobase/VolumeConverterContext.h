#ifndef _gkg_processing_algobase_VolumeConverterContext_h_
#define _gkg_processing_algobase_VolumeConverterContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-algobase/Converter.h>


namespace gkg
{


template < class IN, class OUT >
class VolumeConverterContext : public LoopContext< int32_t >
{

  public:

    VolumeConverterContext( const Volume< IN >& in, Volume< OUT >& out );
			     
    void doIt( int32_t startIndex, int32_t count );

  private:

    const Volume< IN >& _in;
    Volume< OUT >& _out;
    Converter< IN, OUT > _itemConverter;

};


}


#endif
