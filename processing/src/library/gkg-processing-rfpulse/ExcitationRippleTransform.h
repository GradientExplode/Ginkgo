#ifndef _gkg_processing_rfpulse_ExcitationRippleTransform_h_
#define _gkg_processing_rfpulse_ExcitationRippleTransform_h_


#include <gkg-processing-rfpulse/RippleTransform.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class ExcitationRippleTransform : public RippleTransform,
                                  public Singleton< ExcitationRippleTransform >
{

  public:

    ~ExcitationRippleTransform();

    double getPassBandRipple( double effectivePassBandRipple ) const;
    double getStopBandRipple( double effectiveStopBandRipple ) const;

  protected:

    friend class Singleton< ExcitationRippleTransform >;

    ExcitationRippleTransform();


};


}


#endif

