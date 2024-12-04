#ifndef _gkg_processing_rfpulse_InversionCayleyKleinParameters_h_
#define _gkg_processing_rfpulse_InversionCayleyKleinParameters_h_


#include <gkg-processing-rfpulse/CayleyKleinParameters.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class InversionCayleyKleinParameters :
                             public CayleyKleinParameters,
                             public Singleton< InversionCayleyKleinParameters >
{

  public:

    ~InversionCayleyKleinParameters();

  protected:

    friend class Singleton< InversionCayleyKleinParameters >;

    InversionCayleyKleinParameters();

};


}


#endif
