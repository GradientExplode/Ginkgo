#include <gkg-dmri-qspace-sampling/SingleShellQSpaceSampling.h>
#include <gkg-processing-algobase/AverageFilter_i.h>
#include <gkg-core-exception/Exception.h>


gkg::SingleShellQSpaceSampling::SingleShellQSpaceSampling(
                       const gkg::HomogeneousTransform3d< float >& transform3d,
                       const gkg::Dictionary& gradientCharacteristics )
                               : gkg::SphericalQSpaceSampling(
                                                       transform3d,
                                                       gradientCharacteristics )
{
}


gkg::SingleShellQSpaceSampling::~SingleShellQSpaceSampling()
{
}


bool gkg::SingleShellQSpaceSampling::isSingleShell() const
{

  try
  {

    return true;

  }
  GKG_CATCH( "bool gkg::SingleShellQSpaceSampling::isSingleShell() const" );

}


bool gkg::SingleShellQSpaceSampling::isMultipleShell() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool gkg::SingleShellQSpaceSampling::isMultipleShell() const" );

}


float gkg::SingleShellQSpaceSampling::getBValue() const
{

  try
  {

    gkg::AverageFilter< std::vector< float >, float > averageFilter;
    float averageBValue = 0.0;
    averageFilter.filter( this->_bValues, averageBValue );

    return averageBValue;

  }
  GKG_CATCH( "float gkg::SingleShellQSpaceSampling::getBValue() const" );

}


float gkg::SingleShellQSpaceSampling::getDiffusionTime() const
{

  try
  {

    return this->_diffusionTimes[ 0 ];

  }
  GKG_CATCH( "float gkg::SingleShellQSpaceSampling::getDiffusionTime() const" );

}


const gkg::OrientationSet&
gkg::SingleShellQSpaceSampling::getOrientationSet() const
{

  try
  {

    return _orientationSet;

  }
  GKG_CATCH( "const gkg::OrientationSet& "
             "gkg::SingleShellQSpaceSampling::getOrientationSet() const" );

}
