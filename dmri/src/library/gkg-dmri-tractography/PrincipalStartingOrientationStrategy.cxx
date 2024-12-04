#include <gkg-dmri-tractography/PrincipalStartingOrientationStrategy.h>
#include <gkg-core-exception/Exception.h>


gkg::PrincipalStartingOrientationStrategy::
                                          PrincipalStartingOrientationStrategy()
                                          : gkg::StartingOrientationStrategy()
{
}


gkg::PrincipalStartingOrientationStrategy::
                                         ~PrincipalStartingOrientationStrategy()
{
}


int32_t gkg::PrincipalStartingOrientationStrategy::getStartingOrientation(
                               const gkg::OrientationDistributionFunction& odf )
{

  try
  {

    return odf.getPrincipalOrientationIndexFast();

  }
  GKG_CATCH( "int32_t gkg::PrincipalStartingOrientationStrategy::"
             "getStartingOrientation( "
             "const gkg::OrientationDistributionFunction& odf )" );

}
