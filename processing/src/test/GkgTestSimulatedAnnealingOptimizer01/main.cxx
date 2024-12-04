#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/SimulatedAnnealingConfiguration.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <cmath>


#define TRIAL_COUNT  40


class MyConfiguration : public gkg::SimulatedAnnealingConfiguration
{

  public:

    MyConfiguration( double initialValue );
    virtual ~MyConfiguration();

    double getValue() const;

    double getEnergy() const;
    void step( double uniformRandomNumber, double stepSize );

    double getDistance(
                      const gkg::SimulatedAnnealingConfiguration& other ) const;
    void display() const;
    void copy( const gkg::SimulatedAnnealingConfiguration& other );
    gkg::SimulatedAnnealingConfiguration* copyConstruct() const;

  protected:

    double _value;

};


MyConfiguration::MyConfiguration( double initialValue )
                : gkg::SimulatedAnnealingConfiguration(),
                  _value( initialValue )
{
}


MyConfiguration::~MyConfiguration()
{
}


double MyConfiguration::getValue() const
{

  return _value;

}


double MyConfiguration::getEnergy() const
{

  return std::exp( -std::pow( ( _value - 1.0 ), 2.0 ) ) *
         std::sin( 8.0 * _value );

}


void MyConfiguration::step( double uniformRandomNumber, double stepSize )
{

  _value += uniformRandomNumber * 2.0 * stepSize - stepSize;

}


double MyConfiguration::getDistance(
                       const gkg::SimulatedAnnealingConfiguration& other ) const
{

  const MyConfiguration* myConfiguration =
    static_cast< const MyConfiguration* >( &other );
  return std::fabs( _value - myConfiguration->getValue() );

}


void MyConfiguration::display() const
{

  std::cout << " -> optimum value = " << _value << std::endl;

}


void MyConfiguration::copy(
                       const gkg::SimulatedAnnealingConfiguration& other )
{
  
  const MyConfiguration* myConfiguration =
    static_cast< const MyConfiguration* >( &other );
  _value = myConfiguration->getValue();

}


gkg::SimulatedAnnealingConfiguration* MyConfiguration::copyConstruct() const
{

  MyConfiguration* myConfiguration = new MyConfiguration( _value );
  return myConfiguration;

}


int main( int /*argc*/, char* argv[] )
{


  int result = EXIT_SUCCESS;

  try
  {

    gkg::PluginLoader::getInstance().load();

    // in fact, 'gsl' library is the default; so the following line is not
    // mandatory: it is only for didactic purpose
    gkg::NumericalAnalysisSelector::getInstance().select( "gsl" );

    // getting numerical analysis implementation factory
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();


    gkg::SimulatedAnnealingConfiguration::BoltzmannParameters
      boltzmannParameters( 1.0, 1.003, 0.008, 2.0e-6 );

    MyConfiguration myConfiguration( 15.5 );


    int32_t trialCountPerStep = 200;
    int32_t iterationCountPerTemperature = 1000;
    double maximumStepSize = 1.0;
    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );
    bool verbose = true;
    factory->getSimulatedAnnealingOptimumConfiguration(
                     myConfiguration,
                     trialCountPerStep,
                     iterationCountPerTemperature,
                     maximumStepSize,
                     boltzmannParameters,
                     randomGenerator,
                     verbose );

    std::cout << "Optimum found : " << myConfiguration.getValue() << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;


}
