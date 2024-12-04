#ifndef _gkg_processing_rfpulse_InversionRippleTransform_h_
#define _gkg_processing_rfpulse_InversionRippleTransform_h_


#include <gkg-processing-rfpulse/RippleTransform.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class InversionRippleTransform : public RippleTransform,
                                 public Singleton< InversionRippleTransform >
{

  public:

    ~InversionRippleTransform();

    double getPassBandRipple( double effectivePassBandRipple ) const;
    double getStopBandRipple( double effectiveStopBandRipple ) const;

  protected:

    friend class Singleton< InversionRippleTransform >;

    InversionRippleTransform();


};


}


#endif

