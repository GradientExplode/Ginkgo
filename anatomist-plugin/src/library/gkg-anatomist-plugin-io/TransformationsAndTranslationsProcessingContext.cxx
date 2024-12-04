#include <gkg-anatomist-plugin-io/TransformationsAndTranslationsProcessingContext.h>
#include <gkg-core-exception/Exception.h>

#include <cstring>


gkg::TransformationsAndTranslationsProcessingContext::
                                TransformationsAndTranslationsProcessingContext(
      const gkg::BoundingBox< float >& boundingBox,
      int32_t outputOrientationCount,
      const gkg::Vector3d< double >& resolution,
      const std::vector< std::vector< float > >& transformations,
      const std::vector< gkg::Translation3d< float > >& translations,
      std::vector< std::vector< std::vector< GLfloat*  > > >& glTransformations,
      std::vector< std::vector< std::vector< QVector4D  > > >& glTranslations )
    : gkg::LoopContext< int32_t >(),
      _boundingBox( boundingBox ),
      _outputOrientationCount( outputOrientationCount ),
      _resolution( resolution ),
      _transformations( transformations ),
      _translations( translations ),
      _glTransformations( glTransformations ),
      _glTranslations( glTranslations )
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

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    for ( x = 0; x < sizeX; ++x )
    {

      _glTransformations[ x ].resize( sizeY );
      _glTranslations[ x ].resize( sizeY );
      for ( y = 0; y < sizeY; ++y )
      {

        _glTransformations[ x ][ y ].resize( sizeZ );
        _glTranslations[ x ][ y ].resize( sizeZ );
        for ( z = 0; z < sizeZ; ++z )
        {

          GLfloat* transformation = new GLfloat[ _outputOrientationCount ];
          std::memset( transformation, 0,
                       _outputOrientationCount * sizeof( float ) );
          _glTransformations[ x ][ y ][ z ] = transformation;
          _glTranslations[ x ][ y ][ z ] = QVector4D( 0.0, 0.0, 0.0, 0.0 );

        }

      }

    }

  }
  GKG_CATCH( "gkg::TransformationsAndTranslationsProcessingContext::"
             "TransformationsAndTranslationsProcessingContext( "
             "const gkg::BoundingBox< float >& boundingBox, "
             "int32_t outputOrientationCount, "
             "const gkg::Vector3d< double >& resolution, "
             "const std::vector< std::vector< float > >& transformations, "
             "const std::vector< gkg::Translation3d< float > >& translations, "
             "std::vector< std::vector< std::vector< GLfloat*  > > >& "
             "glTransformations, "
             "std::vector< std::vector< std::vector< QVector4D  > > >& "
             "glTranslations )" );
}


gkg::TransformationsAndTranslationsProcessingContext::
                     ~TransformationsAndTranslationsProcessingContext()
{
}


void gkg::TransformationsAndTranslationsProcessingContext::doIt(
                                                            int32_t startIndex,
                                                            int32_t indexCount )
{

  try
  {

    float x = 0.0;
    float y = 0.0;
    float z = 0.0;
    int32_t vx;
    int32_t vy;
    int32_t vz;
    GLfloat* transformation = 0;
    int32_t transformationSize = 0;
    const gkg::Translation3d< float >* translations = 
                                                   &_translations[ startIndex ];
    const std::vector< float >* transformations = 
                                                &_transformations[ startIndex ];
    const float* t = 0;
    while ( indexCount-- )
    {

      translations->getDirectTranslation( x, y, z );
      vx = ( int32_t )( ( x - _boundingBox.getLowerX() ) /
                        _resolution.x + 0.5 );
      vy = ( int32_t )( ( y - _boundingBox.getLowerY() ) /
                        _resolution.y + 0.5 );
      vz = ( int32_t )( ( z - _boundingBox.getLowerZ() ) /
                        _resolution.z + 0.5 );


      transformation = _glTransformations[ vx ][ vy ][ vz ];
      transformationSize = transformations->size();
      t = &(*transformations)[ 0 ];
      while ( transformationSize-- )
      {

        *transformation++ = ( GLfloat )*t++;

      }
      _glTranslations[ vx ][ vy ][ vz ] = QVector4D( x, y, z, 0.0 );
      ++translations;
      ++transformations;

    }

  }
  GKG_CATCH( "void gkg::TransformationsAndTranslationsProcessingContext::doIt("
             "int32_t startIndex,"
             "int32_t indexCount )" );
}
