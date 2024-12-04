#ifndef _gkg_processing_signal_ParksMcClellanMinimumPhaseMethod_h_
#define _gkg_processing_signal_ParksMcClellanMinimumPhaseMethod_h_


#include <gkg-processing-signal/FIRFilterDesignMethod.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class ParksMcClellanMinimumPhaseMethod :
                            public FIRFilterDesignMethod,
                            public Singleton< ParksMcClellanMinimumPhaseMethod >
{

  public:

    ~ParksMcClellanMinimumPhaseMethod();

    std::vector< std::complex< double > >
      getImpulseResponse( int32_t sampleCount,
                          double timeBandWidth,
                          double passBandRipple,
                          double stopBandRipple ) const;

  protected:

    friend class Singleton< ParksMcClellanMinimumPhaseMethod >;

    ParksMcClellanMinimumPhaseMethod();

    double getDInfinity( double passBandRipple, double stopBandRipple ) const;
    void getMaxPhaseFromLinPhaseResponse( 
     std::vector< std::complex< double > >& impulseResponse,
     const std::vector< std::complex< double > >& impulseResponseLinear ) const;
    void getMinPhaseFromMaxPhaseResponse( 
                 std::vector< std::complex< double > >& impulseResponse ) const;

};


}


#endif
