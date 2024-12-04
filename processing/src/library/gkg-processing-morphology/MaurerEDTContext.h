#ifndef _gkg_processing_morphology_MaurerEDTContext_h_
#define _gkg_processing_morphology_MaurerEDTContext_h_


#include <gkg-communication-thread/LoopContext.h>

#include <vector>


namespace gkg
{


template < class T >
class MaurerEDTContext : public LoopContext< int32_t >
{

  public:

    MaurerEDTContext();

    bool voronoiEDT( std::vector< double >& f, 
                     int32_t size,
                     double resolution,
                     T sqMaxDistance ) const;

};


}


#endif
