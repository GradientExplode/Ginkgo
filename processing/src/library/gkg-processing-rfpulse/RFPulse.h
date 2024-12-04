#ifndef _gkg_processing_rfpulse_RFPulse_h_
#define _gkg_processing_rfpulse_RFPulse_h_


#include <vector>
#include <complex>
#include <gkg-processing-rfpulse/CayleyKleinParameters.h>


namespace gkg
{


class FIRFilterDesignMethod;


class RFPulse
{

  public:

    virtual ~RFPulse();
             
    double getTimeBandWidth() const;
    double getPassBandRipple() const;
    double getStopBandRipple() const;

    int32_t getSampleCount() const;
    const std::complex< double >& getSample( int32_t index ) const;
    const std::vector< std::complex< double > >& getSamples() const;

    CayleyKleinParameters getEffectiveCayleyKleinParameters(
                                         const std::vector< double >& gradientX,
                                         const std::vector< double >& gradientY,
                                         double coordX,
                                         double coordY ) const;

  protected:

    RFPulse( double timeBandWidth,
             double passBandRipple,
             double stopBandRipple,
             FIRFilterDesignMethod* firFilterDesignMethod );

    double _timeBandWidth;
    double _passBandRipple;
    double _stopBandRipple;
    std::vector< std::complex< double > > _samples;

    FIRFilterDesignMethod* _firFilterDesignMethod;

};


}


#endif
