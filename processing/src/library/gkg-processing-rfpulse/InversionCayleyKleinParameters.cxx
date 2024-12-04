#include <gkg-processing-rfpulse/InversionCayleyKleinParameters.h>
#include <cmath>


gkg::InversionCayleyKleinParameters::InversionCayleyKleinParameters()
                                    : gkg::CayleyKleinParameters(
                                      M_PI,
                                      gkg::Vector3d< double >( 1.0, 0.0, 0.0 ) )
{
}


gkg::InversionCayleyKleinParameters::~InversionCayleyKleinParameters()
{
}
