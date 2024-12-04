#ifndef _gkg_processing_rfpulse_SaturationRippleTransform_h_
#define _gkg_processing_rfpulse_SaturationRippleTransform_h_


#include <gkg-processing-rfpulse/RippleTransform.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class SaturationRippleTransform : public RippleTransform,
                                  public Singleton< SaturationRippleTransform >
{

  public:

    ~SaturationRippleTransform();

    double getPassBandRipple( double effectivePassBandRipple ) const;
    double getStopBandRipple( double effectiveStopBandRipple ) const;

  protected:

    friend class Singleton< SaturationRippleTransform >;

    SaturationRippleTransform();


};


}


#endif

