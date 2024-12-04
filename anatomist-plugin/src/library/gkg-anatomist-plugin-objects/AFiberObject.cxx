#include <gkg-anatomist-plugin-objects/AFiberObject.h>
#include <gkg-anatomist-plugin-objects/AFiberShader.h>
#include <gkg-anatomist-plugin-rendering-options/AFiberRenderingOptionsWidget.h>
#include <gkg-anatomist-plugin-io/AFiberReader.h>
#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Reader_i.h>

#include <anatomist/object/oReader.h>
#include <anatomist/object/actions.h>
#include <anatomist/control/qObjTree.h>
#include <anatomist/window/Window.h>
#include <aims/resampling/quaternion.h>

#include <iostream>
#include <cmath>
#include <limits>

#include <QFileDialog>
#include <QPixmap>

#include <locale.h>


int32_t gkg::AFiberObject::_classType = gkg::AFiberObject::registerClass();
Tree* gkg::AFiberObject::_optionTree = 0;


gkg::AFiberObject::AFiberObject( const std::string& fileNameBundleMap )
                 : _fileNameBundleMap( fileNameBundleMap ),
                   _dataType( "no-data" ),
                   _isOneValuePerBundleShaderType( true ),
                   _fileNameData( "" ),
                   _fileNameVolume( "" ),
                   _samplingValue( 60 ),
                   _minimumValue( std::numeric_limits< double >::lowest() ),
                   _maximumValue( std::numeric_limits< double >::max() ),
                   _minimumRange( std::numeric_limits< double >::lowest() ),
                   _maximumRange( std::numeric_limits< double >::max() )
{

  try
  {

    if ( _fileNameBundleMap != "" )
    {
      // initializing the plugin(s)
      gkg::PluginLoader::getInstance().load( false );

      // adding icon
      _type = _classType;
      if ( QObjectTree::TypeNames.find( _classType ) ==
           QObjectTree::TypeNames.end() )
      {

        std::string iconPath = 
          gkg::ConfigurationInfo::getInstance().getIconPath( "icon-odf.png" );

        if ( !QObjectTree::TypeIcons[ _classType ].load( iconPath.c_str() ) )
        {

          QObjectTree::TypeIcons.erase( _classType );
          std::cerr << "icon " << iconPath << " not found!" << std::endl;

        }

        QObjectTree::TypeNames[ _classType ] = "BUNDLEMAP";

      }

      // setting Fiber reader
      loadReader();

    }

  }
  GKG_CATCH( "gkg::AFiberObject::AFiberObject( "
             "const std::string& fileNameBundleMap )");

}


gkg::AFiberObject::~AFiberObject()
{

  cleanup();
  deleteReader();

}


bool gkg::AFiberObject::Is2DObject()
{

  return true;

}


bool gkg::AFiberObject::Is3DObject()
{

  return true;

}


Tree* gkg::AFiberObject::optionTree() const
{

  try
  {

    if ( !_optionTree )
    {

      Tree* t;
      Tree* t2;
      _optionTree = new Tree( true, "option tree" );
      t = new Tree( true, "File" );
      _optionTree->insert( t );
      t2 = new Tree( true, "Reload" );
      t2->setProperty( "callback", &anatomist::ObjectActions::fileReload );
      t->insert( t2 );
      t2 = new Tree( true, "Save" );
      t2->setProperty( "callback", &anatomist::ObjectActions::saveStatic );
      t->insert( t2 );
      t2 = new Tree( true, "Rename object" );
      t2->setProperty( "callback", &anatomist::ObjectActions::renameObject );
      t->insert( t2 );

      t = new Tree( true, "Color" );
      _optionTree->insert( t );
      t2 = new Tree( true, "Material" );
      t2->setProperty( "callback", &anatomist::ObjectActions::colorMaterial );
      t->insert( t2 );

      t = new Tree( true, "Referential" );
      _optionTree->insert( t );
      t2 = new Tree( true, "Load" );
      t2->setProperty( "callback", &anatomist::ObjectActions::referentialLoad );
      t->insert( t2 );

      t = new Tree( true, "Render" );
      _optionTree->insert( t );
      t2 = new Tree( true, "Options" );
      t2->setProperty( "callback",
                       &gkg::AFiberRenderingOptionsWidget::create );
      t->insert( t2 );

    }

    return( _optionTree );

  }
  GKG_CATCH( "Tree* gkg::AFiberObject::optionTree() const" );

}


float gkg::AFiberObject::MinX2D() const
{

  try
  {

    return _boundingBox.getLowerX();

  }
  GKG_CATCH( "float gkg::AFiberObject::MinX2D() const" );

}


float gkg::AFiberObject::MinY2D() const
{

  try
  {

    return _boundingBox.getLowerY();

  }
  GKG_CATCH( "float gkg::AFiberObject::MinY2D() const" );

}


float gkg::AFiberObject::MinZ2D() const
{

  try
  {

    return _boundingBox.getLowerZ();

  }
  GKG_CATCH( "float gkg::AFiberObject::MinZ2D() const" );

}


float gkg::AFiberObject::MaxX2D() const
{

  try
  {

    return _boundingBox.getUpperX();

  }
  GKG_CATCH( "float gkg::AFiberObject::MaxX2D() const" );

}


float gkg::AFiberObject::MaxY2D() const
{

  try
  {

    return _boundingBox.getUpperY();

  }
  GKG_CATCH( "float gkg::AFiberObject::MaxY2D() const" );

}


float gkg::AFiberObject::MaxZ2D() const
{

  try
  {

    return _boundingBox.getUpperZ();

  }
  GKG_CATCH( "float gkg::AFiberObject::MaxZ2D() const" );

}


bool gkg::AFiberObject::boundingBox( std::vector<float>& bmin,
                                     std::vector<float>& bmax ) const
{

  try
  {

    bmin.resize( 3 );
    bmax.resize( 3 );

    bmin[ 0 ] = _boundingBox.getLowerX();
    bmin[ 1 ] = _boundingBox.getLowerY();
    bmin[ 2 ] = _boundingBox.getLowerZ();

    bmax[ 0 ] = _boundingBox.getUpperX();
    bmax[ 1 ] = _boundingBox.getUpperY();
    bmax[ 2 ] = _boundingBox.getUpperZ();

    if ( bmin[ 0 ] == bmax[ 0 ] )
    {

      bmax[ 0 ] = bmin[ 0 ] + 1;

    }

    if ( bmin[ 1 ] == bmax[ 1 ] )
    {

      bmax[ 1 ] = bmin[ 1 ] + 1;

    }
    if ( bmin[ 2 ] == bmax[ 2 ] )
    {

      bmax[ 2 ] = bmin[ 2 ] + 1;

    }

    return true;

  }
  GKG_CATCH( "bool AFiberObject::boundingBox("
             "Point3df & bmin,"
             "Point3df & bmax ) const" );

}


bool gkg::AFiberObject::render( anatomist::PrimList& primitiveList,
                                const anatomist::ViewState& viewState )
{

  try
  {

    _isDisplayed = true;
    if ( !_qGLShader )
    {

      updateShaderCode();
      setShaderAttributes();

    }

    GLComponent* glComponent = GLComponent::glAPI();
    glComponent->glSetChanged( glBODY );

    if ( !_glList->item() )
    {

      _glList->generate();

    }

    return anatomist::AObject::render( primitiveList, viewState );

  }
  GKG_CATCH( "bool gkg::AFiberObject::render( "
             "anatomist::PrimList& primitiveList, "
             "const anatomist::ViewState& viewState )" );

}


bool 
gkg::AFiberObject::glMakeBodyGLL( const anatomist::ViewState& /* viewState */,
                                  const anatomist::GLList& glList ) const
{

  try
  {

    glNewList( glList.item(), GL_COMPILE );

    bool isBind = _qGLShaderProgram->bind();

    if ( isBind )
    {

      this->draw();

      _qGLShaderProgram->setUniformValue( _uniformIsShaderedObject, 0 );
      _qGLShaderProgram->release();

    }

    glEndList();

    glNewList( _glList->item(), GL_COMPILE );

    int32_t vertextCount = 0;
    
    glBegin( GL_LINE_STRIP );

    for ( vertextCount = 0; vertextCount < _samplingValue; ++vertextCount )
    {

      glVertex3d( vertextCount, 0, 0 );

    }

    glEnd();
    glEndList();

    return true;

  }
  GKG_CATCH( "bool gkg::AFiberObject::glMakeBodyGLL( "
             "const anatomist::ViewState& viewState, "
             "const anatomist::GLList& glList ) const" );

}


void gkg::AFiberObject::setRenderingOptions( double minimumValue,
                                             double maximumValue,
                                             double minimumRange,
                                             double maximumRange,
                                             int32_t samplingValue,
                                             std::string& dataType,
                                             std::string& fileNameData,
                                             std::string& fileNameVolume )
{

  try
  {

    if ( samplingValue != _samplingValue )
    {

      _samplingValue = samplingValue;
      clearReader();
      _aFiberReader->getFiberCoordinates( _samplingValue,
                                          _coordinates,
                                          _fiberCount,
                                          _bundleNames );

      updateDataValues();

    }
    else
    {

      if ( _dataType != dataType ||
           _minimumValue != minimumValue ||
           _maximumValue != maximumValue ||
           _minimumRange != minimumRange ||
           _maximumRange != maximumRange ||
           _fileNameData != fileNameData ||
           _fileNameVolume != fileNameVolume )
      {

        _fileNameData = fileNameData;
        _fileNameVolume = fileNameVolume;
        _minimumValue = minimumValue;
        _maximumValue = maximumValue;
        _minimumRange = minimumRange;
        _maximumRange = maximumRange;
        _dataType = dataType;

        updateDataValues();

      }

    }

    updateShaderCode();
    setShaderAttributes();

  }
  GKG_CATCH( "void gkg::AFiberObject::setRenderingOptions( "
             "double minimumValue,"
             "double maximumValue,"
             "std::string& dataType,"
             "std::string& fileNameData, "
             "std::string& fileNameVolume " );

}


int32_t gkg::AFiberObject::getClassType()
{

  return _classType;

}


#if ( ANATOMIST_VERSION_MAJOR >= 5 ) || \
    ( ( ANATOMIST_VERSION_MAJOR == 4 ) && \
      ( ANATOMIST_VERSION_MINOR >= 6 ) && \
      ( ANATOMIST_VERSION_TINY >= 0 ) )
std::list< anatomist::AObject* > gkg::AFiberObject::load(
#else
anatomist::AObject* gkg::AFiberObject::load(
#endif
                       const std::string& fileName,
                       anatomist::ObjectReader::PostRegisterList& /* regList */,
                       carto::Object /* options */ )
{

  try
  {

    std::setlocale( LC_NUMERIC, "C" );

    gkg::AFiberObject* fiberObject = new gkg::AFiberObject( fileName );

#if ( ANATOMIST_VERSION_MAJOR >= 5 ) || \
    ( ( ANATOMIST_VERSION_MAJOR == 4 ) && \
      ( ANATOMIST_VERSION_MINOR >= 6 ) && \
      ( ANATOMIST_VERSION_TINY >= 0 ) )
    std::list< anatomist::AObject* > objectList;

    objectList.push_back( fiberObject );

    return objectList;
#else
    return fiberObject;
#endif

  }
  GKG_CATCH( "anatomist::AObject* gkg::AFiberObject::load( "
             "const std::string& fileName, "
             "anatomist::ObjectReader::PostRegisterList& regList, "
             "carto::Object options )" );

}


void gkg::AFiberObject::loadReader()
{

  try
  {

    _referential = 0;

    _qGLShaderProgram = 0;
    _qGLShader = 0;

    _uniformLocationCoordinates = 0;
    _uniformLocationDataValues = 0;

    _uniformIsShaderedObject = 0;

    anatomist::AObjectPalette* palette = new anatomist::AObjectPalette( 
                           theAnatomist->palettes().find( "Rainbow1-fusion" ) );


#if ( ( ANATOMIST_VERSION_MAJOR == 5 ) && \
      ( ANATOMIST_VERSION_MINOR >= 1 ) && \
      ( ANATOMIST_VERSION_TINY >= 0 ) )

    _palette = palette->colors()->deepcopy();

#else

    _palette = new AimsData<AimsRGBA>( palette->colors()->clone() );

#endif


    _aFiberReader = new gkg::AFiberReader( _fileNameBundleMap );

    _aFiberReader->getFiberCoordinates( _samplingValue,
                                        _coordinates,
                                        _fiberCount,
                                        _bundleNames );

    _boundingBox = _aFiberReader->getBoundingBox();

    this->updateDataValues();

  }
  GKG_CATCH( "void gkg::AFiberObject::loadReader()" );

}


void gkg::AFiberObject::deleteReader()
{

  try
  {

    delete _qGLShader;
    delete _qGLShaderProgram;

    uint32_t i = 0;
    for ( i = 0; i < _coordinates.size(); ++i )
    {

      delete [] _coordinates[ i ];

    }

    _coordinates.clear();
    delete _aFiberReader;

  }
  GKG_CATCH( "void gkg::AFiberObject::deleteReader()" );

}

void gkg::AFiberObject::clearReader()
{

  try
  {

    uint32_t i = 0;
    for ( i = 0; i < _coordinates.size(); ++i )
    {

      delete [] _coordinates[ i ];

    }

    _coordinates.clear();
    _fiberCount.clear();
    _bundleNames.clear();

  }
  GKG_CATCH( "void gkg::AFiberObject::clearReader()" );

}


void gkg::AFiberObject::draw() const
{

  try
  {

    if ( _isOneValuePerBundleShaderType == true )
    {

      _qGLShaderProgram->setUniformValue( _uniformIsShaderedObject, 1 );
      uint32_t i = 0;
      uint32_t b = 0;

      for ( b = 0; b < _fiberCount.size(); ++b )
      {

        _qGLShaderProgram->setUniformValue( _uniformLocationDataValues,
                                            _dataValues[ b ][ 0 ] );
        uint32_t fiberBegin = 0;

        if ( b )
        {

          fiberBegin = _fiberCount[ b - 1 ];

        }

        uint32_t fiberEnd = fiberBegin + _fiberCount[ b ];

        for ( i = fiberBegin; i < fiberEnd; ++i )
        {

          _qGLShaderProgram->setUniformValue( _uniformPointCount,
                                              _samplingValue );
          _qGLShaderProgram->setUniformValueArray( _uniformLocationCoordinates,
                                                   _coordinates[ i ],
                                                   _samplingValue );
          _glList->callList();

        }

      }

    }
    else
    {

      _qGLShaderProgram->setUniformValue( _uniformIsShaderedObject, 1 );

      uint32_t i = 0;

      for ( i = 0; i < _coordinates.size(); ++i )
      {

        _qGLShaderProgram->setUniformValue( _uniformPointCount,
                                            _samplingValue );
        _qGLShaderProgram->setUniformValueArray( _uniformLocationDataValues,
                                                 _dataValues[ i ],
                                                 _samplingValue );
        _qGLShaderProgram->setUniformValueArray( _uniformLocationCoordinates,
                                                 _coordinates[ i ],
                                                 _samplingValue );

        _glList->callList();

      }

    }

  }
  GKG_CATCH( "void gkg::AFiberObject::draw() const" );

}


void gkg::AFiberObject::updateShaderCode()
{

  try
  {

    if ( _qGLShader )
    {

      delete _qGLShader;
      delete _qGLShaderProgram;

    }

    _qGLShader = new QGLShader( QGLShader::Vertex );

    if ( _isOneValuePerBundleShaderType )
    {

      _qGLShader->compileSourceCode(
         gkg::shaderCodeBundle(
                   gkg::StringConverter::toString( _samplingValue ) ).c_str() );

    }
    else
    {

      _qGLShader->compileSourceCode(
         gkg::shaderCodeProfile(
                   gkg::StringConverter::toString( _samplingValue ) ).c_str() );

    }

    _qGLShaderProgram = new QGLShaderProgram();
    _qGLShaderProgram->addShader( _qGLShader );
    _qGLShaderProgram->link();

  }
  GKG_CATCH( "void gkg::AFiberObject::updateShaderCode()" );

}


void gkg::AFiberObject::setShaderAttributes()
{

  try
  {

    _qGLShaderProgram->bind();

    _uniformIsShaderedObject = _qGLShaderProgram->uniformLocation( 
                                                           "isShaderedObject" );
    _uniformPointCount = _qGLShaderProgram->uniformLocation( "pointCount" );
    _uniformLocationCoordinates = _qGLShaderProgram->uniformLocation(
                                                                "coordinates" );
    _uniformLocationDataValues = _qGLShaderProgram->uniformLocation(
                                                                  "dataValue" );

    _qGLShaderProgram->release();
    _glList = new anatomist::GLList();

  }
  GKG_CATCH( "void gkg::AFiberObject::setShaderAttributes()" );

}


int32_t gkg::AFiberObject::registerClass()
{

  try
  {

    anatomist::ObjectReader::registerLoader( "bundlemap", load );
    _classType = registerObjectType( "BUNDLEMAP" );

    return _classType;

  }
  GKG_CATCH( "int32_t gkg::AFiberObject::registerClass()" );

}


void gkg::AFiberObject::updateDataValues()
{

  //clear previous data values
  uint32_t i = 0;
  std::vector< std::pair< std::string, int32_t > > tmpBundleNames = 
               std::vector< std::pair< std::string, int32_t > >( _bundleNames );

  for ( i = 0; i < _dataValues.size(); ++i )
  {

    delete [] _dataValues[ i ];

  }

  _dataValues.clear();
  bool noFileFound = false;

  if ( _dataType == "orientation" )
  {

    _isOneValuePerBundleShaderType = false;
    uint32_t fiberIndex = 0;
    int32_t pointIndex = 0;

    for ( fiberIndex = 0; fiberIndex < _coordinates.size(); ++fiberIndex )
    {

      _dataValues.push_back(  new QVector3D[ _samplingValue ] );

      float x = 0.0;
      float y = 0.0;
      float z = 0.0;

      x = std::abs( _coordinates[ fiberIndex ][ 0 ].x() - 
                                          _coordinates[ fiberIndex ][ 1 ].x() );
      y = std::abs( _coordinates[ fiberIndex ][ 0 ].y() - 
                                          _coordinates[ fiberIndex ][ 1 ].y() );
      z = std::abs( _coordinates[ fiberIndex ][ 0 ].z() - 
                                          _coordinates[ fiberIndex ][ 1 ].z() );

      float sum = x + y + z;
      x = x / sum;
      y = y / sum;
      z = z / sum;

      _dataValues[ fiberIndex ][ 0 ].setX( x );
      _dataValues[ fiberIndex ][ 0 ].setY( y );
      _dataValues[ fiberIndex ][ 0 ].setZ( z );
      _dataValues[ fiberIndex ][ 1 ].setX( x );
      _dataValues[ fiberIndex ][ 1 ].setY( y );
      _dataValues[ fiberIndex ][ 1 ].setZ( z );

      for ( pointIndex = 2; pointIndex < _samplingValue; ++pointIndex )
      {

        x = std::abs( _coordinates[ fiberIndex ][ pointIndex - 1 ].x() - 
                                 _coordinates[ fiberIndex ][ pointIndex ].x() );
        y = std::abs( _coordinates[ fiberIndex ][ pointIndex - 1 ].y() - 
                                 _coordinates[ fiberIndex ][ pointIndex ].y() );
        z = std::abs( _coordinates[ fiberIndex ][ pointIndex - 1 ].z() - 
                                 _coordinates[ fiberIndex ][ pointIndex ].z() );

        sum = x + y + z;
        x = x / sum;
        y = y / sum;
        z = z / sum;

        _dataValues[ fiberIndex ][ pointIndex ].setX( x );
        _dataValues[ fiberIndex ][ pointIndex ].setY( y );
        _dataValues[ fiberIndex ][ pointIndex ].setZ( z );

      }

    }

  }

  if ( _dataType == "length" )
  {

    _isOneValuePerBundleShaderType = false;
    double minimumValue = std::numeric_limits< double >::max();
    double maximumValue = std::numeric_limits< double >::lowest();
    std::vector< float > dataValues( _coordinates.size() );
    uint32_t fiberIndex = 0;
    int32_t pointIndex = 0;

    for ( fiberIndex = 0; fiberIndex < _coordinates.size(); ++fiberIndex )
    {

      float length = 0.0;
      int32_t pointEnd = _samplingValue - 1;

      for ( pointIndex = 0; pointIndex < pointEnd; ++pointIndex )
      {

        float distance = 
           std::sqrt( std::pow( _coordinates[ fiberIndex ][ pointIndex ].x() - 
                        _coordinates[ fiberIndex ][ pointIndex + 1 ].x(), 2 ) + 
                      std::pow( _coordinates[ fiberIndex ][ pointIndex ].y() - 
                        _coordinates[ fiberIndex ][ pointIndex + 1 ].y(), 2 ) + 
                      std::pow( _coordinates[ fiberIndex ][ pointIndex ].z() - 
                        _coordinates[ fiberIndex ][ pointIndex + 1 ].z(), 2 ) );
        length += distance;

      }

      dataValues[ fiberIndex ] = length;

      if ( length < minimumValue )
      {

        minimumValue = ( double )length;

      }
      if ( length > maximumValue )
      {

        maximumValue = ( double )length;

      }

    }

    if ( _minimumRange == std::numeric_limits< double >::lowest() )
    {

       _minimumRange = minimumValue;

    }
    if ( _maximumRange == std::numeric_limits< double >::max() )
    {

       _maximumRange = maximumValue;

    }
    if ( _minimumValue != std::numeric_limits< double >::lowest() )
    {

      minimumValue = _minimumValue;

    }
    if ( _maximumValue != std::numeric_limits< double >::max() )
    {

      maximumValue = _maximumValue;

    }

    for ( i = 0; i < dataValues.size(); ++i )
    {

      int32_t v = 0;
      _dataValues.push_back( new QVector3D[ _samplingValue ] );

      for ( v = 0; v < _samplingValue; ++v )
      {

        int32_t value = ( dataValues[ i ] - minimumValue ) * 255 /
                        ( maximumValue - minimumValue );

        if ( value <= 0 )
        {

          value = 1;

        }
        if ( value > 255 )
        {

          value = 255;

        }

#if ( ( ANATOMIST_VERSION_MAJOR == 5 ) && \
      ( ANATOMIST_VERSION_MINOR >= 1 ) && \
      ( ANATOMIST_VERSION_TINY >= 0 ) )

        _dataValues[ i ][ v ].setX( _palette( value )[ 0 ] / 255.0 );
        _dataValues[ i ][ v ].setY( _palette( value )[ 1 ] / 255.0 );
        _dataValues[ i ][ v ].setZ( _palette( value )[ 2 ] / 255.0 );

#else

        _dataValues[ i ][ v ].setX( ( *_palette )( value )[ 0 ] / 255.0 );
        _dataValues[ i ][ v ].setY( ( *_palette )( value )[ 1 ] / 255.0 );
        _dataValues[ i ][ v ].setZ( ( *_palette )( value )[ 2 ] / 255.0 );

#endif

      }

    }

  }

  if ( _dataType == "profile-along-bundle" )
  {

    std::ifstream datafile( _fileNameData.c_str(), std::ios::in );
    double minimumValue = std::numeric_limits< double >::max();
    double maximumValue = std::numeric_limits< double >::lowest();
    std::vector< std::vector< float > > dataValues;
    double dataValue = 0.0;

    gkg::File fileVolume( _fileNameVolume );

    if ( datafile.fail() || 
         ( !fileVolume.isExisting() || fileVolume.isDirectory() ) )
    {

      noFileFound = true;

      if ( datafile.fail() )
      {

        std::cout << "Data file: " << _fileNameData << " not found."
                  << std::endl << std::flush; 

      }
      if ( !fileVolume.isExisting() )
      {

        std::cout << "Volume file: " << _fileNameVolume << " not existing."
                  << std::endl << std::flush; 

      }

    }
    else
    {

      // read scalar volume
      gkg::Volume< int32_t > scalarVolume;
      gkg::Reader::getInstance().read(  _fileNameVolume, scalarVolume );
      gkg::Vector3d< double > scalarVolumeResolution;
      scalarVolume.getResolution( scalarVolumeResolution );
      gkg::Vector3d< int32_t > scalarVolumeSize;
      scalarVolume.getSize( scalarVolumeSize );

      std::string bundleName = "";
      std::string profileType = "";
      int32_t bundleIndex = 0;
      int32_t valueCount = 0;
      int32_t bundleNameIndex = -1;

      datafile >> bundleName >> profileType >> bundleIndex >> valueCount;

      if ( valueCount == 0 )
      {

        dataValues.resize( _bundleNames.size() );

      }
      else
      {

        dataValues.resize( _coordinates.size() );

      }

      int32_t bundleCount = 0;

      while ( datafile )
      {

        // find bundleName index
        bundleNameIndex = -1;

        for ( bundleNameIndex = 0; 
              bundleNameIndex < int32_t( tmpBundleNames.size() );
              ++bundleNameIndex )
        {

          if ( tmpBundleNames[ bundleNameIndex ].first == bundleName )
          {

            break;

          }

        }
        if ( bundleNameIndex >= int32_t( tmpBundleNames.size() ) )
        {

          bundleNameIndex = bundleCount;

        }

        if ( bundleNameIndex != -1 )
        {

          std::vector< std::string > values( valueCount );
          int32_t valueIndex = 0;

          for ( valueIndex = 0; valueIndex < valueCount; ++valueIndex )
          {

            datafile >> values[ valueIndex ];
 
            if ( values[ valueIndex ] != "null" )
            {

              gkg::StringConverter::toScalar( values[ valueIndex ], dataValue );

              if ( dataValue < minimumValue )
              {

                minimumValue = ( double )dataValue;

              }
              if ( dataValue > maximumValue )
              {

                maximumValue = ( double )dataValue;

              }

            }

          }
          if ( _minimumRange == std::numeric_limits< double >::lowest() )
          {

            _minimumRange = minimumValue;

          }
          if ( _maximumRange == std::numeric_limits< double >::max() )
          {

            _maximumRange = maximumValue;

          }

          if ( _minimumValue != std::numeric_limits< double >::lowest() )
          {

            minimumValue = _minimumValue;

          }
          if ( _maximumValue != std::numeric_limits< double >::max() )
          {

            maximumValue = _maximumValue;

          }

          int32_t fiberIndex = _bundleNames[ bundleNameIndex ].second;

          uint32_t i = 0;

          // one value per bundle
          if ( valueCount == 1 )
          {

            _isOneValuePerBundleShaderType = true;
            gkg::StringConverter::toScalar( values[ 0 ], dataValue );
            dataValues[ bundleNameIndex ].push_back( dataValue );

          }
          else
          {

            _isOneValuePerBundleShaderType = false;
            uint32_t bundleEnd = fiberIndex + _fiberCount[ bundleNameIndex ];
            int32_t val = 0;
            int32_t pointIndex;
            float scalarVal = 0.0;

            for ( i = fiberIndex; i < bundleEnd ; ++i )
            { 

              for ( pointIndex = 0; pointIndex < _samplingValue; ++pointIndex )
              {

                val = scalarVolume( _coordinates[ i ][ pointIndex ].x() /
                                    scalarVolumeResolution.x + 0.5,
                                    _coordinates[ i ][ pointIndex ].y() /
                                    scalarVolumeResolution.y + 0.5,
                                    _coordinates[ i ][ pointIndex ].z() /
                                    scalarVolumeResolution.z + 0.5 );
 
                if ( values[ val ] == "null" )
                {

                  dataValues[ i ].push_back( minimumValue ) ;

                }
                else
                {

                  gkg::StringConverter::toScalar( values[ val ], scalarVal );
                  dataValues[ i ].push_back( scalarVal );

                }

              }

            }

          }

        }

        datafile >> bundleName >> profileType >> bundleNameIndex >> valueCount;
        ++bundleCount;

      }

      datafile.close();

    }

    for ( i = 0; i < dataValues.size(); ++i )
    {

      int32_t v = 0;

      if ( dataValues.size() == 0 )
      {

        _dataValues.push_back( new QVector3D[ _samplingValue ] );

        for ( v = 0; v < _samplingValue; ++v )
        {


#if ( ( ANATOMIST_VERSION_MAJOR == 5 ) && \
      ( ANATOMIST_VERSION_MINOR >= 1 ) && \
      ( ANATOMIST_VERSION_TINY >= 0 ) )

        _dataValues[ i ][ v ].setX( _palette( minimumValue )[ 0 ] / 
                                    255.0 );
        _dataValues[ i ][ v ].setY( _palette( minimumValue )[ 1 ] / 
                                    255.0 );
        _dataValues[ i ][ v ].setZ( _palette( minimumValue )[ 2 ] / 
                                    255.0 );

#else

        _dataValues[ i ][ v ].setX( ( *_palette )( minimumValue )[ 0 ] / 
                                    255.0 );
        _dataValues[ i ][ v ].setY( ( *_palette )( minimumValue )[ 1 ] / 
                                    255.0 );
        _dataValues[ i ][ v ].setZ( ( *_palette )( minimumValue )[ 2 ] / 
                                    255.0 );

#endif

        }

      }
      else
      {

        _dataValues.push_back( new QVector3D[ dataValues[ i ].size() ] );

        for ( v = 0; v < int32_t( dataValues[ i ].size() ); ++v )
        {

          int32_t value = ( dataValues[ i ][ v ] - minimumValue ) * 255 /
                          ( maximumValue - minimumValue );

          if ( value <= 0 )
          {

            value = 1;

          }
          if ( value > 255 )
          {

            value = 255;

          }

#if ( ( ANATOMIST_VERSION_MAJOR == 5 ) && \
      ( ANATOMIST_VERSION_MINOR >= 1 ) && \
      ( ANATOMIST_VERSION_TINY >= 0 ) )

          _dataValues[ i ][ v ].setX( _palette( value )[ 0 ] / 255.0 );
          _dataValues[ i ][ v ].setY( _palette( value )[ 1 ] / 255.0 );
          _dataValues[ i ][ v ].setZ( _palette( value )[ 2 ] / 255.0 );

#else

          _dataValues[ i ][ v ].setX( ( *_palette )( value )[ 0 ] / 255.0 );
          _dataValues[ i ][ v ].setY( ( *_palette )( value )[ 1 ] / 255.0 );
          _dataValues[ i ][ v ].setZ( ( *_palette )( value )[ 2 ] / 255.0 );

#endif

        }

      }

    }

  }

  if ( _dataType == "color-along-bundle" )
  {

    std::ifstream datafile( _fileNameData.c_str(), std::ios::in );

    if ( datafile.fail() )
    {

      noFileFound = true;
      std::cout << "Data file: " << _fileNameData << " not found."
                << std::endl << std::flush; 

    }
    else
    {

      std::string bundleName = "";
      std::string profileType = "";
      int32_t bundleIndex = 0;
      int32_t valueCount = 0;
      int32_t bundleNameIndex = -1;

      datafile >> bundleName >> profileType >> bundleIndex >> valueCount;

      _dataValues.resize( _coordinates.size() );
      _isOneValuePerBundleShaderType = true;

      while ( datafile )
      {

        if ( valueCount != 3 )
        {

          throw std::runtime_error( "Value count should be 3" );

        }

        // find bundleName index
        bundleNameIndex = -1;

        for ( bundleNameIndex = 0; 
              bundleNameIndex < int32_t( tmpBundleNames.size() );
              ++bundleNameIndex )
        {

          if ( tmpBundleNames[ bundleNameIndex ].first == bundleName )
          {

  
            break;
          }

        }

        if ( bundleNameIndex != -1 )
        {

          _dataValues[ bundleNameIndex ] = new QVector3D[ 1 ];
          float value = 0;
          datafile >> value;
          _dataValues[ bundleNameIndex ][ 0 ].setX( value / 255.0 );
          datafile >> value;
          _dataValues[ bundleNameIndex ][ 0 ].setY( value / 255.0 );
          datafile >> value;
          _dataValues[ bundleNameIndex ][ 0 ].setZ( value / 255.0 );

        }

        datafile >> bundleName >> profileType >> bundleNameIndex >> valueCount;

      }

      datafile.close();
    
      for ( bundleNameIndex = 0; 
            bundleNameIndex < int32_t( tmpBundleNames.size() );
            ++bundleNameIndex )
      {

        _dataValues[ bundleNameIndex ] = new QVector3D[ 1 ];
        float value = 0;
        datafile >> value;
        _dataValues[ bundleNameIndex ][ 0 ].setX( 0.0 );
        datafile >> value;
        _dataValues[ bundleNameIndex ][ 0 ].setY( 0.0 );
        datafile >> value;
        _dataValues[ bundleNameIndex ][ 0 ].setZ( 0.0 );

      }

    }

  }

  if ( noFileFound || ( _dataType == "no-data" ) )
  {

    _isOneValuePerBundleShaderType = true;
    uint32_t fiberIndex = 0;

    for ( fiberIndex = 0; fiberIndex < _coordinates.size(); ++fiberIndex )
    {

      _dataValues.push_back( new QVector3D[ 1 ] );
      _dataValues[ fiberIndex ][ 0 ].setX( 0 );
      _dataValues[ fiberIndex ][ 0 ].setY( 0.2 );
      _dataValues[ fiberIndex ][ 0 ].setZ( 0.8 );

    }

  }

}


double gkg::AFiberObject::getMinimumValue() const
{

  return _minimumValue;

}


double gkg::AFiberObject::getMaximumValue() const
{

  return _maximumValue;

}


double gkg::AFiberObject::getMinimumRange() const
{

  return _minimumRange;

}

double gkg::AFiberObject::getMaximumRange() const
{

  return _maximumRange;

}


int32_t gkg::AFiberObject::getSamplingValue() const
{

  return _samplingValue;

}


std::string gkg::AFiberObject::getFileNameData() const
{

  return _fileNameData;

}


std::string gkg::AFiberObject::getFileNameVolume() const
{

  return _fileNameVolume;

}


std::string gkg::AFiberObject::getDataType() const
{

  return _dataType;

}
