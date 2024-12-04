#ifndef _gkg_processing_signal_ParksMcClellanMaximumPhaseMethod_h_
#define _gkg_processing_signal_ParksMcClellanMaximumPhaseMethod_h_


#include <gkg-processing-signal/FIRFilterDesignMethod.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class ParksMcClellanMaximumPhaseMethod :
                            public FIRFilterDesignMethod,
                            public Singleton< ParksMcClellanMaximumPhaseMethod >
{

  public:

    ~ParksMcClellanMaximumPhaseMethod();

    std::vector< std::complex< double > >
      getImpulseResponse( int32_t sampleCount,
                          double timeBandWidth,
                          double passBandRipple,
                          double stopBandRipple ) const;

  protected:

    friend class Singleton< ParksMcClellanMaximumPhaseMethod >;

    ParksMcClellanMaximumPhaseMethod();

    double getDInfinity( double passBandRipple, double stopBandRipple ) const;
    void getMaxPhaseFromLinPhaseResponse( 
     std::vector< std::complex< double > >& impulseResponse,
     const std::vector< std::complex< double > >& impulseResponseLinear ) const;

};


}


#endif
