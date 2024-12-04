#include <gkg-dmri-tractography/GibbsSampledStartingOrientationStrategy.h>
#include <gkg-core-exception/Exception.h>


gkg::GibbsSampledStartingOrientationStrategy::
                    GibbsSampledStartingOrientationStrategy( float temperature )
                                          : gkg::StartingOrientationStrategy(),
                                            _randomGenerator(
                                                   gkg::RandomGenerator::Taus ),
                                            _temperature( temperature )
{
}


gkg::GibbsSampledStartingOrientationStrategy::
                                      ~GibbsSampledStartingOrientationStrategy()
{
}


int32_t gkg::GibbsSampledStartingOrientationStrategy::getStartingOrientation(
                               const gkg::OrientationDistributionFunction& odf )
{

  try
  {

    return odf.getGibbsSampledOrientationIndexFast( _randomGenerator,
                                                    _temperature );

  }
  GKG_CATCH( "int32_t gkg::GibbsSampledStartingOrientationStrategy::"
             "getStartingOrientation( "
             "const gkg::OrientationDistributionFunction& odf )" );

}
