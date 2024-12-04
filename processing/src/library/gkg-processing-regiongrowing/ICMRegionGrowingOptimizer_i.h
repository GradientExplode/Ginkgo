#ifndef _gkg_processing_regiongrowing_ICMRegionGrowingOptimizer_i_h_
#define _gkg_processing_regiongrowing_ICMRegionGrowingOptimizer_i_h_


#include <gkg-processing-regiongrowing/ICMRegionGrowingOptimizer.h>
#include <gkg-processing-regiongrowing/RegionGrowingOptimizer_i.h>
#include <gkg-core-cppext/Limits.h>


template < class L >
inline
gkg::ICMRegionGrowingOptimizer< L >::ICMRegionGrowingOptimizer(
                              gkg::RCPointer< gkg::RegionGrowingAlgorithm< L > >
                                                       regionGrowingAlgorithm,
                              bool verbose )
                                    : gkg::RegionGrowingOptimizer< L >(
                                                        regionGrowingAlgorithm,
                                                        verbose )
{
}


template < class L >
inline
gkg::ICMRegionGrowingOptimizer< L >::~ICMRegionGrowingOptimizer()
{
}


template < class L >
inline
L gkg::ICMRegionGrowingOptimizer< L >::chooseNewLabel(
                                       const gkg::Vector3d< int32_t >& site,
                                       const L& currentLabel,
                                       const std::set< L >& newPutativeLabels )
{

  try
  {

    L bestLabel = currentLabel;
    double bestDeltaEnergy = std::numeric_limits< double >::max();
    double deltaEnergy = 0.0;
    typename std::set< L >::const_iterator
      l = newPutativeLabels.begin(),
      le = newPutativeLabels.end();
    while ( l != le )
    {

      deltaEnergy = this->_regionGrowingAlgorithm->getDeltaEnergy( site,
                                                                   currentLabel,
                                                                   *l );
      if ( deltaEnergy < bestDeltaEnergy )
      {

        bestDeltaEnergy = deltaEnergy;
        bestLabel = *l;

      }
      ++ l;

    }
    if ( bestDeltaEnergy > 0.0 )
    {

      bestLabel = currentLabel;

    }

    return bestLabel;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "L gkg::ICMRegionGrowingOptimizer< L >::chooseNewLabel( "
             "const gkg::Vector3d< int32_t >& site, "
             "const L& currentLabel, "
             "const std::set< L >& newPutativeLabels )" );

}


#endif
