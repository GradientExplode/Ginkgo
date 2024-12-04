#include <gkg-processing-rfpulse/SpinEchoCayleyKleinParameters.h>
#include <cmath>


gkg::SpinEchoCayleyKleinParameters::SpinEchoCayleyKleinParameters()
                                   : gkg::CayleyKleinParameters(
                                      M_PI,
                                      gkg::Vector3d< double >( 0.0, 1.0, 0.0 ) )
{
}


gkg::SpinEchoCayleyKleinParameters::~SpinEchoCayleyKleinParameters()
{
}
