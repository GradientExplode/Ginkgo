#include <gkg-dmri-qspace-sampling/MultipleSameShellQSpaceSampling.h>
#include <gkg-core-exception/Exception.h>


gkg::MultipleSameShellQSpaceSampling::MultipleSameShellQSpaceSampling(
                       const gkg::HomogeneousTransform3d< float >& transform3d,
                       const gkg::Dictionary& gradientCharacteristics )
                                     : gkg::MultipleShellQSpaceSampling(
                                                       transform3d,
                                                       gradientCharacteristics )
{
}


gkg::MultipleSameShellQSpaceSampling::~MultipleSameShellQSpaceSampling()
{
}


const gkg::OrientationSet&
gkg::MultipleSameShellQSpaceSampling::getOrientationSet(
                                                  int32_t /*shellIndex*/ ) const
{

  try
  {

    return _orientationSets[ 0 ];

  }
  GKG_CATCH( "const gkg::OrientationSet& "
             "gkg::MultipleSameShellQSpaceSampling::getOrientationSet( "
             "int32_t /*shellIndex*/ ) const" );

}
