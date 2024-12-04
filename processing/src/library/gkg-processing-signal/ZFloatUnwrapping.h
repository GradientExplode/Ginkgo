#ifndef _gkg_processing_signal_ZFloatUnwrapping_h_
#define _gkg_processing_signal_ZFloatUnwrapping_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


template < class T > class Volume;

void ZFloatUnwrapping( Volume< float >& phaseMap,
                       const Volume< float >& qualityMap,
                       const Volume< int16_t >& mask,
                       int32_t referenceSlice );


}


#endif
