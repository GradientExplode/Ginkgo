#ifndef _gkg_processing_signal_ZUnwrapping_h_
#define _gkg_processing_signal_ZUnwrapping_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


template < class T > class Volume;

void ZUnwrapping( Volume< float >& wrappedPhaseDifference,
                  const Volume< float >& qualityMap,
                  const Volume< int16_t >& mask,
                  int32_t referenceSlice );


void ZUnwrapping( Volume< float >& wrappedPhaseDifference,
                  const Volume< float >& qualityMap,
                  int32_t referenceSlice );


void ZLineUnwrapping( Volume< float >& wrappedPhaseDifference,
                      const Volume< float >& qualityMask,
                      int32_t referenceSlice );


}


#endif
