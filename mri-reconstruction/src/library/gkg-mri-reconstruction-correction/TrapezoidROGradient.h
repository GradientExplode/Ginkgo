#ifndef _gkg_mri_reconstruction_correction_TrapezoidROGradient_h_
#define _gkg_mri_reconstruction_correction_TrapezoidROGradient_h_


#include <gkg-mri-reconstruction-correction/ReadOutGradient.h>


namespace gkg
{


template < class T >
class TrapezoidROGradient : public ReadOutGradient< T >
{

  public:

    TrapezoidROGradient( int32_t sampleCount,
                         const T& rampTime,
                         const T& topFlatTime );
    virtual ~TrapezoidROGradient();

};


}


#endif
