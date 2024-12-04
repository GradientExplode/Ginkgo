#ifndef _gkg_processing_signal_PhaseUnwrapperFunction_h_
#define _gkg_processing_signal_PhaseUnwrapperFunction_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


template < class T > class Volume;


void removeOffset( Volume< float >& unwrappedPhaseDifference,
                   const Volume< uint8_t >& qualityMask );

void removeRamp( Volume< float >& unwrappedPhaseDifference,
                 const Volume< float >& qualityMap );


}


#endif
