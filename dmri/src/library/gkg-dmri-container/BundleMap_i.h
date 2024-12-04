#ifndef _gkg_dmri_container_BundleMap_i_h_
#define _gkg_dmri_container_BundleMap_i_h_


#include <gkg-dmri-container/BundleMap.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-exception/Exception.h>
#include <map>


template < class L, class Compare >
inline
gkg::BundleMap< L, Compare >::BundleMap()
                             : gkg::HeaderedObject()
{

  // adding a generic syntax set
  _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "label_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "item_count" ] = 
    gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );
  _syntaxSet[ "__generic__" ][ "labels" ] = 
    gkg::Semantic( gkg::TypeOf< std::vector< std::string > >::getName(), true );
  _syntaxSet[ "__generic__" ][ "curve3d_counts" ] = 
    gkg::Semantic( gkg::TypeOf< std::vector< int32_t > >::getName(), true );

  // adding attributes to header
  _header.addAttribute( "object_type", std::string( "BundleMap" ) );
  _header.addAttribute( "label_type", gkg::TypeOf< L >::getName() );
  _header.addAttribute( "item_count", 0 );

}


template < class L, class Compare >
inline
gkg::BundleMap< L, Compare >::BundleMap(
              const std::list< L >& labels,
              const std::list< gkg::BundleMap< L, Compare >::Bundle >& bundles )
                             : gkg::HeaderedObject()
{

  // adding a generic syntax set
  _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "label_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "item_count" ] = 
    gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );
  _syntaxSet[ "__generic__" ][ "labels" ] = 
    gkg::Semantic( gkg::TypeOf< std::vector< std::string > >::getName(), true );
  _syntaxSet[ "__generic__" ][ "curve3d_counts" ] = 
    gkg::Semantic( gkg::TypeOf< std::vector< int32_t > >::getName(), true );

  // adding attributes to header
  _header.addAttribute( "object_type", std::string( "BundleMap" ) );
  _header.addAttribute( "label_type", gkg::TypeOf< L >::getName() );
  _header.addAttribute( "item_count", 0 );

  addBundles( labels, bundles );

}


template < class L, class Compare >
inline
gkg::BundleMap< L, Compare >::BundleMap(
            const std::vector< L >& labels,
            const std::vector< gkg::BundleMap< L, Compare >::Bundle >& bundles )
                             : gkg::HeaderedObject()
{

  // adding a generic syntax set
  _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "label_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "item_count" ] = 
    gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );
  _syntaxSet[ "__generic__" ][ "labels" ] = 
    gkg::Semantic( gkg::TypeOf< std::vector< std::string > >::getName(), true );
  _syntaxSet[ "__generic__" ][ "curve3d_counts" ] = 
    gkg::Semantic( gkg::TypeOf< std::vector< int32_t > >::getName(), true );

  // adding attributes to header
  _header.addAttribute( "object_type", std::string( "BundleMap" ) );
  _header.addAttribute( "label_type", gkg::TypeOf< L >::getName() );
  _header.addAttribute( "item_count", 0 );

  addBundles( labels, bundles );

}


template < class L, class Compare >
inline
gkg::BundleMap< L, Compare >::BundleMap(
                                     const gkg::BundleMap< L, Compare >& other )
                             : gkg::HeaderedObject( other ),
                               _bundles( other._bundles ),
                               _labels( other._labels )
{
}


template < class L, class Compare >
inline
gkg::BundleMap< L, Compare >::~BundleMap()
{
}


template < class L, class Compare >
inline
gkg::BundleMap< L, Compare >& 
gkg::BundleMap< L, Compare >::operator=(
                                     const gkg::BundleMap< L, Compare >& other )
{

  try
  {

    this->gkg::HeaderedObject::operator=( other );
    _bundles = other._bundles;
    _labels = other._labels;

    return *this;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMap< L, Compare >& "
             "gkg::BundleMap< L, Compare >::operator=( "
             "const gkg::BundleMap< L, Compare >& other )" );

}


template < class L, class Compare >
inline
typename gkg::BundleMap< L, Compare >::iterator 
gkg::BundleMap< L, Compare >::begin()
{

  return _bundles.begin();

}


template < class L, class Compare >
inline
typename gkg::BundleMap< L, Compare >::iterator 
gkg::BundleMap< L, Compare >::end()
{

  return _bundles.end();

}


template < class L, class Compare >
inline
typename gkg::BundleMap< L, Compare >::const_iterator 
gkg::BundleMap< L, Compare >::begin() const
{

  return _bundles.begin();

}


template < class L, class Compare >
inline
typename gkg::BundleMap< L, Compare >::const_iterator 
gkg::BundleMap< L, Compare >::end() const
{

  return _bundles.end();

}


template < class L, class Compare >
inline
typename gkg::BundleMap< L, Compare >::reverse_iterator 
gkg::BundleMap< L, Compare >::rbegin()
{

  return _bundles.rbegin();

}


template < class L, class Compare >
inline
typename gkg::BundleMap< L, Compare >::reverse_iterator 
gkg::BundleMap< L, Compare >::rend()
{

  return _bundles.rend();

}


template < class L, class Compare >
inline
typename gkg::BundleMap< L, Compare >::const_reverse_iterator 
gkg::BundleMap< L, Compare >::rbegin() const
{

  return _bundles.rbegin();

}


template < class L, class Compare >
inline
typename gkg::BundleMap< L, Compare >::const_reverse_iterator
gkg::BundleMap< L, Compare >::rend() const
{

  return _bundles.rend();

}


template < class L, class Compare >
inline
typename gkg::BundleMap< L, Compare >::iterator 
gkg::BundleMap< L, Compare >::lower_bound( const L& label )
{

  return _bundles.lower_bound( label );

}


template < class L, class Compare >
inline
typename gkg::BundleMap< L, Compare >::iterator
gkg::BundleMap< L, Compare >::upper_bound( const L& label )
{

  return _bundles.upper_bound( label );

}


template < class L, class Compare >
inline
typename gkg::BundleMap< L, Compare >::const_iterator
gkg::BundleMap< L, Compare >::lower_bound( const L& label ) const
{

  return _bundles.lower_bound( label );

}


template < class L, class Compare >
inline
typename gkg::BundleMap< L, Compare >::const_iterator
gkg::BundleMap< L, Compare >::upper_bound( const L& label ) const
{

  return _bundles.upper_bound( label );

}


template < class L, class Compare >
inline
std::pair< typename gkg::BundleMap< L, Compare >::iterator,
           typename gkg::BundleMap< L, Compare >::iterator >
gkg::BundleMap< L, Compare >::equal_range( const L& label )
{

  return _bundles.equal_range( label );

}


template < class L, class Compare >
inline
std::pair< typename gkg::BundleMap< L, Compare >::const_iterator,
           typename gkg::BundleMap< L, Compare >::const_iterator >
gkg::BundleMap< L, Compare >::equal_range( const L& label ) const
{

  return _bundles.equal_range( label );

}


template < class L, class Compare >
inline
typename gkg::BundleMap< L, Compare >::iterator
gkg::BundleMap< L, Compare >::find( const L& label )
{

  return _bundles.find( label );

}


template < class L, class Compare >
inline
typename gkg::BundleMap< L, Compare >::Bundle& 
gkg::BundleMap< L, Compare >::addBundle(
                   const L& label,
                   const typename gkg::BundleMap< L, Compare >::Bundle& bundle )
{


  try
  {

    if ( _bundles.find( label ) == _bundles.end() )
    {

      _labels.insert( label );

    }
    else
    {

      throw std::runtime_error( "label already used" );

    }
    typename gkg::BundleMap< L, Compare >::Bundle& 
      theBundle = _bundles[ label ] = bundle;

    updateHeader();

    return theBundle;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "typename gkg::BundleMap< L, Compare >::Bundle&  "
             "gkg::BundleMap< L, Compare >::addBundle( "
             "const L& label, "
             "const gkg::BundleMap< L, Compare >::Bundle& bundle )" );

}


template < class L, class Compare >
inline
void gkg::BundleMap< L, Compare >::addBundles(
     const std::list< L >& labels,
     const std::list< typename gkg::BundleMap< L, Compare >::Bundle >& bundles )
{


  try
  {

    if ( labels.size() != bundles.size() )
    {

      throw std::runtime_error( "labels and bundles have incoherent size(s)" );

    }

    typename std::list< L >::const_iterator l = labels.begin();
    typename 
    std::list< typename gkg::BundleMap< L, Compare >::Bundle >::const_iterator
      b = bundles.begin(),
      be = bundles.end();
    while ( b != be )
    {

      if ( _bundles.find( *l ) == _bundles.end() )
      {

        _labels.insert( *l );

      }
      else
      {

        throw std::runtime_error( "label already used" );

      }
      _bundles[ *l ] = *b;

      ++ l;
      ++ b;

    }
    updateHeader();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMap< L, Compare >::addBundles( "
             "const std::list< L >& labels, "
             "const std::list< "
             "typename gkg::BundleMap< L, Compare >::Bundle >& "
             "bundles )" );

}


template < class L, class Compare >
inline
void gkg::BundleMap< L, Compare >::addBundles(
   const std::vector< L >& labels,
   const std::vector< typename gkg::BundleMap< L, Compare >::Bundle >& bundles )
{


  try
  {

    if ( labels.size() != bundles.size() )
    {

      throw std::runtime_error( "labels and bundles have incoherent size(s)" );

    }

    typename std::vector< L >::const_iterator l = labels.begin();
    typename
    std::vector< typename gkg::BundleMap< L, Compare >::Bundle >::const_iterator
      b = bundles.begin(),
      be = bundles.end();
    while ( b != be )
    {

      if ( _bundles.find( *l ) == _bundles.end() )
      {

        _labels.insert( *l );

      }
      else
      {

        throw std::runtime_error( "label already used" );

      }
      _bundles[ *l ] = *b;

      ++ l;
      ++ b;

    }
    updateHeader();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMap< L, Compare >::addBundles( "
             "const std::vector< L >& labels, "
             "const std::vector< "
             "typename gkg::BundleMap< L, Compare >::Bundle >& "
             "bundles )" );

}


template < class L, class Compare >
inline
bool gkg::BundleMap< L, Compare >::hasBundle( const L& label ) const
{

  try
  {

    typename std::map< L,
                       typename gkg::BundleMap< L, Compare >::Bundle,
                       Compare >::const_iterator
      l =  _bundles.find( label );

    if ( l == _bundles.end() )
    {

      return false;


    }
    return true;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "bool gkg::BundleMap< L, Compare >::hasBundle( "
             "const L& label ) const" );

}


template < class L, class Compare >
inline
void 
gkg::BundleMap< L, Compare >::addCurve3ds(
                  const L& label,
                  const std::list< gkg::LightCurve3d< float > >& lightCurve3ds )
{


  try
  {

    typename std::map< L,
                       typename gkg::BundleMap< L, Compare >::Bundle,
                       Compare >::iterator
      b =  _bundles.find( label );

    if ( b == _bundles.end() )
    {

      _labels.insert( label );
      _bundles[ label ] = typename gkg::BundleMap< L, Compare >::Bundle(
                                                                lightCurve3ds );

    }
    else
    {

      // was the origin of net computation time increase, so discarded it
      // pb dealt by O. Riff
      // b->second.reserve( b->second.getCurve3dCount() +
      //                    ( int32_t )lightCurve3ds.size() );
      b->second.addCurve3ds( lightCurve3ds );

    }

    updateHeader();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void  "
             "gkg::BundleMap< L, Compare >::addBundle( "
             "const L& label, "
             "const std::list< gkg::LightCurve3d< float > >& "
             "lightCurve3ds )" );

}


template < class L, class Compare >
inline
void 
gkg::BundleMap< L, Compare >::addCurve3ds(
                const L& label,
                const std::vector< gkg::LightCurve3d< float > >& lightCurve3ds )
{


  try
  {

    typename std::map< L,
                       typename gkg::BundleMap< L, Compare >::Bundle,
                       Compare >::iterator
      b =  _bundles.find( label );

    if ( b == _bundles.end() )
    {

      _labels.insert( label );
      _bundles[ label ] = typename gkg::BundleMap< L, Compare >::Bundle(
                                                                lightCurve3ds );

    }
    else
    {

      b->second.reserve( b->second.getCurve3dCount() +
                         ( int32_t )lightCurve3ds.size() );
      b->second.addCurve3ds( lightCurve3ds );

    }

    updateHeader();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void  "
             "gkg::BundleMap< L, Compare >::addCurve3ds( "
             "const L& label, "
             "const std::vector< gkg::LightCurve3d< float > >& "
             "lightCurve3ds )" );

}


template < class L, class Compare >
inline
void 
gkg::BundleMap< L, Compare >::addCurve3ds(
                            const L& label,
                            const std::list< gkg::Curve3d< float > >& curve3ds )
{


  try
  {

    typename std::map< L,
                       typename gkg::BundleMap< L, Compare >::Bundle,
                       Compare >::iterator
      b =  _bundles.find( label );

    if ( b == _bundles.end() )
    {

      _labels.insert( label );
      _bundles[ label ] = typename gkg::BundleMap< L, Compare >::Bundle(
                                                                     curve3ds );

    }
    else
    {

      b->second.reserve( b->second.getCurve3dCount() +
                         ( int32_t )curve3ds.size() );
      b->second.addCurve3ds( curve3ds );

    }

    updateHeader();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void  "
             "gkg::BundleMap< L, Compare >::addCurve3ds( "
             "const L& label, "
             "const std::list< gkg::Curve3d< float > >& curve3ds )" );

}


template < class L, class Compare >
inline
void 
gkg::BundleMap< L, Compare >::addCurve3ds(
                          const L& label,
                          const std::vector< gkg::Curve3d< float > >& curve3ds )
{


  try
  {

    typename std::map< L,
                       typename gkg::BundleMap< L, Compare >::Bundle,
                       Compare >::iterator
      b =  _bundles.find( label );

    if ( b == _bundles.end() )
    {

      _labels.insert( label );
      _bundles[ label ] = typename gkg::BundleMap< L, Compare >::Bundle(
                                                                     curve3ds );

    }
    else
    {

      b->second.reserve( b->second.getCurve3dCount() +
                         ( int32_t )curve3ds.size() );
      b->second.addCurve3ds( curve3ds );

    }

    updateHeader();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void  "
             "gkg::BundleMap< L, Compare >::addCurve3ds( "
             "const L& label, "
             "const std::vector< gkg::Curve3d< float > >& curve3ds )" );

}


template < class L, class Compare >
inline
void gkg::BundleMap< L, Compare >::removeBundle( const L& label )
{

  try
  {

    _bundles.erase( label );

    updateHeader();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMap< L, Compare >::removeBundle( "
             "const L& label )" );

}


template < class L, class Compare >
inline
void gkg::BundleMap< L, Compare >::clear()
{

  try
  {

    _bundles.clear();
    _labels.clear();

    updateHeader();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMap< L, Compare >::clear()" );

}


template < class L, class Compare >
inline
const std::set< L, Compare >&
gkg::BundleMap< L, Compare >::getLabels() const
{

  return _labels;

}



template < class L, class Compare >
inline
const typename gkg::BundleMap< L, Compare >::Bundle&
gkg::BundleMap< L, Compare >::getBundle( const L& label ) const
{

  try
  {

    typename std::map< L,
                       typename gkg::BundleMap< L, Compare >::Bundle,
                       Compare >::const_iterator
      b =  _bundles.find( label );

    if ( b == _bundles.end() )
    {

      throw std::runtime_error( "label not present in the bundle map" );

    }
    return b->second;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "const gkg::BundleMap< L, Compare >::Bundle& "
             "gkg::BundleMap< L, Compare >::getBundle( "
             "const L& label ) const" );


}


template < class L, class Compare >
inline
int32_t gkg::BundleMap< L, Compare >::getBundleCount() const
{

  try
  {

    return ( int32_t )_bundles.size();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "int32_t gkg::BundleMap< L, Compare >::getBundleCount() const" );


}


template < class L, class Compare >
inline
void gkg::BundleMap< L, Compare >::setEquidistant(
                                                 int32_t equidistantPointCount )
{

  try
  {

    typename std::map< L,
                       typename gkg::BundleMap< L, Compare >::Bundle,
                       Compare >::iterator
      b =  _bundles.begin(),
      be =  _bundles.end();

    while ( b != be )
    {

      b->second.setEquidistant( equidistantPointCount );
      ++ b;

    }    

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMap< L, Compare >::setEquidistant( "
             "int32_t equidistantPointCount )" );

}


template < class L, class Compare >
inline
gkg::BoundingBox< float > gkg::BundleMap< L, Compare >::getBoundingBox() const
{

  try
  {


    typename std::map< L,
                       typename gkg::BundleMap< L, Compare >::Bundle,
                       Compare >::const_iterator
      b =  _bundles.begin(),
      be =  _bundles.end();

    if ( b == be )
    {

      throw std::runtime_error( "empty bundle map" );

    }

    
    gkg::BoundingBox< float > globalBoundingBox = b->second.getBoundingBox();
    ++ b;

    while ( b != be )
    {

      gkg::BoundingBox< float > boundingBox = b->second.getBoundingBox();

      if ( boundingBox.getUpperX() > globalBoundingBox.getUpperX() )
      {

        globalBoundingBox.setUpperX( boundingBox.getUpperX() );

      }
      else if ( boundingBox.getLowerX() < globalBoundingBox.getLowerX() )
      {

        globalBoundingBox.setLowerX( boundingBox.getLowerX() );

      }

      if ( boundingBox.getUpperY() > globalBoundingBox.getUpperY() )
      {

        globalBoundingBox.setUpperY( boundingBox.getUpperY() );

      }
      else if ( boundingBox.getLowerY() < globalBoundingBox.getLowerY() )
      {

        globalBoundingBox.setLowerY( boundingBox.getLowerY() );

      }

      if ( boundingBox.getUpperZ() > globalBoundingBox.getUpperZ() )
      {

        globalBoundingBox.setUpperZ( boundingBox.getUpperZ() );

      }
      else if ( boundingBox.getLowerZ() < globalBoundingBox.getLowerZ() )
      {

        globalBoundingBox.setLowerZ( boundingBox.getLowerZ() );

      }

      ++ b;

    }    

    return globalBoundingBox;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BoundingBox< float > "
             "gkg::BundleMap< L, Compare >::getBoundingBox() const" );

}


template < class L, class Compare >
inline
void gkg::BundleMap< L, Compare >::getDensityMask(
                     gkg::Volume< float >& densityMask,
                     float resamplingStep,
                     const gkg::Vector3d< double >& outputResolution,
                     bool normalizedDensity,
                     bool verbose ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting bundle count
    ////////////////////////////////////////////////////////////////////////////

    int32_t bundleCount = this->getBundleCount();

    if ( verbose )
    {

      std::cout << "  - bundle count : " << bundleCount << std::endl;
      std::cout << "  - fiber resampling step : " << resamplingStep 
                << "mm"
                << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // computing output volume size
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
    
      std::cout << "  - computing bounding box : " << std::flush;
    
    }
    gkg::BoundingBox< float > boundingBox = this->getBoundingBox();
    if ( verbose )
    {
    
      std::cout << "( " << boundingBox.getLowerX() << ", "
                        << boundingBox.getLowerY() << ", "
                        << boundingBox.getLowerZ() << " ) -> ( "
                        << boundingBox.getUpperX() << ", "
                        << boundingBox.getUpperY() << ", "
                        << boundingBox.getUpperZ() << " )"
                        << std::endl;
    
    }

    
    if ( verbose )
    {
    
      std::cout << "  - computing output volume size : "
                << std::flush;
    
    }

    gkg::Vector3d< int32_t > 
      outputSize( ( int32_t )( boundingBox.getUpperX() /
                               outputResolution.x ),
                  ( int32_t )( boundingBox.getUpperY() /
                               outputResolution.y ),
                  ( int32_t )( boundingBox.getUpperZ() /
                               outputResolution.z ) );
    if ( ( outputSize.x * outputResolution.x ) < boundingBox.getUpperX() )
    {

      outputSize.x ++;

    }
    if ( ( outputSize.y * outputResolution.y ) < boundingBox.getUpperY() )
    {

      outputSize.y ++;

    }
    if ( ( outputSize.z * outputResolution.z ) < boundingBox.getUpperZ() )
    {

      outputSize.z ++;

    }

    if ( verbose )
    {
    
      std::cout << outputSize << " with resolution "
                << outputResolution << " mm3" 
                << std::endl;
    
    }

    ////////////////////////////////////////////////////////////////////////////
    // creating density mask
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
    
      std::cout << "  - creating density mask volume : "
                << std::flush;
    
    }
    densityMask = gkg::Volume< float >( outputSize, bundleCount );
    densityMask.setResolution( outputResolution );
    densityMask.fill( 0.0 );
    if ( verbose )
    {
    
      std::cout << "done " << std::endl;
    
    }

    ////////////////////////////////////////////////////////////////////////////
    // filling density mask
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > voxel;
    typename gkg::BundleMap< L >::const_iterator
      b = begin(),
      be = end();
    int32_t bundleIndex = 0;
    std::vector< float > densitySums( bundleCount, 0.0f );
    while ( b != be )
    {

      //////////////////////////////////////////////////////////////////////////
      // start of current bundle
      //////////////////////////////////////////////////////////////////////////
      const L& label = b->first;
      std::string labelString = gkg::StringConverter::toString( label );
      if ( verbose )
      {

        std::cout << "  - processing bundle '" << label << "' : " << std::flush;

      }
      const typename gkg::BundleMap< L >::Bundle& bundle = b->second;


      //////////////////////////////////////////////////////////////////////////
      // filling density mask
      //////////////////////////////////////////////////////////////////////////
      gkg::Curve3dMap< float >::const_iterator f = bundle.begin(),
                                               fe = bundle.end();

      while ( f != fe )
      {

        const gkg::LightCurve3d< float >& fiber = *f;

        
        float length = fiber.getLength();

        int32_t newPointCount =
         ( int32_t )( length / resamplingStep + 0.5 );

        gkg::LightCurve3d< float > 
          resampledFiber = fiber.getEquidistantCurve( newPointCount );
        gkg::LightCurve3d< float >::const_iterator
          s = resampledFiber.begin(),
          se = resampledFiber.end();
        while ( s != se )
        {

          voxel.x = ( int32_t )( s->x / outputResolution.x + 0.5 );
          voxel.y = ( int32_t )( s->y / outputResolution.y + 0.5 );
          voxel.z = ( int32_t )( s->z / outputResolution.z + 0.5 );

          if ( ( voxel.x < outputSize.x ) &&
               ( voxel.y < outputSize.y ) &&
               ( voxel.z < outputSize.z ) && 
               ( voxel.x > 0 ) &&
               ( voxel.y > 0 ) &&
               ( voxel.z > 0 ) )
          {
            
            ++ densityMask( voxel, bundleIndex );
            if ( normalizedDensity )
            {

              densitySums[ bundleIndex ] += densityMask( voxel, bundleIndex );

            }

          }
          ++ s;

        }

        ++ f;

      }
 
      //////////////////////////////////////////////////////////////////////////
      // end of current bundle
      //////////////////////////////////////////////////////////////////////////
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      ++ bundleIndex;
      ++ b;

    }


    //////////////////////////////////////////////////////////////////////////
    // normalizing density mask on demand
    //////////////////////////////////////////////////////////////////////////

    if ( normalizedDensity )
    {

      int32_t bundleIndex = 0;
      float densitySum = 0.0f;
      for ( bundleIndex = 0; bundleIndex < bundleCount; bundleIndex++ )
      {

        densitySum = densitySums[ bundleIndex ];
        if ( densitySum > 0.0f )
        {

          for ( voxel.z = 0; voxel.z < outputSize.z; voxel.z++ )
          {

            for ( voxel.y = 0; voxel.y < outputSize.y; voxel.y++ )
            {

              for ( voxel.x = 0; voxel.x < outputSize.x; voxel.x++ )
              {

                densityMask( voxel, bundleIndex ) /= densitySum;

              }

            }

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMap< L, Compare >::getDensityMask( "
             "gkg::Volume< float >& densityMask, "
             "float resamplingStep, "
             "const gkg::Vector3d< double >& outputResolution, "
             "bool normalizedDensity, "
             "bool verbose ) const" );

}


template < class L, class Compare >
inline
void gkg::BundleMap< L, Compare >::getDensityAndSectionMasks(
                     gkg::Volume< float >& densityMask,
                     gkg::Volume< int16_t >& sectionMask,
                     std::vector< int32_t >& sectionCounts,
                     const std::vector< gkg::LightCurve3d< float > >& centroids,
                     const std::map< std::string, int32_t >& labelToIndexLut,
                     float sectionWidth,
                     float resamplingStep,
                     const gkg::Vector3d< double >& outputResolution,
                     bool normalizedDensity,
                     bool verbose ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting bundle and centroid count(s)
    ////////////////////////////////////////////////////////////////////////////

    int32_t bundleCount = this->getBundleCount();

    if ( verbose )
    {

      std::cout << "  - bundle count : " << bundleCount << std::endl;
      std::cout << "  - fiber resampling step : " << resamplingStep 
                << "mm"
                << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // computing output volume size
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
    
      std::cout << "  - computing bounding box : " << std::flush;
    
    }
    gkg::BoundingBox< float > boundingBox = this->getBoundingBox();
    if ( verbose )
    {
    
      std::cout << "( " << boundingBox.getLowerX() << ", "
                        << boundingBox.getLowerY() << ", "
                        << boundingBox.getLowerZ() << " ) -> "
                        << boundingBox.getUpperX() << ", "
                        << boundingBox.getUpperY() << ", "
                        << boundingBox.getUpperZ() << " )"
                        << std::endl;
    
    }

    
    if ( verbose )
    {
    
      std::cout << "  - computing output volume size : "
                << std::flush;
    
    }

    gkg::Vector3d< int32_t > 
      outputSize( ( int32_t )( boundingBox.getUpperX() /
                               outputResolution.x ),
                  ( int32_t )( boundingBox.getUpperY() /
                               outputResolution.y ),
                  ( int32_t )( boundingBox.getUpperZ() /
                               outputResolution.z ) );
    if ( ( outputSize.x * outputResolution.x ) < boundingBox.getUpperX() )
    {

      outputSize.x ++;

    }
    if ( ( outputSize.y * outputResolution.y ) < boundingBox.getUpperY() )
    {

      outputSize.y ++;

    }
    if ( ( outputSize.z * outputResolution.z ) < boundingBox.getUpperZ() )
    {

      outputSize.z ++;

    }

    if ( verbose )
    {
    
      std::cout << outputSize << " with resolution "
                << outputResolution << " mm3" 
                << std::endl;
    
    }

    ////////////////////////////////////////////////////////////////////////////
    // creating density mask
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
    
      std::cout << "  - creating density mask volume : "
                << std::flush;
    
    }
    densityMask = gkg::Volume< float >( outputSize, bundleCount );
    densityMask.setResolution( outputResolution );
    densityMask.fill( 0.0 );
    if ( verbose )
    {
    
      std::cout << "done " << std::endl;
    
    }

    ////////////////////////////////////////////////////////////////////////////
    // creating section mask
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
    
      std::cout << "  - creating section mask volume : "
                << std::flush;
    
    }
    sectionMask = gkg::Volume< int16_t >( outputSize, bundleCount );
    sectionMask.setResolution( outputResolution );
    sectionMask.fill( 0 );
    if ( verbose )
    {
    
      std::cout << "done " << std::endl;
    
    }

    ////////////////////////////////////////////////////////////////////////////
    // allocating sectionCounts
    ////////////////////////////////////////////////////////////////////////////

    sectionCounts.resize( bundleCount );

    ////////////////////////////////////////////////////////////////////////////
    // filling density mask and section mask
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > voxel;
    typename gkg::BundleMap< L >::const_iterator
      b = begin(),
      be = end();
    int32_t bundleIndex = 0;
    std::vector< float > densitySums( bundleCount, 0.0f );
    while ( b != be )
    {

      //////////////////////////////////////////////////////////////////////////
      // start of current bundle
      //////////////////////////////////////////////////////////////////////////
      const L& label = b->first;
      std::string labelString = gkg::StringConverter::toString( label );
      if ( verbose )
      {

        std::cout << "  - processing bundle '" << label << "' : " << std::flush;

      }
      const typename gkg::BundleMap< L >::Bundle& bundle = b->second;

      //////////////////////////////////////////////////////////////////////////
      // filling density mask
      //////////////////////////////////////////////////////////////////////////
      gkg::Curve3dMap< float >::const_iterator f = bundle.begin(),
                                               fe = bundle.end();

      while ( f != fe )
      {

        const gkg::LightCurve3d< float >& fiber = *f;

        
        float length = fiber.getLength();

        int32_t newPointCount =
         ( int32_t )( length / resamplingStep + 0.5 );

        gkg::LightCurve3d< float > 
          resampledFiber = fiber.getEquidistantCurve( newPointCount );
        gkg::LightCurve3d< float >::const_iterator
          s = resampledFiber.begin(),
          se = resampledFiber.end();
        while ( s != se )
        {

          voxel.x = ( int32_t )( s->x / outputResolution.x + 0.5 );
          voxel.y = ( int32_t )( s->y / outputResolution.y + 0.5 );
          voxel.z = ( int32_t )( s->z / outputResolution.z + 0.5 );

          if ( ( voxel.x < outputSize.x ) &&
               ( voxel.y < outputSize.y ) &&
               ( voxel.z < outputSize.z ) )
          {

            ++ densityMask( voxel, bundleIndex );
            if ( normalizedDensity )
            {

              densitySums[ bundleIndex ] += 1;

            }

          }
          ++ s;

        }

        ++ f;

      }

      //////////////////////////////////////////////////////////////////////////
      // filling section mask
      //////////////////////////////////////////////////////////////////////////

      // finding index corresponding to current centroid label
      std::map< std::string, int32_t >::const_iterator
        labelIterator = labelToIndexLut.find( labelString );
      int32_t labelIndex = labelIterator->second;

      // getting reference to its single fiber
      const gkg::LightCurve3d< float >&
        centroidFiber = centroids[ labelIndex ];

      // computing the length of the fiber
      float length = centroidFiber.getLength();

      // computing the new number of points to be alloted to the centroid
      int32_t newCentroidPointCount = ( int32_t )( length / resamplingStep );
      if ( ( newCentroidPointCount * resamplingStep ) < length )
      {

        newCentroidPointCount ++;

      }
      // computing the resampled centroid
      gkg::LightCurve3d< float >
        centroid = centroidFiber.getEquidistantCurve( newCentroidPointCount );

      // computing the number of sections
      sectionCounts[ bundleIndex ] = gkg::ceil( length / sectionWidth );

      // precomputing section labels
      std::vector< int16_t > sectionLabels( newCentroidPointCount );

      int32_t l = 0;
      for ( l = 0; l < newCentroidPointCount; l++ )
      {

        sectionLabels[ l ] = ( int16_t )( l * resamplingStep / sectionWidth ) +
                             1;

      } 

      // filling the section mask
      int32_t x = 0;
      int32_t y = 0;
      int32_t z = 0;
      gkg::Vector3d< float > site;
      float distance = 0.0;
      float minimumDistance = 0.0;
      int32_t index = 0;
      int16_t section = 0;
      gkg::LightCurve3d< float >::const_iterator s = centroid.begin(),
                                                 se = centroid.end();
      for ( z = 0; z < outputSize.z; z++ )
      {

        site.z = ( ( float )z + 0.5 ) * ( float )outputResolution.z;
        for ( y = 0; y < outputSize.y; y++ )
        {

          site.y = ( ( float )y + 0.5 ) * ( float )outputResolution.y;
          for ( x = 0; x < outputSize.x; x++ )
          {

            site.x = ( ( float )x + 0.5 ) * ( float )outputResolution.x;

            if ( densityMask( x, y, z, bundleIndex ) > 0.0 )
            {


              s = centroid.begin();
              index = 0;
              minimumDistance = ( site - *s ).getNorm2();
              section = sectionLabels[ index ];
              ++ index;
              ++ s;
              while ( s != se )
              {

                distance = ( site - *s ).getNorm2();
                if ( distance < minimumDistance )
                {

                  minimumDistance = distance;
                  section = sectionLabels[ index ];

                }
                ++ index;
                ++ s;

              }

              sectionMask( x, y, z, bundleIndex ) = section;

            }

          }

        }

      }

      //////////////////////////////////////////////////////////////////////////
      // end of current bundle
      //////////////////////////////////////////////////////////////////////////
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      ++ bundleIndex;
      ++ b;

    }


    //////////////////////////////////////////////////////////////////////////
    // normalizing density mask on demand
    //////////////////////////////////////////////////////////////////////////

    if ( normalizedDensity )
    {

      int32_t bundleIndex = 0;
      float densitySum = 0.0f;
      for ( bundleIndex = 0; bundleIndex < bundleCount; bundleIndex++ )
      {

        densitySum = densitySums[ bundleIndex ];
        if ( densitySum > 0.0f )
        {

          for ( voxel.z = 0; voxel.z < outputSize.z; voxel.z++ )
          {

            for ( voxel.y = 0; voxel.y < outputSize.y; voxel.y++ )
            {

              for ( voxel.x = 0; voxel.x < outputSize.x; voxel.x++ )
              {

                densityMask( voxel, bundleIndex ) /= densitySum;

              }

            }

          }

        }

      }

    }


  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMap< L, Compare >::getDensityAndSectionMasks( "
             "gkg::Volume< float >& densityMask, "
             "gkg::Volume< int16_t >& sectionMask, "
             "std::vector< int32_t >& sectionCounts, "
             "const std::vector< gkg::LightCurve3d< float > >& centroids, "
             "const std::map< std::string, int32_t >& labelToIndexLut, "
             "float sectionWidth, "
             "float resamplingStep, "
             "const gkg::Vector3d< double >& outputResolution, "
             "bool normalizedDensity, "
             "bool verbose ) const" );

}


template < class L, class Compare >
inline
int32_t gkg::BundleMap< L, Compare >::getTotalSegmentCount() const
{

  try
  {

    int32_t totalSegmentCount = 0;
    typename gkg::BundleMap< L >::const_iterator
      b = begin(),
      be = end();
    while ( b != be )
    {

      const typename gkg::BundleMap< L >::Bundle& bundle = b->second;
      gkg::Curve3dMap< float >::const_iterator f = bundle.begin(),
                                               fe = bundle.end();

      while ( f != fe )
      {

        const gkg::LightCurve3d< float >& fiber = *f;

        totalSegmentCount += fiber.getPointCount() - 1;

        ++ f;

      }

      ++ b;

    }

    return totalSegmentCount;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "int32_t "
             "gkg::BundleMap< L, Compare >::getTotalSegmentCount() const" );

}


template < class L, class Compare >
inline
int32_t gkg::BundleMap< L, Compare >::getTotalPointCount() const
{

  try
  {

    int32_t totalPointCount = 0;
    typename gkg::BundleMap< L >::const_iterator
      b = begin(),
      be = end();
    while ( b != be )
    {

      const typename gkg::BundleMap< L >::Bundle& bundle = b->second;
      gkg::Curve3dMap< float >::const_iterator f = bundle.begin(),
                                               fe = bundle.end();

      while ( f != fe )
      {

        const gkg::LightCurve3d< float >& fiber = *f;

        totalPointCount += fiber.getPointCount();

        ++ f;

      }

      ++ b;

    }

    return totalPointCount;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "int32_t "
             "gkg::BundleMap< L, Compare >::getTotalPointCount() const" );

}


template < class L, class Compare >
inline
void gkg::BundleMap< L, Compare >::updateHeader()
{

  try
  {

    int32_t labelCount = ( int32_t )_labels.size();

    std::vector< L > labels( labelCount );
    std::vector< int32_t > curve3dCounts( labelCount );

    int32_t count = 0;
    typename std::map< L,
                       typename gkg::BundleMap< L, Compare >::Bundle,
                       Compare >::const_iterator
      b =  _bundles.begin(),
      be =  _bundles.end();

    while ( b != be )
    {

      labels[ count ] = b->first;
      curve3dCounts[ count ] = b->second.getCurve3dCount();

      ++ b;
      ++ count;

    }    

    _header[ "item_count" ] = labelCount;

    if ( _header.hasAttribute( "labels" ) )
    {

      _header[ "labels" ] = labels;

    }
    else
    {

      _header.addAttribute( "labels", labels );

    }

    if ( _header.hasAttribute( "curve3d_counts" ) )
    {

      _header[ "curve3d_counts" ] = curve3dCounts;

    }
    else
    {

      _header.addAttribute( "curve3d_counts", curve3dCounts );

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMap< L, Compare >::updateHeader() const" );

}


#endif
