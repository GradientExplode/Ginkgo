#include <gkg-anatomist-plugin-io/APdfFieldReader.h>
#include <gkg-anatomist-plugin-io/APdfFieldProcessingContext.h>
#include <gkg-anatomist-plugin-io/TransformationsAndTranslationsProcessingContext.h>
#include <gkg-anatomist-plugin-io/TransformationsAndTranslationsAndNormalsProcessingContext.h>
#include <gkg-anatomist-plugin-io/NormalsProcessingContext.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/CartesianField_i.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-mesh/ConvexHull_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::APdfFieldReader::APdfFieldReader( const std::string& fileNameSiteMap,
                                       const std::string& fileNameTextureMap,
                                       float& displacementMagnitude,
                                       int32_t outputOrientationCount,
                                       int32_t resamplingNeighborCount,
                                       float resamplingGaussianSigma )
                     : _boundingBox( -1.0, -1.0, -1.0, -1.0, -1.0, -1.0 ),
                       _displacementMagnitude( displacementMagnitude ),
                       _outputOrientationCount( outputOrientationCount ),
                       _resamplingNeighborCount( resamplingNeighborCount ),
                       _resamplingGaussianSigma( resamplingGaussianSigma ),
                       _isPolarHarmonics( false ),
                       _meshScale( 1.0 )
{

  try
  {

    // reading the site and PDF texture map(s)
    gkg::Reader::getInstance().read( fileNameSiteMap, _siteMap );
    gkg::Reader::getInstance().read( fileNameTextureMap, _pdfs );

    // checking that the texture map is not empty
    gkg::TextureMap< gkg::ProbabilityDensityFunction >::iterator
      p = _pdfs.begin(),
      pe = _pdfs.end();

    if ( p == pe )
    {

      throw std::runtime_error( "PDF field is empty!" );

    }

    // checking for the basis type
    gkg::ProbabilityDensityFunction::BasisType
      basisType = p->second.getBasisType();

    if ( basisType == gkg::ProbabilityDensityFunction::PolarHarmonics )
    {

      _isPolarHarmonics = true;
      _originalOutputMultipleShellSampling = 0;

    }
    else if ( basisType == gkg::ProbabilityDensityFunction::Standard )
    {

      // reading the original output multiple shell sampling
      _originalOutputMultipleShellSampling =
                                     new std::map< float, gkg::OrientationSet >;

      // extracting displacement space sampling from header
      gkg::Dictionary displacementSpaceSampling;
      if ( !_pdfs.getHeader().hasAttribute( "displacement_space_sampling" ) )
      {

        throw std::runtime_error(
                            "missing 'displacement_space_sampling' attribute" );

      }
      _pdfs.getHeader().getAttribute( "displacement_space_sampling",
                                      displacementSpaceSampling );


      // checking that it is not Cartesian (not implemented yet)
      if ( displacementSpaceSampling[ "type" ]->getString() == "cartesian" )
      {

        throw std::runtime_error( "not implemented yet" );

      }

      // extracting parameters
      gkg::Dictionary parameters = 
       displacementSpaceSampling[ "parameters" ]->getValue< gkg::Dictionary >();

      // extracting displacement magnitudes
      gkg::GenericObjectList
        golDisplacementMagnitudes = parameters[ "displacement_magnitudes" ]->
                                           getValue< gkg::GenericObjectList >();

      int32_t displacementMagnitudeCount = 
                                    ( int32_t )golDisplacementMagnitudes.size();
      std::vector< float > displacementMagnitudes( displacementMagnitudeCount );
      int32_t d = 0;
      for ( d = 0; d < displacementMagnitudeCount; d++ )
      {

        displacementMagnitudes[ d ] = 
                 ( float )golDisplacementMagnitudes[ d ]->getScalar();

      }

      // extracting orientation set(s)
      gkg::GenericObjectList 
        golOrientationSets = parameters[ "orientation_sets" ]->
                                           getValue< gkg::GenericObjectList >();
      int32_t orientationSetCount = ( int32_t )golOrientationSets.size();


      int32_t orientationSetIndex = 0;
      for ( orientationSetIndex = 0;
            orientationSetIndex < orientationSetCount;
            orientationSetIndex++ )
      {

        gkg::GenericObjectList 
          golOrientationSet = golOrientationSets[ orientationSetIndex ]->
                                           getValue< gkg::GenericObjectList >();
        int32_t orientationCount = ( int32_t )golOrientationSet.size();

        std::vector< gkg::Vector3d< float > > orientations( orientationCount );

        int32_t o = 0;
        for ( o = 0; o < orientationCount; o++ )
        {

          gkg::GenericObjectList golOrientation = 
                   golOrientationSet[ o ]->getValue< gkg::GenericObjectList >();

          orientations[ o ].x = golOrientation[ 0 ]->getScalar();
          orientations[ o ].y = golOrientation[ 1 ]->getScalar();
          orientations[ o ].z = golOrientation[ 2 ]->getScalar();

        }

        ( *_originalOutputMultipleShellSampling )[ 
                  displacementMagnitudes[ orientationSetIndex ] ] =
                                            gkg::OrientationSet( orientations );

      }

      // setting the output sampling back into the PDFs
      while ( p != pe )
      {

         p->second.setOutputSampling( _originalOutputMultipleShellSampling );
         ++ p;

      }

    }

    // creating the output multiple shell sampling from the prescribed
    // displacement magnitude and output orientation count
    _outputMultipleShellSampling = new std::map< float, gkg::OrientationSet >;
    ( *_outputMultipleShellSampling )[ displacementMagnitude ] =
       gkg::OrientationSet( gkg::ElectrostaticOrientationSet(
                   _outputOrientationCount / 2 ).getSymmetricalOrientations() );

    // processing the PDF cartesian field
    _pdfCartesianField = new gkg::PdfCartesianField(
                                                _siteMap,
                                                _pdfs,
                                                *_outputMultipleShellSampling,
                                                _resamplingNeighborCount,
                                                _resamplingGaussianSigma );

    // in case of standard multiple shell sampling, the sampling might be
    // modified to stay within the displacement magnitude boundaries
    *_outputMultipleShellSampling = _pdfCartesianField->
                                               getOutputMultipleShellSampling();

    displacementMagnitude = _outputMultipleShellSampling->begin()->first;
    _displacementMagnitude = displacementMagnitude;

    // processing the convex hull of a sphere containing a given number of
    // vertices
    gkg::ConvexHull::getInstance().addConvexHull(
                _outputMultipleShellSampling->begin()->second.getOrientations(),
                0,
                _sphereMeshMap );

    // processing the PDF field
    computePdfField();

    // computing bounding box
    computeBoundingBox();

  }
  GKG_CATCH( "gkg::APdfFieldReader::APdfFieldReader( "
             "const std::string& fileNameSiteMap, "
             "const std::string& fileNameTextureMap, "
             "int32_t outputOrientationCount, "
             "int32_t resamplingNeighborCount, "
             "float resamplingGaussianSigma )" );

}


gkg::APdfFieldReader::~APdfFieldReader()
{

  delete _pdfCartesianField;
  delete _originalOutputMultipleShellSampling;
  delete _outputMultipleShellSampling;

}


gkg::Vector3d< double > gkg::APdfFieldReader::getResolution() const
{

  try
  {

    return _pdfCartesianField->getResolution();

  }
  GKG_CATCH( "gkg::Vector3d< double > "
             "gkg::APdfFieldReader::getResolution() const" );

}


const gkg::BoundingBox< float >& gkg::APdfFieldReader::getBoundingBox() const
{

  try
  {

    return _boundingBox;

  }
  GKG_CATCH( "const gkg::BoundingBox< float >& "
             "gkg::APdfFieldReader::getBoundingBox() const" );

}


bool gkg::APdfFieldReader::isPolarHarmonics() const
{

  try
  {

    return _isPolarHarmonics;

  }
  GKG_CATCH( "bool gkg::APdfFieldReader::isPolarHarmonics() const" );

}


float gkg::APdfFieldReader::getDisplacementMagnitude() const
{

  try
  {

    return _displacementMagnitude;

  }
  GKG_CATCH( "float gkg::APdfFieldReader::getDisplacementMagnitude() const" );

}


int32_t gkg::APdfFieldReader::getOutputOrientationCount() const
{

  try
  {

    return _outputOrientationCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::APdfFieldReader::getOutputOrientationCount() const" );

}


void gkg::APdfFieldReader::getSphereVertexCoordinatesAndIndices(
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
  GKG_CATCH( "void gkg::APdfFieldReader::getSphereVertexCoordinatesAndIndices( "
             "QVector< GLfloat >& vertexCoordinates, "
             "QVector<GLuint>& vertexIndices ) const" );

}


void gkg::APdfFieldReader::getTransformationsAndTranslations(
      std::vector< std::vector< std::vector< GLfloat*  > > >& glTransformations,
      std::vector< std::vector< std::vector< QVector4D  > > >& glTranslations )
{

  try
  {

    std::cout << "computing transformations and translations..."
              << std::flush;

    gkg::TransformationsAndTranslationsProcessingContext
      context( _boundingBox,
               _outputOrientationCount, 
               _pdfCartesianField->getResolution(),
               _transformations, 
               _translations,
               glTransformations,
               glTranslations );

    gkg::ThreadedLoop< int32_t >
      threadedLoop( &context,
                    0,
                    ( int32_t )_transformations.size() );
    threadedLoop.launch();

    std::cout << "done" << std::endl << std::flush;

  }
  GKG_CATCH( "void gkg::APdfFieldReader::getTransformationsAndTranslations( "
             "std::vector< std::vector< std::vector< GLfloat*  > > >& "
             "transformations, "
             "std::vector< std::vector< std::vector< QVector4D  > > >& "
             "translations )" );

}


void gkg::APdfFieldReader::getTransformationsAndTranslationsAndNormals( 
      std::vector< std::vector< std::vector< GLfloat*  > > >& glTransformations,
      std::vector< std::vector< std::vector< QVector4D  > > >& glTranslations,
      std::vector < std::vector < std::vector < QVector3D* > > >& glNormals )
{

  try
  {

    std::cout << "computing transformations, translations and normals..."
              << std::flush;

    gkg::TransformationsAndTranslationsAndNormalsProcessingContext
      context( _boundingBox,
               _outputOrientationCount,
               _sphereMeshMap,
               _pdfCartesianField->getResolution(),
               _transformations, 
               _translations,
               glTransformations,
               glTranslations,
               glNormals );

    gkg::ThreadedLoop< int32_t >
      threadedLoop( &context,
                    0,
                    ( int32_t )_transformations.size() );
    threadedLoop.launch();

    std::cout << "done" << std::endl << std::flush;

  }
  GKG_CATCH( "void gkg::APdfFieldReader::"
             "getTransformationsAndTranslationsAndNormals( "
             "std::vector< std::vector< std::vector< GLfloat*  > > >& "
             "transformations, "
             "std::vector< std::vector< std::vector< QVector4D  > > >& "
             "translations, "
             "std::vector < std::vector < std::vector < QVector3D* > > >& "
             "normals )" );

}


void gkg::APdfFieldReader::getNormals(
          std::vector < std::vector < std::vector < QVector3D * > > >& normals )
{

  try
  {

    std::cout << "processing normals..." << std::flush;

    gkg::NormalsProcessingContext
      context( _boundingBox,
               _outputOrientationCount,
               _sphereMeshMap,
               _pdfCartesianField->getResolution(),
               _transformations, 
               _translations,
               normals );

    gkg::ThreadedLoop< int32_t >
      threadedLoop( &context,
                    0,
                    ( int32_t )_transformations.size() );
    threadedLoop.launch();

    std::cout << "done" << std::endl << std::flush;

  }
  GKG_CATCH( "void gkg::APdfFieldReader::getNormals( "
             "std::vector < std::vector < std::vector < QVector3D * > > >& "
             "normals )" );

}


void gkg::APdfFieldReader::computePdfField()
{

  try
  {

    std::cout << "processing PDF field..." << std::flush;

    int32_t siteCount = _siteMap.getSiteCount( 0 );
    _transformations.resize( siteCount );
    _translations.resize( siteCount );

    gkg::APdfFieldProcessingContext context( _pdfCartesianField,
                                             _transformations,
                                             _translations,
                                             _meshScale );

    gkg::ThreadedLoop< int32_t >
      threadedLoop( &context,
                    0,
                    siteCount );
    threadedLoop.launch();

    std::cout << "done" << std::endl << std::flush;

  }
  GKG_CATCH( "void gkg::APdfFieldReader::computePdfField()" );

}


void gkg::APdfFieldReader::computeBoundingBox()
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
  GKG_CATCH( "void gkg::APdfFieldReader::computeBoundingBox()" );

}

