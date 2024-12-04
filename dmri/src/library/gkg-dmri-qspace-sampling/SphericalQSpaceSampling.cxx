#include <gkg-dmri-qspace-sampling/SphericalQSpaceSampling.h>
#include <gkg-core-exception/Exception.h>


gkg::SphericalQSpaceSampling::SphericalQSpaceSampling(
                       const gkg::HomogeneousTransform3d< float >& transform3d,
                       const gkg::Dictionary& gradientCharacteristics )
                             : gkg::QSpaceSampling( transform3d,
                                                    gradientCharacteristics )
{
}


gkg::SphericalQSpaceSampling::~SphericalQSpaceSampling()
{
}


bool gkg::SphericalQSpaceSampling::isCartesian() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool gkg::SphericalQSpaceSampling::isCartesian() const" );

}


bool gkg::SphericalQSpaceSampling::isSpherical() const
{

  try
  {

    return true;

  }
  GKG_CATCH( "bool gkg::SphericalQSpaceSampling::isSpherical() const" );

}

