#include <gkg-processing-mesh/MeshSlicer.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-core-exception/Exception.h>
#include <cstdlib>
#include <cmath>
#include <algorithm>

// #include <gkg-core-io/StringConverter.h>
// #include <gkg-core-io/Writer_i.h>


// rounds x to an integer multiple of {eps}. If {mod} > 1,
// the factor will be congruent to {rem} modulo {mod}
inline float xround( float x, double eps, int32_t mod, int32_t rem )
{

  double y = std::round( ( double )x / ( mod * eps ) );
  double z = ( y * mod + rem ) * eps;
  return ( float )z;

}


inline int32_t binarySearch( const std::vector< float >& planes,
                             float minimumZ )
{

  int32_t planeCount = ( int32_t )planes.size();
  if ( minimumZ >= planes[ planeCount - 1 ] )
  {

    return planeCount;

  }
  // binary search
  int32_t l = -1;          // inferior z index
  int32_t r = planeCount;  // superior z index
  while ( r - l > 1 )
  {

    // at this point, minimumZ is between [planes[l];planes[r]]
    int32_t m = ( l + r ) / 2;
    if ( minimumZ >= planes[ m ] )
    {

      l = m;

    }
    else
    {

      r = m;

    }

  }
  return r;

}


////////////////////////////////////////////////////////////////////////////////
// class MeshSlicer
////////////////////////////////////////////////////////////////////////////////

gkg::MeshSlicer::MeshSlicer( float epsilon,
                             float minimumThickness,
                             float maximumThickness,
                             bool adaptive,
                             bool clockwise,
                             bool verbose )
                : _epsilon( epsilon ),
                  _minimumThickness( minimumThickness ),
                  _maximumThickness( maximumThickness ),
                  _adaptive( adaptive ),
                  _clockwise( clockwise ),
                  _verbose( verbose )
{
}



gkg::MeshSlicer::~MeshSlicer()
{
}


void gkg::MeshSlicer::slicing(
              const gkg::MeshMap< int32_t, float, 3U >& meshes,
              std::map< int32_t, gkg::Curve3dMap< float > >& curve3dMaps ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // looping over meshes
    ////////////////////////////////////////////////////////////////////////////

    gkg::SiteMap< int32_t, float >::const_iterator
      mv = meshes.vertices.begin(),
      mve = meshes.vertices.end();
    gkg::PolygonMap< 3U >::const_iterator
      mp = meshes.polygons.begin();
    while ( mv != mve )
    {

      //////////////////////////////////////////////////////////////////////////
      // collecting rank for the current mesh and vertices
      //////////////////////////////////////////////////////////////////////////

      int32_t rank = mv->first;

      if ( _verbose )
      {

        std::cout << "slicing rank " << rank << ":" << std::endl;
        std::cout << "================" << std::endl;

      }
      std::vector< gkg::Vector3d< float > > vertices( mv->second.begin(),
                                                      mv->second.end() );

      if ( _verbose )
      {

        std::cout << vertices.size() << " vertices" << std::endl;

      }

      //////////////////////////////////////////////////////////////////////////
      // collecting rank index for the current mesh and triangle(s)
      //////////////////////////////////////////////////////////////////////////

      //int32_t rankIndex = mp->first;
      std::vector< gkg::Polygon< 3U > > polygons( mp->second.begin(),
                                                  mp->second.end() );

      if ( _verbose )
      {

        std::cout << polygons.size() << " triangles" << std::endl;

      }

      //////////////////////////////////////////////////////////////////////////
      // computing the vertex bounding box
      //////////////////////////////////////////////////////////////////////////

      float lowerX = +1e38;
      float lowerY = +1e38;
      float lowerZ = +1e38;
      float upperX = -1e38;
      float upperY = -1e38;
      float upperZ = -1e38;

      std::vector< gkg::Vector3d< float > >::const_iterator
        v = vertices.begin(),
        ve = vertices.end();
      int32_t vertexIndex = 0;
      while ( v != ve )
      {

        if ( v->x < lowerX )
        {

          lowerX = v->x;

        }
        if ( v->x > upperX )
        {

          upperX = v->x;

        }

        if ( v->y < lowerY )
        {

          lowerY = v->y;

        }
        if ( v->y > upperY )
        {

          upperY = v->y;

        }
        if ( v->z < lowerZ )
        {

          lowerZ = v->z;

        }
        if ( v->z > upperZ )
        {

          upperZ = v->z;

        }
        ++ vertexIndex;
        ++ v;

      }
      gkg::BoundingBox< float > boundingBox( lowerX, upperX,
                                             lowerY, upperY,
                                             lowerZ, upperZ );

      if ( _verbose )
      {

        std::cout << "bounding box : "
                  << "(xinf, yinf, zinf)=("
                  << lowerX << ", " 
                  << lowerY << ", " 
                  << lowerZ << ") " 
                  << "(xsup, ysup, zsup)=("
                  << upperX << ", " 
                  << upperY << ", " 
                  << upperZ << ")"
                  << std::endl;

      }

      //////////////////////////////////////////////////////////////////////////
      // computing the minimum z of triangles
      //////////////////////////////////////////////////////////////////////////

      std::vector< float > minimumZ( polygons.size(), +1e38 );
      std::vector< float > maximumZ( polygons.size(), -1e38 );

      std::vector< gkg::Polygon< 3U > >::const_iterator
        t = polygons.begin(),
        te = polygons.end();
      std::vector< float >::iterator minz = minimumZ.begin();
      std::vector< float >::iterator maxz = maximumZ.begin();
      int32_t triangleIndex = 0;
      while ( t != te )
      {

        const float& z1 = vertices[ t->indices[ 0 ] ].z;
        if ( z1 < *minz )
        {

          *minz = z1;

        }
        if ( z1 > *maxz )
        {

          *maxz = z1;

        }
        const float& z2 = vertices[ t->indices[ 1 ] ].z;
        if ( z2 < *minz )
        {

          *minz = z2;

        }
        if ( z2 > *maxz )
        {

          *maxz = z2;

        }
        const float& z3 = vertices[ t->indices[ 2 ] ].z;
        if ( z3 < *minz )
        {

          *minz = z3;

        }
        if ( z3 > *maxz )
        {

          *maxz = z3;

        }
                  
        ++ triangleIndex;
        ++ t;
        ++ minz;
        ++ maxz;

      }

      //////////////////////////////////////////////////////////////////////////
      // computing plane(s)
      //////////////////////////////////////////////////////////////////////////

      std::vector< float > planes;
      float delta = this->computePlanes( boundingBox.getLowerZ(),
                                         boundingBox.getUpperZ(),
                                         planes );

      if ( _verbose )
      {

        std::cout << planes.size() << " planes : " << std::flush;
        std::vector< float >::const_iterator
          p = planes.begin(),
          pe = planes.end();
        -- pe;
        while ( p != pe )
        {

          std::cout << *p << "mm, ";
          ++ p;

        }
        std::cout << *p << "mm" << std::endl;

      }

      //////////////////////////////////////////////////////////////////////////
      // performing incremental slicing
      //////////////////////////////////////////////////////////////////////////

      std::list< gkg::LightCurve3d< float > > curve3ds;
      this->incrementalSlicing( vertices,
                                polygons,
                                minimumZ,
                                maximumZ,
                                planes,
                                delta,
                                curve3ds );
      curve3dMaps[ rank ] = gkg::Curve3dMap< float >( curve3ds );


      ++ mv;
      ++ mp;


    }


  }
  GKG_CATCH( "void gkg::MeshSlicer::slicing( "
             "const gkg::MeshMap< int32_t, float, 3U >& meshes, "
             "std::map< int32_t, Curve3dMap< float > >& curve3dMaps ) const" );

}


float gkg::MeshSlicer::computePlanes( float lowerZ, float upperZ,
                                      std::vector< float >& planes ) const
{

  try
  {

    float delta = 0.0f;
    std::list< float > planeList;
    if ( _adaptive )
    {

      double random = 0.0;
      float currentZ = lowerZ;
      double roundedCurrentZ = 0.0;

      planeList.push_back( currentZ );

      while ( currentZ <= upperZ )
      {

        random = _minimumThickness +
                 ( _maximumThickness - _minimumThickness ) * 
                 ( rand() / ( double )RAND_MAX );
        roundedCurrentZ = xround( currentZ + random, _epsilon, 2, 1 );
        if ( roundedCurrentZ >= upperZ )
        {

          break;

        }
        planeList.push_back( roundedCurrentZ );
        currentZ += random;

      }


    }
    else
    {

      // computing the plane spacing (even multiple of _epsilon)
      double spacing = xround( _maximumThickness, _epsilon, 2, 0 );

      // computing the first plane (odd multiple of _epsilon)
      double firstPlane = xround( lowerZ - spacing, _epsilon, 2, 1 );

      // computing the plane count
      int32_t planeCount = 1 + ( int32_t )( ( upperZ + spacing - firstPlane ) /
                                            spacing );

      // filling planes uniformly
      int32_t p = 0;
      float currentPlane = 0.0f;
      for ( p = 0; p < planeCount; p++ )
      {

        currentPlane = ( float )( firstPlane + ( double )p * spacing );
        if ( ( currentPlane > lowerZ ) && ( currentPlane < upperZ ) )
        {

          planeList.push_back( currentPlane );

        }

      }

      delta = ( float )spacing;

    }

    planes = std::vector< float >( planeList.begin(), planeList.end() );

    return delta;

  }
  GKG_CATCH( "float gkg::MeshSlicer::computePlanes( "
             "float lowerZ, float upperZ, "
             "std::vector< float >& planes ) const" );

}


void gkg::MeshSlicer::incrementalSlicing( 
                       const std::vector< gkg::Vector3d< float > >& vertices,
                       const std::vector< gkg::Polygon< 3U > >& polygons,
                       const std::vector< float >& minimumZ,
                       const std::vector< float >& maximumZ,
                       const std::vector< float >& planes,
                       float delta,
                       std::list< gkg::LightCurve3d< float > >& curve3ds ) const
{

  try
  {

    int32_t planeCount = ( int32_t )planes.size();

    ////////////////////////////////////////////////////////////////////////////
    // creating a vector of lists, each list of index l corresponding to the set
    // of triangles that have their lower z belonging to the range
    // [planes[l-1];planes[l]], assuming that planes[-1]=-inf and 
    // planes[planeCount]=+inf
    ////////////////////////////////////////////////////////////////////////////

    std::vector< std::list< int32_t > >
      triangleIndexLists( planeCount + 1, std::list< int32_t >() );

    // case of uniform slicing
    if ( delta > 0.0f )
    {

      if ( _verbose )
      {

        std::cout << "case of uniform slicing with delta="
                  << delta << "mm" << std::endl;

      }
      std::vector< gkg::Polygon< 3U > >::const_iterator
        t = polygons.begin(),
        te = polygons.end();
      std::vector< float >::const_iterator
        minz = minimumZ.begin();
      std::vector< float >::const_iterator
        maxz = maximumZ.begin();
      int32_t p = 0;
      int32_t pmin = 0;
      int32_t pmax = 0;
      int32_t triangleIndex = 0;
      while ( t != te )
      {

        if ( *minz < planes[ 0 ] )
        {

          pmin = 0;
          pmax = 0;

        }
        else if ( *minz > planes[ planeCount - 1 ] )
        {

          pmin = planeCount;
          pmax = planeCount;

        }
        else
        {

          pmin = std::floor( ( *minz - planes[ 0 ] ) / delta ) + 1;
          pmax = std::floor( ( *maxz - planes[ 0 ] ) / delta ) + 1;

        }

        for ( p = pmin; p <= pmax; p++ )
        {

          triangleIndexLists[ p ].push_back( triangleIndex );

        }

        ++ triangleIndex;
        ++ t;
        ++ minz;
        ++ maxz;

      }

    }
    // case of adaptive slicing
    else
    {

      if ( _verbose )
      {

        std::cout << "case of adaptive slicing" << std::endl;

      }
      std::vector< float >::const_iterator
        minz = minimumZ.begin(),
        minze = minimumZ.end();
      int32_t p = 0;
      int32_t triangleIndex = 0;
      while ( minz != minze )
      {

        p = binarySearch( planes, *minz );

        triangleIndexLists[ p ].push_back( triangleIndex );

        ++ triangleIndex;
        ++ minz;

      }

    }

    if ( _verbose )
    {

      std::cout << "number of triangles for each plane : " << std::flush;
      std::vector< std::list< int32_t > >::const_iterator
        l = triangleIndexLists.begin(),
        le = triangleIndexLists.end();
      -- le;
      int32_t p = 0;
      while ( l != le )
      {

        std::cout << planes[ p ] << "mm->" << l->size() << "triangles, ";
        ++ p;
        ++ l;

      }
      std::cout << planes[ p ] << "mm->" << l->size() << "triangles"
                << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // performing a plane sweep from bottom to top
    ////////////////////////////////////////////////////////////////////////////

    int32_t p = 0;
    float currentPlane = 0.0f;
    for ( p = 0; p < planeCount; p++ )
    {

      // allocating segments
      std::list< gkg::MeshSlicer::Segment > segments;

      // adding triangles in the range [planes[p-1];planes[p]]
      std::list< int32_t > activeTriangleIndexList = triangleIndexLists[ p ];

      currentPlane = planes[ p ];

      if ( _verbose )
      {

        std::cout << "sweeping plane " << p << "=" << currentPlane << "mm"
                  << std::endl;

      }

      //std::list< gkg::Polygon< 3U > > keptPolygons;

      std::list< int32_t >::iterator
        ti = activeTriangleIndexList.begin(),
        tie = activeTriangleIndexList.end();
      while ( ti != tie )
      {

        const float& minZ = minimumZ[ *ti ];
        const float& maxZ = maximumZ[ *ti ];

        // if triangle is not exhausted
        if ( maxZ >= currentPlane )
        {

          if ( ( minZ < currentPlane ) &&
               ( maxZ > currentPlane ) )
          {

            const gkg::Polygon< 3U >& triangle = polygons[ *ti ];

            gkg::MeshSlicer::Segment segment;
            this->getTriangleToPlaneIntersection( vertices,
                                                  triangle,
                                                  currentPlane,
                                                  segment );
            segments.push_back( segment );

            //keptPolygons.push_back( triangle );

          }

        }
        ++ ti;

      }

      // gkg::MeshMap< int32_t, float, 3U > meshMap;
      // meshMap.vertices.addSites( 0, vertices );
      // meshMap.polygons.addPolygons( 0, keptPolygons );
      // gkg::Writer::getInstance().write(
      //           gkg::StringConverter::toString( p ) + ".mesh",
      //           meshMap,
      //           false,
      //           "aimsmesh" );

      if ( !segments.empty() )
      {

        // building the set of contours for a given plane
        std::list< gkg::MeshSlicer::Contour > contours;
        this->constructContour( segments, contours );

        // if demanded, rearrange point to follow clockwise orientation
        if ( _clockwise )
        {

          this->rayCasting( contours );

        }

        // the exporting the set of contours as 3D curves
        std::list< gkg::MeshSlicer::Contour >::const_iterator
          c = contours.begin(),
          ce = contours.end();
        while ( c != ce )
        {
        
          curve3ds.push_back( gkg::LightCurve3d< float >( c->P, false ) );
          ++ c;

        }

      }

    }

  }
  GKG_CATCH( "void gkg::MeshSlicer::incrementalSlicing( "
             "const std::vector< gkg::Vector3d< float > >& vertices, "
             "const std::vector< gkg::Polygon< 3U > >& polygons, "
             "const std::vector< float >& minimumZ, "
             "const std::vector< float >& maximumZ, "
             "const std::vector< float >& planes, "
             "float delta, "
             "std::list< gkg::LightCurve3d< float > >& curve3ds ) const" );

}


void gkg::MeshSlicer::getTriangleToPlaneIntersection(
                          const std::vector< gkg::Vector3d< float > >& vertices,
                          const gkg::Polygon< 3U >& triangle,
                          float currentPlane,
                          gkg::MeshSlicer::Segment& segment ) const
{

  try
  {


    int32_t extremityCount = 0;
    int32_t i = 0;
    int32_t j = 0;
    float minimumZ = 0.0f;
    float maximumZ = 0.0f;
    for ( i = 0; i < 3; i++ )
    {

      // getting side i of triangle
      j = ( ( i == 2 ) ? 0 : i + 1 );
      const Vector3d< float >& vi = vertices[ triangle.indices[ i ] ];
      const Vector3d< float >& vj = vertices[ triangle.indices[ j ] ];

      // checking for intersection of plane with {vi--vj}
      // must consider segment closed at bottom and open at top in case z goes 
      // through a vertex
      minimumZ = ( ( vi.z < vj.z ) ? vi.z : vj.z );
      maximumZ = ( ( vi.z > vj.z ) ? vi.z : vj.z );
      if ( ( minimumZ <= currentPlane ) &&
           ( maximumZ > currentPlane ) )
      {

        this->getExtremity( vi, vj, currentPlane,
                            segment.extremities[ extremityCount ] );
        ++ extremityCount;

      }

    }

  }
  GKG_CATCH( "void gkg::MeshSlicer::getTriangleToPlaneIntersection( "
             "const std::vector< gkg::Vector3d< float > >& vertices, "
             "const gkg::Polygon< 3U >& triangle, "
             "float currentPlane, "
             "gkg::MeshSlicer::Segment& segment ) const" );

}


void gkg::MeshSlicer::getExtremity( const gkg::Vector3d< float >& vertex1,
                                    const gkg::Vector3d< float >& vertex2,
                                    float currentPlane,
                                    gkg::Vector3d< float >& extremity ) const
{

  try
  {

    float deltaX = vertex1.x - vertex2.x;
    float deltaY = vertex1.y - vertex2.y;
    float deltaZ = vertex1.z - vertex2.z;

    float ratio = ( currentPlane - vertex1.z ) / deltaZ;

    extremity.x = ratio * deltaX + vertex1.x;
    extremity.y = ratio * deltaY + vertex1.y;
    extremity.z = currentPlane;

  }
  GKG_CATCH( "void gkg::MeshSlicer::getExtremity( "
             "const gkg::Vector3d< float >& vertex1, "
             "const gkg::Vector3d< float >& vertex2, "
             "float currentPlane, "
             "gkg::Vector3d< float >& extremity ) const" );

}


void gkg::MeshSlicer::constructContour( 
                         const std::list< gkg::MeshSlicer::Segment >& segments,
                         std::list< gkg::MeshSlicer::Contour >& contours ) const
{

  try
  {

    // creating the hash table
    std::vector< gkg::MeshSlicer::UnorderedMap > H( 1 );

   // rounding vertices and filling the hash table
   double eps = 1/128.0;
    std::list< gkg::MeshSlicer::Segment >::const_iterator
      s = segments.begin(),
      se = segments.end();
    while ( s != se )
    {

      gkg::MeshSlicer::Segment segment = *s;

      gkg::Vector3d< float > extremity1 = segment.extremities[ 0 ];
      extremity1.x = std::round( extremity1.x / eps ) * eps;
      extremity1.y = std::round( extremity1.y / eps ) * eps;
      extremity1.z = std::round( extremity1.z / eps ) * eps;

      gkg::Vector3d< float > extremity2 = segment.extremities[ 1 ];
      extremity2.x = std::round( extremity2.x / eps ) * eps;
      extremity2.y = std::round( extremity2.y / eps ) * eps;
      extremity2.z = std::round( extremity2.z / eps ) * eps;

      if ( ( extremity2 - extremity1 ).getNorm() > 0.0001f )
      {

        H[ 0 ][ extremity1 ].push_back( extremity2 );
        H[ 0 ][ extremity2 ].push_back( extremity1 );

      }

      ++ s;

    }

    // constructing contour
    bool maximal = true;
    while ( !H[0].empty() )
    {

      std::vector< gkg::Vector3d< float > > P = this->startLoop( H );
      this->extendLoop( P, H );
      if ( maximal )
      {

        // if chain P is open
        if ( P.front() != P.back() )
        {

          std::reverse( P.begin(), P.end() );
          this->extendLoop( P, H );

        }

      }
      contours.push_back( { false, false, P } );

    }

  }
  GKG_CATCH( "void gkg::MeshSlicer::constructContour( "
             "const std::list< gkg::MeshSlicer::Segment >& segments, "
             "std::list< gkg::MeshSlicer::Contour >& contours ) const" );

}


std::vector< gkg::Vector3d< float > >
gkg::MeshSlicer::startLoop( 
                         std::vector< gkg::MeshSlicer::UnorderedMap >& H ) const
{

  try
  {

    std::vector< gkg::Vector3d< float > > P;
    gkg::MeshSlicer::UnorderedMap::const_iterator 
      i = H[ 0 ].begin();
    gkg::Vector3d< float > u = i->first;
    std::vector< gkg::Vector3d< float > > vw = i->second;
    gkg::Vector3d< float > v = vw.at( 0 );
    P.push_back( u );
    P.push_back( v );
    H[ 0 ][ u ].erase( std::remove( H[ 0 ][ u ].begin(),
                                    H[ 0 ][ u ].end(), v ), 
                       H[ 0 ][ u ].end() );
    if ( H[ 0 ][ u ].size() == 0 )
    {

      H[ 0 ].erase( u );

    }
    H[ 0 ][ v].erase( std::remove( H[ 0 ][ v ].begin(),
                                   H[ 0 ][ v ].end(), u ),
                      H[ 0 ][ v ].end() );
    if ( H[ 0 ][ v ].size() == 0 )
    {

      H[ 0 ].erase( v );

    }
    return P;

  }
  GKG_CATCH( "std::vector< gkg::Vector3d< float > > "
             "gkg::MeshSlicer::startLoop( "
             "std::vector< gkg::MeshSlicer::UnorderedMap >& H ) const" );

}


void gkg::MeshSlicer::extendLoop(
                         std::vector< gkg::Vector3d< float > >& P,
                         std::vector< gkg::MeshSlicer::UnorderedMap >& H ) const
{

  try
  {

    gkg::Vector3d< float > first = P.front();  
    gkg::Vector3d< float > current = P.back(); 
    gkg::Vector3d< float > last;
         
    // collecting other vertices
    while ( true )
    {

      gkg::MeshSlicer::UnorderedMap::const_iterator 
        i = H[ 0 ].find( current );

      // if current vertex is a dead end
      if ( i == H[ 0 ].end() )
      {

        break;

      }
            
      // getting next vertex, the first unused neighbor of the current vertex

      // unused neighbors of current vertex
      std::vector< gkg::Vector3d< float > > vw = i->second;

      // first unused neighbor of current vertex
      gkg::Vector3d< float > next = vw.at( 0 );

      // appending the segment (current,next) to P and delete from H
      P.push_back( next );

      // removing the segment (current,next) from H
      H[ 0 ][ current ].erase( std::remove( H[ 0 ][ current ].begin(),
                                            H[ 0 ][ current ].end(), next ),
                               H[ 0 ][ current ].end() );
      if ( H[ 0 ][ current ].size() == 0 )
      {

        H[ 0 ].erase( current );

      }
      H[ 0 ][ next ].erase( std::remove( H[ 0 ][ next ].begin(),
                                         H[ 0 ][ next ].end(), current ),
                            H[ 0 ][ next ].end() );
      if ( H[ 0 ][ next ].size() == 0 )
      {

        H[ 0 ].erase( next );

      } 

      // closing a loop
      if ( next == first )
      {

        break;

      }

      // moving on
      current = next;

    }

  }
  GKG_CATCH( "void gkg::MeshSlicer::extendLoop( "
             "std::vector< gkg::Vector3d< float > >& P, "
             "std::vector< gkg::MeshSlicer::UnorderedMap >& H ) const" );

}


void gkg::MeshSlicer::rayCasting(
                         std::list< gkg::MeshSlicer::Contour >& contours ) const
{

  try
  {

    std::vector< gkg::MeshSlicer::Segment > segments;
  
    gkg::BoundingBox< float > boundingBox;

    // creating the line segments of each contour
    double area = 0.0;
    int32_t i = 0;
    int32_t j = 0;
    std::list< gkg::MeshSlicer::Contour >::iterator
      c = contours.begin(),
      ce = contours.end();
    while ( c != ce )
    {

      area = 0.0;

      std::vector< gkg::Vector3d< float > > P = c->P;

      for ( j = 1; j < ( int32_t )P.size(); j++ )
      {

        gkg::Vector3d< float > p0 = P[ j - 1 ];
        gkg::Vector3d< float > p1 = P[ j ];
        area += ( p0.x * p1.y - p0.y * p1.x );
        this->addPoint( p0, p1, segments, boundingBox,
                        ( j == 1 ? true : false ), i );
        if ( j == ( int32_t )P.size() - 1 )
        {

          this->addPoint( p1, P[ 0 ], segments, boundingBox,
                          ( j == 1 ? true : false ), i );
          area += ( p1.x * P[ 0 ].y - p1.y * P[ 0 ].x );

        }

      }
      c->clockwise = ( area < 0.0 ? true : false );

      ++ i;
      ++ c;

    }
  
    // using the point in polygon algorithm to test the first segment of each
    // contour
    i = 0;
    c = contours.begin();
    while ( c != ce )
    {

      std::vector< gkg::Vector3d< float > > P = c->P;

      // internal contour = false, external contour = true
      c->external = ( this->contains( P[ 0 ], boundingBox, segments, i ) ?
                      false : true );

      // reversing contours
      if ( c->external && c->clockwise )
      {

        std::reverse( c->P.begin(), c->P.end() );
        c->clockwise = false;

      }
      else if ( !c->external && !c->clockwise )
      {

        std::reverse( c->P.begin(), c->P.end() );
        c->clockwise = true;

      }

      ++ i;
      ++ c;

    }

  }
  GKG_CATCH( "void gkg::MeshSlicer::rayCasting( "
             "std::list< gkg::MeshSlicer::Contour >& contours ) const" );

}
 

void gkg::MeshSlicer::addPoint(
                               const gkg::Vector3d< float >& p1,
                               const gkg::Vector3d< float >& p2,
                               std::vector< gkg::MeshSlicer::Segment>& segments,
                               gkg::BoundingBox< float >& boundingBox,
                               bool first,
                               int32_t index ) const
{

  try
  {

    if ( first )
    {

      if ( p1.x < boundingBox.getLowerX() )
      {

        boundingBox.setLowerX( p1.x );

      }
      else if ( p1.x > boundingBox.getUpperX() )
      {

        boundingBox.setUpperX( p1.x );

      }
      if ( p1.y < boundingBox.getLowerY() )
      {

        boundingBox.setLowerY( p1.y );

      }
      else if ( p1.y > boundingBox.getUpperY() )
      {

        boundingBox.setUpperY( p1.y );

      }

    }
    if ( p2.x < boundingBox.getLowerX() )
    {

      boundingBox.setLowerX( p2.x );

    }
    else if ( p2.x > boundingBox.getUpperX() )
    {

      boundingBox.setUpperX( p2.x );

    }
    if ( p2.y < boundingBox.getLowerY() )
    {

      boundingBox.setLowerY( p2.y );

    }
    else if ( p2.y > boundingBox.getUpperY() )
    {

      boundingBox.setUpperY( p2.y );

    }
    gkg::MeshSlicer::Segment segment( p1, p2, index );
    segments.push_back( segment );

  }
  GKG_CATCH( "void gkg::MeshSlicer::addPoint( "
             "const gkg::Vector3d< float >& p1, "
             "const gkg::Vector3d< float >& p2, "
             "std::vector< gkg::MeshSlicer::Segment>& segments, "
             "bounding_box* boundingBox, "
             "bool first, "
             "int32_t index ) const" );

}


bool gkg::MeshSlicer::contains(
                        const gkg::Vector3d< float >& point,
                        const gkg::BoundingBox< float >& boundingBox,
                        const std::vector< gkg::MeshSlicer::Segment >& segments,
                        int32_t index ) const
{

  try
  {

    if ( boundingBox.contains( point.x, point.y ) )
    {

      double epsilon = boundingBox.getSpanX() / 100.0;
      gkg::Vector3d< float > outside ( boundingBox.getLowerX() - epsilon,
                                       boundingBox.getLowerY(),
                                       0.0f );
      gkg::MeshSlicer::Segment segment( outside, point, index );

      int32_t intersection = 0;
      int32_t i = 0;
      for ( i = 0; i < ( int32_t )segments.size(); i++ )
      {

        if ( ( segments[ i ].index != index ) &&
             this->hasSegmentIntersection( segment, segments[ i ] ) )
        {

          ++ intersection;

        }

      }
      // if the number of intersections is odd, then the point is inside the
      // polygon
      if ( ( intersection % 2 ) == 1 )
      {

        return true;

      }

    }

    return false;

  }
  GKG_CATCH( "bool gkg::MeshSlicer::contains( "
             "const gkg::Vector3d< float >& point, "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const std::vector< gkg::MeshSlicer::Segment >& segments, "
             "int32_t index ) const" );

}


bool gkg::MeshSlicer::hasSegmentIntersection(
                                const gkg::MeshSlicer::Segment& segment1,
                                const gkg::MeshSlicer::Segment& segment2 ) const
{

  try
  {

    gkg::Vector3d< float > intersectPoint;
    // if both vectors aren't from the kind of x=1 lines then go into
    if ( !segment1.isVertical && !segment2.isVertical)
    {

      // checking if both vectors are parallel. If they are parallel then no 
      // intersection point will exist
      if ( segment1.a - segment2.a == 0 )
      {

        return false;

      }
      intersectPoint.x = ( ( segment2.b - segment1.b ) /
                           ( segment1.a - segment2.a ) );
      intersectPoint.y = segment2.a * intersectPoint.x + segment2.b;
 
    }
    else if ( segment1.isVertical && !segment2.isVertical )
    {

      intersectPoint.x = segment1.extremities[ 0 ].x;
      intersectPoint.y = segment2.a * intersectPoint.x + segment2.b;

    }
    else if ( !segment1.isVertical && segment2.isVertical )
    {

      intersectPoint.x = segment2.extremities[ 0 ].x;
      intersectPoint.y = segment1.a * intersectPoint.x + segment1.b;

    }
    else
    {

      return false;

    }
    if ( this->isInside( segment2, intersectPoint ) &&
         this->isInside( segment1, intersectPoint ) )
    {

      return true;

    }
    return false;

  }
  GKG_CATCH( "bool gkg::MeshSlicer::hasSegmentIntersection( "
             "const gkg::MeshSlicer::Segment& segment1, "
             "const gkg::MeshSlicer::Segment& segment2 ) const" );

}


bool gkg::MeshSlicer::isInside( const gkg::MeshSlicer::Segment& segment,
                                const gkg::Vector3d< float >& point ) const
{

  try
  {

    const gkg::Vector3d< float >& extremity1 = segment.extremities[ 0 ];
    const gkg::Vector3d< float >& extremity2 = segment.extremities[ 1 ];

    float maxX = ( extremity1.x > extremity2.x) ? extremity1.x : extremity2.x;
    float minX = ( extremity1.x < extremity2.x) ? extremity1.x : extremity2.x;
    float maxY = ( extremity1.y > extremity2.y) ? extremity1.y : extremity2.y;
    float minY = ( extremity1.y < extremity2.y) ? extremity1.y : extremity2.y;

    if ( ( point.x >= minX ) &&
         ( point.x <= maxX ) &&
         ( point.y >= minY ) &&
         ( point.y <= maxY ) )
    {

      return true;

    }
    return false;

  }
  GKG_CATCH( "bool gkg::MeshSlicer::isInside( "
             "const gkg::MeshSlicer::Segment& segment, "
             "const gkg::Vector3d< float >& point ) const" );

}
