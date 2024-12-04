#ifndef _gkg_processing_rfpulse_ShinnarLeRouxRFPulse_h_
#define _gkg_processing_rfpulse_ShinnarLeRouxRFPulse_h_


#include <gkg-processing-rfpulse/RFPulse.h>


namespace gkg
{


class RippleTransform;


class ShinnarLeRouxRFPulse : public RFPulse
{

  public:

    ShinnarLeRouxRFPulse( CayleyKleinParameters* cayleyKleinParameters,
                          RippleTransform* rippleTransform,
                          int32_t sampleCount,
                          double timeBandWidth,
                          double passBandRipple,
                          double stopBandRipple,
                          FIRFilterDesignMethod* firFilterDesignMethod );
    virtual ~ShinnarLeRouxRFPulse();

    const std::vector< std::complex< double > >& getPolynomA() const;
    const std::vector< std::complex< double > >& getPolynomB() const;

    const CayleyKleinParameters& getPrescribedCayleyKleinParameters() const;

  protected:

    void doPolynomB( int32_t sampleCount );
    void scalePolynomB();
    void doPolynomA();
    void inverseShinnarLeRouxTransform();

    CayleyKleinParameters* _cayleyKleinParameters;
    RippleTransform* _rippleTransform;

    std::vector< std::complex< double > > _polynomA;
    std::vector< std::complex< double > > _polynomB;

};


}


#endif
