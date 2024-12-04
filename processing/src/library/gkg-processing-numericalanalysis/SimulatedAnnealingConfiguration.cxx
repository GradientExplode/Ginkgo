#include <gkg-processing-numericalanalysis/SimulatedAnnealingConfiguration.h>


gkg::SimulatedAnnealingConfiguration::BoltzmannParameters::BoltzmannParameters(
                                                  double theK,
                                                  double theMu,
                                                  double theInitialTemperature,
                                                  double theMinimumTemperature )
                                  : k( theK ),
                                    mu( theMu ),
                                    initialTemperature( theInitialTemperature ),
                                    minimumTemperature( theMinimumTemperature )
{
}


gkg::SimulatedAnnealingConfiguration::SimulatedAnnealingConfiguration()
{
}


gkg::SimulatedAnnealingConfiguration::~SimulatedAnnealingConfiguration()
{
}


