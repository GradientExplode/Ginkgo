#ifndef _gkg_processing_numericalanalysis_SimulatedAnnealingConfiguration_h_
#define _gkg_processing_numericalanalysis_SimulatedAnnealingConfiguration_h_



namespace gkg
{


class SimulatedAnnealingConfiguration
{

  public:

    // At any iteration, the probability of the simualted annealing system is
    // given by the following expression:
    //   p = exp( - ( E(i+1) - E(i) ) / ( k T ) )
    //   T = max( initialTemperature / ( mu ^ i ); minimumTemperature )
    struct BoltzmannParameters
    {

      BoltzmannParameters( double theK,
                           double theMu,
                           double theInitialTemperature,
                           double theMinimumTemperature );
      double k;
      double mu;
      double initialTemperature;
      double minimumTemperature;

    };

    virtual ~SimulatedAnnealingConfiguration();

    virtual double getEnergy() const = 0;
    virtual void step( double uniformRandomNumber, double stepSize ) = 0;

    virtual double getDistance(
                       const SimulatedAnnealingConfiguration& other ) const = 0;
    virtual void display() const = 0;
    virtual void copy( const SimulatedAnnealingConfiguration& other ) = 0;
    virtual SimulatedAnnealingConfiguration* copyConstruct() const = 0;

  protected:

    SimulatedAnnealingConfiguration();

};


}


#endif
