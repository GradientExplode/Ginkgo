#include <gkg-anatomist-plugin-io/TransformationsAndTranslationsAndNormalsProcessingContext.h>
#include <gkg-processing-mesh/MeshScaler_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>



gkg::TransformationsAndTranslationsAndNormalsProcessingContext::
                       TransformationsAndTranslationsAndNormalsProcessingContext(
      const gkg::BoundingBox< float >& boundingBox,
      int32_t outputOrientationCount,
      const gkg::MeshMap< int32_t, float, 3U >& sphereMeshMap,
      const gkg::Vector3d< double >& resolution,
      const std::vector< std::vector< float > >& transformations,
      const std::vector< gkg::Translation3d< float > >& translations,
      std::vector< std::vector< std::vector< GLfloat*  > > >& glTransformations,
      std::vector< std::vector< std::vector< QVector4D  > > >& glTranslations,
      std::vector < std::vector < std::vector < QVector3D* > > >& glNormals )
    : gkg::LoopContext< int32_t >(),
      _boundingBox( boundingBox ),
      _outputOrientationCount( outputOrientationCount ),
      _sphereMeshMap( sphereMeshMap ),
      _resolution( resolution ),
      _transformations( transformations ),
      _translations( translations ),
      _glTransformations( glTransformations ),
      _glTranslations( glTranslations ),
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

    _glTransformations.resize( sizeX );
    _glTranslations.resize( sizeX );
    _glNormals.resize( sizeX );

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    for ( x = 0; x < sizeX; ++x )
    {

      _glTransformations[ x ].resize( sizeY );
      _glTranslations[ x ].resize( sizeY );
      _glNormals[ x ].resize( sizeY );
      for ( y = 0; y < sizeY; ++y )
      {

        _glTransformations[ x ][ y ].resize( sizeZ );
        _glTranslations[ x ][ y ].resize( sizeZ );
        _glNormals[ x ][ y ].resize( sizeZ );
        for ( z = 0; z < sizeZ; ++z )
        {

          _glTransformations[ x ][ y ][ z ] = 0;
          _glTranslations[ x ][ y ][ z ] = QVector4D( 0.0, 0.0, 0.0, 0.0 );
          _glNormals[ x ][ y ][ z ] = 0;

        }

      }

    }

  }
  GKG_CATCH(
     "gkg::TransformationsAndTranslationsAndNormalsProcessingContext::"
     "TransformationsAndTranslationsAndNormalsProcessingContext( "
     "const gkg::BoundingBox< float >& boundingBox, "
     "int32_t outputOrientationCount, "
     "const gkg::Vector3d< double >& resolution, "
     "const std::vector< std::vector< float > >& transformations, "
     "const std::vector< gkg::Translation3d< float > >& translations, "
     "std::vector< std::vector< std::vector< GLfloat*  > > >& "
     "glTransformations, "
     "std::vector< std::vector< std::vector< QVector4D  > > >& "
     "glTranslations, "
     "std::vector < std::vector < std::vector < QVector3D* > > >& "
     "glNormals )" );

}


gkg::TransformationsAndTranslationsAndNormalsProcessingContext::
                    ~TransformationsAndTranslationsAndNormalsProcessingContext()
{
}


void 
gkg::TransformationsAndTranslationsAndNormalsProcessingContext::doIt(
                                                            int32_t startIndex,
                                                            int32_t indexCount )
{

  try
  {


    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    gkg::Vector3d< int32_t > voxel;
    int32_t i = 0;
    int32_t j = 0;
    GLfloat* transformation = 0;
    int32_t transformationSize = 0;
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

      transformation = new GLfloat[ _outputOrientationCount ];

      transformationSize = _transformations[ i ].size();
      for ( j = 0; j < transformationSize; j++ )
      {

        transformation[ j ] = ( GLfloat )_transformations[ i ][ j ];

      }

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

      lock();
      _glTransformations[ voxel.x ]
                        [ voxel.y ]
                        [ voxel.z ] = transformation;
      _glTranslations[ voxel.x ]
                     [ voxel.y ]
                     [ voxel.z ] = QVector4D( x, y, z, 0.0 );
      _glNormals[ voxel.x ]
                [ voxel.y ]
                [ voxel.z ] = normal;
      unlock();

    }

  }
  GKG_CATCH( "void "
             "gkg::TransformationsAndTranslationsAndNormalsProcessingContext::"
             "doIt("
             "int32_t startIndex,"
             "int32_t indexCount )" );
}
