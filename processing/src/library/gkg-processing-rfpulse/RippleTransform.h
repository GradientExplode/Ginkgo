#ifndef _gkg_processing_rfpulse_RippleTransform_h_
#define _gkg_processing_rfpulse_RippleTransform_h_


namespace gkg
{


class RippleTransform
{

  public:

    virtual ~RippleTransform();

    virtual double getPassBandRipple( double effectivePassBandRipple ) const = 0;
    virtual double getStopBandRipple( double effectiveStopBandRipple ) const = 0;

  protected:

    RippleTransform();

};


}


#endif
