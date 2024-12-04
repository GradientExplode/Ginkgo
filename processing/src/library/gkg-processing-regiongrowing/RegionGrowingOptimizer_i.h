#ifndef _gkg_processing_regiongrowing_RegionGrowingOptimizer_i_h_
#define _gkg_processing_regiongrowing_RegionGrowingOptimizer_i_h_


#include <gkg-processing-regiongrowing/RegionGrowingOptimizer.h>
#include <gkg-processing-regiongrowing/RegionGrowingAlgorithm_i.h>
#include <gkg-core-exception/Exception.h>


template < class L >
inline
gkg::RegionGrowingOptimizer< L >::RegionGrowingOptimizer(
                              gkg::RCPointer< gkg::RegionGrowingAlgorithm< L > >
                                                       regionGrowingAlgorithm,
                              bool verbose )
                                 : _regionGrowingAlgorithm(
                                                       regionGrowingAlgorithm ),
                                   _verbose( verbose )
{
}


template < class L >
inline
gkg::RegionGrowingOptimizer< L >::~RegionGrowingOptimizer()
{
}


template < class L >
inline
void gkg::RegionGrowingOptimizer< L >::iterate()
{

  try
  {

    // selecting randomly a site and getting is current label
    gkg::Vector3d< int32_t > randomSite;
    L oldLabel;
    _regionGrowingAlgorithm->getRandomSite( randomSite, oldLabel );

    // determining the putative new labels
    std::set< L > newPutativeLabels =
      _regionGrowingAlgorithm->getNewPutativeLabels( randomSite, oldLabel );  

    if ( !newPutativeLabels.empty() )
    {

      L newLabel = chooseNewLabel( randomSite,
                                   oldLabel,
                                   newPutativeLabels );
      if ( newLabel != oldLabel )
      {

        _regionGrowingAlgorithm->applyModification( randomSite,
                                                    oldLabel,
                                                    newLabel );

      }

    }

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "void gkg::RegionGrowingOptimizer< L >::iterate()" );

}


#endif
