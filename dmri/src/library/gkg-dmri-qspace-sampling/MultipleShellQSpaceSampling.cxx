#include <gkg-dmri-qspace-sampling/MultipleShellQSpaceSampling.h>
#include <gkg-core-exception/Exception.h>


gkg::MultipleShellQSpaceSampling::MultipleShellQSpaceSampling(
                       const gkg::HomogeneousTransform3d< float >& transform3d,
                       const gkg::Dictionary& gradientCharacteristics )
                                 : gkg::SphericalQSpaceSampling(
                                                       transform3d,
                                                       gradientCharacteristics )
{
}


gkg::MultipleShellQSpaceSampling::~MultipleShellQSpaceSampling()
{
}


bool gkg::MultipleShellQSpaceSampling::isSingleShell() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool gkg::MultipleShellQSpaceSampling::isSingleShell() const" );

}


bool gkg::MultipleShellQSpaceSampling::isMultipleShell() const
{

  try
  {

    return true;

  }
  GKG_CATCH( "bool gkg::MultipleShellQSpaceSampling::isMultipleShell() const" );

}


int32_t gkg::MultipleShellQSpaceSampling::getShellCount() const
{

  try
  {

    return ( int32_t )_shellBValues.size();

  }
  GKG_CATCH( "int32_t "
             "gkg::MultipleShellQSpaceSampling::getShellCount() const" );

}


float gkg::MultipleShellQSpaceSampling::getBValue( int32_t shellIndex ) const
{

  try
  {

    return _shellBValues[ shellIndex ];

  }
  GKG_CATCH( "float gkg::MultipleShellQSpaceSampling::getBValue( "
             "int32_t shellIndex ) const" );

}


float 
gkg::MultipleShellQSpaceSampling::getDiffusionTime( int32_t shellIndex ) const
{

  try
  {

    return _shellDiffusionTimes[ shellIndex ];

  }
  GKG_CATCH( "float gkg::MultipleShellQSpaceSampling::getDiffusionTime( "
             "int32_t shellIndex ) const" );

}
