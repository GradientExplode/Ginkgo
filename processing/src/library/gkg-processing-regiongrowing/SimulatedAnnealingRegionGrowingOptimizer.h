#ifndef _gkg_processing_regiongrowing_SimulatedAnnealingRegionGrowingOptimizer_h_
#define _gkg_processing_regiongrowing_SimulatedAnnealingRegionGrowingOptimizer_h_


#include <gkg-processing-regiongrowing/RegionGrowingOptimizer.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>


#define DEFAULT_INITIAL_TEMPERATURE 0.1


namespace gkg
{


template < class L >
class SimulatedAnnealingRegionGrowingOptimizer :
                                             public RegionGrowingOptimizer< L >
{

  public:

    SimulatedAnnealingRegionGrowingOptimizer(
             RCPointer< RegionGrowingAlgorithm< L > > regionGrowingAlgorithm,
             double initialTemperature = DEFAULT_INITIAL_TEMPERATURE,
             bool verbose = false );
    virtual ~SimulatedAnnealingRegionGrowingOptimizer();

  protected:

    L chooseNewLabel( const Vector3d< int32_t >& site,
                      const L& currentLabel, 
                      const std::set< L >& newPutativeLabels );

    double _temperature;
    RandomGenerator _randomGenerator;

};


}


#endif
