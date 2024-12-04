#ifndef _gkg_processing_container_Polygon_h_
#define _gkg_processing_container_Polygon_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/RankSiteLut_i.h>
#include <functional>
#include <iostream>


namespace gkg
{


template < uint32_t D >
struct Polygon
{

  Polygon();
  Polygon( uint32_t indice1,
           uint32_t indice2 = 0U,
           uint32_t indice3 = 0U,
           uint32_t indice4 = 0U );
  Polygon( const uint32_t* values );
  Polygon( const Polygon< D >& other );

  Polygon< D >& operator = ( const Polygon< D >& other );

  bool operator == ( const gkg::Polygon< D >& other ) const;
  bool operator != ( const gkg::Polygon< D >& other ) const;

  template < class S >
  S getSurfaceArea( const std::vector< Vector3d< S > >& vertices ) const;

  uint32_t indices[ D ];

};


typedef struct Polygon< 2U > Segment;
typedef struct Polygon< 3U > Triangle;
typedef struct Polygon< 4U > Quad;


template < uint32_t D >
struct PolygonCompare /*: public std::binary_function< Polygon< D >,
                                                     Polygon< D >,
                                                     bool >
                        deprecated in c++17 */
{

  typedef Polygon< D > first_argument_type;
  typedef Polygon< D > second_argument_type;
  typedef bool result_type;

  bool operator()( const Polygon< D >& p1, const Polygon< D >& p2 ) const
  {

    uint32_t d;
    if ( p1.indices[ D - 1 ] < p2.indices[ D - 1 ] )
    {

      return true;

    }
    else
    {

      for ( d = D-1; d > 0; d-- )
      {

        if ( ( p1.indices[ d ] == p2.indices[ d ] ) &&
             ( p1.indices[ d - 1 ] < p2.indices[ d - 1 ] ) )
        {

          return true;

        }

      }

    }
    return false;

  }

};


template < uint32_t D >
struct RCPolygonCompare /*: public std::binary_function<
                                                     RCPointer< Polygon< D > >,
                                                     RCPointer< Polygon< D > >,
                                                     bool >
                          depecrated in c++17 */
{

  typedef RCPointer< Polygon< D > > first_argument_type;
  typedef RCPointer< Polygon< D > > second_argument_type;
  typedef bool result_type;

  bool operator()( const RCPointer< Polygon< D > >& p1,
                   const RCPointer< Polygon< D > >& p2 ) const
  {

    uint32_t d;
    if ( p1->indices[ D - 1 ] < p2->indices[ D - 1 ] )
    {

      return true;

    }
    else
    {

      for ( d = D-1; d > 0; d-- )
      {

        if ( ( p1->indices[ d ] == p2->indices[ d ] ) &&
             ( p1->indices[ d - 1 ] < p2->indices[ d - 1 ] ) )
        {

          return true;

        }

      }

    }
    return false;

  }

};


template < class R, class S, uint32_t D, class Compare = std::less< R > >
struct PolygonNormalFunctor
{

  Vector3d< S > 
    operator()( const Polygon< D >& /*p*/,
                const RankSiteLut< R, S, Compare >& /*rankSiteLut */ ) const
  {

    throw std::runtime_error( "not implemented yet" );
    return Vector3d< S >( ( S )0, ( S )0, ( S )0 );

  }

};


template <>
inline
Vector3d< float > 
PolygonNormalFunctor< int32_t, float, 3U, std::less< int32_t > >::operator()(
  const Polygon< 3U >& p,
  const RankSiteLut< int32_t, float, std::less< int32_t > >& rankSiteLut ) const
{

  Vector3d< float > polygonVertices[ 3U ];
  uint32_t d;
  for ( d = 0; d < 3U; d++ )
  {

    polygonVertices[ d ] = rankSiteLut.getRankSite( p.indices[ d ] ).site;

  }
  Vector3d< float >
    normal = ( polygonVertices[ 1 ] - polygonVertices[ 0 ] ).cross(
                                  polygonVertices[ 2 ] - polygonVertices[ 0 ] );
  normal *= 0.5f;

  return normal;

}


template <>
inline
Vector3d< double >
PolygonNormalFunctor< int32_t, double, 3U, std::less< int32_t > >::operator()(
 const Polygon< 3U >& p,
 const RankSiteLut< int32_t, double, std::less< int32_t > >& rankSiteLut ) const
{

  Vector3d< double > polygonVertices[ 3U ];
  uint32_t d;
  for ( d = 0; d < 3U; d++ )
  {

    polygonVertices[ d ] = rankSiteLut.getRankSite( p.indices[ d ] ).site;

  }
  Vector3d< double >
    normal = ( polygonVertices[ 1 ] - polygonVertices[ 0 ] ).cross(
                                  polygonVertices[ 2 ] - polygonVertices[ 0 ] );
  normal *= 0.5;

  return normal;

}


}


namespace std
{

/*
bool operator == ( const gkg::Polygon< 2U >& thing1,
                   const gkg::Polygon< 2U >& thing2 );
bool operator == ( const gkg::Polygon< 3U >& thing1,
                   const gkg::Polygon< 3U >& thing2 );
bool operator == ( const gkg::Polygon< 4U >& thing1,
                   const gkg::Polygon< 4U >& thing2 );
*/

ostream& operator << ( ostream& os, const gkg::Polygon< 2U >& thing );
ostream& operator << ( ostream& os, const gkg::Polygon< 3U >& thing );
ostream& operator << ( ostream& os, const gkg::Polygon< 4U >& thing );

istream& operator>>( istream& is, gkg::Polygon< 2U >& thing );
istream& operator>>( istream& is, gkg::Polygon< 3U >& thing );
istream& operator>>( istream& is, gkg::Polygon< 4U >& thing );

}


#endif
