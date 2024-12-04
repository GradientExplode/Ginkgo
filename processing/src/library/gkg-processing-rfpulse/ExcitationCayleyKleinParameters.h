#ifndef _gkg_processing_rfpulse_ExcitationCayleyKleinParameters_h_
#define _gkg_processing_rfpulse_ExcitationCayleyKleinParameters_h_


#include <gkg-processing-rfpulse/CayleyKleinParameters.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class ExcitationCayleyKleinParameters :
                             public CayleyKleinParameters,
                             public Singleton< ExcitationCayleyKleinParameters >
{

  public:

    ~ExcitationCayleyKleinParameters();

  protected:

    friend class Singleton< ExcitationCayleyKleinParameters >;

    ExcitationCayleyKleinParameters();

};


}


#endif
