#include <gkg-processing-container/Curve3dMap.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>

#ifdef GKG_DEBUG
#include <cstdio>
#endif

//
// class Curve3dMap
//

template < class T >
gkg::Curve3dMap< T >::Curve3dMap()
                     : gkg::HeaderedObject()
{

  // adding a generic syntax set
  _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "item_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "curve3d_count" ] = 
    gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );

  // adding attributes to header
  _header.addAttribute( "object_type", std::string( "Curve3dMap" ) );
  _header.addAttribute( "item_type", gkg::TypeOf< T >::getName() );
  _header.addAttribute( "curve3d_count", 0 );

}


template < class T >
gkg::Curve3dMap< T >::Curve3dMap( int32_t curve3dCount )
                     : gkg::HeaderedObject(),
                       _curve3ds( curve3dCount )
{

  // adding a generic syntax set
  _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "item_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "curve3d_count" ] = 
    gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );

  // adding attributes to header
  _header.addAttribute( "object_type", std::string( "Curve3dMap" ) );
  _header.addAttribute( "item_type", gkg::TypeOf< T >::getName() );
  _header.addAttribute( "curve3d_count", curve3dCount );

}


template < class T >
gkg::Curve3dMap< T >::Curve3dMap(
                      const std::list< gkg::LightCurve3d< T > >& lightCurve3ds )
                     : gkg::HeaderedObject()
{

  // adding a generic syntax set
  _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "item_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "curve3d_count" ] = 
    gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );

  // adding attributes to header
  _header.addAttribute( "object_type", std::string( "Curve3dMap" ) );
  _header.addAttribute( "item_type", gkg::TypeOf< T >::getName() );
  _header.addAttribute( "curve3d_count", ( int32_t )lightCurve3ds.size() );

  reserve( ( int32_t )lightCurve3ds.size() );
  addCurve3ds( lightCurve3ds );

}


template < class T >
gkg::Curve3dMap< T >::Curve3dMap(
                    const std::vector< gkg::LightCurve3d< T > >& lightCurve3ds )
                     : gkg::HeaderedObject()
{

  // adding a generic syntax set
  _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "item_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "curve3d_count" ] = 
    gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );

  // adding attributes to header
  _header.addAttribute( "object_type", std::string( "Curve3dMap" ) );
  _header.addAttribute( "item_type", gkg::TypeOf< T >::getName() );
  _header.addAttribute( "curve3d_count", ( int32_t )lightCurve3ds.size() );

  reserve( ( int32_t )lightCurve3ds.size() );
  addCurve3ds( lightCurve3ds );

}


template < class T >
gkg::Curve3dMap< T >::Curve3dMap(
                                const std::list< gkg::Curve3d< T > >& curve3ds )
                     : gkg::HeaderedObject()
{

  // adding a generic syntax set
  _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "item_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "curve3d_count" ] = 
    gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );

  // adding attributes to header
  _header.addAttribute( "object_type", std::string( "Curve3dMap" ) );
  _header.addAttribute( "item_type", gkg::TypeOf< T >::getName() );
  _header.addAttribute( "curve3d_count", ( int32_t )curve3ds.size() );

  reserve( ( int32_t )curve3ds.size() );
  addCurve3ds( curve3ds );

}


template < class T >
gkg::Curve3dMap< T >::Curve3dMap(
                              const std::vector< gkg::Curve3d< T > >& curve3ds )
                     : gkg::HeaderedObject()
{

  // adding a generic syntax set
  _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "item_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "curve3d_count" ] = 
    gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );

  // adding attributes to header
  _header.addAttribute( "object_type", std::string( "Curve3dMap" ) );
  _header.addAttribute( "item_type", gkg::TypeOf< T >::getName() );
  _header.addAttribute( "curve3d_count", ( int32_t )curve3ds.size() );

  reserve( ( int32_t )curve3ds.size() );
  addCurve3ds( curve3ds );

}


template < class T >
gkg::Curve3dMap< T >::Curve3dMap( const gkg::Curve3dMap< T >& other )
                     : gkg::HeaderedObject( other ),
                       _curve3ds( other._curve3ds )
{
}


template < class T >
gkg::Curve3dMap< T >::~Curve3dMap()
{
}


template < class T >
gkg::Curve3dMap< T >&
gkg::Curve3dMap< T >::operator=( const gkg::Curve3dMap< T >& other )
{

  try
  {

    this->gkg::HeaderedObject::operator=( other );
    _curve3ds = other._curve3ds;

    return *this;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::Curve3dMap< T >& "
             "gkg::Curve3dMap< T >::operator=( "
             "const gkg::Curve3dMap< T >& other )" );

}

template < class T >
void gkg::Curve3dMap< T >::reserve( int32_t curve3dCount )
{

  try
  {

    _curve3ds.reserve( curve3dCount );

  }
  GKG_CATCH( "template < class T >"
             "void gkg::Curve3dMap< T >::reserve( int32_t curve3dCount )" );

}


template < class T >
typename gkg::Curve3dMap< T >::iterator
gkg::Curve3dMap< T >::begin()
{

  return _curve3ds.begin();

}


template < class T >
typename gkg::Curve3dMap< T >::iterator
gkg::Curve3dMap< T >::end()
{

  return _curve3ds.end();

}


template < class T >
typename gkg::Curve3dMap< T >::const_iterator
gkg::Curve3dMap< T >::begin() const
{

  return _curve3ds.begin();

}


template < class T >
typename gkg::Curve3dMap< T >::const_iterator
gkg::Curve3dMap< T >::end() const
{

  return _curve3ds.end();

}


template < class T >
typename gkg::Curve3dMap< T >::reverse_iterator
gkg::Curve3dMap< T >::rbegin()
{

  return _curve3ds.rbegin();

}


template < class T >
typename gkg::Curve3dMap< T >::reverse_iterator
gkg::Curve3dMap< T >::rend()
{

  return _curve3ds.rend();

}


template < class T >
typename gkg::Curve3dMap< T >::const_reverse_iterator
gkg::Curve3dMap< T >::rbegin() const
{

  return _curve3ds.rbegin();

}


template < class T >
typename gkg::Curve3dMap< T >::const_reverse_iterator 
gkg::Curve3dMap< T >::rend() const
{

  return _curve3ds.rend();

}


template < class T >
void gkg::Curve3dMap< T >::addCurve3d(
                                    const gkg::LightCurve3d< T >& lightCurve3d )
{

  try
  {

    _curve3ds.push_back( lightCurve3d );
    updateHeader();

  }
  GKG_CATCH( "template < class T >"
             "void gkg::Curve3dMap< T >::addCurve3d( "
             "const gkg::LightCurve3d< T >& lightCurve3d )" );

}


template < class T >
void gkg::Curve3dMap< T >::setCurve3d(
                                    int32_t index,
                                    const gkg::LightCurve3d< T >& lightCurve3d )
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( index < 0 ) || ( index >= this->getCurve3dCount() ) )
    {

      char message[ 256 ];
      sprintf( message, "invalid index ( %d )", index );
      throw std::runtime_error( message );

    }

#endif

    _curve3ds[ index ] = lightCurve3d;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Curve3dMap< T >::setCurve3d( "
             "int32_t index, const Curve3d< T >& curve3d )" );

}


template < class T >
void gkg::Curve3dMap< T >::addCurve3ds(
                      const std::list< gkg::LightCurve3d< T > >& lightCurve3ds )
{

  try
  {

    typename std::list< gkg::LightCurve3d< T > >::const_iterator
      c = lightCurve3ds.begin(),
      ce = lightCurve3ds.end();
    while ( c != ce )
    {

      _curve3ds.push_back( *c );
      ++ c;

    }
    updateHeader();

  }
  GKG_CATCH( "template < class T >"
             "void gkg::Curve3dMap< T >::addCurve3ds( "
             "const std::list< gkg::LightCurve3d< T > >& lightCurve3ds )" );

}


template < class T >
void gkg::Curve3dMap< T >::addCurve3ds(
                    const std::vector< gkg::LightCurve3d< T > >& lightCurve3ds )
{

  try
  {

    typename std::vector< gkg::LightCurve3d< T > >::const_iterator
      c = lightCurve3ds.begin(),
      ce = lightCurve3ds.end();
    while ( c != ce )
    {

      _curve3ds.push_back( *c );
      ++ c;

    }
    updateHeader();

  }
  GKG_CATCH( "template < class T >"
             "void gkg::Curve3dMap< T >::addCurve3ds( "
             "const std::vector< gkg::LightCurve3d< T > >& lightCurve3ds )" );

}


template < class T >
void gkg::Curve3dMap< T >::addCurve3ds(
                                const std::list< gkg::Curve3d< T > >& curve3ds )
{

  try
  {

    typename std::list< gkg::Curve3d< T > >::const_iterator
      c = curve3ds.begin(),
      ce = curve3ds.end();
    while ( c != ce )
    {

      _curve3ds.push_back( *c );
      ++ c;

    }
    updateHeader();

  }
  GKG_CATCH( "template < class T >"
             "void gkg::Curve3dMap< T >::addCurve3ds( "
             "const std::list< gkg::Curve3d< T > >& curve3ds )" );

}


template < class T >
void gkg::Curve3dMap< T >::addCurve3ds(
                              const std::vector< gkg::Curve3d< T > >& curve3ds )
{

  try
  {

    typename std::vector< gkg::Curve3d< T > >::const_iterator
      c = curve3ds.begin(),
      ce = curve3ds.end();
    while ( c != ce )
    {

      _curve3ds.push_back( *c );
      ++ c;

    }
    updateHeader();

  }
  GKG_CATCH( "template < class T >"
             "void gkg::Curve3dMap< T >::addCurve3ds( "
             "const std::vector< gkg::Curve3d< T > >& curve3ds )" );

}


template < class T >
void gkg::Curve3dMap< T >::removeCurve3d( const int32_t& index )
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( index < 0 ) || ( index >= this->getCurve3dCount() ) )
    {

      char message[ 256 ];
      sprintf( message, "invalid index ( %d )", index );
      throw std::runtime_error( message );

    }

#endif

    typename std::vector< gkg::LightCurve3d< T > >::iterator
      c = _curve3ds.begin();
    c += index;

    // erasing curve at given index
    _curve3ds.erase( c );

    // updating header
    updateHeader();

  }
  GKG_CATCH( "template < class T >"
             "void gkg::Curve3dMap< T >::removeCurve3d( "
             "const int32_t& index )" );

}


template < class T >
void gkg::Curve3dMap< T >::removeCurve3ds( const std::set< int32_t >& indices )
{

  try
  {

#ifdef GKG_DEBUG

    std::set< int32_t >::const_iterator index = indices.begin(),
                                        indexe = indices.end();
    while ( index != indexe )
    {

      if ( ( *index < 0 ) || ( *index >= this->getCurve3dCount() ) )
      {

        char message[ 256 ];
        sprintf( message, "invalid index ( %d )", *index );
        throw std::runtime_error( message );

      }
      ++ index;

    }

#endif

    std::vector< gkg::LightCurve3d< T > >
      newCurve3ds( _curve3ds.size() - indices.size() );

    int32_t i, size = ( int32_t )_curve3ds.size();
    int32_t count = 0;

    std::set< int32_t >::const_iterator ie = indices.end();

    // building new point vector
    for ( i = 0; i < size; i++ )
    {

      if ( indices.find( i ) != ie )
      {

        newCurve3ds[ count ] = _curve3ds[ i ];
        ++ count;

      }

    }

    // copying back new Curve3d vector
    _curve3ds = newCurve3ds;

    // updating header
    updateHeader();

  }
  GKG_CATCH( "template < class T >"
             "void gkg::Curve3dMap< T >::removeCurve3ds( "
             "const std::set< int32_t >& indices )" );

}


template < class T >
void gkg::Curve3dMap< T >::clear()
{

  try
  {

    // erasing curve(s)
    _curve3ds.clear();

    // updating header
    updateHeader();

  }
  GKG_CATCH( "template < class T >"
             "void gkg::Curve3dMap< T >::clear()" );

}


template < class T >
gkg::LightCurve3d< T >&
gkg::Curve3dMap< T >::operator[]( int32_t i )
{

  try
  {

    return _curve3ds[ 
               ( typename std::vector< gkg::LightCurve3d< T > >::size_type )i ];

  }
  GKG_CATCH( "template < class T > "
             "gkg::LightCurve3d< T >& "
             "gkg::Curve3dMap< T >::operator[]( "
             "int32_t i )" );

}


template < class T >
const gkg::LightCurve3d< T >&
gkg::Curve3dMap< T >::operator[]( int32_t i ) const
{

  try
  {

    return _curve3ds[ 
               ( typename std::vector< gkg::LightCurve3d< T > >::size_type )i ];

  }
  GKG_CATCH( "template < class T > "
             "const gkg::LightCurve3d< T >& "
             "gkg::Curve3dMap< T >::operator[]( "
             "int32_t i )" );

}


template < class T >
const gkg::LightCurve3d< T >& 
gkg::Curve3dMap< T >::getCurve3d( int32_t index ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( index < 0 ) || ( index >= this->getCurve3dCount() ) )
    {

      char message[ 256 ];
      sprintf( message, "invalid index ( %d )", index );
      throw std::runtime_error( message );

    }

#endif

    return _curve3ds[ index ];

  }
  GKG_CATCH( "template < class T > "
             "const gkg::LightCurve3d< T >& "
             "gkg::Curve3dMap< T >::getCurve3d( int32_t index ) const" );

}


template < class T >
const std::vector< gkg::LightCurve3d< T > >& 
gkg::Curve3dMap< T >::getCurve3ds() const
{

  return _curve3ds;

}


template < class T >
int32_t gkg::Curve3dMap< T >::getCurve3dCount() const
{

  return ( int32_t )_curve3ds.size();

}


template < class T >
void gkg::Curve3dMap< T >::setEquidistant( int32_t equidistantPointCount )
{

  try
  {

    typename std::vector< gkg::LightCurve3d< T > >::iterator
      c = _curve3ds.begin(),
      ce = _curve3ds.end();
    while ( c != ce )
    {

      *c = c->getEquidistantCurve( equidistantPointCount );
      ++ c;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Curve3dMap< T >::setEquidistant( "
             "int32_t equidistantPointCount )" );

}


template < class T >
T gkg::Curve3dMap< T >::getInnerDistance() const
{

  try
  {

    T distance2 = 0.0;

    typename gkg::Curve3dMap< T >::const_iterator
      c1 = begin(),
      c1e = end();
    while ( c1 != c1e )
    {

      typename gkg::Curve3dMap< T >::const_iterator
        c2 = c1,
        c2e = end();
      ++ c2;

      while ( c2 != c2e )
      {

        distance2 += c1->getDistance2( *c2 );
        ++ c2;

      }
      ++ c1;

    }
    return std::sqrt( distance2 );

  }
  GKG_CATCH( "template < class T > "
             "T gkg::Curve3dMap< T >::getInnerDistance() const" );

}


template < class T >
bool gkg::Curve3dMap< T >::allCurvesHaveSamePointCount() const
{

  try
  {

    int32_t curve3dCount = getCurve3dCount();
    int32_t pointCount = getCurve3d( 0 ).getPointCount();
    int32_t c;
    for ( c = 1; c < curve3dCount; c++ )
    {

      if ( getCurve3d( c ).getPointCount() != pointCount )
      {

        return false;

      }

    }
    return true;

  }
  GKG_CATCH( "template < class T > "
             "bool gkg::Curve3dMap< T >::allCurvesHaveSamePointCount() const" );

}


template < class T >
gkg::LightCurve3d< T > 
gkg::Curve3dMap< T >::getAverage( int32_t excludedIndex ) const
{

  try
  {

    if ( !allCurvesHaveSamePointCount() )
    {

      throw std::runtime_error(
                               "all curves need to have the same point count" );

    }

    int32_t curve3dCount = getCurve3dCount();
    int32_t pointCount = getCurve3d( 0 ).getPointCount();

    // allocating points of the curve
    std::vector< gkg::Vector3d< T > > 
      points( pointCount, gkg::Vector3d< T >( 0.0, 0.0, 0.0 ) );

    // processing average points
    int32_t c, p;
    for ( c = 0; c < curve3dCount; c++ )
    {

      if ( c != excludedIndex )
      {

        const gkg::LightCurve3d< T >& curve3d = getCurve3d( c );
        for ( p = 0; p < pointCount; p++ )
        {

          points[ p ] += curve3d.getPoint( p );

        }

      }

    }

    T divisor = ( T )curve3dCount + ( excludedIndex >= 0 ? -1.0 : 0.0 );

    for ( p = 0; p < pointCount; p++ )
    {

      points[ p ] /= divisor;

    }

    // building and returning3D curve
    return gkg::LightCurve3d< T >( points );

  }
  GKG_CATCH( "template < class T > "
             "gkg::LightCurve3d< T > "
             "gkg::Curve3dMap< T >::getAverage( "
             "int32_t excludedIndex ) const" );

}


template < class T >
T gkg::Curve3dMap< T >::getCurve3dDistanceToCurve3dMap(
                              const gkg::LightCurve3d< T >& lightCurve3d ) const
{

  try
  {

    T distance2 = 0.0;

    typename gkg::Curve3dMap< T >::const_iterator
      c = begin(),
      ce = end();
    while ( c != ce )
    {

      distance2 += lightCurve3d.getDistance2( *c );
      ++ c;

    }
    return std::sqrt( distance2 );

  }
  GKG_CATCH( "template < class T > "
             "T gkg::Curve3dMap< T >::getCurve3dDistanceToCurve3dMap( "
             "const gkg::LightCurve3d< T >& lightCurve3d ) const " );

}


template < class T >
T gkg::Curve3dMap< T >::getCurve3dDistanceToCurve3dMap(
                                                   int32_t excludedIndex ) const
{

  try
  {

    if ( excludedIndex >= ( int32_t )_curve3ds.size() )
    {

      throw std::runtime_error( "not a valid excluded index" );

    }

    T distance2 = 0.0;
    const gkg::LightCurve3d< T >& excludedCurve3d = _curve3ds[ excludedIndex ];

    int32_t index = 0;
    typename gkg::Curve3dMap< T >::const_iterator
      c = begin(),
      ce = end();
    while ( c != ce )
    {
      
      if ( index != excludedIndex )
      {

        distance2 += excludedCurve3d.getDistance2( *c );

      }
      ++ c;
      ++ index;

    }
    return std::sqrt( distance2 );

  }
  GKG_CATCH( "template < class T > "
             "T gkg::Curve3dMap< T >::getCurve3dDistanceToCurve3dMap( "
             "int32_t excludedIndex ) const " );
}


template < class T >
T gkg::Curve3dMap< T >::getCurve3dMaximumDistanceToCurve3dMap(
                              const gkg::LightCurve3d< T >& lightCurve3d ) const
{

  try
  {

    T distance;
    T maximumDistance = 0.0;

    typename gkg::Curve3dMap< T >::const_iterator
      c = begin(),
      ce = end();
    while ( c != ce )
    {

      distance = std::sqrt( lightCurve3d.getMaximumDistance2( *c ) );
      if ( distance > maximumDistance )
      {

        maximumDistance = distance;

      }
      ++ c;

    }
    return maximumDistance;

  }
  GKG_CATCH( "template < class T > "
             "T gkg::Curve3dMap< T >::getCurve3dMaximumDistanceToCurve3dMap( "
             "const gkg::LightCurve3d< T >& lightCurve3d ) const " );

}


template < class T >
T gkg::Curve3dMap< T >::getCurve3dMaximumDistanceToCurve3dMap(
                                                   int32_t excludedIndex ) const
{

  try
  {

    if ( excludedIndex >= ( int32_t )_curve3ds.size() )
    {

      throw std::runtime_error( "not a valid excluded index" );

    }

    T distance = 0.0;
    T maximumDistance = 0.0;
    const gkg::LightCurve3d< T >& excludedCurve3d = _curve3ds[ excludedIndex ];

    int32_t index = 0;
    typename gkg::Curve3dMap< T >::const_iterator
      c = begin(),
      ce = end();
    while ( c != ce )
    {

      if ( index != excludedIndex )
      {

        distance = std::sqrt( excludedCurve3d.getMaximumDistance2( *c ) );
        if ( distance > maximumDistance )
        {

          maximumDistance = distance;

        }

      }
      ++ c;
      ++ index;

    }
    return maximumDistance;

  }
   GKG_CATCH( "template < class T > "
              "T gkg::Curve3dMap< T >::getCurve3dMaximumDistanceToCurve3dMap( "
              "int32_t excludedIndex ) const " );
}


template < class T >
int32_t  gkg::Curve3dMap< T >::getClosestCurve3dIndexByMinMaxDistance(
                              const gkg::LightCurve3d< T >& lightCurve3d ) const
{

  try
  {
    
    int32_t curve3dIndex = 0;
    int32_t minimumCurve3dIndex = 0;

    typename gkg::Curve3dMap< T >::const_iterator
      c = begin(),
      ce = end();   
    T distance;
    T minimumDistance = std::sqrt( lightCurve3d.getMaximumDistance2( *c ) );
    minimumCurve3dIndex = curve3dIndex;

    ++ c;
    ++ curve3dIndex; 
    while ( c != ce )
    {
       
      distance = std::sqrt( lightCurve3d.getMaximumDistance2( *c ) );
      if ( distance < minimumDistance )
      {

        minimumDistance = distance;
        minimumCurve3dIndex = curve3dIndex;

      }
      ++ c;
      ++ curve3dIndex;

    }
    return minimumCurve3dIndex;

  }
  GKG_CATCH( "template < class T > "
             "int32_t  gkg::Curve3dMap< T >::"
             "getClosestCurve3dIndexByMinMaxDistance( "
             "const gkg::LightCurve3d< T >& lightCurve3d ) const" );

}


template < class T >
int32_t  gkg::Curve3dMap< T >::getClosestCurve3dIndexByMinMaxDistance(
                                                   int32_t excludedIndex ) const
{

  try
  {
    
    if ( excludedIndex >= ( int32_t )_curve3ds.size() )
    {

      throw std::runtime_error( "not a valid excluded index" );

    }
    const gkg::LightCurve3d< T >& excludedCurve3d = _curve3ds[ excludedIndex ];
    int32_t curve3dIndex = 0;
    int32_t minimumCurve3dIndex = 0;

    typename gkg::Curve3dMap< T >::const_iterator
      c = begin(),
      ce = end();
    T distance = 0;
    T minimumDistance = 0;

    if ( curve3dIndex != excludedIndex )
    {

      minimumDistance = std::sqrt( excludedCurve3d.getMaximumDistance2( *c ) );
      minimumCurve3dIndex = curve3dIndex;
	 
      ++ c;
      ++ curve3dIndex;
      while ( c != ce )
      {

        if ( curve3dIndex != excludedIndex )
        {

          distance = std::sqrt( excludedCurve3d.getMaximumDistance2( *c ) );
          if ( distance < minimumDistance )
          {

            minimumDistance = distance;
            minimumCurve3dIndex = curve3dIndex;

          }
   
        }
	++ c;
	++ curve3dIndex;

      }

    }
    else
    {

      ++ c;
      ++ curve3dIndex;

      minimumDistance = std::sqrt( excludedCurve3d.getMaximumDistance2( *c ) );
      minimumCurve3dIndex = curve3dIndex;
	 
      ++ c;
      ++ curve3dIndex;
      while ( c != ce )
      {
       
        distance = std::sqrt( excludedCurve3d.getMaximumDistance2( *c ) );
        if ( distance < minimumDistance )
        {

          minimumDistance = distance;
          minimumCurve3dIndex = curve3dIndex;

        }
	++ c;
	++ curve3dIndex;
	       
      }

    }
    return minimumCurve3dIndex;

  }
  GKG_CATCH( "template < class T > "
             "int32_t  gkg::Curve3dMap< T >::"
             "getClosestCurve3dIndexByMinMaxDistance( "
             "int32_t excludedIndex ) const" );

}


template < class T >
gkg::BoundingBox< T > gkg::Curve3dMap< T >::getBoundingBox() const
{

  try
  {


    typename gkg::Curve3dMap< T >::const_iterator
      c = begin(),
      ce = end();

    if ( c == ce )
    {

      throw std::runtime_error( "empty 3D curve map" );

    }

    typename gkg::LightCurve3d< T >::const_iterator
      p = c->begin(),
      pe = c->end();

    if ( p == pe )
    {

      throw std::runtime_error( "empty 3D light curve" );

    }

    gkg::BoundingBox< T > boundingBox( p->x, p->y, p->z,
                                       p->x, p->y, p->z );


    c = begin();
    while ( c != ce )
    {

      typename gkg::LightCurve3d< T >::const_iterator
      p = c->begin(),
      pe = c->end();

      while ( p != pe )
      {

        if ( p->x > boundingBox.getUpperX() )
        {

          boundingBox.setUpperX( p->x );

        }
        else if ( p->x < boundingBox.getLowerX() )
        {

          boundingBox.setLowerX( p->x );

        }

        if ( p->y > boundingBox.getUpperY() )
        {

          boundingBox.setUpperY( p->y );

        }
        else if ( p->y < boundingBox.getLowerY() )
        {

          boundingBox.setLowerY( p->y );

        }

        if ( p->z > boundingBox.getUpperZ() )
        {

          boundingBox.setUpperZ( p->z );

        }
        else if ( p->z < boundingBox.getLowerZ() )
        {

          boundingBox.setLowerZ( p->z );

        }

        ++ p;

      }

      ++ c;

    }

    return boundingBox;

  }
  GKG_CATCH( "template < class T > "
             "gkg::BoundingBox< T > "
             "gkg::Curve3dMap< T >::getBoundingBox() const" );

}


template < class T >
void gkg::Curve3dMap< T >::updateHeader()
{

  try
  {

    _header[ "curve3d_count" ] = this->getCurve3dCount();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Curve3dMap< T >::updateHeader()" );

}


namespace std
{


ostream& operator<<( ostream& os,
                     const gkg::Curve3dMap< float >& thing )
{

  gkg::Curve3dMap< float >::const_iterator
    c = thing.begin(),
    ce = thing.end();

  while ( c != ce )
  {

    os << *c << std::endl;
    ++ c;

  }
  return os;

}


ostream& operator<<( ostream& os,
                     const gkg::Curve3dMap< double >& thing )
{

  gkg::Curve3dMap< double >::const_iterator
    c = thing.begin(),
    ce = thing.end();

  while ( c != ce )
  {

    os << *c << std::endl;
    ++ c;

  }
  return os;

}


}



// 
// forcing instanciations
//

template class gkg::Curve3dMap< float >;
template class gkg::Curve3dMap< double >;
