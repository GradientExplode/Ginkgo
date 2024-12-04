#include <gkg-processing-rfpulse/SaturationCayleyKleinParameters.h>
#include <cmath>


gkg::SaturationCayleyKleinParameters::SaturationCayleyKleinParameters()
                                  : gkg::CayleyKleinParameters(
                                      M_PI / 2.0,
                                      gkg::Vector3d< double >( 1.0, 0.0, 0.0 ) )
{
}


gkg::SaturationCayleyKleinParameters::~SaturationCayleyKleinParameters()
{
}
