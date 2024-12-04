#ifndef _gkg_processing_rfpulse_SpinEchoRippleTransform_h_
#define _gkg_processing_rfpulse_SpinEchoRippleTransform_h_


#include <gkg-processing-rfpulse/RippleTransform.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class SpinEchoRippleTransform : public RippleTransform,
                                public Singleton< SpinEchoRippleTransform >
{

  public:

    ~SpinEchoRippleTransform();

    double getPassBandRipple( double effectivePassBandRipple ) const;
    double getStopBandRipple( double effectiveStopBandRipple ) const;

  protected:

    friend class Singleton< SpinEchoRippleTransform >;

    SpinEchoRippleTransform();


};


}


#endif

