#ifndef _gkg_processing_algobase_VolumeBinarizerContext_h_
#define _gkg_processing_algobase_VolumeBinarizerContext_h_


#include <gkg-communication-thread/LoopContext.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-algobase/Binarizer.h>


namespace gkg
{


template < class IN, class OUT >
class VolumeBinarizerContext : public LoopContext< int32_t >
{

  public:

    VolumeBinarizerContext( const Volume< IN >& in,
                            Volume< OUT >& out,
                            const Binarizer< IN, OUT >& itemBinarizer );
			     
    void doIt( int32_t startIndex, int32_t count );

  private:

    const Volume< IN >& _in;
    Volume< OUT >& _out;
    const Binarizer< IN, OUT >& _itemBinarizer;

};


}


#endif
