#include <gkg-processing-rfpulse/ExcitationCayleyKleinParameters.h>
#include <cmath>


gkg::ExcitationCayleyKleinParameters::ExcitationCayleyKleinParameters()
                                     : gkg::CayleyKleinParameters(
                                      M_PI / 2.0,
                                      gkg::Vector3d< double >( 1.0, 0.0, 0.0 ) )
{
}


gkg::ExcitationCayleyKleinParameters::~ExcitationCayleyKleinParameters()
{
}
