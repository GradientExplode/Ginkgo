#ifndef _gkg_processing_signal_PseudoCorrelation_h_
#define _gkg_processing_signal_PseudoCorrelation_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


template < class T > class Volume;

void PseudoCorrelation( const Volume< float >& phase,
                        Volume< float >& result, 
                        int32_t tsize );


}


#endif
