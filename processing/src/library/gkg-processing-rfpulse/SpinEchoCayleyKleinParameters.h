#ifndef _gkg_processing_rfpulse_SpinEchoCayleyKleinParameters_h_
#define _gkg_processing_rfpulse_SpinEchoCayleyKleinParameters_h_


#include <gkg-processing-rfpulse/CayleyKleinParameters.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class SpinEchoCayleyKleinParameters :
                             public CayleyKleinParameters,
                             public Singleton< SpinEchoCayleyKleinParameters >
{

  public:

    ~SpinEchoCayleyKleinParameters();

  protected:

    friend class Singleton< SpinEchoCayleyKleinParameters >;

    SpinEchoCayleyKleinParameters();

};


}


#endif
