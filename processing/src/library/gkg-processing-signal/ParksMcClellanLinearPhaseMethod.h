#ifndef _gkg_processing_signal_ParksMcClellanLinearPhaseMethod_h_
#define _gkg_processing_signal_ParksMcClellanLinearPhaseMethod_h_


#include <gkg-processing-signal/FIRFilterDesignMethod.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class ParksMcClellanLinearPhaseMethod : public FIRFilterDesignMethod,
                             public Singleton< ParksMcClellanLinearPhaseMethod >
{

  public:

    ~ParksMcClellanLinearPhaseMethod();

    std::vector< std::complex< double > >
      getImpulseResponse( int32_t sampleCount,
                          double timeBandWidth,
                          double passBandRipple,
                          double stopBandRipple ) const;

  protected:

    friend class Singleton< ParksMcClellanLinearPhaseMethod >;

    ParksMcClellanLinearPhaseMethod();

    double getDInfinity( double passBandRipple, double stopBandRipple ) const;

};


}


#endif
