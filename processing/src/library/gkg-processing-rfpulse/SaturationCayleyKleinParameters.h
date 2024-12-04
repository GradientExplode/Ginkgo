#ifndef _gkg_processing_rfpulse_SaturationCayleyKleinParameters_h_
#define _gkg_processing_rfpulse_SaturationCayleyKleinParameters_h_


#include <gkg-processing-rfpulse/CayleyKleinParameters.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class SaturationCayleyKleinParameters :
                             public CayleyKleinParameters,
                             public Singleton< SaturationCayleyKleinParameters >
{

  public:

    ~SaturationCayleyKleinParameters();

  protected:

    friend class Singleton< SaturationCayleyKleinParameters >;

    SaturationCayleyKleinParameters();

};


}


#endif
