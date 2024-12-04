#include <gkg-dmri-qspace-sampling/MultipleDifferentShellQSpaceSampling.h>
#include <gkg-core-exception/Exception.h>


gkg::MultipleDifferentShellQSpaceSampling::MultipleDifferentShellQSpaceSampling(
                       const gkg::HomogeneousTransform3d< float >& transform3d,
                       const gkg::Dictionary& gradientCharacteristics )
                                     : gkg::MultipleShellQSpaceSampling(
                                                       transform3d,
                                                       gradientCharacteristics )
{
}


gkg::MultipleDifferentShellQSpaceSampling::
                                         ~MultipleDifferentShellQSpaceSampling()
{
}


const gkg::OrientationSet&
gkg::MultipleDifferentShellQSpaceSampling::getOrientationSet(
                                                      int32_t shellIndex ) const
{

  try
  {

    return _orientationSets[ shellIndex ];

  }
  GKG_CATCH( "const gkg::OrientationSet& "
             "gkg::MultipleDifferentShellQSpaceSampling::getOrientationSet( "
             "int32_t shellIndex ) const" );

}
