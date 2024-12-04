#include <gkg-anatomist-plugin-io/NormalsProcessingContext.h>
#include <gkg-processing-mesh/MeshScaler_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>
#include <QtOpenGL>


gkg::NormalsProcessingContext::NormalsProcessingContext(
      const gkg::BoundingBox< float >& boundingBox,
      int32_t outputOrientationCount,
      const gkg::MeshMap< int32_t, float, 3U >& sphereMeshMap,
      const gkg::Vector3d< double >& resolution,
      const std::vector< std::vector< float > >& transformations,
      const std::vector< gkg::Translation3d< float > >& translations,
      std::vector < std::vector < std::vector < QVector3D* > > >& glNormals )
    : gkg::LoopContext< int32_t >(),
      _boundingBox( boundingBox ),
      _outputOrientationCount( outputOrientationCount ),
      _sphereMeshMap( sphereMeshMap ),
      _resolution( resolution ),
      _transformations( transformations ),
      _translations( translations ),
      _glNormals( glNormals )
{

  try
  {

    int32_t sizeX = ( int32_t )( ( _boundingBox.getUpperX() -
                                   _boundingBox.getLowerX() ) /
                                 _resolution.x + 0.5 ) + 1;
    int32_t sizeY = ( int32_t )( ( _boundingBox.getUpperY() -
                                   _boundingBox.getLowerY() ) /
                                 _resolution.y + 0.5 ) + 1;
    int32_t sizeZ = ( int32_t )( ( _boundingBox.getUpperZ() -
                                   _boundingBox.getLowerZ() ) /
                                 _resolution.z + 0.5 ) + 1;

    _glNormals.resize( sizeX );

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    for ( x = 0; x < sizeX; ++x )
    {

      _glNormals[ x ].resize( sizeY );
      for ( y = 0; y < sizeY; ++y )
      {

        _glNormals[ x ][ y ].resize( sizeZ );
        for ( z = 0; z < sizeZ; ++z )
        {

          _glNormals[ x ][ y ][ z ] = 0;

        }

      }

    }

  }
  GKG_CATCH(
     "gkg::NormalsProcessingContext::NormalsProcessingContext( "
     "const gkg::BoundingBox< float >& boundingBox, "
     "int32_t outputOrientationCount, "
     "const gkg::Vector3d< double >& resolution, "
     "const std::vector< std::vector< float > >& transformations, "
     "const std::vector< gkg::Translation3d< float > >& translations, "
     "std::vector < std::vector < std::vector < QVector3D* > > >& "
     "glNormals )" );

}


gkg::NormalsProcessingContext::~NormalsProcessingContext()
{
}


void gkg::NormalsProcessingContext::doIt( int32_t startIndex, 
                                         int32_t indexCount )
{

  try
  {

    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    Vector3d< int32_t > voxel;
    int32_t i = 0;
    int32_t j = 0;
    gkg::MeshScaler< int32_t, float, 3U > meshScaler;

    for ( i = startIndex; i < ( startIndex + indexCount ); i++ )
    {

      _translations[ i ].getDirectTranslation( x, y, z );
      voxel.x = ( int32_t )( ( x - _boundingBox.getLowerX() ) /
                             _resolution.x + 0.5 );
      voxel.y = ( int32_t )( ( y - _boundingBox.getLowerY() ) /
                             _resolution.y + 0.5 );
      voxel.z = ( int32_t )( ( z - _boundingBox.getLowerZ() ) /
                             _resolution.z + 0.5 );

      QVector3D* normal = new QVector3D[ _outputOrientationCount ];
      gkg::MeshMap< int32_t, float, 3U > localMeshMap( _sphereMeshMap );
      meshScaler.scale( localMeshMap, 0, _transformations[ i ], localMeshMap );

      std::list< gkg::Vector3d< float > >
        normals = localMeshMap.getNormals( 0 );

      std::list< gkg::Vector3d< float > >::const_iterator 
        n = normals.begin(),
        ne = normals.end();
      j = 0;

      while ( n != ne )
      {

        normal[ j ].setX( ( GLfloat )n->x );
        normal[ j ].setY( ( GLfloat )n->y );
        normal[ j ].setZ( ( GLfloat )n->z );
        ++ n;
        ++ j;

      }

      _glNormals[ voxel.x ]
                [ voxel.y ]
                [ voxel.z ] = normal;

    }

  }
  GKG_CATCH( "void NormalsProcessingContext::doIt("
             "int32_t startIndex,"
             "int32_t indexCount )" );
}
