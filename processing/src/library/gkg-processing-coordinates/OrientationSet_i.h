#ifndef _gkg_processing_coordinates_OrientationSet_i_h_
#define _gkg_processing_coordinates_OrientationSet_i_h_


#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-mesh/ConvexHull_i.h>
#include <gkg-processing-transform/Transform3d.h>
#include <gkg-core-exception/Exception.h>


template < class R, class Compare >
inline
void gkg::OrientationSet::addMesh(
                       const gkg::Transform3d< float >& transform,
                       const R& rank,
                       gkg::MeshMap< R, float, 3U, Compare >& meshMap ) const
{

  try
  {

    // creating transformed orientation set
    std::vector< Vector3d< float > >
      transformedOrientations( _orientations.size() );

    std::vector< Vector3d< float > >::const_iterator
      o = _orientations.begin(),
      oe = _orientations.end();
    std::vector< Vector3d< float > >::iterator
      to = transformedOrientations.begin();
    while ( o != oe )
    {

      transform.getDirect( *o, *to );
      ++ o;
      ++ to;

    }

    // adding convex hull to given mesh map
    gkg::ConvexHull::getInstance().addConvexHull( transformedOrientations,
                                                  rank,
                                                  meshMap );

  }
  GKG_CATCH( "template < class R, class Compare > "
             "inline "
             "void gkg::OrientationSet::addMesh( "
             "const gkg::Transform3d< float >& transform, "
             "const R& rank, "
             "gkg::MeshMap< R, float, 3U, Compare >& meshMap ) const" );

}


template < class R, class Compare >
inline
void gkg::OrientationSet::addMesh(
                       const gkg::Transform3d< float >& transform,
                       const R& rank,
                       const gkg::TextureMap< float >& scaling,
                       gkg::MeshMap< R, float, 3U, Compare >& meshMap ) const
{

  try
  {

    // sanity checks
    if ( scaling.getCount() != ( int32_t )_orientations.size() )
    {

      throw std::runtime_error( "incompatible orientation and scaling sizes" );

    }

    // scaling orientations
    std::vector< Vector3d< float > > scaledOrientations( _orientations );
    std::vector< Vector3d< float > >::iterator
      so = scaledOrientations.begin(),
      soe = scaledOrientations.end();
    gkg::TextureMap< float >::const_iterator
      s = scaling.begin();
    while ( so != soe )
    {

      *so *= s->second;
      ++ so;
      ++ s;

    }

    // creating transformed orientation set and 
    std::vector< Vector3d< float > >
      transformedOrientations( scaledOrientations.size() );

    std::vector< Vector3d< float > >::const_iterator
      o = scaledOrientations.begin(),
      oe = scaledOrientations.end();
    std::vector< Vector3d< float > >::iterator
      to = transformedOrientations.begin();
    while ( o != oe )
    {

      transform.getDirect( *o, *to );
      ++ o;
      ++ to;

    }


    // adding convex hull to given mesh map
    gkg::ConvexHull::getInstance().addConvexHull( transformedOrientations,
                                                  rank,
                                                  meshMap );
  }
  GKG_CATCH( "template < class R, class Compare > "
             "inline "
             "void gkg::OrientationSet::addMesh( "
             "const gkg::Transform3d< float >& transform, "
             "const R& rank, "
             "const gkg::TextureMap< float >& scaling, "
             "gkg::MeshMap< R, float, 3U, Compare >& meshMap ) const" );

}


template < class R, class Compare >
inline
void gkg::OrientationSet::addMesh(
                       const gkg::Transform3d< float >& transform,
                       const R& rank,
                       const std::vector< float >& scaling,
                       gkg::MeshMap< R, float, 3U, Compare >& meshMap ) const
{

  try
  {

    // sanity checks
    if ( scaling.size() != _orientations.size() )
    {

      throw std::runtime_error( "incompatible orientation and scaling sizes" );

    }

    // scaling orientations
    std::vector< Vector3d< float > > scaledOrientations( _orientations );
    std::vector< Vector3d< float > >::iterator
      so = scaledOrientations.begin(),
      soe = scaledOrientations.end();
    std::vector< float >::const_iterator
      s = scaling.begin();
    while ( so != soe )
    {

      *so *= *s;
      ++ so;
      ++ s;

    }

    // creating transformed orientation set and 
    std::vector< Vector3d< float > >
      transformedOrientations( scaledOrientations.size() );

    std::vector< Vector3d< float > >::const_iterator
      o = scaledOrientations.begin(),
      oe = scaledOrientations.end();
    std::vector< Vector3d< float > >::iterator
      to = transformedOrientations.begin();
    while ( o != oe )
    {

      transform.getDirect( *o, *to );
      ++ o;
      ++ to;

    }


    // adding convex hull to given mesh map
    gkg::ConvexHull::getInstance().addConvexHull( transformedOrientations,
                                                  rank,
                                                  meshMap );
  }
  GKG_CATCH( "template < class R, class Compare > "
             "inline "
             "void gkg::OrientationSet::addMesh( "
             "const gkg::Transform3d< float >& transform, "
             "const R& rank, "
             "const std::vector< float >& scaling, "
             "gkg::MeshMap< R, float, 3U, Compare >& meshMap ) const" );

}


template < class R, class Compare >
inline
void gkg::OrientationSet::addSymmetricalMesh(
                       const gkg::Transform3d< float >& transform,
                       const R& rank,
                       gkg::MeshMap< R, float, 3U, Compare >& meshMap ) const
{

  try
  {

    // creating symmetrical orientation set
    std::vector< Vector3d< float > >
      symmetricalOrientations( 2U * _orientations.size() );

    size_t i;
    for ( i = 0; i < _orientations.size(); i++ )
    {

      symmetricalOrientations[ i ] = _orientations[ i ];
      symmetricalOrientations[ i + _orientations.size() ] = -_orientations[ i ];

    }

    // creating transformed orientation set
    std::vector< Vector3d< float > >
      transformedOrientations( symmetricalOrientations.size() );

    std::vector< Vector3d< float > >::const_iterator
      o = symmetricalOrientations.begin(),
      oe = symmetricalOrientations.end();
    std::vector< Vector3d< float > >::iterator
      to = transformedOrientations.begin();
    while ( o != oe )
    {

      transform.getDirect( *o, *to );
      ++ o;
      ++ to;

    }

    // adding convex hull to given mesh map
    gkg::ConvexHull::getInstance().addConvexHull( transformedOrientations,
                                                  rank,
                                                  meshMap );

  }
  GKG_CATCH( "template < class R, class Compare > "
             "inline "
             "void gkg::OrientationSet::addSymmetricalMesh( "
             "const gkg::Transform3d< float >& transform, "
             "const R& rank, "
             "gkg::MeshMap< R, float, 3U, Compare >& meshMap ) const" );

}


template < class R, class Compare >
inline
void gkg::OrientationSet::addSymmetricalMesh(
                       const gkg::Transform3d< float >& transform,
                       const R& rank,
                       const gkg::TextureMap< float >& scaling,
                       gkg::MeshMap< R, float, 3U, Compare >& meshMap ) const
{

  try
  {

    // sanity checks
    if ( scaling.getCount() != ( int32_t )_orientations.size() )
    {

      throw std::runtime_error( "incompatible orientation and scaling sizes" );

    }

    // creating symmetrical orientation set and scaling at once
    std::vector< Vector3d< float > >
      symmetricalOrientations( 2U * _orientations.size() );
    gkg::TextureMap< float >::const_iterator
      s = scaling.begin();

    size_t i;
    for ( i = 0; i < _orientations.size(); i++ )
    {

      symmetricalOrientations[ i ] = _orientations[ i ] * s->second;
      symmetricalOrientations[ i + _orientations.size() ] =
                                    -_orientations[ i ] * s->second;
      ++ s;

    }

    // creating transformed orientation set
    std::vector< Vector3d< float > >
      transformedOrientations( symmetricalOrientations.size() );

    std::vector< Vector3d< float > >::const_iterator
      o = symmetricalOrientations.begin(),
      oe = symmetricalOrientations.end();
    std::vector< Vector3d< float > >::iterator
      to = transformedOrientations.begin();
    while ( o != oe )
    {

      transform.getDirect( *o, *to );
      ++ o;
      ++ to;

    }

    // adding convex hull to given mesh map
    gkg::ConvexHull::getInstance().addConvexHull( transformedOrientations,
                                                  rank,
                                                  meshMap );

  }
  GKG_CATCH( "template < class R, class Compare > "
             "inline "
             "void gkg::OrientationSet::addSymmetricalMesh( "
             "const gkg::Transform3d< float >& transform, "
             "const R& rank, "
             "const gkg::TextureMap< float >& scaling, "
             "gkg::MeshMap< R, float, 3U, Compare >& meshMap ) const" );

}


template < class R, class Compare >
inline
void gkg::OrientationSet::addSymmetricalMesh(
                       const gkg::Transform3d< float >& transform,
                       const R& rank,
                       const std::vector< float >& scaling,
                       gkg::MeshMap< R, float, 3U, Compare >& meshMap ) const
{

  try
  {

    // sanity checks
    if ( scaling.size() != _orientations.size() )
    {

      throw std::runtime_error( "incompatible orientation and scaling sizes" );

    }

    // creating symmetrical orientation set and scaling at once
    std::vector< Vector3d< float > >
      symmetricalOrientations( 2U * _orientations.size() );

    size_t i;
    for ( i = 0; i < _orientations.size(); i++ )
    {

      symmetricalOrientations[ i ] = _orientations[ i ] * scaling[ i ];
      symmetricalOrientations[ i + _orientations.size() ] =
                                    -_orientations[ i ] * scaling[ i ];

    }

    // creating transformed orientation set
    std::vector< Vector3d< float > >
      transformedOrientations( symmetricalOrientations.size() );

    std::vector< Vector3d< float > >::const_iterator
      o = symmetricalOrientations.begin(),
      oe = symmetricalOrientations.end();
    std::vector< Vector3d< float > >::iterator
      to = transformedOrientations.begin();
    while ( o != oe )
    {

      transform.getDirect( *o, *to );
      ++ o;
      ++ to;

    }

    // adding convex hull to given mesh map
    gkg::ConvexHull::getInstance().addConvexHull( transformedOrientations,
                                                  rank,
                                                  meshMap );

  }
  GKG_CATCH( "template < class R, class Compare > "
             "inline "
             "void gkg::OrientationSet::addSymmetricalMesh( "
             "const gkg::Transform3d< float >& transform, "
             "const R& rank, "
             "const std::vector< float >& scaling, "
             "gkg::MeshMap< R, float, 3U, Compare >& meshMap ) const" );

}


#endif
