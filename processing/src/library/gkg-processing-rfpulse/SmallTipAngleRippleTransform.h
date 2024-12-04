#ifndef _gkg_processing_rfpulse_SmallTipAngleRippleTransform_h_
#define _gkg_processing_rfpulse_SmallTipAngleRippleTransform_h_


#include <gkg-processing-rfpulse/RippleTransform.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class SmallTipAngleRippleTransform : public RippleTransform,
                                public Singleton< SmallTipAngleRippleTransform >
{

  public:

    ~SmallTipAngleRippleTransform();

    double getPassBandRipple( double effectivePassBandRipple ) const;
    double getStopBandRipple( double effectiveStopBandRipple ) const;

  protected:

    friend class Singleton< SmallTipAngleRippleTransform >;

    SmallTipAngleRippleTransform();


};


}


#endif

