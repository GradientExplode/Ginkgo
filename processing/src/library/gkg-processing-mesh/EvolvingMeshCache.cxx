#include <gkg-processing-mesh/EvolvingMeshCache.h>
#include <gkg-processing-mesh/SceneModeler.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


gkg::EvolvingMeshCache::EvolvingMeshCache( 
                               gkg::RCPointer< gkg::SceneModeler > sceneModeler,
                               const gkg::Vector3d< int32_t >& size )
                   : _sceneModeler( sceneModeler ),
                     _lut( size.x, size.y, size.z )
{

  try
  {

    // computing and storing the voxel resolution(s)
    float spanX = _sceneModeler->getBoundingBox().getUpperX() -
                  _sceneModeler->getBoundingBox().getLowerX();
    float spanY = _sceneModeler->getBoundingBox().getUpperY() -
                  _sceneModeler->getBoundingBox().getLowerY();
    float spanZ = _sceneModeler->getBoundingBox().getUpperZ() -
                  _sceneModeler->getBoundingBox().getLowerZ();

    _resolution.x = ( double )( spanX / size.x );
    _resolution.y = ( double )( spanY / size.y );
    _resolution.z = ( double )( spanZ / size.z );

    _lut.setResolution( _resolution );
    _lut.fill( 0 );

  }
  GKG_CATCH( "gkg::EvolvingMeshCache::EvolvingMeshCache( "
             "gkg::RCPointer< gkg::SceneModeler > sceneModeler, "
             "const gkg::Vector3d< int32_t >& size )" );

}


gkg::EvolvingMeshCache::~EvolvingMeshCache()
{
}


std::set< gkg::RCPointer< gkg::EvolvingMesh > >*
gkg::EvolvingMeshCache::getEvolvingMeshes( 
                                   const gkg::Vector3d< int32_t >& voxel ) const
{

  try
  {

    return _lut( voxel );

  }
  GKG_CATCH( "std::set< gkg::RCPointer< gkg::EvolvingMesh > >* "
             "gkg::EvolvingMeshCache::getEvolvingMeshs( "
             "const gkg::Vector3d< int32_t >& voxel ) const" );

}


std::set< gkg::RCPointer< gkg::EvolvingMesh > >*
gkg::EvolvingMeshCache::getEvolvingMeshes( int32_t x,
                                           int32_t y,
                                           int32_t z ) const
{

  try
  {

    return _lut( x, y, z );

  }
  GKG_CATCH( "std::set< gkg::RCPointer< gkg::EvolvingMesh > >* "
             "gkg::EvolvingMeshCache::getEvolvingMeshes( "
             "const gkg::Vector3d< int32_t >& voxel ) const" );

}


std::set< gkg::RCPointer< gkg::EvolvingMesh > >*
gkg::EvolvingMeshCache::getEvolvingMeshes( 
                                     const gkg::Vector3d< float >& point ) const
{

  try
  {

    gkg::Vector3d< int32_t > voxel;
    _sceneModeler->getCacheVoxel( point, voxel );

    return getEvolvingMeshes( voxel );

  }
  GKG_CATCH( "std::set< gkg::RCPointer< gkg::EvolvingMesh > >* "
             "gkg::EvolvingMeshCache::getEvolvingMeshes( "
             "const gkg::Vector3d< float >& point ) const" );

}


void gkg::EvolvingMeshCache::update(
                              gkg::RCPointer< gkg::EvolvingMesh > evolvingMesh )
{

  try
  {

    // looping over each polygon (triangle) and use Bresenham algorithm to
    // collect the voxels contained in the polygon (triangle) with the
    // consideration of the radius of influence

    // collecting the polygons and associated vertices of the mesh
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

      // using rank/site lut to get the vertices of a given polygon
      const gkg::Vector3d< float >& 
        vertex1 = rankSiteLut.getRankSite( ( p->indices )[ 0 ] ).site;
      const gkg::Vector3d< float >& 
        vertex2 = rankSiteLut.getRankSite( ( p->indices )[ 1 ] ).site;
      const gkg::Vector3d< float >& 
        vertex3 = rankSiteLut.getRankSite( ( p->indices )[ 2 ] ).site;

      // collecting a list of associated voxels using Bresenham algorithm
      _sceneModeler->getBresenhamAlgorithm().
                 getThickTriangleVoxels( vertex1,
                                         vertex2,
                                         vertex3,
                                         evolvingMesh->getRadiusOfInfluence(),
                                         voxelSet,
                                         true );

      // looping over each voxel
      std::set< gkg::Vector3d< int32_t >,
                gkg::Vector3dCompare< int32_t > >::const_iterator
        v = voxelSet.begin(),
        ve = voxelSet.end();
      while ( v != ve )
      {

        std::set< gkg::RCPointer< gkg::EvolvingMesh > >*
          evolvingMeshSet = _lut( *v );
        if ( evolvingMeshSet == 0 )
        {

          // no evolvingMesh were already stored, so need to create the 
          // set and add the current evolvingMesh to that set
          std::set< gkg::RCPointer< gkg::EvolvingMesh > >* newEvolvingMeshSet =
            new std::set< gkg::RCPointer< gkg::EvolvingMesh > >();
          newEvolvingMeshSet->insert( evolvingMesh );
          _lut( *v ) = newEvolvingMeshSet;

        }
        else
        {

          // checking if the current evolvingMesh is already present
          // and in that case, do nothing
          bool isStillNotPresent = true;
          std::set< gkg::RCPointer< gkg::EvolvingMesh > >::const_iterator
            m = evolvingMeshSet->begin(),
            me = evolvingMeshSet->end();
          while ( m != me )
          {

            if ( ( *m ) == evolvingMesh )
            {

              isStillNotPresent = false;

            }
            ++ m;

          }

          if ( isStillNotPresent )
          {

            evolvingMeshSet->insert( evolvingMesh );

          }

        }
        ++ v;

      }
      ++ p;

    }
    
  }
  GKG_CATCH( "void gkg::EvolvingMeshCache::update( "
             "gkg::RCPointer< gkg::EvolvingMesh > evolvingMesh )" );

}


void gkg::EvolvingMeshCache::getBinaryMask( gkg::Volume< uint8_t >& mask ) const
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
    gkg::Volume< std::set< gkg::RCPointer< 
                                        gkg::EvolvingMesh > >* >::const_iterator
      l = _lut.begin();

    while ( m != me )
    {

      *m = ( *l ? 1 : 0 );
      ++ m;
      ++ l;

    }

  }
  GKG_CATCH( "void gkg::EvolvingMeshCache::getBinaryMask( "
             "gkg::Volume< uint8_t >& mask ) const" );
  
}
