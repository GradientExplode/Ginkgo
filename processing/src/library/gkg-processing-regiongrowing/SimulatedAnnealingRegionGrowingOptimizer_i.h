#ifndef _gkg_processing_regiongrowing_SimulatedAnnealingRegionGrowingOptimizer_i_h_
#define _gkg_processing_regiongrowing_SimulatedAnnealingRegionGrowingOptimizer_i_h_


#include <gkg-processing-regiongrowing/SimulatedAnnealingRegionGrowingOptimizer.h>
#include <gkg-processing-regiongrowing/RegionGrowingOptimizer_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-cppext/Limits.h>
#include <cmath>


template < class L >
inline
gkg::SimulatedAnnealingRegionGrowingOptimizer< L >::
                                     SimulatedAnnealingRegionGrowingOptimizer(
    gkg::RCPointer< gkg::RegionGrowingAlgorithm< L > > regionGrowingAlgorithm,
    double initialTemperature,
    bool verbose )
                               : gkg::RegionGrowingOptimizer< L >(
                                                       regionGrowingAlgorithm,
                                                       verbose ),
                                 _temperature( initialTemperature ),
                                 _randomGenerator( gkg::RandomGenerator::Gfsr4 )
{
}


template < class L >
inline
gkg::SimulatedAnnealingRegionGrowingOptimizer< L >::
                                    ~SimulatedAnnealingRegionGrowingOptimizer()
{
}


template < class L >
inline
L gkg::SimulatedAnnealingRegionGrowingOptimizer< L >::chooseNewLabel(
                                       const gkg::Vector3d< int32_t >& site,
                                       const L& currentLabel,
                                       const std::set< L >& newPutativeLabels )
{

  try
  {

    // dans la version de Linda current Label etait ajoute a newPutativeLabels


    L bestLabel = currentLabel;
    double deltaEnergy = 0.0;

    std::list< double > probabilities;
    double probability = 0.0;
    double normalizationFactor = 0.0;

    // looping over putative new label(s)  and building Gibb's sampler
    typename std::set< L >::const_iterator
      l = newPutativeLabels.begin(),
      le = newPutativeLabels.end();
    while ( l != le )
    {

      deltaEnergy = this->_regionGrowingAlgorithm->getDeltaEnergy( site,
                                                                   currentLabel,
                                                                   *l );
      probability = std::exp( -deltaEnergy / _temperature );
      normalizationFactor += probability;

      probabilities.push_back( probability );

      ++ l;

    }

    //picking a random probability
    double randomProbability = gkg::NumericalAnalysisSelector::getInstance().
                        getImplementationFactory()->getUniformRandomNumber(
                                                _randomGenerator,
                                                0.0,
                                                1.0 );

    // choosing label considering random probability and cumulative
    // probabilities
    double sumOfProbabilities = 0.0;
    std::list< double >::const_iterator
      p = probabilities.begin(),
      pe = probabilities.end();
    l = newPutativeLabels.begin();
    while ( p != pe )
    {

      sumOfProbabilities += *p / normalizationFactor;
      if ( sumOfProbabilities >= randomProbability )
      {

        bestLabel = *l;
        break;

      }
      ++ p;
      ++ l;

    }

    // updating temperature
    _temperature *= 0.9999;

    return bestLabel;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "L gkg::SimulatedAnnealingRegionGrowingOptimizer< L >::"
             "chooseNewLabel( "
             "const gkg::Vector3d< int32_t >& site, "
             "const L& currentLabel, "
             "const std::set< L >& newPutativeLabels )" );

}


#endif
