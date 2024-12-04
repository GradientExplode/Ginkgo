#include <gkg-processing-mesh/PolygonCache.h>
#include <gkg-processing-mesh/EvolvingMesh.h>
#include <gkg-processing-mesh/SceneModeler.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/Polygon.h>
#include <gkg-core-exception/Exception.h>
#include <list>


gkg::PolygonCache::PolygonCache(
                               gkg::RCPointer< gkg::EvolvingMesh > evolvingMesh,
                               const gkg::Vector3d< int32_t >& size,
                               float radiusOfInfluence )
                  : _evolvingMesh( evolvingMesh ),
                    _lut( size.x, size.y, size.z )
{

  try
  {

    // computing and storing the voxel resolution(s);
    float spanX = 
      _evolvingMesh->getSceneModeler()->getBoundingBox().getUpperX() -
      _evolvingMesh->getSceneModeler()->getBoundingBox().getLowerX();
    float spanY =
      _evolvingMesh->getSceneModeler()->getBoundingBox().getUpperY() -
      _evolvingMesh->getSceneModeler()->getBoundingBox().getLowerY();
    float spanZ = 
      _evolvingMesh->getSceneModeler()->getBoundingBox().getUpperZ() -
      _evolvingMesh->getSceneModeler()->getBoundingBox().getLowerZ();

    _resolution.x = spanX / size.x;
    _resolution.y = spanY / size.y;
    _resolution.z = spanZ / size.z;

    _lut.getHeader().addAttribute( "resolutionX", ( double )_resolution.x );
    _lut.getHeader().addAttribute( "resolutionY", ( double )_resolution.y );
    _lut.getHeader().addAttribute( "resolutionZ", ( double )_resolution.z );

    _lut.fill( 0 );

    // collecting the polygons and associated vertices of the evolvingMesh mesh
    const std::list< gkg::Polygon< 3U > >&
      polygonList = evolvingMesh->getMesh()->polygons.getPolygons( 0 );
    gkg::RankSiteLut< int32_t, float >
      rankSiteLut( evolvingMesh->getMesh()->vertices );

    std::set< gkg::Vector3d< int32_t >,
              gkg::Vector3dCompare< int32_t > > voxelSet;

    std::list< gkg::Polygon< 3U > >::const_iterator
      p = polygonList.begin(),
      pe = polygonList.end();
    while ( p != pe )
    {

      // using rank/site lut to get the vertices of the given polygon
      const gkg::Vector3d< float >& 
        vertex1 = rankSiteLut.getRankSite( p->indices[ 0 ] ).site;
      const gkg::Vector3d< float >& 
        vertex2 = rankSiteLut.getRankSite( p->indices[ 1 ] ).site;
      const gkg::Vector3d< float >& 
        vertex3 = rankSiteLut.getRankSite( p->indices[ 2 ] ).site;

      // collecting a list of associated voxels using Bresenham algorithm
      evolvingMesh->getSceneModeler()->getBresenhamAlgorithm().
                     getThickTriangleVoxels( vertex1,
                                             vertex2,
                                             vertex3,
                                             radiusOfInfluence,
                                             voxelSet,
                                             true );

      // looping over each voxel
      std::set< gkg::Vector3d< int32_t >,
                gkg::Vector3dCompare< int32_t > >::const_iterator
        v = voxelSet.begin(),
        ve = voxelSet.end();
      while ( v != ve )
      {

        std::set< const gkg::Polygon< 3U >* >*
          polygonSet = getPolygons( *v );
        if ( polygonSet == 0 )
        {

          // initializing the polygon set for the first time
          std::set< const gkg::Polygon< 3U >* >*
            newPolygonSet = new std::set< const gkg::Polygon< 3U >* >();
          newPolygonSet->insert( &( *p ) );
          _lut( *v ) = newPolygonSet;

        }
        else
        {

          polygonSet->insert( &( *p ) );

        }
        ++ v;

      }
      ++ p;

    }

  }
  GKG_CATCH( "gkg::PolygonCache::PolygonCache( "
             "gkg::RCPointer< gkg::EvolvingMesh > evolvingMesh, "
             "const gkg::Vector3d< int32_t >& size, "
             "float radiusOfInfluence )" );

}


gkg::PolygonCache::~PolygonCache()
{
}


std::set< const gkg::Polygon< 3U >* >* 
gkg::PolygonCache::getPolygons( const gkg::Vector3d< int32_t >& voxel ) const
{

  try
  {

    return _lut( voxel );

  }
  GKG_CATCH( "std::set< const gkg::Polygon< 3U >* >* "
             "gkg::PolygonCache::getPolygons( "
             "const gkg::Vector3d< int32_t >& voxel ) const" );

}


std::set< const gkg::Polygon< 3U >* >*
gkg::PolygonCache::getPolygons( const gkg::Vector3d< float >& point ) const
{

  try
  {

    gkg::Vector3d< int32_t > voxel;
    _evolvingMesh->getSceneModeler()->getCacheVoxel( point, voxel );

    return getPolygons( voxel );

  }
  GKG_CATCH( "std::set< const gkg::Polygon< 3U >* >* "
             "gkg::PolygonCache::getPolygons( "
             "const gkg::Vector3d< float >& point ) const" );

}


void gkg::PolygonCache::getBinaryMask( gkg::Volume< uint8_t >& mask ) const
{

  try
  {

    mask.reallocate( _lut.getSizeX(), _lut.getSizeY(), _lut.getSizeZ() );

    mask.getHeader().addAttribute( "resolutionX", ( double )_resolution.x );
    mask.getHeader().addAttribute( "resolutionY", ( double )_resolution.y );
    mask.getHeader().addAttribute( "resolutionZ", ( double )_resolution.z );

    gkg::Volume< uint8_t >::iterator
      m = mask.begin(),
      me = mask.end();
    gkg::Volume< std::set< const gkg::Polygon< 3U >* >* >::const_iterator
      l = _lut.begin();

    while ( m != me )
    {

      *m = ( *l ? 1 : 0 );
      ++ m;
      ++ l;

    }

  }
  GKG_CATCH( "void gkg::PolygonCache::getBinaryMask( "
             "gkg::Volume< uint8_t >& mask ) const" );
  
}

