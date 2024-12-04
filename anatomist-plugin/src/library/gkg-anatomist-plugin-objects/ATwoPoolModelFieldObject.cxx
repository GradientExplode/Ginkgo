#include <gkg-anatomist-plugin-objects/ATwoPoolModelFieldObject.h>
#include <gkg-anatomist-plugin-objects/VertexShader.h>
#include <gkg-anatomist-plugin-rendering-options/ATwoPoolModelFieldRenderingOptionsWidget.h>
#include <gkg-anatomist-plugin-io/ATwoPoolModelFieldReader.h>
#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>

#include <anatomist/object/oReader.h>
#include <anatomist/object/actions.h>
#include <anatomist/control/qObjTree.h>
#include <anatomist/window/Window.h>
#include <aims/resampling/quaternion.h>

#include <iostream>
#include <cmath>

#include <QFileDialog>
#include <QPixmap>


#define DEFAULT_OUTPUT_ORIENTATION_COUNT 300
#define DEFAULT_MESH_SCALE 1.0

int32_t gkg::ATwoPoolModelFieldObject::_classType =
  gkg::ATwoPoolModelFieldObject::registerClass();
Tree* gkg::ATwoPoolModelFieldObject::_optionTree = 0;


gkg::ATwoPoolModelFieldObject::ATwoPoolModelFieldObject( 
                                       const std::string& fileNameSiteMap,
                                       const std::string& fileNameTextureMap )
                              : _fileNameSiteMap( fileNameSiteMap ),
                                _fileNameTextureMap( fileNameTextureMap ),
                                _outputOrientationCount(
                                             DEFAULT_OUTPUT_ORIENTATION_COUNT ),
                                _meshScale( DEFAULT_MESH_SCALE ),
                                _hasApproximatedNormals( true ),
                                _hasHiddenShadows( false ),
                                _isDisplayed( false ),
                                _glList( 0 ) 
{

  try
  {

    if ( !_fileNameSiteMap.empty() )
    {

      // initializing the plugin(s)
      gkg::PluginLoader::getInstance().load( false );

      // adding icon
      _type = _classType;
      if ( QObjectTree::TypeNames.find( _classType ) ==
           QObjectTree::TypeNames.end() )
      {

        std::string
          iconPath = gkg::ConfigurationInfo::getInstance().getIconPath(
                                                    "icon-two-pool-model.png" );
        if ( !QObjectTree::TypeIcons[ _classType ].load( iconPath.c_str() ) )
        {

          QObjectTree::TypeIcons.erase( _classType );
          std::cerr << "icon " << iconPath << " not found!" << std::endl;

        }
        QObjectTree::TypeNames[ _classType ] = "GKG Two Pool Model Field";

      }

      // setting ODF reader
      loadReader();

    }

  }
  GKG_CATCH( "gkg::ATwoPoolModelFieldObject::ATwoPoolModelFieldObject( "
             "const std::string& fileNameSiteMap, "
             "const std::string& fileNameTextureMap )" );

}


gkg::ATwoPoolModelFieldObject::~ATwoPoolModelFieldObject()
{

  cleanup();
  deleteReader();

}


bool gkg::ATwoPoolModelFieldObject::Is2DObject()
{

  return true;

}


bool gkg::ATwoPoolModelFieldObject::Is3DObject()
{

  return false;

}


float gkg::ATwoPoolModelFieldObject::getMeshScale() const
{

  return _meshScale;

}


int32_t gkg::ATwoPoolModelFieldObject::getOutputOrientationCount() const
{

  return _outputOrientationCount;

}



bool gkg::ATwoPoolModelFieldObject::hasApproximatedNormals() const
{

  return _hasApproximatedNormals;

}


bool gkg::ATwoPoolModelFieldObject::hasHiddenShadows() const
{

  return _hasHiddenShadows;

}


bool gkg::ATwoPoolModelFieldObject::areRealNormalsAlreadyComputed() const
{

  return _realNormalsAlreadyComputed;

}


Tree* gkg::ATwoPoolModelFieldObject::optionTree() const
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
                       &gkg::ATwoPoolModelFieldRenderingOptionsWidget::create );
      t->insert( t2 );

    }
    return( _optionTree );

  }
  GKG_CATCH( "Tree* gkg::ATwoPoolModelFieldObject::optionTree() const" );

}


float gkg::ATwoPoolModelFieldObject::MinX2D() const
{

  try
  {

    return _boundingBox.getLowerX() - _resolution.x / 2;

  }
  GKG_CATCH( "float gkg::ATwoPoolModelFieldObject::MinX2D() const" );

}


float gkg::ATwoPoolModelFieldObject::MinY2D() const
{

  try
  {


    return _boundingBox.getLowerY() - _resolution.y / 2;

  }
  GKG_CATCH( "float gkg::ATwoPoolModelFieldObject::MinY2D() const" );

}


float gkg::ATwoPoolModelFieldObject::MinZ2D() const
{

  try
  {

    return _boundingBox.getLowerZ() - _resolution.z / 2;

  }
  GKG_CATCH( "float gkg::ATwoPoolModelFieldObject::MinZ2D() const" );

}


float gkg::ATwoPoolModelFieldObject::MaxX2D() const
{

  try
  {

    return _boundingBox.getUpperX() + _resolution.x / 2;

  }
  GKG_CATCH( "float gkg::ATwoPoolModelFieldObject::MaxX2D() const" );

}


float gkg::ATwoPoolModelFieldObject::MaxY2D() const
{

  try
  {

    return _boundingBox.getUpperY() + _resolution.y / 2;

  }
  GKG_CATCH( "float gkg::ATwoPoolModelFieldObject::MaxY2D() const" );

}


float gkg::ATwoPoolModelFieldObject::MaxZ2D() const
{

  try
  {

    return _boundingBox.getUpperZ() + _resolution.z / 2;

  }
  GKG_CATCH( "float gkg::ATwoPoolModelFieldObject::MaxZ2D() const" );

}


float gkg::ATwoPoolModelFieldObject::MinT() const
{

  try
  {

    return 0.0;

  }
  GKG_CATCH( "float gkg::ATwoPoolModelFieldObject::MinT() const" );

}


float gkg::ATwoPoolModelFieldObject::MaxT() const
{

  try
  {

    return 1.0;

  }
  GKG_CATCH( "float gkg::ATwoPoolModelFieldObject::MaxT() const" );

}


bool gkg::ATwoPoolModelFieldObject::boundingBox(
                                                std::vector<float>& bmin,
                                                std::vector<float>& bmax ) const
{

  try
  {

    bmin.resize( 3 );
    bmax.resize( 3 );

    bmin[ 0 ] = _boundingBox.getLowerX() - _resolution.x / 2;
    bmin[ 1 ] = _boundingBox.getLowerY() - _resolution.y / 2;
    bmin[ 2 ] = _boundingBox.getLowerZ() - _resolution.z / 2;

    bmax[ 0 ] = _boundingBox.getUpperX() + _resolution.x / 2;
    bmax[ 1 ] = _boundingBox.getUpperY() + _resolution.y / 2;
    bmax[ 2 ] = _boundingBox.getUpperZ() + _resolution.z / 2;

    return true;

  }
  GKG_CATCH( "bool AOdfObject::boundingBox("
             "Point3df & bmin,"
             "Point3df & bmax ) const" );

}


Point3df gkg::ATwoPoolModelFieldObject::VoxelSize() const
{

  try
  {

    return Point3df( _resolution.x, _resolution.y, _resolution.z ); 

  }
  GKG_CATCH( "Point3df gkg::ATwoPoolModelFieldObject::VoxelSize() const" );

}


bool gkg::ATwoPoolModelFieldObject::render( 
                                         anatomist::PrimList& primitiveList,
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
  GKG_CATCH( "bool gkg::ATwoPoolModelFieldObject::render( "
             "anatomist::PrimList& primitiveList, "
             "const anatomist::ViewState& viewState )" );

}


bool 
gkg::ATwoPoolModelFieldObject::glMakeBodyGLL(
                                         const anatomist::ViewState& viewState,
                                         const anatomist::GLList& glList ) const
{

  try
  {

    const anatomist::SliceViewState* sliceVState = viewState.sliceVS();

#if ( ANATOMIST_VERSION_MAJOR >= 5 ) || \
    ( ( ANATOMIST_VERSION_MAJOR == 4 ) && \
      ( ANATOMIST_VERSION_MINOR >= 5 ) && \
      ( ANATOMIST_VERSION_TINY >= 0 ) )
    Point3df position = viewState.window->getPosition();
    int32_t time = ( int32_t )(viewState.window->getTime() + 0.5 );
#else
    Point3df position = viewState.window->GetPosition();
    int32_t time = ( int32_t )(viewState.window->GetTime() + 0.5 );
#endif

    gkg::TwoPoolModel::Pool pool = ( ( time == 0 ) ? gkg::TwoPoolModel::Fast :
                                                     gkg::TwoPoolModel::Slow );
                                                     

    glEnableClientState( GL_VERTEX_ARRAY );
    _qGLShaderProgram->enableAttributeArray(
                                       _attributeLocationCurrentVertexIndices );

    if ( _hasApproximatedNormals && ( _hasHiddenShadows == false ) )
    {

      _qGLShaderProgram->enableAttributeArray( 
                                _attributeLocationSecondAndThirdVertexIndices );
      _qGLShaderProgram->enableAttributeArray( 
                                _attributeLocationSecondVertexCoordinates );
      _qGLShaderProgram->enableAttributeArray( 
                                _attributeLocationThirdVertexCoordinates );

    }

    glVertexPointer( 3, GL_FLOAT, 0, _vertexCoordinates.constData() );

    gkg::Vector3d< int32_t > begin;
    gkg::Vector3d< int32_t > end = gkg::Vector3d< int32_t >( 0.0, 0.0, 0.0 );

    glNewList( glList.item(), GL_COMPILE );

      bool isBind = _qGLShaderProgram->bind();

      if ( isBind )
      {

        _qGLShaderProgram->setAttributeArray(
                                         _attributeLocationCurrentVertexIndices,
                                         _currentVertexIndices,
                                         1 );

        const aims::Quaternion* quaternion = sliceVState->orientation;
        bool isOrthogonal = false;
        Point4df quaternionVector = quaternion->vector();
        if( quaternionVector == Point4df( 0, 0, 0, 1 ) )
        {

          isOrthogonal = true;
          begin.x = 0;
          begin.y = 0;
          begin.z = int32_t( ( position[ 2 ] - _boundingBox.getLowerZ() ) /
                               _resolution.z + 0.5 );      
          end.x = _translations.size();
          end.y = _translations[ 0 ].size();
          end.z = begin.z + 1;
          int32_t maximumZ = _translations[ 0 ][ 0 ].size() ;

          if ( end.z > maximumZ )
          {

            end.z = maximumZ;

          }

          if ( begin.z < 0 )
          {

            begin.z = 0;

          }

        }

        float c = 1.0 / std::sqrt( 2 );
        if( quaternionVector == Point4df( c, 0, 0, c  ) )
        {

          isOrthogonal = true;
          begin.x = 0;
          begin.y = int32_t( ( position[ 1 ] - _boundingBox.getLowerY() ) /
                               _resolution.y + 0.5 );
          begin.z = 0;
          end.x = _translations.size();
          end.y = begin.y + 1;
          end.z = _translations[ 0 ][ 0 ].size();
          int32_t maximumY = _translations[ 0 ].size() ;

          if ( end.y > maximumY )
          {

            end.y = maximumY;

          }
          if ( begin.y < 0 )
          {

            begin.y = 0;

          }

        }

        if( quaternionVector == Point4df( -0.5, -0.5, -0.5, 0.5 ) )
        {

          isOrthogonal = true;
          begin.x = int32_t( ( position[ 0 ] - _boundingBox.getLowerX() ) /
                               _resolution.x + 0.5 );
          begin.y = 0;
          begin.z = 0;

          end.x = begin.x + 1;
          end.y = _translations[ 0 ].size();
          end.z = _translations[ 0 ][ 0 ].size();
          int32_t maximumX = _translations.size() ;

          if ( end.x > maximumX )
          {

            end.x = maximumX;

          }
          if ( begin.x < 0 )
          {

            begin.x = 0;

          }

        }

        if ( isOrthogonal )
        {

          if ( _hasApproximatedNormals  || _hasHiddenShadows )
          {

            this->drawOrthogonalPlane( begin, end, pool );

          }
          else
          {

            this->drawOrthogonalPlaneWithExactNormals( begin, end, pool );

          }

        }
        else
        {

          if ( _hasApproximatedNormals || _hasHiddenShadows )
          {

            this->drawTransversalPlane( sliceVState, pool );

          }
          else
          {

            this->drawTransversalPlaneWithExactNormals( sliceVState, pool );

          }

        }


        _qGLShaderProgram->setUniformValue( _uniformIsShaderedObject, 0 );
        _qGLShaderProgram->release();

      glEndList();

      glNewList( _glList->item(), GL_COMPILE );

        glDrawElements( GL_TRIANGLES,
                        _vertexIndices.size(),
                        GL_UNSIGNED_INT,
                        _vertexIndices.constData() );
      glEndList();

    }
    glDisableClientState( GL_VERTEX_ARRAY );

    return true;

  }
  GKG_CATCH( "bool gkg::ATwoPoolModelFieldObject::glMakeBodyGLL( "
             "const anatomist::ViewState& viewState, "
             "const anatomist::GLList& glList ) const" );

}

void gkg::ATwoPoolModelFieldObject::setMeshScale( float meshScale )
{

  try
  {

    _meshScale = meshScale;
    if ( _isDisplayed )
    {

      updateShaderCode();
      setShaderAttributes();

    }

  }
  GKG_CATCH( "void gkg::ATwoPoolModelFieldObject::setMeshScale( "
             "float meshScale" );

}


void gkg::ATwoPoolModelFieldObject::setShadowOptions(
                                                   bool hasHiddenShadows,
                                                   bool hasApproximatedNormals )
{

  try
  {

    _hasHiddenShadows = hasHiddenShadows;
    if ( !_hasHiddenShadows )
    {

      if ( hasApproximatedNormals )
      {

        if ( !_approximatedNormalsAlreadyComputed )
        {

          getApproximatedNormals();

        }
        _hasApproximatedNormals = hasApproximatedNormals;

      }
      else
      {

        if ( _realNormalsAlreadyComputed )
        {

          _hasApproximatedNormals = hasApproximatedNormals;

        }

      }

    }
    if ( _isDisplayed )
    {

      if ( _realNormalsAlreadyComputed || _approximatedNormalsAlreadyComputed )
      {

        updateShaderCode();
        setShaderAttributes();

      }
      else
      {

        _hasHiddenShadows = true;

      }

    }

  }
  GKG_CATCH( "void gkg::ATwoPoolModelFieldObject::setShadowsOptions( "
             "bool hiddenShadows, "
             "bool approximatedNormals )" );

}


void gkg::ATwoPoolModelFieldObject::setRenderingOptions(
                                                 bool hasHiddenShadows,
                                                 int32_t outputOrientationCount,
                                                 bool hasApproximatedNormals )
{

  try
  {

    _hasApproximatedNormals = hasApproximatedNormals;
    _hasHiddenShadows = hasHiddenShadows;
    if ( _outputOrientationCount != outputOrientationCount  )
    {

      _outputOrientationCount = outputOrientationCount;
      deleteReader();
      loadReader();

    }
    if ( !hasHiddenShadows &&
         !hasApproximatedNormals &&
         !_realNormalsAlreadyComputed )
    {

      _aTwoPoolModelFieldReader->getNormals( _fastNormals, _slowNormals );
      _realNormalsAlreadyComputed = true;
      if ( _isDisplayed )
      {

        updateShaderCode();
        setShaderAttributes();

      }

    }

  }
  GKG_CATCH( "void gkg::ATwoPoolModelFieldObject::setRenderOptions( "
             "int32_t outputOrientationCount, "
             "bool hasApproximatedNormals )" );

}


int32_t gkg::ATwoPoolModelFieldObject::getClassType()
{

  return _classType;

}


#if ( ANATOMIST_VERSION_MAJOR >= 5 ) || \
    ( ( ANATOMIST_VERSION_MAJOR == 4 ) && \
      ( ANATOMIST_VERSION_MINOR >= 6 ) && \
      ( ANATOMIST_VERSION_TINY >= 0 ) )
std::list< anatomist::AObject* > gkg::ATwoPoolModelFieldObject::load(
#else
anatomist::AObject* gkg::ATwoPoolModelFieldObject::load(
#endif
                                    const std::string& fileName,
                                    anatomist::ObjectReader::PostRegisterList &,
                                    carto::Object /* options */ )
{

  try
  {

    std::string fileNameSiteMap = fileName;
    std::string fileNameTextureMap = fileName;

    // try to find site map name automatically
    std::string textureMapExtension = "texturemap";
    std::string::size_type 
      textureMapPosition = fileNameTextureMap.find( textureMapExtension );
    if ( textureMapPosition != std::string::npos )
    {

      fileNameSiteMap.replace( textureMapPosition,
                               textureMapExtension.length(),
                               "sitemap" );

    }

    std::string textureMapString = "texture_map";
    textureMapPosition = fileNameTextureMap.find( textureMapString );
    if ( textureMapPosition != std::string::npos )
    {

      fileNameSiteMap.replace( textureMapPosition,
                               textureMapString.length(),
                               "site_map" );

    }

    // if automatical load it
    std::ifstream is( fileNameSiteMap.c_str() );
    bool openFileDialog = true;

    if ( !is.fail() )
    {

      QString fileName( fileNameSiteMap.c_str() );
      openFileDialog = false;

    }
    // else choose it manually
    if ( openFileDialog )
    {

      QFileDialog qFileDialog( 0,
                               QString( "" ),
                               QString( fileNameTextureMap.c_str() ) );
      qFileDialog.setFileMode( QFileDialog::ExistingFile );
      qFileDialog.setNameFilter( tr( "Sitemap (*.sitemap)" ) );
      QStringList fileNames;
      if ( qFileDialog.exec() )
      {

        fileNames = qFileDialog.selectedFiles();
        fileNameSiteMap = fileNames[ 0 ].toStdString();


      }
      else
      {

        fileNameSiteMap = "";

      }


    }

    gkg::ATwoPoolModelFieldObject* aTwoPoolModelFieldObject = 0;

#if ( ANATOMIST_VERSION_MAJOR >= 5 ) || \
    ( ( ANATOMIST_VERSION_MAJOR == 4 ) && \
      ( ANATOMIST_VERSION_MINOR >= 6 ) && \
      ( ANATOMIST_VERSION_TINY >= 0 ) )
    std::list< anatomist::AObject* > objectList;
    try
    {

      aTwoPoolModelFieldObject = 
        new gkg::ATwoPoolModelFieldObject( fileNameSiteMap,
                                           fileNameTextureMap );
      objectList.push_back( aTwoPoolModelFieldObject );

    }
    catch( std::exception& )
    {
    }

    return objectList;
#else
    try
    {

      aTwoPoolModelFieldObject = 
        new gkg::ATwoPoolModelFieldObject( fileNameSiteMap,
                                           fileNameTextureMap );

    }
    catch( std::exception& )
    {

      return 0;

    }

    return aTwoPoolModelFieldObject;
#endif

  }
  GKG_CATCH( "anatomist::AObject* gkg::ATwoPoolModelFieldObject::load( "
             "const std::string& fileName, "
             "carto::Object options )" );

}


void gkg::ATwoPoolModelFieldObject::loadReader()
{

  try
  {

    _referential = 0;

    _qGLShaderProgram = 0;
    _qGLShader = 0;

    _currentVertexIndices = 0;
    _secondAndThirdVertexIndices = 0;

    _secondVertexCoordinates = 0;
    _thirdVertexCoordinates = 0;

    _uniformLocationTranslation = 0;
    _uniformLocationTransformation = 0;
    _uniformLocationNormal = 0;

    _uniformIsShaderedObject = 0;

    _attributeLocationMeshScale = 0;

    _attributeLocationCurrentVertexIndices = 0;
    _attributeLocationSecondAndThirdVertexIndices = 0;

    _attributeLocationSecondVertexCoordinates = 0;
    _attributeLocationThirdVertexCoordinates = 0;

    _boundingBox = gkg::BoundingBox< float >( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 );

    _approximatedNormalsAlreadyComputed = false;
    _realNormalsAlreadyComputed = false;

    _aTwoPoolModelFieldReader = new gkg::ATwoPoolModelFieldReader(
                                                      _fileNameSiteMap,
                                                      _fileNameTextureMap,
                                                      _outputOrientationCount );

    _outputOrientationCount = 
                         _aTwoPoolModelFieldReader->getOutputOrientationCount();
    _aTwoPoolModelFieldReader->getSphereVertexCoordinatesAndIndices(
                                                             _vertexCoordinates,
                                                             _vertexIndices );
    _boundingBox = _aTwoPoolModelFieldReader->getBoundingBox();
    _resolution = _aTwoPoolModelFieldReader->getResolution();

    if ( _hasApproximatedNormals || _hasHiddenShadows )
    {

      _aTwoPoolModelFieldReader->getTransformationsAndTranslations(
                                                           _fastTransformations,
                                                           _slowTransformations,
                                                           _translations );

    }
    else
    {

      _aTwoPoolModelFieldReader->getTransformationsAndTranslationsAndNormals(
                                                           _fastTransformations,
                                                           _slowTransformations,
                                                           _translations,
                                                           _fastNormals,
                                                           _slowNormals );
      _realNormalsAlreadyComputed = true;

    }

    // building current vertex indices
    _currentVertexIndices = new GLfloat[ _outputOrientationCount ];
    int32_t i;
    for ( i = 0; i < _outputOrientationCount; i++ )
    {

      _currentVertexIndices[ i ] = ( GLfloat )i;

    }

    if ( _hasApproximatedNormals )
    {

      getApproximatedNormals();

    }

  }
  GKG_CATCH( "void gkg::ATwoPoolModelFieldObject::loadReader()" );

}


void gkg::ATwoPoolModelFieldObject::deleteReader()
{

  try
  {

    delete _qGLShader;
    delete _qGLShaderProgram;

    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t z = 0;
    uint32_t sizeX = _translations.size();
    uint32_t sizeY = 0;
    uint32_t sizeZ = 0;

    for( x = 0; x < sizeX; ++x )
    {

      sizeY = _translations[ x ].size();
      for( y = 0; y < sizeY; ++y )
      {

        sizeZ = _translations[ x ][ y ].size();
        for( z = 0; z < sizeZ; ++z )
        {

          if ( _fastTransformations[ x ][ y ][ z ] )
          {

            if ( _realNormalsAlreadyComputed )
            {

              delete [] _fastNormals[ x ][ y ][ z ];

            }
            delete [] _fastTransformations[ x ][ y ][ z ];

          }
          if ( _slowTransformations[ x ][ y ][ z ] )
          {

            if ( _realNormalsAlreadyComputed )
            {

              delete [] _slowNormals[ x ][ y ][ z ];

            }
            delete [] _slowTransformations[ x ][ y ][ z ];

          }

        }
        _translations[ x ][ y ].clear();
        _fastTransformations[ x ][ y ].clear();
        _slowTransformations[ x ][ y ].clear();
        if ( _realNormalsAlreadyComputed )
        {

          _fastNormals[ x ][ y ].clear();
          _slowNormals[ x ][ y ].clear();

        }

      }
      _fastTransformations[ x ].clear();
      _slowTransformations[ x ].clear();
      _translations[ x ].clear();
      if ( _realNormalsAlreadyComputed )
      {

        _fastNormals[ x ].clear();
        _slowNormals[ x ].clear();

      }

    }
    _translations.clear();
    _fastTransformations.clear();
    _slowTransformations.clear();
    if ( _realNormalsAlreadyComputed )
    {

      _fastNormals.clear();
      _slowNormals.clear();

    }

    delete [] _currentVertexIndices;
    if ( _approximatedNormalsAlreadyComputed )
    {

      delete [] _secondAndThirdVertexIndices;
      delete [] _secondVertexCoordinates;
      delete [] _thirdVertexCoordinates;

    }
    delete _aTwoPoolModelFieldReader;

    _realNormalsAlreadyComputed = false;
    _approximatedNormalsAlreadyComputed = false;
    _vertexCoordinates.clear();
    _vertexIndices.clear();

  }
  GKG_CATCH( "void gkg::ATwoPoolModelFieldObject::deleteReader()" );

}


void gkg::ATwoPoolModelFieldObject::drawOrthogonalPlane(
                                            const gkg::Vector3d< int32_t >& begin,
                                            const gkg::Vector3d< int32_t >& end,
                                            gkg::TwoPoolModel::Pool pool ) const
{

  try
  {

    _qGLShaderProgram->setAttributeValue( _attributeLocationMeshScale,
                                          _meshScale );
    _qGLShaderProgram->setUniformValue( _uniformIsShaderedObject, 1 );
    if ( _hasHiddenShadows == false )
    {

      _qGLShaderProgram->setAttributeArray(
                                  _attributeLocationSecondAndThirdVertexIndices,
                                  _secondAndThirdVertexIndices );
      _qGLShaderProgram->setAttributeArray(
                                  _attributeLocationSecondVertexCoordinates,
                                  _secondVertexCoordinates );
      _qGLShaderProgram->setAttributeArray(
                                  _attributeLocationThirdVertexCoordinates,
                                  _thirdVertexCoordinates );

    }

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    for ( x = begin.x; x < end.x; ++x )
    {

      for ( y = begin.y; y < end.y; ++y )
      {

        for ( z = begin.z; z < end.z; ++z )
        {

          if( _fastTransformations[ x ][ y ][ z ]   )
          {

            _qGLShaderProgram->setUniformValue( _uniformLocationTranslation,
                                                _translations[ x ][ y ][ z ] );
            if ( pool == gkg::TwoPoolModel::Fast )
            {

              _qGLShaderProgram->setUniformValueArray(
                                           _uniformLocationTransformation,
                                           _fastTransformations[ x ][ y ][ z ],
                                           _outputOrientationCount,
                                           1 );

            }
            else
            {

              _qGLShaderProgram->setUniformValueArray(
                                          _uniformLocationTransformation,
                                          _slowTransformations[ x ][ y ][ z ],
                                          _outputOrientationCount,
                                          1 );

            }
            _glList->callList();

          }

        }

      }

    }

  }
  GKG_CATCH( "void gkg::ATwoPoolModelFieldObject::drawOrthogonalPlane( "
             "const QVector3D& begin, "
             "const QVector3D& end, "
             "gkg::TwoPoolModel::Pool pool ) const" );

}


void gkg::ATwoPoolModelFieldObject::drawOrthogonalPlaneWithExactNormals( 
                                            const gkg::Vector3d< int32_t >& begin,
                                            const gkg::Vector3d< int32_t >& end,
                                            gkg::TwoPoolModel::Pool pool ) const
{

  try
  {

    _qGLShaderProgram->setAttributeValue( _attributeLocationMeshScale,
                                          _meshScale );
    _qGLShaderProgram->setUniformValue( _uniformIsShaderedObject, 1 );

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    for ( x = begin.x; x < end.x; ++x )
    {

      for ( y = begin.y; y < end.y; ++y )
      {

        for ( z = begin.z; z < end.z; ++z )
        {

          if( _fastTransformations[ x ][ y ][ z ]   )
          {

            _qGLShaderProgram->setUniformValue( _uniformLocationTranslation,
                                      _translations[ x ][ y ][ z ] );

            if ( pool == gkg::TwoPoolModel::Fast )
            {

              _qGLShaderProgram->setUniformValueArray( 
                                          _uniformLocationTransformation,
                                          _fastTransformations[ x ][ y ][ z ],
                                          _outputOrientationCount,
                                          1 );

              _qGLShaderProgram->setUniformValueArray( 
                                          _uniformLocationNormal,
                                          _fastNormals[ x ][ y ][ z ],
                                          _outputOrientationCount );

            }
            else
            {

              _qGLShaderProgram->setUniformValueArray( 
                                          _uniformLocationTransformation,
                                          _slowTransformations[ x ][ y ][ z ],
                                          _outputOrientationCount,
                                          1 );

              _qGLShaderProgram->setUniformValueArray( 
                                          _uniformLocationNormal,
                                          _slowNormals[ x ][ y ][ z ],
                                          _outputOrientationCount );

            }
            _glList->callList();

          }

        }

      }

    }

  }
  GKG_CATCH(
     "void gkg::ATwoPoolModelFieldObject::drawOrthogonalPlaneWithExactNormals( "
     "const QVector3D& begin, "
     "const QVector3D& end, "
     "gkg::TwoPoolModel::Pool pool ) const" );

}


void gkg::ATwoPoolModelFieldObject::drawTransversalPlane(
                                   const anatomist::SliceViewState* sliceVState,
                                   gkg::TwoPoolModel::Pool pool ) const
{

  try
  {
    _qGLShaderProgram->setAttributeValue( _attributeLocationMeshScale,
                                          _meshScale );
    _qGLShaderProgram->setUniformValue( _uniformIsShaderedObject, 1 );
    if ( _hasHiddenShadows == false )
    {

      _qGLShaderProgram->setAttributeArray(
                                  _attributeLocationSecondAndThirdVertexIndices,
                                  _secondAndThirdVertexIndices );
      _qGLShaderProgram->setAttributeArray( 
                                  _attributeLocationSecondVertexCoordinates,
                                  _secondVertexCoordinates );
      _qGLShaderProgram->setAttributeArray(
                                  _attributeLocationThirdVertexCoordinates, 
                                  _thirdVertexCoordinates );

    }

    const aims::Quaternion* quaternion = sliceVState->orientation;
    float voxelSize = ( _resolution.x + _resolution.y + _resolution.z ) / 6;

    Point4df plane;
    Point3df n = quaternion->transformInverse( Point3df( 0, 0, 1 ) );
    plane[ 0 ] = n[ 0 ];
    plane[ 1 ] = n[ 1 ];
    plane[ 2 ] = n[ 2 ];
    plane[ 3 ] = -n.dot( sliceVState->position );
    float normalizationFactor = std::sqrt( std::pow( plane[ 0 ], 2 ) +
                                           std::pow( plane[ 1 ], 2 ) +
                                           std::pow( plane[ 2 ], 2 ) );

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    int32_t sizeX = _translations.size();
    int32_t sizeY = 0;
    int32_t sizeZ = 0;
    float result = 0.0;
    for ( x = 0; x < sizeX; ++x )
    {

      sizeY = _translations[ x ].size();
      for ( y = 0; y < sizeY; ++y )
      {

        sizeZ = _translations[ x ][ y ].size();
        for ( z = 0; z < sizeZ; ++z )
        {

          result = std::abs( ( plane[ 0 ] * _translations[ x ][ y ][ z ].x() ) +
                             ( plane[ 1 ] * _translations[ x ][ y ][ z ].y() ) +
                             ( plane[ 2 ] * _translations[ x ][ y ][ z ].z() ) +
                              plane[ 3 ] ) /
                   normalizationFactor;


          if ( result < voxelSize )
          {

            if( _fastTransformations[ x ][ y ][ z ] )
            {

              _qGLShaderProgram->setUniformValue( 
                                                 _uniformLocationTranslation,
                                                 _translations[ x ][ y ][ z ] );
              if ( pool == gkg::TwoPoolModel::Fast )
              {

                _qGLShaderProgram->setUniformValueArray(
                                          _uniformLocationTransformation,
                                          _fastTransformations[ x ][ y ][ z ],
                                          _outputOrientationCount,
                                          1 );

              }
              else
              {

                _qGLShaderProgram->setUniformValueArray(
                                          _uniformLocationTransformation,
                                          _slowTransformations[ x ][ y ][ z ],
                                          _outputOrientationCount,
                                          1 );

              }
              _glList->callList();

            }

          }

        }

      }

    }

  }
  GKG_CATCH( "void gkg::ATwoPoolModelFieldObject::drawTransversalPlane( "
             "const anatomist::SliceViewState* sliceVState, "
             "gkg::TwoPoolModel::Pool pool ) const" );

}


void gkg::ATwoPoolModelFieldObject::drawTransversalPlaneWithExactNormals(
                                   const anatomist::SliceViewState* sliceVState,
                                   gkg::TwoPoolModel::Pool pool ) const
{

  try
  {
    _qGLShaderProgram->setAttributeValue( _attributeLocationMeshScale,
                                          _meshScale );
    _qGLShaderProgram->setUniformValue( _uniformIsShaderedObject, 1 );

    const aims::Quaternion* quaternion = sliceVState->orientation;
    float voxelSize = ( _resolution.x + _resolution.y + _resolution.z ) / 6;

    Point4df plane;
    Point3df n = quaternion->transformInverse( Point3df( 0, 0, 1 ) );
    plane[ 0 ] = n[ 0 ];
    plane[ 1 ] = n[ 1 ];
    plane[ 2 ] = n[ 2 ];
    plane[ 3 ] = -n.dot( sliceVState->position );
    float normalizationFactor = std::sqrt( std::pow( plane[ 0 ], 2 ) +
                                           std::pow( plane[ 1 ], 2 ) +
                                           std::pow( plane[ 2 ], 2 ) );

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    int32_t sizeX = _translations.size();
    int32_t sizeY = 0;
    int32_t sizeZ = 0;
    float result = 0.0;
    for ( x = 0; x < sizeX; ++x )
    {

      sizeY = _translations[ x ].size();
      for ( y = 0; y < sizeY; ++y )
      {

        sizeZ = _translations[ x ][ y ].size();
        for ( z = 0; z < sizeZ; ++z )
        {

          result = std::abs( ( plane[ 0 ] * _translations[ x ][ y ][ z ].x() ) +
                             ( plane[ 1 ] * _translations[ x ][ y ][ z ].y() ) +
                             ( plane[ 2 ] * _translations[ x ][ y ][ z ].z() ) +
                              plane[ 3 ] ) /
                   normalizationFactor;

          if ( result < voxelSize )
          {

            if( _fastTransformations[ x ][ y ][ z ] )
            {

              _qGLShaderProgram->setUniformValue( 
                                                 _uniformLocationTranslation,
                                                 _translations[ x ][ y ][ z ] );
              if ( pool == gkg::TwoPoolModel::Fast )
              {

                _qGLShaderProgram->setUniformValueArray(
                                          _uniformLocationTransformation,
                                          _fastTransformations[ x ][ y ][ z ],
                                          _outputOrientationCount,
                                          1 );
                _qGLShaderProgram->setUniformValueArray( 
                                          _uniformLocationNormal,
                                          _fastNormals[ x ][ y ][ z ],
                                          _outputOrientationCount );

              }
              else
              {

                _qGLShaderProgram->setUniformValueArray(
                                          _uniformLocationTransformation,
                                          _slowTransformations[ x ][ y ][ z ],
                                          _outputOrientationCount,
                                          1 );
                _qGLShaderProgram->setUniformValueArray( 
                                          _uniformLocationNormal,
                                          _slowNormals[ x ][ y ][ z ],
                                          _outputOrientationCount );

              }
              _glList->callList();

            }

          }

        }

      }

    }

  }
  GKG_CATCH( "void gkg::ATwoPoolModelFieldObject::drawTransversalPlaneWithExactNormals( "
             "const anatomist::SliceViewState* sliceVState, "
             "gkg::TwoPoolModel::Pool pool ) const" );

}


void gkg::ATwoPoolModelFieldObject::updateShaderCode()
{

  try
  {

    if ( _qGLShader )
    {

      delete _qGLShader;
      delete _qGLShaderProgram;

    }

    _qGLShader = new QGLShader( QGLShader::Vertex );


    double minimumResolution = std::min( std::min( _resolution.x,
                                                   _resolution.y ),
                                         _resolution.z ) / 2.0;

    if ( _hasHiddenShadows )
    {

      _qGLShader->compileSourceCode(
        gkg::shaderCodeWithoutNormals(
         gkg::StringConverter::toString( _outputOrientationCount ),
         gkg::StringConverter::toString( minimumResolution, 10, 3 )
                                      ).c_str() );

    }
    else if ( _hasApproximatedNormals )
    {

      _qGLShader->compileSourceCode(
        gkg::shaderCodeWithApproximatedNormals(
         gkg::StringConverter::toString( _outputOrientationCount ),
         gkg::StringConverter::toString( minimumResolution, 10, 3 )
                                      ).c_str() );

    }
    else
    {

      _qGLShader->compileSourceCode(
       gkg::shaderCode(
         gkg::StringConverter::toString( _outputOrientationCount ),
         gkg::StringConverter::toString( minimumResolution, 10, 3 )
                                      ).c_str() );

    }

    _qGLShaderProgram = new QGLShaderProgram();
    _qGLShaderProgram->addShader( _qGLShader );
    _qGLShaderProgram->link();

  }
  GKG_CATCH( "void gkg::ATwoPoolModelFieldObject::updateShaderCode()" );

}


void gkg::ATwoPoolModelFieldObject::setShaderAttributes()
{

  try
  {

    _qGLShaderProgram->bind();

    _attributeLocationMeshScale =
      _qGLShaderProgram->attributeLocation( "meshScale" );
    _uniformIsShaderedObject =
      _qGLShaderProgram->uniformLocation( "isShaderedObject" );

    if ( _hasApproximatedNormals && !_hasHiddenShadows )
    {

      _attributeLocationSecondVertexCoordinates =
        _qGLShaderProgram->attributeLocation( "coordinates2" );
      _attributeLocationThirdVertexCoordinates =
        _qGLShaderProgram->attributeLocation( "coordinates3" );
      _attributeLocationSecondAndThirdVertexIndices =
        _qGLShaderProgram->attributeLocation( "polygonIndices" );

    }
    else if ( !_hasApproximatedNormals && !_hasHiddenShadows )
    {

      _uniformLocationNormal = _qGLShaderProgram->uniformLocation( "normal" );

    }

    _uniformLocationTranslation = _qGLShaderProgram->uniformLocation(
                                                                "translation" );
    _uniformLocationTransformation = _qGLShaderProgram->uniformLocation(
                                                             "transformation" );
    _attributeLocationCurrentVertexIndices =
                                _qGLShaderProgram->attributeLocation( "index" );
    _qGLShaderProgram->release();
    _glList = new anatomist::GLList();

  }
  GKG_CATCH( "void gkg::ATwoPoolModelFieldObject::setShaderAttributes()" );

}


void gkg::ATwoPoolModelFieldObject::getApproximatedNormals()
{

  try
  {

    std::vector< bool > countVector( _outputOrientationCount, false );
    _secondAndThirdVertexIndices = new QVector2D[ _outputOrientationCount ];
    _secondVertexCoordinates = new QVector3D[ _outputOrientationCount ];
    _thirdVertexCoordinates = new QVector3D[ _outputOrientationCount ];

    int32_t count = _outputOrientationCount;
    int32_t i = 0;
    while ( ( i  < _vertexIndices.size() ) && ( count > 0 ) )
    {

      if ( countVector[ _vertexIndices[ i ] ] == false )
      {

        countVector[ _vertexIndices[ i ] ] = true;
        _secondAndThirdVertexIndices[ _vertexIndices[ i ] ].setX(
                                                    _vertexIndices[ i + 1 ] );
        _secondAndThirdVertexIndices[ _vertexIndices[ i ] ].setY(
                                                      _vertexIndices[ i + 2 ] );
        _secondVertexCoordinates[ _vertexIndices[ i ] ].setX(
                           _vertexCoordinates[ _vertexIndices[ i + 1 ] * 3 ] ) ;
        _secondVertexCoordinates[ _vertexIndices[ i ] ].setY(
                       _vertexCoordinates[ _vertexIndices[ i + 1 ] * 3 + 1 ] ) ;
        _secondVertexCoordinates[ _vertexIndices[ i ] ].setZ(
                       _vertexCoordinates[ _vertexIndices[ i + 1 ] * 3 + 2 ] ) ;
        _thirdVertexCoordinates[ _vertexIndices[ i ] ].setX(
                           _vertexCoordinates[ _vertexIndices[ i + 2 ] * 3 ] ) ;
        _thirdVertexCoordinates[ _vertexIndices[ i ] ].setY(
                       _vertexCoordinates[ _vertexIndices[ i + 2 ] * 3 + 1 ] ) ;
        _thirdVertexCoordinates[ _vertexIndices[ i ] ].setZ(
                       _vertexCoordinates[ _vertexIndices[ i + 2 ] * 3 + 2 ] ) ;

        -- count;

      }

      if ( countVector[ _vertexIndices[ i + 1 ] ] == false )
      {

        _secondAndThirdVertexIndices[ _vertexIndices[ i + 1 ] ].setX(
                                                      _vertexIndices[ i + 2 ] ); 
        _secondAndThirdVertexIndices[ _vertexIndices[ i + 1 ] ].setY(
                                                          _vertexIndices[ i ] ); 
        countVector[ _vertexIndices[ i + 1 ] ] = true;
        _secondVertexCoordinates[ _vertexIndices[ i + 1 ] ].setX(
                           _vertexCoordinates[ _vertexIndices[ i + 2 ] * 3 ] ) ;
        _secondVertexCoordinates[ _vertexIndices[ i + 1 ] ].setY(
                       _vertexCoordinates[ _vertexIndices[ i + 2 ] * 3 + 1 ] ) ;
        _secondVertexCoordinates[ _vertexIndices[ i + 1 ] ].setZ(
                       _vertexCoordinates[ _vertexIndices[ i + 2 ] * 3 + 2 ] ) ; 
        _thirdVertexCoordinates[ _vertexIndices[ i + 1 ] ].setX(
                              _vertexCoordinates[ _vertexIndices[ i ] * 3 ]  ) ;
        _thirdVertexCoordinates[ _vertexIndices[ i + 1 ] ].setY(
                           _vertexCoordinates[ _vertexIndices[ i ] * 3 + 1 ] ) ;
        _thirdVertexCoordinates[ _vertexIndices[ i + 1 ]  ].setZ(
                           _vertexCoordinates[ _vertexIndices[ i ] * 3 + 2 ] ) ;

        -- count;

      }

      if ( countVector[ _vertexIndices[ i + 2 ] ] == false )
      {

        _secondAndThirdVertexIndices[ _vertexIndices[ i + 2 ] ].setX(
                                                          _vertexIndices[ i ] );
        _secondAndThirdVertexIndices[ _vertexIndices[ i + 2 ] ].setY(
                                                      _vertexIndices[ i + 1 ] );
        countVector[ _vertexIndices[ i + 2 ] ] = true;
        _secondVertexCoordinates[ _vertexIndices[ i + 2 ] ].setX(
                               _vertexCoordinates[ _vertexIndices[ i ] * 3 ] ) ;
        _secondVertexCoordinates[ _vertexIndices[ i + 2 ] ].setY(
                           _vertexCoordinates[ _vertexIndices[ i ] * 3 + 1 ] ) ;
        _secondVertexCoordinates[ _vertexIndices[ i + 2 ] ].setZ(
                           _vertexCoordinates[ _vertexIndices[ i ] * 3 + 2 ] ) ;
        _thirdVertexCoordinates[ _vertexIndices[ i + 2 ] ].setX(
                           _vertexCoordinates[ _vertexIndices[ i + 1 ] * 3 ] ) ;
        _thirdVertexCoordinates[ _vertexIndices[ i + 2 ] ].setY(
                       _vertexCoordinates[ _vertexIndices[ i + 1 ] * 3 + 1 ] ) ;
        _thirdVertexCoordinates[ _vertexIndices[ i + 2 ] ].setZ(
                      _vertexCoordinates[ _vertexIndices[ i + 1 ] * 3 + 2 ]  ) ;

        -- count;

      }
      i = i + 3;

    }
    _approximatedNormalsAlreadyComputed = true;

  }
  GKG_CATCH( "void AOdfObject::getRenderOptions("
             "int& outputOrientationCount,"
             "bool& approximatedNormals,"
             "bool& hideShadows )" );

}


int32_t gkg::ATwoPoolModelFieldObject::registerClass()
{

  try
  {

    anatomist::ObjectReader::registerLoader( "texturemap", load );
    _classType = registerObjectType( "TwoPoolModel" );

#ifdef GKG_DEBUG

    std::cout << "TwoPoolModel loader registered : " << _classType << std::endl;

#endif

    return _classType;

  }
  GKG_CATCH( "int32_t gkg::ATwoPoolModelFieldObject::registerClass()" );

}


#undef DEFAULT_OUTPUT_ORIENTATION_COUNT
#undef DEFAULT_MESH_SCALE

