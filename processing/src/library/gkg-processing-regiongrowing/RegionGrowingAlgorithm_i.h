#ifndef _gkg_processing_regiongrowing_RegionGrowingAlgorithm_i_h_
#define _gkg_processing_regiongrowing_RegionGrowingAlgorithm_i_h_


#include <gkg-processing-regiongrowing/RegionGrowingAlgorithm.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


template < class L >
inline
gkg::RegionGrowingAlgorithm< L >::RegionGrowingAlgorithm()
                                 : _energy( new gkg::RegionGrowingEnergy< L > )
{
}


template < class L >
inline
gkg::RegionGrowingAlgorithm< L >::RegionGrowingAlgorithm(
                                gkg::RCPointer< gkg::Volume< L > > labelVolume )
                                 : _labelVolume( labelVolume ),
    	                           _labelVolumeBoundingBox( *labelVolume ),
                                   _energy( new gkg::RegionGrowingEnergy< L > )
{
}


template < class L >
inline
gkg::RegionGrowingAlgorithm< L >::~RegionGrowingAlgorithm()
{
}


template < class L >
inline
gkg::RCPointer< gkg::Volume< L > > 
gkg::RegionGrowingAlgorithm< L >::getLabelVolume() const
{

  try
  {

    return _labelVolume;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "const gkg::RCPointer< gkg::Volume< L > > "
             "gkg::RegionGrowingAlgorithm< L >::getLabelVolume() "
             "const" );

}




template < class L >
inline
gkg::RCPointer< gkg::RegionGrowingEnergy< L > >
gkg::RegionGrowingAlgorithm< L >::getEnergy()
{

  try
  {

    return _energy;

  }
  GKG_CATCH( "template < class L > "
             "inline "
             "gkg::RCPointer< gkg::RegionGrowingEnergy< L > > "
             "gkg::RegionGrowingAlgorithm< L >::getEnergy()" );

}


#endif
