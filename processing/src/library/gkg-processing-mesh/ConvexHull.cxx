#include <gkg-processing-mesh/ConvexHull.h>
#include <gkg-core-exception/Exception.h>
#include <algorithm>


// code rewritten from the one provided at
// http://www.cs.sunysb.edu/~algorith/implement/orourke/distrib/
//
// This code is described in "Computational Geometry in C" (Second Edition),
// Chapter 4.  It is not written to be comprehensible without the explanation 
// in that book.
//
// Input: 3n integer coordinates for the points.
// Output: the 3D convex hull, in postscript with embedded comments showing the
// vertices and faces.
//
// Compile: gcc -o chull chull.c
//
// Written by Joseph O'Rourke, with contributions by Kristy Anderson,
// John Kutcher, Catherine Schevon, Susan Weller.
// Last modified: March 1998
// Questions to orourke@cs.smith.edu.
// --------------------------------------------------------------------
// This code is Copyright 1998 by Joseph O'Rourke.  It may be freely 
// redistributed in its entirety provided that this copyright notice is 
// not removed.
// --------------------------------------------------------------------


#define SAFE   1000000

#define ADD( head, item ) \
  if ( head ) \
  { \
    item->next = head; \
    item->previous = ( head )->previous; \
    ( head )->previous = item; \
    item->previous->next = item; \
  } \
  else \
  { \
    head = item; \
    ( head )->next = ( head )->previous = item; \
  }


#define REMOVE( head, item ) \
  if ( head ) \
  { \
    if ( head == ( head )->next ) \
    { \
      head = 0; \
    } \
    else if ( item == head ) \
    { \
      head = ( head )->next; \
    } \
    item->next->previous = item->previous; \
    item->previous->next = item->next; \
    if ( item ) \
    { \
      delete item; \
      item = 0; \
    } \
  }


static bool areCollinear( const gkg::Vector3d< int32_t >& v1,
                          const gkg::Vector3d< int32_t >& v2 )
{

  double x = ( double )v1.y * ( double )v2.z - ( double )v1.z * ( double )v2.y;
  double y = ( double )v1.z * ( double )v2.x - ( double )v1.x * ( double )v2.z;
  double z = ( double )v1.x * ( double )v2.y - ( double )v1.y * ( double )v2.x;

  int32_t X = ( int32_t )x;
  int32_t Y = ( int32_t )y;
  int32_t Z = ( int32_t )z;

  return ( X == 0 ) && ( Y == 0 ) && ( Z == 0 );

}
                          

//
// struct ConvexHull::Vertex
//

gkg::ConvexHull::Vertex::Vertex( int32_t x,
                                 int32_t y,
                                 int32_t z,
                                 int32_t i )
                        : vertex( x, y, z ),
                          index( i ),
                          duplicate( 0 ),
                          onHull( false ),
                          mark( false )
{
}


//
// struct ConvexHull::Face
//

gkg::ConvexHull::Face::Face( gkg::ConvexHull::Face** theFaces,
                             gkg::ConvexHull::Edge** theEdges,
                             gkg::ConvexHull::Vertex* vertex0,
                             gkg::ConvexHull::Vertex* vertex1,
                             gkg::ConvexHull::Vertex* vertex2,
                             gkg::ConvexHull::Face* fold )
{


  gkg::ConvexHull::Edge* edge0;
  gkg::ConvexHull::Edge* edge1;
  gkg::ConvexHull::Edge* edge2;

  if ( !fold )
  {

    // create edges of the initial triangle
    edge0 = new gkg::ConvexHull::Edge( theEdges );
    edge1 = new gkg::ConvexHull::Edge( theEdges );
    edge2 = new gkg::ConvexHull::Edge( theEdges );

  }
  else
  {

    // copy from fold, in reverse order
    edge0 = fold->edges[ 2 ];
    edge1 = fold->edges[ 1 ];
    edge2 = fold->edges[ 0 ];

  }

  edge0->endPoints[ 0 ] = vertex0;
  edge0->endPoints[ 1 ] = vertex1;
  edge1->endPoints[ 0 ] = vertex1;
  edge1->endPoints[ 1 ] = vertex2;
  edge2->endPoints[ 0 ] = vertex2;
  edge2->endPoints[ 1 ] = vertex0;

  // create face for triangle
  edges[ 0 ] = edge0;
  edges[ 1 ] = edge1;
  edges[ 2 ] = edge2;
  vertices[ 0 ] = vertex0;
  vertices[ 1 ] = vertex1;
  vertices[ 2 ] = vertex2;
  visible = false;

  ADD( *theFaces, this );

  // link edges to face
  edge0->adjacentFaces[ 0 ] = this;
  edge1->adjacentFaces[ 0 ] = this;
  edge2->adjacentFaces[ 0 ] = this;

}


// making a new face and two new edges between the edge and the point that are 
// passed to it
gkg::ConvexHull::Face::Face( gkg::ConvexHull::Face** theFaces,
                             gkg::ConvexHull::Edge** theEdges,
                             gkg::ConvexHull::Edge* edge,
                             gkg::ConvexHull::Vertex* vertex )
{

  // making two new edges (if don't already exist)
  gkg::ConvexHull::Edge* newEdge[ 2 ];
  int i;
  for ( i = 0; i < 2; i++ )
  {

    // if the edge exists, copy it into newEdge
    newEdge[ i ] = edge->endPoints[ i ]->duplicate;
    // otherwise (duplicate is null), make null edge
    if ( !newEdge[ i ] )
    {

      newEdge[ i ] = new gkg::ConvexHull::Edge( theEdges );
      newEdge[ i ]->endPoints[ 0 ] = edge->endPoints[ i ];
      newEdge[ i ]->endPoints[ 1 ] = vertex;
      edge->endPoints[ i ]->duplicate = newEdge[ i ];

    }

  }

  // making the new face
  edges[ 0 ] = edge;
  edges[ 1 ] = newEdge[ 0 ];
  edges[ 2 ] = newEdge[ 1 ];

  // putting the vertices in the face structure in counterclock wise order; we
  // want to store the vertices in the same order as in the visible face; the 
  // third vertex is always the given vertex
  gkg::ConvexHull::Face* visibleFace;   // the visible face adjacent to edge
  if ( edge->adjacentFaces[ 0 ]->visible )
  {

    visibleFace = edge->adjacentFaces[ 0 ];

  }
  else
  {

    visibleFace = edge->adjacentFaces[ 1 ];

  }
  // setting vertices[ 0 ] & [ 1 ] of visibleFace to have the same orientation
  // as do the corresponding vertices of visibleFace 
  for ( i = 0; visibleFace->vertices[ i ] != edge->endPoints[ 0 ]; i++ );

  // orienting newFace the same as visibleFace
  if ( visibleFace->vertices[ ( i + 1 ) % 3 ] != edge->endPoints[ 1 ] )
  {

    vertices[ 0 ] = edge->endPoints[ 1 ];  
    vertices[ 1 ] = edge->endPoints[ 0 ];    

  }
  else
  {                               

    vertices[ 0 ] = edge->endPoints[ 0 ];   
    vertices[ 1 ] = edge->endPoints[ 1 ];      

    // this swap is tricky; edge is edges[ 0 ]; edge[ 1 ] is based on
    // endPoints[ 0 ], edges[ 2 ] on endPoints[ 1 ]; so if edge is oriented 
    // "forwards", we need to move edges[ 1 ] to follow [ 0 ], because it 
    // precedes
    std::swap( edges[ 1 ], edges[ 2 ] );

  }
  vertices[ 2 ] = vertex;

  visible = false;

  ADD( *theFaces, this );

  // setting the adjacent face pointers
  int32_t j;
  for ( i = 0; i < 2; i++ )
  {

    for ( j = 0; j < 2; j++ )
    {

      // only one null link should be set to new face
      if ( !newEdge[ i ]->adjacentFaces[ j ] )
      {

        newEdge[ i ]->adjacentFaces[ j ] = this;
        break;

      }

    }

  }

}


void gkg::ConvexHull::Face::linkAdjacentFace( gkg::ConvexHull::Face* other )
{

  edges[ 0 ]->adjacentFaces[ 1 ] = other;
  edges[ 1 ]->adjacentFaces[ 1 ] = other;
  edges[ 2 ]->adjacentFaces[ 1 ] = other;

}


//
// struct ConvexHull::Edge
//

gkg::ConvexHull::Edge::Edge( gkg::ConvexHull::Edge** theEdges )
{

  adjacentFaces[ 0 ] = 0;
  adjacentFaces[ 1 ] = 0;
  endPoints[ 0 ] = 0;
  endPoints[ 1 ] = 0;
  newFace = 0;
  cDelete = false;

  ADD( *theEdges, this );

}


//
// class ConvexHull
//

gkg::ConvexHull::ConvexHull()
{
}


gkg::ConvexHull::~ConvexHull()
{
}


void gkg::ConvexHull::getConvexHull(
                     const std::vector< Vector3d< float > >& points,
                     std::list< gkg::Vector3d< float > >& vertices,
                     std::list< gkg::Polygon< 3U > >& polygons ) const
{

  try
  {

    //
    // clearing vertices and polygons
    //
    vertices.clear();
    polygons.clear();

    //
    // building vertex vector and processing min/max coordinate values
    //
    float minimum = points[ 0 ].x;
    float maximum = minimum;
    std::vector< gkg::Vector3d< float > >::const_iterator
      p = points.begin(),
      pe = points.end();
    while ( p != pe )
    {

      minimum = std::min( std::min( minimum , p->x ),
                          std::min( p->y, p->z ) );
      maximum = std::max( std::max( maximum , p->x ),
                          std::max( p->y, p->z ) );
      vertices.push_back( *p );
      ++ p;

    }

    //
    // building integer vertex list
    //
    gkg::ConvexHull::Vertex* integerVertices = 0;
    std::list< gkg::Vector3d< float > >::const_iterator
      fv = vertices.begin(),
      fve = vertices.end();
    int32_t index = 0;
    while ( fv != fve )
    {

      gkg::ConvexHull::Vertex* vertex = new gkg::ConvexHull::Vertex(
              ( int32_t )( ( fv->x - minimum ) / ( maximum - minimum ) * SAFE ),
              ( int32_t )( ( fv->y - minimum ) / ( maximum - minimum ) * SAFE ),
              ( int32_t )( ( fv->z - minimum ) / ( maximum - minimum ) * SAFE ),
              index );
      ADD( integerVertices, vertex );
      ++ index;
      ++ fv;

    }

    //
    // building double triangle
    //
    gkg::ConvexHull::Vertex*  v0;
    gkg::ConvexHull::Vertex*  v1;
    gkg::ConvexHull::Vertex*  v2;
    gkg::ConvexHull::Vertex*  v3;
    gkg::Vector3d< int32_t > nullVector( 0, 0, 0 );

    v0 = integerVertices;
    v1 = v0->next;
    v2 = v1->next;

    while ( areCollinear( v2->vertex - v0->vertex, v1->vertex - v0->vertex ) )
    {

      v0 = v0->next;
      if ( v0 == integerVertices )
      {

        throw std::runtime_error( "all points are collinear" );

      }
      v1 = v0->next;
      v2 = v1->next;

    }

    // mark the vertices as processed
    v0->mark = true;
    v1->mark = true;
    v2->mark = true;

    // create the two "twin" faces
    gkg::ConvexHull::Edge* edges = 0;
    gkg::ConvexHull::Face* faces = 0;
    gkg::ConvexHull::Face*
      face0 = new gkg::ConvexHull::Face( &faces,
                                         &edges,
                                         v0, v1, v2, 0 );
    gkg::ConvexHull::Face*
      face1 = new gkg::ConvexHull::Face( &faces,
                                         &edges,
                                         v2, v1, v0, face0 );
    
    // link adjacent face fields
    face0->linkAdjacentFace( face1 );
    face1->linkAdjacentFace( face0 );

    // find a fourth, noncoplanar point to form tetrahedron
    v3 = v2->next;
    int32_t volumSign = getVolumeSign( face0, v3 );
    while ( !volumSign )
    {

      v3 = v3->next;
      if ( v3 == v0 )
      {

        throw std::runtime_error( "all points are coplanar" );

      }
      volumSign = getVolumeSign( face0, v3 );

    }

    // insure that v3 will be the first added
    integerVertices = v3;

    //
    // constructing hull
    //
    gkg::ConvexHull::Vertex* v = integerVertices;
    gkg::ConvexHull::Vertex* nextV;
    do
    {

      nextV = v->next;
      if ( !v->mark )
      {

        v->mark = true;
        addOne( &faces, &edges, v );
        cleanup( &faces, &edges, &integerVertices );

      }
      v = nextV;

    }
    while ( v != integerVertices );

    //
    // extracting result
    //

    gkg::ConvexHull::Face* f = faces;
    do
    {

      polygons.push_back( gkg::Polygon< 3U >( f->vertices[ 0 ]->index,
                                              f->vertices[ 1 ]->index,
                                              f->vertices[ 2 ]->index ) );
      f = f->next;

    }
    while ( f != faces );

    //
    // deleting everything at the end
    //
    f = faces;
    do
    {

      f = f->next;
      delete f;

    }
    while ( f != faces );

    v = integerVertices;
    do
    {

      v = v->next;
      delete v;

    }
    while ( v != integerVertices );

    gkg::ConvexHull::Edge* e = edges;
    do
    {

      e = e->next;
      delete e;

    }
    while ( e != edges );

  }
  GKG_CATCH( "void gkg::ConvexHull::getConvexHull( "
             "const std::vector< Vector3d< float > >& points, "
             "std::set< gkg::Vector3d< float >, "
             "          gkg::Vector3dCompare< float > >& vertices, "
             "std::set< gkg::Polygon< 3U >, "
             "          gkg::PolygonCompare< 3U > >& polygons ) const" );

}


int32_t gkg::ConvexHull::getVolumeSign( gkg::ConvexHull::Face* face,
                                        gkg::ConvexHull::Vertex* vertex ) const
{

  try
  {

    gkg::Vector3d< int32_t > a = face->vertices[ 0 ]->vertex - vertex->vertex;
    gkg::Vector3d< int32_t > b = face->vertices[ 1 ]->vertex - vertex->vertex;
    gkg::Vector3d< int32_t > c = face->vertices[ 2 ]->vertex - vertex->vertex;

    float volumeSign = ( float )a.x * ( ( float )b.y * ( float )c.z -
                                        ( float )b.z * ( float )c.y ) +
                       ( float )a.y * ( ( float )b.z * ( float )c.x -
                                        ( float )b.x * ( float )c.z ) +
                       ( float )a.z * ( ( float )b.x * ( float )c.y -
                                        ( float )b.y * ( float )c.x );

    // the volume should be an integer
    if ( volumeSign >  0.5 )
    {

      return +1;

    }
    else if ( volumeSign < -0.5 )
    {

      return -1;

    }
    else
    {

      return 0;

    }

  }
  GKG_CATCH( "int32_t gkg::ConvexHull::getVolumeSign( "
             "gkg::ConvexHull::Face* face, "
             "gkg::ConvexHull::Vertex* vertex ) const" );

}


// addOne() first determines all faces visible from that point; if none are 
// visible then the point is marked as not on hull; next is a loop over edges;
// if both faces adjacent to an edge are visible, then the edge is marked for 
// deletion; if just one of the adjacent faces is visible then a new face is 
// constructed
void gkg::ConvexHull::addOne( gkg::ConvexHull::Face** theFaces,
                              gkg::ConvexHull::Edge** theEdges,
                              gkg::ConvexHull::Vertex* vertex ) const
{

  try
  {

    int32_t volumeSign = 0;
    bool visible = false;

    // mark faces visible from given vertex
    gkg::ConvexHull::Face* f = *theFaces;
    do
    {

      volumeSign = getVolumeSign( f, vertex );
      if ( volumeSign < 0 )
      {

        f->visible = true;
        visible = true;

      }
      f = f->next;

    }
    while ( f != *theFaces );

    // if no faces are visible from given vertex, then vertex is inside the hull
    if ( !visible )
    {

      vertex->onHull = false;
      return;

    }

    // mark faces visible from given vertex
    gkg::ConvexHull::Edge* e = *theEdges;
    gkg::ConvexHull::Edge* tmpEdge;
    do
    {

      tmpEdge = e->next;
      // e interior: mark for deletion
      if ( e->adjacentFaces[ 0 ]->visible &&
           e->adjacentFaces[ 1 ]->visible )
      {

        e->cDelete = true;

      }
      // e border: make a new face
      else if ( e->adjacentFaces[ 0 ]->visible ||
                e->adjacentFaces[ 1 ]->visible )
      {

        e->newFace = new gkg::ConvexHull::Face( theFaces, theEdges, e, vertex );

      }
      e = tmpEdge;

    }
    while ( e != *theEdges );

  }
  GKG_CATCH( "void gkg::ConvexHull::addOne( "
             "gkg::ConvexHull::Face** theFaces, "
             "gkg::ConvexHull::Edge** theEdges, "
             "gkg::ConvexHull::Vertex* vertex ) const" );

}


// going through each data structure list and clearing all flags and nulling out
// some pointers; the order of processing (edges, faces, vertices) is important
void gkg::ConvexHull::cleanup(
                            gkg::ConvexHull::Face** theFaces,
                            gkg::ConvexHull::Edge** theEdges,
                            gkg::ConvexHull::Vertex** theIntegerVertices ) const
{

  try
  {

    //
    // cleaning edges
    //

    // integrate the newface's into the data structure; check every edge
    gkg::ConvexHull::Edge* e = *theEdges;
    gkg::ConvexHull::Edge* tmpEdge;
    do
    {

      if ( e->newFace )
      {

        if ( e->adjacentFaces[ 0 ]->visible )
        {

          e->adjacentFaces[ 0 ] = e->newFace;

        }
        else
        {

          e->adjacentFaces[ 1 ] = e->newFace;

        }
	e->newFace = 0;

      }
      e = e->next;

    }
    while ( e != *theEdges );

    // delete any edge marked for deletion
    while ( *theEdges && ( *theEdges )->cDelete )
    {

      e = *theEdges;
      REMOVE( *theEdges, e );

    }
    e = ( *theEdges )->next;
    do
    {

      if ( e->cDelete )
      {

        tmpEdge = e;
        e = e->next;
        REMOVE( *theEdges, tmpEdge );

      }
      else
      {

        e = e->next;

      }

    }
    while ( e != *theEdges );

    //
    // cleaning faces
    //

    gkg::ConvexHull::Face* f;
    gkg::ConvexHull::Face* tmpFace;
    while ( *theFaces && ( *theFaces )->visible )
    {

      f = *theFaces;
      REMOVE( *theFaces, f );

    }

    f = ( *theFaces )->next;
    do
    {

      if ( f->visible )
      {

        tmpFace = f;
        f = f->next;
        REMOVE( *theFaces, tmpFace );

      }
      else
      {

        f = f->next;

      }

    }
    while ( f != *theFaces );

    //
    // cleaning vertices
    //

    // mark all vertices incident to some undeleted edge as on the hull
    e = *theEdges;
    do
    {

      e->endPoints[ 0 ]->onHull = true;
      e->endPoints[ 1 ]->onHull = true;

      e = e->next;

    }
    while ( e != *theEdges );

    // delete all vertices that have been processed but are not on the hull
    gkg::ConvexHull::Vertex* v;
    gkg::ConvexHull::Vertex* tmpVertex;
    while ( ( *theIntegerVertices ) &&
            ( *theIntegerVertices )->mark &&
            !( *theIntegerVertices )->onHull )
    {

      v = *theIntegerVertices;
      REMOVE( *theIntegerVertices, v );

    }

    v = ( *theIntegerVertices )->next;
    do
    {

      if ( v->mark && !v->onHull )
      {

        tmpVertex = v;
        v = v->next;
        REMOVE( *theIntegerVertices, tmpVertex );

      }
      else
      {

        v = v->next;

      }

    }
    while ( v != *theIntegerVertices );

    // reset flags
    v = *theIntegerVertices;
    do
    {

      v->duplicate = 0;
      v->onHull = false;
      v = v->next;

    }
    while ( v != *theIntegerVertices );

  }
  GKG_CATCH( "void gkg::ConvexHull::cleanup( "
             "gkg::ConvexHull::Face** theFaces, "
             "gkg::ConvexHull::Edge** theEdges, "
             "gkg::ConvexHull::Vertex** theIntegerVertices ) const" );

}
