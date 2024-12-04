#ifndef _gkg_dmri_container_FiberMeasureSpreadSheet_i_h_
#define _gkg_dmri_container_FiberMeasureSpreadSheet_i_h_


#include <gkg-dmri-container/FiberMeasureSpreadSheet.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-dmri-fiber-measure/FiberMaximumCurvature.h>
#include <gkg-dmri-fiber-measure/FiberCurvatureProfile.h>
#include <gkg-dmri-fiber-measure/FiberCurvatureStatistics.h>
#include <gkg-dmri-fiber-measure/FiberFromVolumeProfile.h>
#include <gkg-dmri-fiber-measure/FiberFromVolumeStatistics.h>
#include <gkg-dmri-fiber-measure/FiberLength.h>
#include <gkg-dmri-fiber-measure/FiberTortuosity.h>
#include <gkg-core-exception/Exception.h>


template < class L, class Compare >
inline
gkg::FiberMeasureSpreadSheet< L, Compare >::FiberMeasureSpreadSheet()
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
                                     std::string( "FiberMeasureSpreadSheet" ) );
    _header.addAttribute( "label_type", gkg::TypeOf< L >::getName() );

    //  adding maximum curvature
    _functorLists[ "maximum_curvature" ].push_back(
      &gkg::FiberMaximumCurvature::spreadSheetFunctor< L, Compare > );

    //  adding curvature profile
    _functorLists[ "curvature_profile" ].push_back(
      &gkg::FiberCurvatureProfile::spreadSheetFunctor< L, Compare > );

    // adding curvature statistics
    _functorLists[ "curvature_statistics" ].push_back(
      &gkg::FiberCurvatureStatistics::spreadSheetFunctor< L, Compare > );

    // adding from volume profile
    _functorLists[ "from_volume_profile" ].push_back(
      &gkg::FiberFromVolumeProfile< int8_t >::spreadSheetFunctor< L,
                                                                  Compare > );
    _functorLists[ "from_volume_profile" ].push_back(
      &gkg::FiberFromVolumeProfile< uint8_t >::spreadSheetFunctor< L,
                                                                   Compare > );
    _functorLists[ "from_volume_profile" ].push_back(
      &gkg::FiberFromVolumeProfile< int16_t >::spreadSheetFunctor< L,
                                                                   Compare > );
    _functorLists[ "from_volume_profile" ].push_back(
      &gkg::FiberFromVolumeProfile< uint16_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile" ].push_back(
      &gkg::FiberFromVolumeProfile< int32_t >::spreadSheetFunctor< L,
                                                                   Compare > );
    _functorLists[ "from_volume_profile" ].push_back(
      &gkg::FiberFromVolumeProfile< uint32_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile" ].push_back(
      &gkg::FiberFromVolumeProfile< int64_t >::spreadSheetFunctor< L,
                                                                   Compare > );
    _functorLists[ "from_volume_profile" ].push_back(
      &gkg::FiberFromVolumeProfile< uint64_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile" ].push_back(
      &gkg::FiberFromVolumeProfile< float >::spreadSheetFunctor< L,
                                                                 Compare > );
    _functorLists[ "from_volume_profile" ].push_back(
      &gkg::FiberFromVolumeProfile< double >::spreadSheetFunctor< L,
                                                                  Compare > );

    // adding from volume statistics
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::FiberFromVolumeStatistics< int8_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::FiberFromVolumeStatistics< uint8_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::FiberFromVolumeStatistics< int16_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::FiberFromVolumeStatistics< uint16_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::FiberFromVolumeStatistics< int32_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::FiberFromVolumeStatistics< uint32_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::FiberFromVolumeStatistics< int64_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::FiberFromVolumeStatistics< uint64_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::FiberFromVolumeStatistics< float >::spreadSheetFunctor< L,
                                                                   Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::FiberFromVolumeStatistics< double >::spreadSheetFunctor< L,
                                                                    Compare > );

    // adding length
    _functorLists[ "length" ].push_back(
      &gkg::FiberLength::spreadSheetFunctor< L, Compare > );

    // adding tortuosity
    _functorLists[ "tortuosity" ].push_back(
      &gkg::FiberTortuosity::spreadSheetFunctor< L, Compare > ); 

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::FiberMeasureSpreadSheet< L, Compare >::"
             "FiberMeasureSpreadSheet()" );

}


template < class L, class Compare >
inline
gkg::FiberMeasureSpreadSheet< L, Compare >::FiberMeasureSpreadSheet(
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
                                     std::string( "FiberMeasureSpreadSheet" ) );
    _header.addAttribute( "label_type", gkg::TypeOf< L >::getName() );

    //  adding maximum curvature
    _functorLists[ "maximum_curvature" ].push_back(
      &gkg::FiberMaximumCurvature::spreadSheetFunctor< L, Compare > );

    //  adding curvature profile
    _functorLists[ "curvature_profile" ].push_back(
      &gkg::FiberCurvatureProfile::spreadSheetFunctor< L, Compare > );

    // adding curvature statistics
    _functorLists[ "curvature_statistics" ].push_back(
      &gkg::FiberCurvatureStatistics::spreadSheetFunctor< L, Compare > );

    // adding from volume profile
    _functorLists[ "from_volume_profile" ].push_back(
      &gkg::FiberFromVolumeProfile< int8_t >::spreadSheetFunctor< L,
                                                                  Compare > );
    _functorLists[ "from_volume_profile" ].push_back(
      &gkg::FiberFromVolumeProfile< uint8_t >::spreadSheetFunctor< L,
                                                                   Compare > );
    _functorLists[ "from_volume_profile" ].push_back(
      &gkg::FiberFromVolumeProfile< int16_t >::spreadSheetFunctor< L,
                                                                   Compare > );
    _functorLists[ "from_volume_profile" ].push_back(
      &gkg::FiberFromVolumeProfile< uint16_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile" ].push_back(
      &gkg::FiberFromVolumeProfile< int32_t >::spreadSheetFunctor< L,
                                                                   Compare > );
    _functorLists[ "from_volume_profile" ].push_back(
      &gkg::FiberFromVolumeProfile< uint32_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile" ].push_back(
      &gkg::FiberFromVolumeProfile< int64_t >::spreadSheetFunctor< L,
                                                                   Compare > );
    _functorLists[ "from_volume_profile" ].push_back(
      &gkg::FiberFromVolumeProfile< uint64_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_profile" ].push_back(
      &gkg::FiberFromVolumeProfile< float >::spreadSheetFunctor< L,
                                                                 Compare > );
    _functorLists[ "from_volume_profile" ].push_back(
      &gkg::FiberFromVolumeProfile< double >::spreadSheetFunctor< L,
                                                                  Compare > );

    // adding from volume statistics
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::FiberFromVolumeStatistics< int8_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::FiberFromVolumeStatistics< uint8_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::FiberFromVolumeStatistics< int16_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::FiberFromVolumeStatistics< uint16_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::FiberFromVolumeStatistics< int32_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::FiberFromVolumeStatistics< uint32_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::FiberFromVolumeStatistics< int64_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::FiberFromVolumeStatistics< uint64_t >::spreadSheetFunctor< L,
                                                                    Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::FiberFromVolumeStatistics< float >::spreadSheetFunctor< L,
                                                                   Compare > );
    _functorLists[ "from_volume_statistics" ].push_back(
      &gkg::FiberFromVolumeStatistics< double >::spreadSheetFunctor< L,
                                                                    Compare > );

    // adding length
    _functorLists[ "length" ].push_back(
      &gkg::FiberLength::spreadSheetFunctor< L, Compare > );

    // adding tortuosity
    _functorLists[ "tortuosity" ].push_back(
      &gkg::FiberTortuosity::spreadSheetFunctor< L, Compare > ); 

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::FiberMeasureSpreadSheet< L, Compare >::"
             "FiberMeasureSpreadSheet( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap )" );

}


template < class L, class Compare >
inline
gkg::FiberMeasureSpreadSheet< L, Compare >::FiberMeasureSpreadSheet(
                       const gkg::FiberMeasureSpreadSheet< L, Compare >& other )
                             : gkg::HeaderedObject( other ),
                               _functorLists( other._functorLists ),
                               _bundleMap( other._bundleMap ),
                               _measures( other._measures )
{
}


template < class L, class Compare >
inline
gkg::FiberMeasureSpreadSheet< L, Compare >::~FiberMeasureSpreadSheet()
{
}


template < class L, class Compare >
inline
gkg::FiberMeasureSpreadSheet< L, Compare >& 
gkg::FiberMeasureSpreadSheet< L, Compare >::operator=(
                       const gkg::FiberMeasureSpreadSheet< L, Compare >& other )
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
             "gkg::FiberMeasureSpreadSheet< L, Compare >& "
             "gkg::FiberMeasureSpreadSheet< L, Compare >::operator=( "
             "const gkg::FiberMeasureSpreadSheet< L, Compare >& other )" );

}


template < class L, class Compare >
inline
typename gkg::FiberMeasureSpreadSheet< L, Compare >::iterator 
gkg::FiberMeasureSpreadSheet< L, Compare >::begin()
{

  return _measures.begin();

}


template < class L, class Compare >
inline
typename gkg::FiberMeasureSpreadSheet< L, Compare >::iterator 
gkg::FiberMeasureSpreadSheet< L, Compare >::end()
{

  return _measures.end();

}


template < class L, class Compare >
inline
typename gkg::FiberMeasureSpreadSheet< L, Compare >::const_iterator 
gkg::FiberMeasureSpreadSheet< L, Compare >::begin() const
{

  return _measures.begin();

}


template < class L, class Compare >
inline
typename gkg::FiberMeasureSpreadSheet< L, Compare >::const_iterator 
gkg::FiberMeasureSpreadSheet< L, Compare >::end() const
{

  return _measures.end();

}


template < class L, class Compare >
inline
typename gkg::FiberMeasureSpreadSheet< L, Compare >::reverse_iterator 
gkg::FiberMeasureSpreadSheet< L, Compare >::rbegin()
{

  return _measures.rbegin();

}


template < class L, class Compare >
inline
typename gkg::FiberMeasureSpreadSheet< L, Compare >::reverse_iterator 
gkg::FiberMeasureSpreadSheet< L, Compare >::rend()
{

  return _measures.rend();

}


template < class L, class Compare >
inline
typename gkg::FiberMeasureSpreadSheet< L, Compare >::const_reverse_iterator 
gkg::FiberMeasureSpreadSheet< L, Compare >::rbegin() const
{

  return _measures.rbegin();

}


template < class L, class Compare >
inline
typename gkg::FiberMeasureSpreadSheet< L, Compare >::const_reverse_iterator
gkg::FiberMeasureSpreadSheet< L, Compare >::rend() const
{

  return _measures.rend();

}


template < class L, class Compare >
inline
typename gkg::FiberMeasureSpreadSheet< L, Compare >::iterator 
gkg::FiberMeasureSpreadSheet< L, Compare >::lower_bound( const L& label )
{

  return _measures.lower_bound( label );

}


template < class L, class Compare >
inline
typename gkg::FiberMeasureSpreadSheet< L, Compare >::iterator
gkg::FiberMeasureSpreadSheet< L, Compare >::upper_bound( const L& label )
{

  return _measures.upper_bound( label );

}


template < class L, class Compare >
inline
typename gkg::FiberMeasureSpreadSheet< L, Compare >::const_iterator
gkg::FiberMeasureSpreadSheet< L, Compare >::lower_bound( const L& label ) const
{

  return _measures.lower_bound( label );

}


template < class L, class Compare >
inline
typename gkg::FiberMeasureSpreadSheet< L, Compare >::const_iterator
gkg::FiberMeasureSpreadSheet< L, Compare >::upper_bound( const L& label ) const
{

  return _measures.upper_bound( label );

}


template < class L, class Compare >
inline
std::pair< typename gkg::FiberMeasureSpreadSheet< L, Compare >::iterator,
           typename gkg::FiberMeasureSpreadSheet< L, Compare >::iterator >
gkg::FiberMeasureSpreadSheet< L, Compare >::equal_range( const L& label )
{

  return _measures.equal_range( label );

}


template < class L, class Compare >
inline
std::pair< typename gkg::FiberMeasureSpreadSheet< L, Compare >::const_iterator,
           typename gkg::FiberMeasureSpreadSheet< L, Compare >::const_iterator >
gkg::FiberMeasureSpreadSheet< L, Compare >::equal_range( const L& label ) const
{

  return _measures.equal_range( label );

}


template < class L, class Compare >
inline
typename gkg::FiberMeasureSpreadSheet< L, Compare >::iterator
gkg::FiberMeasureSpreadSheet< L, Compare >::find( const L& label )
{

  return _measures.find( label );

}



template < class L, class Compare >
inline
void gkg::FiberMeasureSpreadSheet< L, Compare >::setBundleMap(
                      gkg::RCPointer< gkg::BundleMap< L, Compare > > bundleMap )
{

  try
  {

    _bundleMap = bundleMap;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::FiberMeasureSpreadSheet< L, Compare >::setBundleMap( "
             "gkg::RCPointer< BundleMap< L, Compare > > bundleMap )" );

}


template < class L, class Compare >
inline
void gkg::FiberMeasureSpreadSheet< L, Compare >::addMeasure(
                             const std::string& name,
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters )
{

  try
  {

    typename
    std::map< std::string, std::list< 
    typename gkg::FiberMeasureSpreadSheet< L,
                                          Compare >::Functor > >::const_iterator
      l = _functorLists.find( name );
    if ( l == _functorLists.end() )
    {

      throw std::runtime_error( "measure name not found in functor map" );

    }
    bool measureAdded = false;
    typename
    std::list< typename 
           gkg::FiberMeasureSpreadSheet< L, Compare >::Functor >::const_iterator
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
             "void gkg::FiberMeasureSpreadSheet< L, Compare >::addMeasure( "
             "const std::string& name, "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters )" );

}


template < class L, class Compare >
inline
void gkg::FiberMeasureSpreadSheet< L, Compare >::addMeasure(
                                              const L& bundleLabel,
                                              int32_t fiberIndex,
                                              const std::string& measureName,
                                              const gkg::Matrix& measureMatrix )
{

  try
  {

    _measures[ bundleLabel ][ fiberIndex ][ measureName ] = measureMatrix;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::FiberMeasureSpreadSheet< L, Compare >::addMeasure( "
             "const L& bundleLabel, "
             "int32_t fiberIndex, "
             "const std::string& measureName, "
             "const gkg::Matrix& measureMatrix )" );

}


template < class L, class Compare >
inline
const gkg::Matrix& 
gkg::FiberMeasureSpreadSheet< L, Compare >::getMeasure(
                                          const L& bundleLabel,
                                          int32_t fiberIndex,
                                          const std::string& measureName ) const
{

  try
  {

    typename
    std::map< L, 
              std::map< int32_t,
                        std::map< std::string, gkg::Matrix > >,
              Compare >::const_iterator
      l = _measures.find( bundleLabel );
    if ( l == _measures.end() )
    {

      throw std::runtime_error( "bundle label not found" );

    }
    std::map< int32_t, std::map< std::string, gkg::Matrix > >::const_iterator
      f = l->second.find( fiberIndex );
    if ( f == l->second.end() )
    {

      throw std::runtime_error( "fiber index not found" );

    }
    std::map< std::string, gkg::Matrix >::const_iterator
      m = f->second.find( measureName );
    if ( m == f->second.end() )
    {

      throw std::runtime_error( "measure name not found" );

    }
    return m->second;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "const gkg::Matrix& "
             "gkg::FiberMeasureSpreadSheet< L, Compare >::getMeasure( "
             "const L& bundleLabel, "
             "int32_t fiberIndex, "
             "const std::string& measureName ) const" );

}


#endif
