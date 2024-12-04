#include <gkg-anatomist-plugin-io/ATwoPoolModelFieldReader.h>
#include <gkg-anatomist-plugin-io/ATwoPoolModelFieldProcessingContext.h>
#include <gkg-anatomist-plugin-io/TransformationsAndTranslationsProcessingContext.h>
#include <gkg-anatomist-plugin-io/TransformationsAndTranslationsAndNormalsProcessingContext.h>
#include <gkg-anatomist-plugin-io/NormalsProcessingContext.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/CartesianField_i.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModel.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-mesh/ConvexHull_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::ATwoPoolModelFieldReader::ATwoPoolModelFieldReader(
                                      const std::string& fileNameSiteMap,
                                       const std::string& fileNameTextureMap,
                                       int32_t outputOrientationCount )
                     : _boundingBox( -1.0, -1.0, -1.0, -1.0, -1.0, -1.0 ),
                       _outputOrientationCount( outputOrientationCount ),
                       _meshScale( 1.0 )
{

  try
  {

    // reading the site and ODF texture map(s)
    gkg::Reader::getInstance().read( fileNameSiteMap, _siteMap );
    gkg::Reader::getInstance().read( fileNameTextureMap, _twoPoolModels );

    // checking that the texture map is not empty
    gkg::TextureMap< gkg::TwoPoolModel >::iterator
      t = _twoPoolModels.begin(),
      te = _twoPoolModels.end();

    if ( t == te )
    {

      throw std::runtime_error( "TwoPoolModel field is empty!" );

    }

    // creating output orientation set
    _outputOrientationSet =  new gkg::OrientationSet(
                  gkg::ElectrostaticOrientationSet(
                   _outputOrientationCount / 2 ).getSymmetricalOrientations() );



    t = _twoPoolModels.begin();
    while ( t != te )
    {

      t->second.setOrientationSet( _outputOrientationSet );
      ++ t;

    }

    // processing the ODF cartesian field
    _twoPoolModelCartesianField = new gkg::TwoPoolModelCartesianField(
                                                     _siteMap,
                                                     _twoPoolModels,
                                                     *_outputOrientationSet );

    // processing the convex hull of a sphere containing a given number of
    // vertices
    gkg::ConvexHull::getInstance().addConvexHull(
                                       _outputOrientationSet->getOrientations(),
                                       0,
                                       _sphereMeshMap );

    // processing the ODF field
    computeTwoPoolModelField();


    // computing bounding box
    computeBoundingBox();


  }
  GKG_CATCH( "gkg::ATwoPoolModelFieldReader::ATwoPoolModelFieldReader( "
             "const std::string& fileNameSiteMap, "
             "const std::string& fileNameTextureMap, "
             "int32_t outputOrientationCount )" );

}


gkg::ATwoPoolModelFieldReader::~ATwoPoolModelFieldReader()
{

  delete _twoPoolModelCartesianField;
  delete _outputOrientationSet;

}


gkg::Vector3d< double > gkg::ATwoPoolModelFieldReader::getResolution() const
{

  try
  {

    return _twoPoolModelCartesianField->getResolution();

  }
  GKG_CATCH( "gkg::Vector3d< double > "
             "gkg::ATwoPoolModelFieldReader::getResolution() const" );

}


const gkg::BoundingBox< float >& gkg::ATwoPoolModelFieldReader::getBoundingBox() const
{

  try
  {

    return _boundingBox;

  }
  GKG_CATCH( "const gkg::BoundingBox< float >& "
             "gkg::ATwoPoolModelFieldReader::getBoundingBox() const" );

}


int32_t gkg::ATwoPoolModelFieldReader::getOutputOrientationCount() const
{

  try
  {

    return _outputOrientationCount;

  }
  GKG_CATCH( "int32_t getOutputOrientationCount()const" );

}


void gkg::ATwoPoolModelFieldReader::getSphereVertexCoordinatesAndIndices(
                                          QVector< GLfloat >& vertexCoordinates,
                                          QVector<GLuint>& vertexIndices ) const
{

  try
  {

    // clearing structures
    vertexCoordinates.clear();
    vertexIndices.clear();

    // creating vertex coordinates
    const std::list< gkg::Vector3d< float > >&
      vertices = _sphereMeshMap.vertices.getSites( 0 );

    vertexCoordinates.resize( 3U * vertices.size() );

    std::list< gkg::Vector3d< float > >::const_iterator
      v = vertices.begin(),
      ve = vertices.end();
    int32_t index = 0;
    while ( v != ve )
    {

      vertexCoordinates[ index ++ ] = ( GLfloat )v->x;
      vertexCoordinates[ index ++ ] = ( GLfloat )v->y;
      vertexCoordinates[ index ++ ] = ( GLfloat )v->z;
      ++ v;

    }

    // creating vertex indices
    const std::list< gkg::Polygon< 3U > >&
      polygons = _sphereMeshMap.polygons.getPolygons( 0 );

    vertexIndices.resize( 3U * polygons.size() );

    std::list< gkg::Polygon< 3U > >::const_iterator
      p = polygons.begin(),
      pe = polygons.end();
    index = 0;
    while ( p != pe )
    {

      vertexIndices[ index ++ ] = ( GLuint )p->indices[ 0 ];
      vertexIndices[ index ++ ] = ( GLuint )p->indices[ 1 ];
      vertexIndices[ index ++ ] = ( GLuint )p->indices[ 2 ];
      ++ p;

    }

  }
  GKG_CATCH( "void gkg::ATwoPoolModelFieldReader::"
             "getSphereVertexCoordinatesAndIndices( "
             "QVector< GLfloat >& vertexCoordinates, "
             "QVector<GLuint>& vertexIndices ) const" );

}


void gkg::ATwoPoolModelFieldReader::getTransformationsAndTranslations(
  std::vector< std::vector< std::vector< GLfloat*  > > >& glFastTransformations,
  std::vector< std::vector< std::vector< GLfloat*  > > >& glSlowTransformations,
  std::vector< std::vector< std::vector< QVector4D  > > >& glTranslations )
{

  try
  {

    std::cout << "computing transformations and translations..."
              << std::flush;
    int32_t iterationCount = ( int32_t )_fastTransformations.size();

    gkg::TransformationsAndTranslationsProcessingContext
      fastContext( _boundingBox,
                   _outputOrientationCount,
                   _twoPoolModelCartesianField->getResolution(),
                   _fastTransformations,
                   _translations,
                   glFastTransformations,
                   glTranslations );

    gkg::ThreadedLoop< int32_t >
      fastThreadedLoop( &fastContext,
                        0,
                        iterationCount );
    fastThreadedLoop.launch();

    gkg::TransformationsAndTranslationsProcessingContext
      slowContext( _boundingBox,
                   _outputOrientationCount,
                   _twoPoolModelCartesianField->getResolution(),
                   _slowTransformations,
                   _translations,
                   glSlowTransformations,
                   glTranslations );

    gkg::ThreadedLoop< int32_t >
      slowThreadedLoop( &slowContext,
                        0,
                        iterationCount );
    slowThreadedLoop.launch();

    std::cout << "done" << std::endl << std::flush;

  }
  GKG_CATCH( "void gkg::ATwoPoolModelFieldReader::"
             "getTransformationsAndTranslations( "
             "std::vector< std::vector< std::vector< GLfloat*  > > >& "
             "glFastTransformations, "
             "std::vector< std::vector< std::vector< GLfloat*  > > >& "
             "glSlowTransformations, "
             "std::vector< std::vector< std::vector< QVector4D  > > >& "
             "glTranslations )" );

}


void gkg::ATwoPoolModelFieldReader::getTransformationsAndTranslationsAndNormals( 
  std::vector< std::vector< std::vector< GLfloat*  > > >& glFastTransformations,
  std::vector< std::vector< std::vector< GLfloat*  > > >& glSlowTransformations,
  std::vector< std::vector< std::vector< QVector4D  > > >& glTranslations,
  std::vector < std::vector < std::vector < QVector3D* > > >& glFastNormals,
  std::vector < std::vector < std::vector < QVector3D* > > >& glSlowNormals )
{

  try
  {

    std::cout << "computing transformations, translations and normals..."
              << std::flush;

    int32_t iterationCount = ( int32_t )_fastTransformations.size();

    gkg::TransformationsAndTranslationsAndNormalsProcessingContext
      fastContext( _boundingBox,
                   _outputOrientationCount,
                   _sphereMeshMap,
                   _twoPoolModelCartesianField->getResolution(),
                   _fastTransformations,
                   _translations,
                   glFastTransformations,
                   glTranslations,
                   glFastNormals );

    gkg::ThreadedLoop< int32_t >
      fastThreadedLoop( &fastContext,
                        0,
                        iterationCount );
    fastThreadedLoop.launch();

    gkg::TransformationsAndTranslationsAndNormalsProcessingContext
      slowContext( _boundingBox,
                   _outputOrientationCount,
                   _sphereMeshMap,
                   _twoPoolModelCartesianField->getResolution(),
                   _slowTransformations,
                   _translations,
                   glSlowTransformations,
                   glTranslations,
                   glSlowNormals );

    gkg::ThreadedLoop< int32_t >
      slowThreadedLoop( &slowContext,
                        0,
                        iterationCount );
    slowThreadedLoop.launch();

    std::cout << "done" << std::endl << std::flush;

  }
  GKG_CATCH( "void gkg::ATwoPoolModelFieldReader::"
             "getTransformationsAndTranslationsAndNormals( "
             "std::vector< std::vector< std::vector< GLfloat*  > > >& "
             "glFastTransformations, "
             "std::vector< std::vector< std::vector< GLfloat*  > > >& "
             "glSlowTransformations, "
             "std::vector< std::vector< std::vector< QVector4D  > > >& "
             "glTranslations, "
             "std::vector < std::vector < std::vector < QVector3D* > > >& "
             "glFastNormals, "
             "std::vector < std::vector < std::vector < QVector3D* > > >& "
             "glSlowNormals )" );

}


void gkg::ATwoPoolModelFieldReader::getNormals(
      std::vector < std::vector < std::vector < QVector3D * > > >& fastNormals,
      std::vector < std::vector < std::vector < QVector3D * > > >& slowNormals )
{

  try
  {

    std::cout << "processing normals..." << std::flush;

    int32_t iterationCount = ( int32_t )_fastTransformations.size();
    gkg::NormalsProcessingContext
      fastContext( _boundingBox,
                   _outputOrientationCount,
                   _sphereMeshMap,
                   _twoPoolModelCartesianField->getResolution(),
                   _fastTransformations,
                   _translations,
                   fastNormals );

    gkg::ThreadedLoop< int32_t >
      fastThreadedLoop( &fastContext,
                        0,
                        iterationCount );
    fastThreadedLoop.launch();

    gkg::NormalsProcessingContext
      slowContext( _boundingBox,
                   _outputOrientationCount,
                   _sphereMeshMap,
                   _twoPoolModelCartesianField->getResolution(),
                   _slowTransformations,
                   _translations,
                   slowNormals );

    gkg::ThreadedLoop< int32_t >
      slowThreadedLoop( &slowContext,
                        0,
                        iterationCount );
    slowThreadedLoop.launch();

    std::cout << "done" << std::endl << std::flush;

  }
  GKG_CATCH( "void gkg::ATwoPoolModelFieldReader::getNormals( "
             "std::vector < std::vector < std::vector < QVector3D * > > >& "
             "fastNormals, "
             "std::vector < std::vector < std::vector < QVector3D * > > >& "
             "slowNormals )" );

}


void gkg::ATwoPoolModelFieldReader::computeTwoPoolModelField()
{

  try
  {

    std::cout << "processing TwoPoolModel field..." << std::flush;

    int32_t siteCount = _siteMap.getSiteCount( 0 );
    _fastTransformations.resize( siteCount );
    _slowTransformations.resize( siteCount );
    _translations.resize( siteCount );

    gkg::ATwoPoolModelFieldProcessingContext
      context( _twoPoolModelCartesianField,
               _fastTransformations,
               _slowTransformations,
               _translations,
               _meshScale );

    gkg::ThreadedLoop< int32_t > threadedLoop( &context,
                                               0,
                                               siteCount );
    threadedLoop.launch();

    std::cout << "done" << std::endl << std::flush;

  }
  GKG_CATCH( "void gkg::ATwoPoolModelFieldReader::computeOdfField()" );

}


void gkg::ATwoPoolModelFieldReader::computeBoundingBox()
{

  try
  {

    std::set< float > orderedX;
    std::set< float > orderedY;
    std::set< float > orderedZ;

    float x = 0;
    float y = 0;
    float z = 0;
    std::vector< gkg::Translation3d< float > >::const_iterator
      t = _translations.begin(),
      te = _translations.end();
    while ( t != te )
    {

      t->getDirectTranslation( x, y, z );
      orderedX.insert( x );
      orderedY.insert( y );
      orderedZ.insert( z );
      ++ t;

    }
 
    _boundingBox.setLowerX( *orderedX.begin() );
    _boundingBox.setLowerY( *orderedY.begin() );
    _boundingBox.setLowerZ( *orderedZ.begin() );

    _boundingBox.setUpperX( *orderedX.rbegin() );
    _boundingBox.setUpperY( *orderedY.rbegin() );
    _boundingBox.setUpperZ( *orderedZ.rbegin() );

  }
  GKG_CATCH( "void gkg::ATwoPoolModelFieldReader::computeBoundingBox()" );

}

