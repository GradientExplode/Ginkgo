#ifndef _gkg_dmri_fiber_selector_FiberSelection_i_h_
#define _gkg_dmri_fiber_selector_FiberSelection_i_h_


#include <gkg-dmri-fiber-selector/FiberSelection.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-dmri-fiber-selector/DensityMapBasedFiberSelector.h>
#include <gkg-dmri-fiber-selector/MeasureBasedFiberSelector_i.h>
#include <gkg-dmri-fiber-selector/RoiBasedFiberSelector.h>
#include <gkg-dmri-fiber-selector/DensityMapBasedFiberSelector.h>

#include <gkg-dmri-fiber-measure/FiberMaximumCurvature.h>
#include <gkg-dmri-fiber-measure/FiberLength.h>
#include <gkg-dmri-fiber-measure/FiberTortuosity.h>

#include <gkg-core-exception/Exception.h>


template < class L, class Compare >
inline
gkg::FiberSelection< L, Compare >::FiberSelection()
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
    _header.addAttribute( "object_type", std::string( "FiberSelection" ) );
    _header.addAttribute( "label_type", gkg::TypeOf< L >::getName() );

    //  adding maximum curvature selector
    _selectorLists[ "maximum_curvature" ].push_back(
      &gkg::FiberMaximumCurvature::selector< L, Compare > );

    // adding length selector
    _selectorLists[ "length" ].push_back(
      &gkg::FiberLength::selector< L, Compare > );

    // adding tortuosity selector
    _selectorLists[ "tortuosity" ].push_back(
      &gkg::FiberTortuosity::selector< L, Compare > );

    // adding ROI based selectors
    _selectorLists[ "roi_based" ].push_back(
      &gkg::RoiBasedFiberSelector< int8_t >::selector< L, Compare > );

    _selectorLists[ "roi_based" ].push_back(
      &gkg::RoiBasedFiberSelector< uint8_t >::selector< L, Compare > );

    _selectorLists[ "roi_based" ].push_back(
      &gkg::RoiBasedFiberSelector< int16_t >::selector< L, Compare > );

    _selectorLists[ "roi_based" ].push_back(
      &gkg::RoiBasedFiberSelector< uint16_t >::selector< L, Compare > );

    _selectorLists[ "roi_based" ].push_back(
      &gkg::RoiBasedFiberSelector< int32_t >::selector< L, Compare > );

    _selectorLists[ "roi_based" ].push_back(
      &gkg::RoiBasedFiberSelector< uint32_t >::selector< L, Compare > );

    _selectorLists[ "roi_based" ].push_back(
      &gkg::RoiBasedFiberSelector< int64_t >::selector< L, Compare > );

    _selectorLists[ "roi_based" ].push_back(
      &gkg::RoiBasedFiberSelector< uint64_t >::selector< L, Compare > );

    _selectorLists[ "roi_based" ].push_back(
      &gkg::RoiBasedFiberSelector< float >::selector< L, Compare > );

    _selectorLists[ "roi_based" ].push_back(
      &gkg::RoiBasedFiberSelector< double >::selector< L, Compare > );

    // adding density map based selectors
    _selectorLists[ "density_map_based" ].push_back(
      &gkg::DensityMapBasedFiberSelector::selector< L, Compare > );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::FiberSelection< L, Compare >::"
             "FiberSelection()" );

}


template < class L, class Compare >
inline
gkg::FiberSelection< L, Compare >::FiberSelection(
                 gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap )
                                  : gkg::HeaderedObject(),
                                    _inputBundleMap( inputBundleMap )
{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
    _syntaxSet[ "__generic__" ][ "label_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", std::string( "FiberSelection" ) );
    _header.addAttribute( "label_type", gkg::TypeOf< L >::getName() );

    //  adding maximum curvature selector
    _selectorLists[ "maximum_curvature" ].push_back(
      &gkg::FiberMaximumCurvature::selector< L, Compare > );

    // adding length selector
    _selectorLists[ "length" ].push_back(
      &gkg::FiberLength::selector< L, Compare > );

    // adding tortuosity selector
    _selectorLists[ "tortuosity" ].push_back(
      &gkg::FiberTortuosity::selector< L, Compare > ); 

    // adding ROI based selectors
    _selectorLists[ "roi_based" ].push_back(
      &gkg::RoiBasedFiberSelector< int8_t >::selector< L, Compare > );

    _selectorLists[ "roi_based" ].push_back(
      &gkg::RoiBasedFiberSelector< uint8_t >::selector< L, Compare > );

    _selectorLists[ "roi_based" ].push_back(
      &gkg::RoiBasedFiberSelector< int16_t >::selector< L, Compare > );

    _selectorLists[ "roi_based" ].push_back(
      &gkg::RoiBasedFiberSelector< uint16_t >::selector< L, Compare > );

    _selectorLists[ "roi_based" ].push_back(
      &gkg::RoiBasedFiberSelector< int32_t >::selector< L, Compare > );

    _selectorLists[ "roi_based" ].push_back(
      &gkg::RoiBasedFiberSelector< uint32_t >::selector< L, Compare > );

    _selectorLists[ "roi_based" ].push_back(
      &gkg::RoiBasedFiberSelector< int64_t >::selector< L, Compare > );

    _selectorLists[ "roi_based" ].push_back(
      &gkg::RoiBasedFiberSelector< uint64_t >::selector< L, Compare > );

    _selectorLists[ "roi_based" ].push_back(
      &gkg::RoiBasedFiberSelector< float >::selector< L, Compare > );

    _selectorLists[ "roi_based" ].push_back(
      &gkg::RoiBasedFiberSelector< double >::selector< L, Compare > );

    // adding density map based selectors
    _selectorLists[ "density_map_based" ].push_back(
      &gkg::DensityMapBasedFiberSelector::selector< L, Compare > );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::FiberSelection< L, Compare >::"
             "FiberSelection( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > "
             "inputBundleMap )" );

}


template < class L, class Compare >
inline
gkg::FiberSelection< L, Compare >::FiberSelection(
                                const gkg::FiberSelection< L, Compare >& other )
                             : gkg::HeaderedObject( other ),
                               _selectorLists( other._selectorLists ),
                               _inputBundleMap( other._inputBundleMap ),
                               _selectedBundleMap( other._selectedBundleMap )
{
}


template < class L, class Compare >
inline
gkg::FiberSelection< L, Compare >::~FiberSelection()
{
}


template < class L, class Compare >
inline
gkg::FiberSelection< L, Compare >& 
gkg::FiberSelection< L, Compare >::operator=(
                                const gkg::FiberSelection< L, Compare >& other )
{


  try
  {

    this->gkg::HeaderedObject::operator=( other );
    _selectorLists = other._selectorLists;
    _inputBundleMap = other._inputBundleMap;
    _selectedBundleMap = other._selectedBundleMap;

    return *this;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::FiberSelection< L, Compare >& "
             "gkg::FiberSelection< L, Compare >::operator=( "
             "const gkg::FiberSelection< L, Compare >& other )" );

}


template < class L, class Compare >
inline
typename gkg::FiberSelection< L, Compare >::iterator 
gkg::FiberSelection< L, Compare >::begin()
{

  return _selectedBundleMap.begin();

}


template < class L, class Compare >
inline
typename gkg::FiberSelection< L, Compare >::iterator 
gkg::FiberSelection< L, Compare >::end()
{

  return _selectedBundleMap.end();

}


template < class L, class Compare >
inline
typename gkg::FiberSelection< L, Compare >::const_iterator 
gkg::FiberSelection< L, Compare >::begin() const
{

  return _selectedBundleMap.begin();

}


template < class L, class Compare >
inline
typename gkg::FiberSelection< L, Compare >::const_iterator 
gkg::FiberSelection< L, Compare >::end() const
{

  return _selectedBundleMap.end();

}


template < class L, class Compare >
inline
typename gkg::FiberSelection< L, Compare >::reverse_iterator 
gkg::FiberSelection< L, Compare >::rbegin()
{

  return _selectedBundleMap.rbegin();

}


template < class L, class Compare >
inline
typename gkg::FiberSelection< L, Compare >::reverse_iterator 
gkg::FiberSelection< L, Compare >::rend()
{

  return _selectedBundleMap.rend();

}


template < class L, class Compare >
inline
typename gkg::FiberSelection< L, Compare >::const_reverse_iterator 
gkg::FiberSelection< L, Compare >::rbegin() const
{

  return _selectedBundleMap.rbegin();

}


template < class L, class Compare >
inline
typename gkg::FiberSelection< L, Compare >::const_reverse_iterator
gkg::FiberSelection< L, Compare >::rend() const
{

  return _selectedBundleMap.rend();

}


template < class L, class Compare >
inline
typename gkg::FiberSelection< L, Compare >::iterator 
gkg::FiberSelection< L, Compare >::lower_bound( const L& label )
{

  return _selectedBundleMap.lower_bound( label );

}


template < class L, class Compare >
inline
typename gkg::FiberSelection< L, Compare >::iterator
gkg::FiberSelection< L, Compare >::upper_bound( const L& label )
{

  return _selectedBundleMap.upper_bound( label );

}


template < class L, class Compare >
inline
typename gkg::FiberSelection< L, Compare >::const_iterator
gkg::FiberSelection< L, Compare >::lower_bound( const L& label ) const
{

  return _selectedBundleMap.lower_bound( label );

}


template < class L, class Compare >
inline
typename gkg::FiberSelection< L, Compare >::const_iterator
gkg::FiberSelection< L, Compare >::upper_bound( const L& label ) const
{

  return _selectedBundleMap.upper_bound( label );

}


template < class L, class Compare >
inline
std::pair< typename gkg::FiberSelection< L, Compare >::iterator,
           typename gkg::FiberSelection< L, Compare >::iterator >
gkg::FiberSelection< L, Compare >::equal_range( const L& label )
{

  return _selectedBundleMap.equal_range( label );

}


template < class L, class Compare >
inline
std::pair< typename gkg::FiberSelection< L, Compare >::const_iterator,
           typename gkg::FiberSelection< L, Compare >::const_iterator >
gkg::FiberSelection< L, Compare >::equal_range( const L& label ) const
{

  return _selectedBundleMap.equal_range( label );

}


template < class L, class Compare >
inline
typename gkg::FiberSelection< L, Compare >::iterator
gkg::FiberSelection< L, Compare >::find( const L& label )
{

  return _selectedBundleMap.find( label );

}


template < class L, class Compare >
inline
void gkg::FiberSelection< L, Compare >::setInputBundleMap(
                 gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap )
{

  try
  {

    _inputBundleMap = inputBundleMap;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::FiberSelection< L, Compare >::setInputBundleMap( "
             "RCPointer< BundleMap< L, Compare > > inputBundleMap )" );

}


template < class L, class Compare >
inline
void gkg::FiberSelection< L, Compare >::addSelector(
                             const std::string& name,
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters )
{

  try
  {

    typename
    std::map< std::string, std::list< 
    typename gkg::FiberSelection< L, Compare >::Selector > >::const_iterator
      l = _selectorLists.find( name );
    if ( l == _selectorLists.end() )
    {

      throw std::runtime_error( "selector name not found in selector map" );

    }
    bool selectorAdded = false;
    typename
    std::list< typename 
           gkg::FiberSelection< L, Compare >::Selector >::const_iterator
      s = l->second.begin(),
      se = l->second.end();
    while ( s != se )
    {

      try
      {

        std::map< L, std::set< int32_t >, Compare > selectedBundles;
        ( *s )( stringParameters,
                scalarParameters,
                _inputBundleMap,
                selectedBundles );

        // looping over label(s)
        typename gkg::BundleMap< L, Compare >::const_iterator
          l = _inputBundleMap->begin(),
          le = _inputBundleMap->end();
        while ( l != le )
        {

          // obtaining current label
          const L& label = l->first;

          typename std::map< L, std::set< int32_t >, Compare >::const_iterator
            b = selectedBundles.find( label );
          // if found then selecting
          if ( b != selectedBundles.end() )
          {

            // getting access to the original bundle
            const typename gkg::BundleMap< L, Compare >::Bundle&
              inputBundle = l->second;

            // getting access to the selected fibers for the current label
            const std::set< int32_t >& selectedFibers = b->second;

            std::cout << "label=" << label
                      << " fiber count=" << selectedFibers.size()
                      << " ... " << std::flush;
            // creating a list of the selected fibers
            std::list< typename gkg::BundleMap< L, Compare >::Fiber > fibers;
            std::set< int32_t >::const_iterator
              i = selectedFibers.begin(),
              ie = selectedFibers.end();
            while ( i != ie )
            {

              fibers.push_back( inputBundle.getCurve3d( *i ) );
              ++ i;

            }

            // adding the selected fibers to the bundle map
            _selectedBundleMap.addCurve3ds( label, fibers );

          }
          ++ l;

        }


        selectorAdded = true;
        break;

      }
      catch ( std::exception& )
      {
      }
      ++ s;

    }
    if ( !selectorAdded )
    {

      throw std::runtime_error( std::string( "could not add selector '" ) +
                                name + "'" );

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::FiberSelection< L, Compare >::addSelector( "
             "const std::string& name, "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters )" );

}


template < class L, class Compare >
inline
gkg::BundleMap< L, Compare >&
gkg::FiberSelection< L, Compare >::getSelectedBundleMap()
{

  try
  {

    return _selectedBundleMap;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMap< L, Compare >& "
             "gkg::FiberSelection< L, Compare >::"
             "getSelectedBundleMap()" );

}



#endif

