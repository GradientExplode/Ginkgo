#include <gkg-processing-container/Curve3d.h>
#include <gkg-processing-numericalanalysis/CubicSpline3d.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-object/HeaderedObject.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>


#define INFINITE_TORTUOSITY   1e37
#define ALMOST_NULL_DISTANCE  1e-10


template < class T >
gkg::Curve3d< T >::Curve3d()
                  : gkg::HeaderedObject(),
                    gkg::LightCurve3d< T >()
{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
    _syntaxSet[ "__generic__" ][ "item_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
    _syntaxSet[ "__generic__" ][ "point_count" ] = 
      gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );
    _syntaxSet[ "__generic__" ][ "closed_curve" ] = 
      gkg::Semantic( gkg::TypeOf< bool >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", std::string( "Curve3d" ) );
    _header.addAttribute( "item_type", gkg::TypeOf< T >::getName() );
    _header.addAttribute( "point_count", 0 );
    _header.addAttribute( "closed_curve", this->_closed );

  }
  GKG_CATCH( "template < class T > "
             "gkg::Curve3d< T >::Curve3d()" );

}


template < class T >
gkg::Curve3d< T >::Curve3d( int32_t pointCount )
                  : gkg::HeaderedObject(),
                    gkg::LightCurve3d< T >( pointCount )
{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] =
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
    _syntaxSet[ "__generic__" ][ "item_type" ] =
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
    _syntaxSet[ "__generic__" ][ "point_count" ] =
      gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );
    _syntaxSet[ "__generic__" ][ "closed_curve" ] =
      gkg::Semantic( gkg::TypeOf< bool >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", std::string( "Curve3d" ) );
    _header.addAttribute( "item_type", gkg::TypeOf< T >::getName() );
    _header.addAttribute( "point_count", pointCount );
    _header.addAttribute( "closed_curve", this->_closed );

  }
  GKG_CATCH( "template < class T > "
             "gkg::Curve3d< T >::Curve3d( int32_t pointCount )" );

}


template < class T >
gkg::Curve3d< T >::Curve3d( const std::list< gkg::Vector3d< T > >& points,
                            bool closed )
                  : gkg::HeaderedObject(),
                    gkg::LightCurve3d< T >( points, closed )
{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] =
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
    _syntaxSet[ "__generic__" ][ "item_type" ] =
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
    _syntaxSet[ "__generic__" ][ "point_count" ] =
      gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );
    _syntaxSet[ "__generic__" ][ "closed_curve" ] =
      gkg::Semantic( gkg::TypeOf< bool >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", std::string( "Curve3d" ) );
    _header.addAttribute( "item_type", gkg::TypeOf< T >::getName() );
    _header.addAttribute( "point_count", ( int32_t )points.size() );
    _header.addAttribute( "closed_curve", closed );

  }
  GKG_CATCH( "template < class T > "
             "gkg::Curve3d< T >::Curve3d( "
             "const std::list< gkg::Vector3d< T > >& points, "
             "bool closed )" );

}


template < class T >
gkg::Curve3d< T >::Curve3d( const std::vector< gkg::Vector3d< T > >& points,
                            bool closed )
                  : gkg::HeaderedObject(),
                    gkg::LightCurve3d< T >( points, closed )
{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] =
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
    _syntaxSet[ "__generic__" ][ "item_type" ] =
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
    _syntaxSet[ "__generic__" ][ "point_count" ] =
      gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );
    _syntaxSet[ "__generic__" ][ "closed_curve" ] =
      gkg::Semantic( gkg::TypeOf< bool >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", std::string( "Curve3d" ) );
    _header.addAttribute( "item_type", gkg::TypeOf< T >::getName() );
    _header.addAttribute( "point_count", ( int32_t )points.size() );
    _header.addAttribute( "closed_curve", this->_closed );

  }
  GKG_CATCH( "template < class T > "
             "gkg::Curve3d< T >::Curve3d( "
             "const std::vector< gkg::Vector3d< T > >& points, "
             "bool closed )" );

}


template < class T >
gkg::Curve3d< T >::Curve3d( const gkg::Matrix& matrix,
                            bool closed )
                  : gkg::HeaderedObject(),
                    gkg::LightCurve3d< T >( matrix, closed )
{

  try
  {

    // sanity checks: only regular and homogeneous matrices are accepted
    if ( ( matrix.getSize2() != 3 ) && ( matrix.getSize2() != 4 ) )
    {

      throw std::runtime_error( "not a valid matrix columns count" );

    }

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] =
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
    _syntaxSet[ "__generic__" ][ "item_type" ] =
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
    _syntaxSet[ "__generic__" ][ "point_count" ] =
      gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );
    _syntaxSet[ "__generic__" ][ "closed_curve" ] =
      gkg::Semantic( gkg::TypeOf< bool >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", std::string( "Curve3d" ) );
    _header.addAttribute( "item_type", gkg::TypeOf< T >::getName() );
    _header.addAttribute( "point_count", matrix.getSize1() );
    _header.addAttribute( "closed_curve", this->_closed );

  }
  GKG_CATCH( "template < class T > "
             "gkg::Curve3d< T >::Curve3d( "
             "const gkg::Matrix& matrix, "
             "bool closed )" );

}


template < class T >
gkg::Curve3d< T >::Curve3d( const gkg::Curve3d< T >& other )
                  : gkg::HeaderedObject( other ),
                    gkg::LightCurve3d< T >( other )
{
}


template < class T >
gkg::Curve3d< T >::Curve3d( const gkg::LightCurve3d< T >& other )
                  : gkg::HeaderedObject(),
                    gkg::LightCurve3d< T >( other )

{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] =
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
    _syntaxSet[ "__generic__" ][ "item_type" ] =
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
    _syntaxSet[ "__generic__" ][ "point_count" ] =
      gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );
    _syntaxSet[ "__generic__" ][ "closed_curve" ] =
      gkg::Semantic( gkg::TypeOf< bool >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type", std::string( "Curve3d" ) );
    _header.addAttribute( "item_type", gkg::TypeOf< T >::getName() );
    _header.addAttribute( "point_count", other.getPointCount() );
    _header.addAttribute( "closed_curve", other.isClosed() );

  }
  GKG_CATCH( "" );

}


template < class T >
gkg::Curve3d< T >::~Curve3d()
{
}


template < class T >
gkg::Curve3d< T >& 
gkg::Curve3d< T >::operator=( const gkg::Curve3d< T >& other )
{

  this->gkg::HeaderedObject::operator=( other );
  this->gkg::LightCurve3d< T >::operator=( other );

  return *this;

}


template < class T >
gkg::Curve3d< T >& 
gkg::Curve3d< T >::operator=( const gkg::LightCurve3d< T >& other )
{

  this->gkg::LightCurve3d< T >::operator=( other );
  this->updateHeader();

  return *this;

}


template < class T >
void gkg::Curve3d< T >::addPoint( const gkg::Vector3d< T >& point )
{

  try
  {

    this->gkg::LightCurve3d< T >::addPoint( point );
    this->updateHeader();

  }
  GKG_CATCH( "template < class T >"
             "void gkg::Curve3d< T >::addPoint( "
             "const gkg::Vector3d< T >& point )" );

}


template < class T >
void 
gkg::Curve3d< T >::addPoints( const std::list< gkg::Vector3d< T > >& points )
{

  try
  {

    this->gkg::LightCurve3d< T >::addPoints( points );
    this->updateHeader();

  }
  GKG_CATCH( "template < class T >"
             "void gkg::Curve3d< T >::addPoints( "
             "const std::list< gkg::Vector3d< T > >& points )" );

}


template < class T >
void 
gkg::Curve3d< T >::addPoints( const std::vector< gkg::Vector3d< T > >& points )
{

  try
  {

    this->gkg::LightCurve3d< T >::addPoints( points );
    this->updateHeader();

  }
  GKG_CATCH( "template < class T >"
             "void gkg::Curve3d< T >::addPoints( "
             "const std::list< gkg::Vector3d< T > >& points )" );

}


template < class T >
void 
gkg::Curve3d< T >::addPoints( const gkg::Matrix& matrix )
{

  try
  {

    this->gkg::LightCurve3d< T >::addPoints( matrix );
    this->updateHeader();

  }
  GKG_CATCH( "template < class T >"
             "void gkg::Curve3d< T >::addPoints( "
             "const gkg::Matrix& matrix )" );

}


template < class T >
void gkg::Curve3d< T >::setEquidistantCurve( int32_t pointCount )
{

  try
  {

    this->gkg::LightCurve3d< T >::setEquidistantCurve( pointCount );
    this->updateHeader();

  }
  GKG_CATCH( "template < class T > "
             "void "
             "gkg::Curve3d< T >::setEquidistantCurve( int32_t pointCount )" );

}


template < class T >
void gkg::Curve3d< T >::removePoint( const int32_t& index )
{

  try
  {

    // erasing point at given index
    this->gkg::LightCurve3d< T >::removePoint( index );

    // updating header
    this->updateHeader();

  }
  GKG_CATCH( "template < class T >"
             "void gkg::Curve3d< T >::removePoint( const int32_t& index )" );

}


template < class T >
void gkg::Curve3d< T >::removePoints( const std::set< int32_t >& indices )
{

  try
  {

    // copying back new point vector
    this->gkg::LightCurve3d< T >::removePoints( indices );

    // updating header
    this->updateHeader();

  }
  GKG_CATCH( "template < class T >"
             "void gkg::Curve3d< T >::removePoints( "
             "const std::set< int32_t >& indices )" );

}


template < class T >
void gkg::Curve3d< T >::clear()
{

  try
  {

    // erasing point(s)
    this->gkg::LightCurve3d< T >::clear();

    // updating header
    this->updateHeader();

  }
  GKG_CATCH( "template < class T >"
             "void gkg::Curve3d< T >::clear()" );

}


template < class T >
void gkg::Curve3d< T >::setClosed()
{

  try
  {

    // clsoing curve
    this->gkg::LightCurve3d< T >::setClosed();

    // updating header
    this->updateHeader();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Curve3d< T >::setClosed()" );

}


template < class T >
gkg::Curve3d< T > 
gkg::Curve3d< T >::getEquidistantCurve( int32_t pointCount ) const
{

  try
  {

    return gkg::Curve3d< T >(
              this->gkg::LightCurve3d< T >::getEquidistantCurve( pointCount ) );

  }
  GKG_CATCH( "template < class T > "
             "gkg::Curve3d< T > "
             "gkg::Curve3d< T >::getEquidistantCurve( "
             "int32_t pointCount ) const" );

}


template < class T >
void gkg::Curve3d< T >::updateHeader()
{

  try
  {

    _header[ "point_count" ] = this->getPointCount();
    _header[ "closed_curve" ] = this->isClosed();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Curve3d< T >::updateHeader()" );

}


// 
// forcing instanciations
//

template class gkg::Curve3d< float >;
template class gkg::Curve3d< double >;



#undef INFINITE_TORTUOSITY
#undef ALMOST_NULL_DISTANCE
