#ifndef _gkg_dmri_container_BundleMeasureSpreadSheet_i_h_
#define _gkg_dmri_container_BundleMeasureSpreadSheet_i_h_


#include <gkg-dmri-container/BundleMeasureSpreadSheet.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-dmri-bundle-measure/BundleFiberCount.h>
#include <gkg-dmri-bundle-measure/BundleFiberLengthStatistics.h>
#include <gkg-dmri-bundle-measure/BundleFiberLengthHistogram.h>
#include <gkg-dmri-bundle-measure/BundleFiberTortuosityStatistics.h>
#include <gkg-dmri-bundle-measure/BundleFromVolumeProfileAlongCentroid.h>
#include <gkg-dmri-bundle-measure/BundleFromVolumeStatisticsAlongCentroid.h>
#include <gkg-dmri-bundle-measure/BundleFromVolumeStatistics.h>
#include <gkg-dmri-bundle-measure/BundleSulciLabelling.h>
#include <gkg-core-exception/Exception.h>


template < class L, class Compare >
inline
gkg::BundleMeasureSpreadSheet< L, Compare >::BundleMeasureSpreadSheet()
                                            : gkg::HeaderedObject()
{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
    _syntaxSet[ "__generic__" ][ "label_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type",
                                    std::string( "BundleMeasureSpreadSheet" ) );
    _header.addAttribute( "label_type", gkg::TypeOf< L >::getName() );

    //  adding bundle fiber count
    _functorLists[ "bundle_fiber_count" ].push_back(
      &gkg::BundleFiberCount::spreadSheetFunctor< L, Compare > );

    //  adding bundle fiber length statistics
    _functorLists[ "bundle_fiber_length_statistics" ].push_back(
      &gkg::BundleFiberLengthStatistics::spreadSheetFunctor< L, Compare > );

    //  adding bundle fiber length histogram
    _functorLists[ "bundle_fiber_length_histogram" ].push_back(
      &gkg::BundleFiberLengthHistogram::spreadSheetFunctor< L, Compare > );

    //  adding bundle fiber tortuosity statistics
    _functorLists[ "bundle_fiber_tortuosity_statistics" ].push_back(
      &gkg::BundleFiberTortuosityStatistics::spreadSheetFunctor< L, Compare > );

    // adding from volume profile along average fiber
    _functorLists[ "from_volume_profile_along_centroid" ].push_back(
      &gkg::BundleFromVolumeProfileAlongCentroid< int8_t >::
                                                spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile_along_centroid" ].push_back(
      &gkg::BundleFromVolumeProfileAlongCentroid< uint8_t >::
                                                spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile_along_centroid" ].push_back(
      &gkg::BundleFromVolumeProfileAlongCentroid< int16_t >::
                                                spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile_along_centroid" ].push_back(
      &gkg::BundleFromVolumeProfileAlongCentroid< uint16_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile_along_centroid" ].push_back(
      &gkg::BundleFromVolumeProfileAlongCentroid< int32_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile_along_centroid" ].push_back(
      &gkg::BundleFromVolumeProfileAlongCentroid< uint32_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile_along_centroid" ].push_back(
      &gkg::BundleFromVolumeProfileAlongCentroid< int64_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile_along_centroid" ].push_back(
      &gkg::BundleFromVolumeProfileAlongCentroid< uint64_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile_along_centroid" ].push_back(
      &gkg::BundleFromVolumeProfileAlongCentroid< float >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile_along_centroid" ].push_back(
      &gkg::BundleFromVolumeProfileAlongCentroid< double >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );

    // adding from volume statistics along average fiber
    _functorLists[ "from_volume_statistics_along_centroid" ].push_back(
      &gkg::BundleFromVolumeStatisticsAlongCentroid< int8_t >::
                                                spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics_along_centroid" ].push_back(
      &gkg::BundleFromVolumeStatisticsAlongCentroid< uint8_t >::
                                                spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics_along_centroid" ].push_back(
      &gkg::BundleFromVolumeStatisticsAlongCentroid< int16_t >::
                                                spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics_along_centroid" ].push_back(
      &gkg::BundleFromVolumeStatisticsAlongCentroid< uint16_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics_along_centroid" ].push_back(
      &gkg::BundleFromVolumeStatisticsAlongCentroid< int32_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics_along_centroid" ].push_back(
      &gkg::BundleFromVolumeStatisticsAlongCentroid< uint32_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics_along_centroid" ].push_back(
      &gkg::BundleFromVolumeStatisticsAlongCentroid< int64_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics_along_centroid" ].push_back(
      &gkg::BundleFromVolumeStatisticsAlongCentroid< uint64_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics_along_centroid" ].push_back(
      &gkg::BundleFromVolumeStatisticsAlongCentroid< float >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics_along_centroid" ].push_back(
      &gkg::BundleFromVolumeStatisticsAlongCentroid< double >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );

    // adding from volume statistics
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::BundleFromVolumeStatistics< int8_t >::
                                                spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::BundleFromVolumeStatistics< uint8_t >::
                                                spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::BundleFromVolumeStatistics< int16_t >::
                                                spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::BundleFromVolumeStatistics< uint16_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::BundleFromVolumeStatistics< int32_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::BundleFromVolumeStatistics< uint32_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::BundleFromVolumeStatistics< int64_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::BundleFromVolumeStatistics< uint64_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::BundleFromVolumeStatistics< float >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::BundleFromVolumeStatistics< double >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );

    //  adding bundle sulci labelling
    _functorLists[ "bundle_sulci_labelling" ].push_back(
      &gkg::BundleSulciLabelling::spreadSheetFunctor< L, Compare > );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMeasureSpreadSheet< L, Compare >::"
             "BundleMeasureSpreadSheet()" );

}


template < class L, class Compare >
inline
gkg::BundleMeasureSpreadSheet< L, Compare >::BundleMeasureSpreadSheet(
                      gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap )
                                            : gkg::HeaderedObject(),
                                              _bundleMap( bundleMap )
{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
    _syntaxSet[ "__generic__" ][ "label_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type",
                                    std::string( "BundleMeasureSpreadSheet" ) );
    _header.addAttribute( "label_type", gkg::TypeOf< L >::getName() );

    //  adding bundle fiber count
    _functorLists[ "bundle_fiber_count" ].push_back(
      &gkg::BundleFiberCount::spreadSheetFunctor< L, Compare > );

    //  adding bundle fiber length statistics
    _functorLists[ "bundle_fiber_length_statistics" ].push_back(
      &gkg::BundleFiberLengthStatistics::spreadSheetFunctor< L, Compare > );

    //  adding bundle fiber length histogram
    _functorLists[ "bundle_fiber_length_histogram" ].push_back(
      &gkg::BundleFiberLengthHistogram::spreadSheetFunctor< L, Compare > );

    //  adding bundle fiber tortuosity statistics
    _functorLists[ "bundle_fiber_tortuosity_statistics" ].push_back(
      &gkg::BundleFiberTortuosityStatistics::spreadSheetFunctor< L, Compare > );

    // adding from volume profile along average fiber
    _functorLists[ "from_volume_profile_along_centroid" ].push_back(
      &gkg::BundleFromVolumeProfileAlongCentroid< int8_t >::
                                                spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile_along_centroid" ].push_back(
      &gkg::BundleFromVolumeProfileAlongCentroid< uint8_t >::
                                                spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile_along_centroid" ].push_back(
      &gkg::BundleFromVolumeProfileAlongCentroid< int16_t >::
                                                spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile_along_centroid" ].push_back(
      &gkg::BundleFromVolumeProfileAlongCentroid< uint16_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile_along_centroid" ].push_back(
      &gkg::BundleFromVolumeProfileAlongCentroid< int32_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile_along_centroid" ].push_back(
      &gkg::BundleFromVolumeProfileAlongCentroid< uint32_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile_along_centroid" ].push_back(
      &gkg::BundleFromVolumeProfileAlongCentroid< int64_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile_along_centroid" ].push_back(
      &gkg::BundleFromVolumeProfileAlongCentroid< uint64_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile_along_centroid" ].push_back(
      &gkg::BundleFromVolumeProfileAlongCentroid< float >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile_along_centroid" ].push_back(
      &gkg::BundleFromVolumeProfileAlongCentroid< double >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );

    // adding from volume statistics along average fiber
    _functorLists[ "from_volume_statistics_along_centroid" ].push_back(
      &gkg::BundleFromVolumeStatisticsAlongCentroid< int8_t >::
                                                spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics_along_centroid" ].push_back(
      &gkg::BundleFromVolumeStatisticsAlongCentroid< uint8_t >::
                                                spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics_along_centroid" ].push_back(
      &gkg::BundleFromVolumeStatisticsAlongCentroid< int16_t >::
                                                spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics_along_centroid" ].push_back(
      &gkg::BundleFromVolumeStatisticsAlongCentroid< uint16_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics_along_centroid" ].push_back(
      &gkg::BundleFromVolumeStatisticsAlongCentroid< int32_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics_along_centroid" ].push_back(
      &gkg::BundleFromVolumeStatisticsAlongCentroid< uint32_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics_along_centroid" ].push_back(
      &gkg::BundleFromVolumeStatisticsAlongCentroid< int64_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics_along_centroid" ].push_back(
      &gkg::BundleFromVolumeStatisticsAlongCentroid< uint64_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics_along_centroid" ].push_back(
      &gkg::BundleFromVolumeStatisticsAlongCentroid< float >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics_along_centroid" ].push_back(
      &gkg::BundleFromVolumeStatisticsAlongCentroid< double >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );

    // adding from volume statistics 
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::BundleFromVolumeStatistics< int8_t >::
                                                spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::BundleFromVolumeStatistics< uint8_t >::
                                                spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::BundleFromVolumeStatistics< int16_t >::
                                                spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::BundleFromVolumeStatistics< uint16_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::BundleFromVolumeStatistics< int32_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::BundleFromVolumeStatistics< uint32_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::BundleFromVolumeStatistics< int64_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::BundleFromVolumeStatistics< uint64_t >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::BundleFromVolumeStatistics< float >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::BundleFromVolumeStatistics< double >::
                                               spreadSheetFunctor< L,
                                                                    Compare > );

    // adding buncle sulci labelling
    _functorLists[ "bundle_sulci_labelling" ].push_back(
      &gkg::BundleSulciLabelling::spreadSheetFunctor< L, Compare > );


  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMeasureSpreadSheet< L, Compare >::"
             "BundleMeasureSpreadSheet( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap )" );

}


template < class L, class Compare >
inline
gkg::BundleMeasureSpreadSheet< L, Compare >::BundleMeasureSpreadSheet(
                      const gkg::BundleMeasureSpreadSheet< L, Compare >& other )
                             : gkg::HeaderedObject( other ),
                               _functorLists( other._functorLists ),
                               _bundleMap( other._bundleMap ),
                               _measures( other._measures )
{
}


template < class L, class Compare >
inline
gkg::BundleMeasureSpreadSheet< L, Compare >::~BundleMeasureSpreadSheet()
{
}


template < class L, class Compare >
inline
gkg::BundleMeasureSpreadSheet< L, Compare >& 
gkg::BundleMeasureSpreadSheet< L, Compare >::operator=(
                       const gkg::BundleMeasureSpreadSheet< L, Compare >& other )
{


  try
  {

    this->gkg::HeaderedObject::operator=( other );
    _functorLists = other._functorLists;
    _bundleMap = other._bundleMap;
    _measures = other._measures;

    return *this;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMeasureSpreadSheet< L, Compare >& "
             "gkg::BundleMeasureSpreadSheet< L, Compare >::operator=( "
             "const gkg::BundleMeasureSpreadSheet< L, Compare >& other )" );

}


template < class L, class Compare >
inline
typename gkg::BundleMeasureSpreadSheet< L, Compare >::iterator 
gkg::BundleMeasureSpreadSheet< L, Compare >::begin()
{

  return _measures.begin();

}


template < class L, class Compare >
inline
typename gkg::BundleMeasureSpreadSheet< L, Compare >::iterator 
gkg::BundleMeasureSpreadSheet< L, Compare >::end()
{

  return _measures.end();

}


template < class L, class Compare >
inline
typename gkg::BundleMeasureSpreadSheet< L, Compare >::const_iterator 
gkg::BundleMeasureSpreadSheet< L, Compare >::begin() const
{

  return _measures.begin();

}


template < class L, class Compare >
inline
typename gkg::BundleMeasureSpreadSheet< L, Compare >::const_iterator 
gkg::BundleMeasureSpreadSheet< L, Compare >::end() const
{

  return _measures.end();

}


template < class L, class Compare >
inline
typename gkg::BundleMeasureSpreadSheet< L, Compare >::reverse_iterator 
gkg::BundleMeasureSpreadSheet< L, Compare >::rbegin()
{

  return _measures.rbegin();

}


template < class L, class Compare >
inline
typename gkg::BundleMeasureSpreadSheet< L, Compare >::reverse_iterator 
gkg::BundleMeasureSpreadSheet< L, Compare >::rend()
{

  return _measures.rend();

}


template < class L, class Compare >
inline
typename gkg::BundleMeasureSpreadSheet< L, Compare >::const_reverse_iterator 
gkg::BundleMeasureSpreadSheet< L, Compare >::rbegin() const
{

  return _measures.rbegin();

}


template < class L, class Compare >
inline
typename gkg::BundleMeasureSpreadSheet< L, Compare >::const_reverse_iterator
gkg::BundleMeasureSpreadSheet< L, Compare >::rend() const
{

  return _measures.rend();

}


template < class L, class Compare >
inline
typename gkg::BundleMeasureSpreadSheet< L, Compare >::iterator 
gkg::BundleMeasureSpreadSheet< L, Compare >::lower_bound( const L& label )
{

  return _measures.lower_bound( label );

}


template < class L, class Compare >
inline
typename gkg::BundleMeasureSpreadSheet< L, Compare >::iterator
gkg::BundleMeasureSpreadSheet< L, Compare >::upper_bound( const L& label )
{

  return _measures.upper_bound( label );

}


template < class L, class Compare >
inline
typename gkg::BundleMeasureSpreadSheet< L, Compare >::const_iterator
gkg::BundleMeasureSpreadSheet< L, Compare >::lower_bound( const L& label ) const
{

  return _measures.lower_bound( label );

}


template < class L, class Compare >
inline
typename gkg::BundleMeasureSpreadSheet< L, Compare >::const_iterator
gkg::BundleMeasureSpreadSheet< L, Compare >::upper_bound( const L& label ) const
{

  return _measures.upper_bound( label );

}


template < class L, class Compare >
inline
std::pair< typename gkg::BundleMeasureSpreadSheet< L, Compare >::iterator,
           typename gkg::BundleMeasureSpreadSheet< L, Compare >::iterator >
gkg::BundleMeasureSpreadSheet< L, Compare >::equal_range( const L& label )
{

  return _measures.equal_range( label );

}


template < class L, class Compare >
inline
std::pair< typename gkg::BundleMeasureSpreadSheet< L, Compare >::const_iterator,
           typename gkg::BundleMeasureSpreadSheet< L, Compare >::const_iterator >
gkg::BundleMeasureSpreadSheet< L, Compare >::equal_range( const L& label ) const
{

  return _measures.equal_range( label );

}


template < class L, class Compare >
inline
typename gkg::BundleMeasureSpreadSheet< L, Compare >::iterator
gkg::BundleMeasureSpreadSheet< L, Compare >::find( const L& label )
{

  return _measures.find( label );

}



template < class L, class Compare >
inline
void gkg::BundleMeasureSpreadSheet< L, Compare >::setBundleMap(
                      gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap )
{

  try
  {

    _bundleMap = bundleMap;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMeasureSpreadSheet< L, Compare >::setBundleMap( "
             "gkg::RCPointer< BundleMap< L, Compare > > bundleMap )" );

}


template < class L, class Compare >
inline
void gkg::BundleMeasureSpreadSheet< L, Compare >::addMeasure(
                             const std::string& name,
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters )
{

  try
  {

    typename
    std::map< std::string, std::list< 
    typename gkg::BundleMeasureSpreadSheet< L,
                                          Compare >::Functor > >::const_iterator
      l = _functorLists.find( name );
    if ( l == _functorLists.end() )
    {

      throw std::runtime_error( "measure name not found in functor map" );

    }
    bool measureAdded = false;
    typename
    std::list< typename 
          gkg::BundleMeasureSpreadSheet< L, Compare >::Functor >::const_iterator
      f = l->second.begin(),
      fe = l->second.end();
    while ( f != fe )
    {

      try
      {

        ( *f )( stringParameters, scalarParameters, _bundleMap, _measures );
        measureAdded = true;
        break;

      }
      catch ( std::exception& )
      {
      }
      ++ f;

    }
    if ( !measureAdded )
    {

      throw std::runtime_error( std::string( "could not add measure '" ) +
                                name + "'" );

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMeasureSpreadSheet< L, Compare >::addMeasure( "
             "const std::string& name, "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters )" );

}


template < class L, class Compare >
inline
void gkg::BundleMeasureSpreadSheet< L, Compare >::addMeasure(
                                              const L& bundleLabel,
                                              const std::string& measureName,
                                              const gkg::Matrix& measureMatrix )
{

  try
  {

    typename
    std::map< L, 
              std::map< std::string, gkg::Matrix >,
              Compare >::const_iterator
      l = _measures.find( bundleLabel );
    if ( l != _measures.end() )
    {

      throw std::runtime_error( "bundle label already present" );

    }
    _measures[ bundleLabel ][ measureName ] = measureMatrix;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMeasureSpreadSheet< L, Compare >::addMeasure( "
             "const L& bundleLabel, "
             "const std::string& measureName, "
             "const gkg::Matrix& measureMatrix )" );

}


template < class L, class Compare >
inline
const gkg::Matrix& 
gkg::BundleMeasureSpreadSheet< L, Compare >::getMeasure(
                                          const L& bundleLabel,
                                          const std::string& measureName ) const
{

  try
  {

    typename
    std::map< L, 
              std::map< std::string, gkg::Matrix >,
              Compare >::const_iterator
      l = _measures.find( bundleLabel );
    if ( l == _measures.end() )
    {

      throw std::runtime_error( "bundle label not found" );

    }
    std::map< std::string, gkg::Matrix >::const_iterator
      m = l->second.find( measureName );
    if ( m == l->second.end() )
    {

      throw std::runtime_error( "measure name not found" );

    }
    return m->second;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "const gkg::Matrix& "
             "gkg::BundleMeasureSpreadSheet< L, Compare >::getMeasure( "
             "const L& bundleLabel, "
             "const std::string& measureName ) const" );

}


#endif
