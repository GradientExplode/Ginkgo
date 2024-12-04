#include <gkg-anatomist-plugin-objects/ARealTimeFiberObject.h>
#include <gkg-anatomist-plugin-objects/AFiberShader.h>
#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-dmri-container/BundleMap_i.h>

#include <anatomist/object/oReader.h>
#include <anatomist/object/actions.h>
#include <anatomist/control/qObjTree.h>
#include <anatomist/window/Window.h>
#include <aims/resampling/quaternion.h>

#include <iostream>
#include <cmath>
#include <limits>


int32_t gkg::ARealTimeFiberObject::_classType = 
                                     gkg::ARealTimeFiberObject::registerClass();
Tree* gkg::ARealTimeFiberObject::_optionTree = 0;


gkg::ARealTimeFiberObject::ARealTimeFiberObject( 
                                           gkg::BundleMap< int16_t >& bundleMap,
                                           int32_t opacity )
                         : _bundleMap( bundleMap ),
                           _dataType( "no-data" ),
                           _glList( 0 ),
                           _qGLShaderProgram( 0 ),
                           _qGLShader( 0 ),
                           _samplingValue( 60 ),
                           _minimumValue( 
                                      std::numeric_limits< double >::lowest() ),
                           _maximumValue( 
                                      std::numeric_limits< double >::max() ),
                           _minimumRange( 
                                      std::numeric_limits< double >::lowest() ),
                           _maximumRange( 
                                      std::numeric_limits< double >::max() )
{

  try
  {

    // adding icon
    _type = _classType;
    _referential = 0;

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

      QObjectTree::TypeNames[ _classType ] = "Real-Time BUNDLEMAP";

    }

    updateBoundingBox();

    _material.SetDiffuseA( 0.01f * float( opacity ) );

  }
  GKG_CATCH( "gkg::AFiberObject::AFiberObject( "
             "gkg::BundleMap< int16_t >& bundleMap )");

}


gkg::ARealTimeFiberObject::~ARealTimeFiberObject()
{

  cleanup();
  clear();

  delete _glList;
  delete _qGLShader;
  delete _qGLShaderProgram;

}


bool gkg::ARealTimeFiberObject::Is2DObject()
{

  return true;

}


bool gkg::ARealTimeFiberObject::Is3DObject()
{

  return true;

}


Tree* gkg::ARealTimeFiberObject::optionTree() const
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

    }

    return( _optionTree );

  }
  GKG_CATCH( "Tree* gkg::ARealTimeFiberObject::optionTree() const" );

}


int gkg::ARealTimeFiberObject::CanBeDestroyed()
{

  try
  {

    return 0;

  }
  GKG_CATCH( "int gkg::ARealTimeFiberObject::CanBeDestroyed()" );

}


float gkg::ARealTimeFiberObject::MinX2D() const
{

  try
  {

    return _boundingBox.getLowerX();

  }
  GKG_CATCH( "float gkg::ARealTimeFiberObject::MinX2D() const" );

}


float gkg::ARealTimeFiberObject::MinY2D() const
{

  try
  {

    return _boundingBox.getLowerY();

  }
  GKG_CATCH( "float gkg::ARealTimeFiberObject::MinY2D() const" );

}


float gkg::ARealTimeFiberObject::MinZ2D() const
{

  try
  {

    return _boundingBox.getLowerZ();

  }
  GKG_CATCH( "float gkg::ARealTimeFiberObject::MinZ2D() const" );

}


float gkg::ARealTimeFiberObject::MaxX2D() const
{

  try
  {

    return _boundingBox.getUpperX();

  }
  GKG_CATCH( "float gkg::ARealTimeFiberObject::MaxX2D() const" );

}


float gkg::ARealTimeFiberObject::MaxY2D() const
{

  try
  {

    return _boundingBox.getUpperY();

  }
  GKG_CATCH( "float gkg::ARealTimeFiberObject::MaxY2D() const" );

}


float gkg::ARealTimeFiberObject::MaxZ2D() const
{

  try
  {

    return _boundingBox.getUpperZ();

  }
  GKG_CATCH( "float gkg::ARealTimeFiberObject::MaxZ2D() const" );

}


bool gkg::ARealTimeFiberObject::boundingBox( std::vector<float>& bmin,
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
  GKG_CATCH( "bool ARealTimeFiberObject::boundingBox("
             "Point3df & bmin,"
             "Point3df & bmax ) const" );

}


void gkg::ARealTimeFiberObject::updateBoundingBox()
{

  try
  {

    int32_t sizeX = 1;
    int32_t sizeY = 1;
    int32_t sizeZ = 1;
    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );

    if ( _bundleMap.getHeader().hasAttribute( "sizeX" ) )
    {

      _bundleMap.getHeader().getAttribute( "sizeX", sizeX );

    }
    if ( _bundleMap.getHeader().hasAttribute( "sizeY" ) )
    {

      _bundleMap.getHeader().getAttribute( "sizeY", sizeY );

    }
    if ( _bundleMap.getHeader().hasAttribute( "sizeZ" ) )
    {

      _bundleMap.getHeader().getAttribute( "sizeZ", sizeZ );

    }
    if ( _bundleMap.getHeader().hasAttribute( "resolutionX" ) )
    {

      _bundleMap.getHeader().getAttribute( "resolutionX", resolution.x );

    }
    if ( _bundleMap.getHeader().hasAttribute( "resolutionY" ) )
    {

      _bundleMap.getHeader().getAttribute( "resolutionY", resolution.y );

    }
    if ( _bundleMap.getHeader().hasAttribute( "resolutionZ" ) )
    {

      _bundleMap.getHeader().getAttribute( "resolutionZ", resolution.z );

    }

    _boundingBox = gkg::BoundingBox< float >( 
                                          -resolution.x,
                                          ( sizeX + 1 ) * resolution.x,
                                          -resolution.y,
                                          ( sizeY + 1 ) * resolution.y,
                                          -resolution.z, 
                                          ( sizeZ + 1 ) * resolution.z );

  }
  GKG_CATCH( "void gkg::ARealTimeFiberObject::updateBoundingBox()" );

}


void gkg::ARealTimeFiberObject::update()
{

  clear();
  getFiberCoordinates();
  updateDataValues();

}


void gkg::ARealTimeFiberObject::setOpacity( int32_t opacity )
{

  if ( ( opacity >= 0 ) && ( opacity <= 100 ) )
  {
  
    _material.SetDiffuseA( 0.01f * float( opacity ) );

  }

}


bool gkg::ARealTimeFiberObject::render( anatomist::PrimList& primitiveList,
                                        const anatomist::ViewState& viewState )
{

  try
  {

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
  GKG_CATCH( "bool gkg::ARealTimeFiberObject::render( "
             "anatomist::PrimList& primitiveList, "
             "const anatomist::ViewState& viewState )" );

}


bool gkg::ARealTimeFiberObject::glMakeBodyGLL( 
                                    const anatomist::ViewState& /* viewState */,
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
  GKG_CATCH( "bool gkg::ARealTimeFiberObject::glMakeBodyGLL( "
             "const anatomist::ViewState& viewState, "
             "const anatomist::GLList& glList ) const" );

}


void gkg::ARealTimeFiberObject::setRenderingOptions( 
                                             double minimumValue,
                                             double maximumValue,
                                             double minimumRange,
                                             double maximumRange,
                                             int32_t samplingValue,
                                             std::string& dataType,
                                             std::string& /* fileNameData */,
                                             std::string& /* fileNameVolume */ )
{

  try
  {

    if ( samplingValue != _samplingValue )
    {

      _samplingValue = samplingValue;
      update();

    }
    else
    {

      if ( _dataType != dataType ||
           _minimumValue != minimumValue ||
           _maximumValue != maximumValue ||
           _minimumRange != minimumRange ||
           _maximumRange != maximumRange )
      {

        _minimumValue = minimumValue;
        _maximumValue = maximumValue;
        _minimumRange = minimumRange;
        _maximumRange = maximumRange;
        _dataType = dataType;

        clearDataValues();
        updateDataValues();

      }

    }

    updateShaderCode();
    setShaderAttributes();

  }
  GKG_CATCH( "void gkg::ARealTimeFiberObject::setRenderingOptions( "
             "double minimumValue,"
             "double maximumValue,"
             "std::string& dataType,"
             "std::string& fileNameData, "
             "std::string& fileNameVolume " );

}


int32_t gkg::ARealTimeFiberObject::getClassType()
{

  return _classType;

}


void gkg::ARealTimeFiberObject::getFiberCoordinates()
{

  try
  {

    int32_t fiberIndex = 0;
    int32_t fiberCount;
    gkg::BundleMap< int16_t >::const_iterator
      b = _bundleMap.begin(),
      be = _bundleMap.end();

    while ( b != be )
    {

      const gkg::BundleMap< int16_t >::Bundle& bundle = b->second;
      gkg::Curve3dMap< float >::const_iterator f = bundle.begin(),
                                               fe = bundle.end();
      fiberCount = 0;

      while ( f != fe )
      {

        const gkg::LightCurve3d< float >& fiber = f->getEquidistantCurve( 
                                                               _samplingValue );

        gkg::LightCurve3d< float >::const_iterator
          s = fiber.begin(),
          se = fiber.end();
        _coordinates.push_back( new QVector3D[ _samplingValue ] );
        int32_t pointIndex = 0;

        while ( s != se )
        {

          _coordinates[ fiberIndex ][ pointIndex ].setX( s->x );
          _coordinates[ fiberIndex ][ pointIndex ].setY( s->y );
          _coordinates[ fiberIndex ][ pointIndex ].setZ( s->z );

          ++ pointIndex;
          ++ s;

        }

        ++ fiberCount;
        ++ fiberIndex;
        ++ f;

      }

      _fiberCount.push_back( fiberCount );
      ++ b;

    }

  }
  GKG_CATCH( "void gkg::ARealTimeFiberObject::getFiberCoordinates()" );

}


void gkg::ARealTimeFiberObject::clear()
{

  try
  {

    std::vector< QVector3D* >::iterator
      v = _coordinates.begin(),
      ve = _coordinates.end();

    while ( v != ve )
    {

      delete [] *v;

      ++v;

    }

    _coordinates.clear();
    _fiberCount.clear();

    clearDataValues();

  }
  GKG_CATCH( "void gkg::ARealTimeFiberObject::clear()" );

}


void gkg::ARealTimeFiberObject::clearDataValues()
{

  try
  {

    std::vector< QVector3D* >::iterator
      d = _dataValues.begin(),
      de = _dataValues.end();

    while ( d != de )
    {

      delete [] *d;

      ++d;

    }

    _dataValues.clear();

  }
  GKG_CATCH( "void gkg::ARealTimeFiberObject::clearDataValues()" );

}


void gkg::ARealTimeFiberObject::draw() const
{

  try
  {

    _qGLShaderProgram->setUniformValue( _uniformIsShaderedObject, 1 );
    int32_t i = 0;
    int32_t b = 0, bEnd = _fiberCount.size();

    for ( b = 0; b < bEnd; ++b )
    {

      _qGLShaderProgram->setUniformValue( _uniformLocationDataValues,
  					  _dataValues[ b ][ 0 ] );
      int32_t fiberBegin = 0;

      if ( b )
      {

  	fiberBegin = _fiberCount[ b - 1 ];

      }

      int32_t fiberEnd = fiberBegin + _fiberCount[ b ];

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
  GKG_CATCH( "void gkg::ARealTimeFiberObject::draw() const" );

}


void gkg::ARealTimeFiberObject::updateShaderCode()
{

  try
  {

    if ( _qGLShader )
    {

      delete _qGLShader;
      delete _qGLShaderProgram;

    }

    _qGLShader = new QGLShader( QGLShader::Vertex );

    _qGLShader->compileSourceCode(
      gkg::shaderCodeBundle(
                   gkg::StringConverter::toString( _samplingValue ) ).c_str() );

    _qGLShaderProgram = new QGLShaderProgram();
    _qGLShaderProgram->addShader( _qGLShader );
    _qGLShaderProgram->link();

  }
  GKG_CATCH( "void gkg::ARealTimeFiberObject::updateShaderCode()" );

}


void gkg::ARealTimeFiberObject::setShaderAttributes()
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

    if ( _glList )
    {

      delete _glList;

    }
    
    _glList = new anatomist::GLList();

  }
  GKG_CATCH( "void gkg::ARealTimeFiberObject::setShaderAttributes()" );

}


int32_t gkg::ARealTimeFiberObject::registerClass()
{

  try
  {

    _classType = registerObjectType( "RealTimeBUNDLEMAP" );

#ifdef GKG_DEBUG

    std::cout << "Real-time BUNDLEMAP registered : " << _classType << std::endl;

#endif

    return _classType;

  }
  GKG_CATCH( "int32_t gkg::ARealTimeFiberObject::registerClass()" );

}


void gkg::ARealTimeFiberObject::updateDataValues()
{

  int32_t i = _coordinates.size();

  while ( i-- )
  {

    _dataValues.push_back( new QVector3D( 0.0, 0.2, 0.8 ) );

  }

}


double gkg::ARealTimeFiberObject::getMinimumValue() const
{

  return _minimumValue;

}


double gkg::ARealTimeFiberObject::getMaximumValue() const
{

  return _maximumValue;

}


double gkg::ARealTimeFiberObject::getMinimumRange() const
{

  return _minimumRange;

}

double gkg::ARealTimeFiberObject::getMaximumRange() const
{

  return _maximumRange;

}


int32_t gkg::ARealTimeFiberObject::getSamplingValue() const
{

  return _samplingValue;

}


std::string gkg::ARealTimeFiberObject::getFileNameData() const
{

  return "";

}


std::string gkg::ARealTimeFiberObject::getFileNameVolume() const
{

  return "";

}


std::string gkg::ARealTimeFiberObject::getDataType() const
{

  return _dataType;

}
