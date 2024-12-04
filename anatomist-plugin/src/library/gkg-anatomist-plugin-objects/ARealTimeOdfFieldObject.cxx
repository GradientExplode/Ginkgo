#include <gkg-anatomist-plugin-objects/ARealTimeOdfFieldObject.h>
#include <gkg-processing-container/CartesianField_i.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-anatomist-plugin-objects/VertexShader.h>
#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-mesh/ConvexHull_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>

#include <anatomist/object/actions.h>
#include <anatomist/control/qObjTree.h>
#include <anatomist/window/Window.h>
#include <anatomist/config/version.h>
#include <aims/resampling/quaternion.h>


int32_t 
gkg::ARealTimeOdfFieldObject::_classType = 
                                  gkg::ARealTimeOdfFieldObject::registerClass();
Tree* gkg::ARealTimeOdfFieldObject::_optionTree = 0;


gkg::ARealTimeOdfFieldObject::ARealTimeOdfFieldObject( 
                 gkg::CartesianField< gkg::MeshDeformation< float > >& odfField,
                 gkg::OrientationSet& orientationSet,
                 float meshScale,
                 bool anatomistStandardMode )
                : _odfCartesianField( odfField ),
                  _orientationSet( orientationSet ),
                  _outputOrientationCount( orientationSet.getCount() ),
                  _meshScale( meshScale ),
                  _isDisplayed( false ),
                  _glList( 0 ),
                  _qGLShaderProgram( 0 ),
                  _qGLShader( 0 )
{

  try
  {

    _type = _classType;
    _referential = 0;

    // adding icon
    if ( QObjectTree::TypeNames.find( _classType ) ==
         QObjectTree::TypeNames.end() )
    {

      if ( anatomistStandardMode )
      {

        std::string
          iconPath = gkg::ConfigurationInfo::getInstance().getIconPath(
                                                              "icon-odf.png" );
        if ( !QObjectTree::TypeIcons[ _classType ].load( iconPath.c_str() ) )
        {

          QObjectTree::TypeIcons.erase( _classType );
          std::cerr << "icon " << iconPath << " not found!" << std::endl;

        }

      }

      QObjectTree::TypeNames[ _classType ] = "Real-Time ODF";

    }

    getSphereVertexCoordinatesAndIndices();

  }
  GKG_CATCH( "gkg::ARealTimeOdfFieldObject::ARealTimeOdfFieldObject( "
             "gkg::CartesianField< gkg::MeshDeformation< float > >& odfField, "
             "float meshScale )" );

}


gkg::ARealTimeOdfFieldObject::~ARealTimeOdfFieldObject()
{

  cleanup();

  delete _glList;
  delete _qGLShader;
  delete _qGLShaderProgram;
  delete[] _currentVertexIndices;

}


bool gkg::ARealTimeOdfFieldObject::Is2DObject()
{

  return true;

}


bool gkg::ARealTimeOdfFieldObject::Is3DObject()
{

  return false;

}


Tree* gkg::ARealTimeOdfFieldObject::optionTree() const
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
  GKG_CATCH( "Tree* gkg::ARealTimeOdfFieldObject::optionTree() const" );

}


int gkg::ARealTimeOdfFieldObject::CanBeDestroyed()
{

  try
  {

    return 0;

  }
  GKG_CATCH( "int gkg::ARealTimeOdfFieldObject::CanBeDestroyed()" );

}


float gkg::ARealTimeOdfFieldObject::MinX2D() const
{

  try
  {

    return 0.0f - _odfCartesianField.getResolutionX();

  }
  GKG_CATCH( "float gkg::ARealTimeOdfFieldObject::MinX2D() const" );

}


float gkg::ARealTimeOdfFieldObject::MinY2D() const
{

  try
  {


    return 0.0f - _odfCartesianField.getResolutionY();

  }
  GKG_CATCH( "float gkg::ARealTimeOdfFieldObject::MinY2D() const" );

}


float gkg::ARealTimeOdfFieldObject::MinZ2D() const
{

  try
  {

    return 0.0f - _odfCartesianField.getResolutionZ();

  }
  GKG_CATCH( "float gkg::ARealTimeOdfFieldObject::MinZ2D() const" );

}


float gkg::ARealTimeOdfFieldObject::MaxX2D() const
{

  try
  {

    return _odfCartesianField.getSizeX() * 
           _odfCartesianField.getResolutionX() + 
           _odfCartesianField.getResolutionX();

  }
  GKG_CATCH( "float gkg::ARealTimeOdfFieldObject::MaxX2D() const" );

}


float gkg::ARealTimeOdfFieldObject::MaxY2D() const
{

  try
  {

    return _odfCartesianField.getSizeY() * 
           _odfCartesianField.getResolutionY() +
           _odfCartesianField.getResolutionY();

  }
  GKG_CATCH( "float gkg::ARealTimeOdfFieldObject::MaxY2D() const" );

}


float gkg::ARealTimeOdfFieldObject::MaxZ2D() const
{

  try
  {

    return _odfCartesianField.getSizeZ() * 
           _odfCartesianField.getResolutionZ() +
           _odfCartesianField.getResolutionZ();

  }
  GKG_CATCH( "float gkg::ARealTimeOdfFieldObject::MaxZ2D() const" );

}


bool gkg::ARealTimeOdfFieldObject::boundingBox( std::vector<float>& bmin,
                                                std::vector<float>& bmax ) const
{

  try
  {

    bmin.resize( 3 );
    bmax.resize( 3 );

    bmin[ 0 ] = 0.0f;
    bmin[ 1 ] = 0.0f;
    bmin[ 2 ] = 0.0f;

    bmax[ 0 ] = _odfCartesianField.getSizeX() * 
                _odfCartesianField.getResolutionX();
    bmax[ 1 ] = _odfCartesianField.getSizeY() * 
                _odfCartesianField.getResolutionY();
    bmax[ 2 ] = _odfCartesianField.getSizeZ() * 
                _odfCartesianField.getResolutionZ();

    if( bmin[ 0 ] == bmax[ 0 ] )
    {

      bmax[ 0 ] += _odfCartesianField.getResolutionX();

    }

    if( bmin[ 1 ] == bmax[ 1 ] )
    {

      bmax[ 1 ] += _odfCartesianField.getResolutionY();

    }
    if( bmin[ 2 ] == bmax[ 2 ] )
    {

      bmax[ 2 ] += _odfCartesianField.getResolutionZ();

    }

    return true;

  }
  GKG_CATCH( "bool ARealTimeOdfObject::boundingBox("
             "Point3df & bmin,"
             "Point3df & bmax ) const" );

}


Point3df gkg::ARealTimeOdfFieldObject::VoxelSize() const
{

  try
  {

    return Point3df( _odfCartesianField.getResolutionX(),
                     _odfCartesianField.getResolutionY(),
                     _odfCartesianField.getResolutionZ() ); 

  }
  GKG_CATCH( "Point3df gkg::ARealTimeOdfFieldObject::VoxelSize() const" );

}


bool gkg::ARealTimeOdfFieldObject::render( 
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
  GKG_CATCH( "bool gkg::ARealTimeOdfFieldObject::render( "
             "anatomist::PrimList& primitiveList, "
             "const anatomist::ViewState& viewState )" );

}


bool gkg::ARealTimeOdfFieldObject::glMakeBodyGLL( 
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
#else
    Point3df position = viewState.window->GetPosition();
#endif

    glEnableClientState( GL_VERTEX_ARRAY );
    _qGLShaderProgram->enableAttributeArray(
                                       _attributeLocationCurrentVertexIndices );
    // approximate normal
    _qGLShaderProgram->enableAttributeArray( 
                                _attributeLocationSecondAndThirdVertexIndices );
    _qGLShaderProgram->enableAttributeArray( 
                                    _attributeLocationSecondVertexCoordinates );
    _qGLShaderProgram->enableAttributeArray( 
                                     _attributeLocationThirdVertexCoordinates );

    glVertexPointer( 3, GL_FLOAT, 0, _vertexCoordinates.constData() );

    gkg::Vector3d< int32_t > begin;
    gkg::Vector3d< int32_t > end = gkg::Vector3d< int32_t >( 0, 0, 0 );

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
      if ( quaternionVector == Point4df( 0, 0, 0, 1 ) )
      {

        isOrthogonal = true;
        int32_t z = (int32_t )ceil( position[ 2 ] /
                                    _odfCartesianField.getResolutionZ() );
        begin.x = 0;
        begin.y = 0;
        begin.z = z;
        end.x = _odfCartesianField.getSizeX();
        end.y = _odfCartesianField.getSizeY();
        end.z = z + 1;
        int32_t maximumZ = _odfCartesianField.getSizeZ();

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
      if ( quaternionVector == Point4df( c, 0, 0, c  ) )
      {

        isOrthogonal = true;
        int32_t y = (int32_t)ceil( position[ 1 ] / 
                                   _odfCartesianField.getResolutionY() );
        begin.x = 0;
        begin.y = y;
        begin.z = 0;
        end.x = _odfCartesianField.getSizeX();
        end.y = y + 1;
        end.z = _odfCartesianField.getSizeZ();
        int32_t maximumY = _odfCartesianField.getSizeY();

        if ( end.y > maximumY )
        {

          end.y = maximumY;

        }
        if ( begin.y < 0 )
        {

          begin.y = 0;

        }

      }

      if ( quaternionVector == Point4df( -0.5, -0.5, -0.5, 0.5 ) )
      {

        isOrthogonal = true;
        int32_t x = (int32_t)ceil( position[ 0 ] / 
                                   _odfCartesianField.getResolutionX() );
        begin.x = x;
        begin.y = 0;
        begin.z = 0;
        end.x = x + 1;
        end.y = _odfCartesianField.getSizeY();
        end.z = _odfCartesianField.getSizeZ();
        int32_t maximumX = _odfCartesianField.getSizeX();

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

        this->drawOrthogonalPlane( begin, end );

      }
      else
      {

        this->drawTransversalPlane( sliceVState );

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
  GKG_CATCH( "bool gkg::ARealTimeOdfFieldObject::glMakeBodyGLL( "
             "const anatomist::ViewState& viewState, "
             "const anatomist::GLList& glList ) const" );

}


int32_t gkg::ARealTimeOdfFieldObject::getClassType()
{

  return _classType;

}


void gkg::ARealTimeOdfFieldObject::getSphereVertexCoordinatesAndIndices()
{

  try
  {

    gkg::MeshMap< int32_t, float, 3U > sphereMeshMap;
    gkg::ConvexHull::getInstance().addConvexHull( 
                                              _orientationSet.getOrientations(),
                                              0,
                                              sphereMeshMap );
    // clearing structures
    _vertexCoordinates.clear();
    _vertexIndices.clear();

    // creating vertex coordinates
    const std::list< gkg::Vector3d< float > >&
      vertices = sphereMeshMap.vertices.getSites( 0 );

    _vertexCoordinates.resize( 3U * vertices.size() );

    std::list< gkg::Vector3d< float > >::const_iterator
      v = vertices.begin(),
      ve = vertices.end();
    int32_t index = 0;
    while ( v != ve )
    {

      _vertexCoordinates[ index ++ ] = ( GLfloat )v->x;
      _vertexCoordinates[ index ++ ] = ( GLfloat )v->y;
      _vertexCoordinates[ index ++ ] = ( GLfloat )v->z;
      ++ v;

    }

    // creating vertex indices
    const std::list< gkg::Polygon< 3U > >&
      polygons = sphereMeshMap.polygons.getPolygons( 0 );

    _vertexIndices.resize( 3U * polygons.size() );

    std::list< gkg::Polygon< 3U > >::const_iterator
      p = polygons.begin(),
      pe = polygons.end();
    index = 0;
    while ( p != pe )
    {

      _vertexIndices[ index ++ ] = ( GLuint )p->indices[ 0 ];
      _vertexIndices[ index ++ ] = ( GLuint )p->indices[ 1 ];
      _vertexIndices[ index ++ ] = ( GLuint )p->indices[ 2 ];
      ++ p;

    }

    _currentVertexIndices = new GLfloat[ _outputOrientationCount ];
    for ( index = 0; index < _outputOrientationCount; index++ )
    {

      _currentVertexIndices[ index ] = ( GLfloat )index;

    }

    getApproximatedNormals();

  }
  GKG_CATCH( 
       "void "
       "gkg::ARealTimeOdfFieldObject::getSphereVertexCoordinatesAndIndices()" );

}


void gkg::ARealTimeOdfFieldObject::drawOrthogonalPlane( 
                                     const gkg::Vector3d< int32_t >& begin,
                                     const gkg::Vector3d< int32_t >& end ) const
{

  try
  {

    _qGLShaderProgram->setAttributeValue( _attributeLocationMeshScale,
                                          _meshScale );
    _qGLShaderProgram->setUniformValue( _uniformIsShaderedObject, 1 );

    _qGLShaderProgram->setAttributeArray(
                                  _attributeLocationSecondAndThirdVertexIndices,
                                  _secondAndThirdVertexIndices );
    _qGLShaderProgram->setAttributeArray(
                                      _attributeLocationSecondVertexCoordinates,
                                      _secondVertexCoordinates );
    _qGLShaderProgram->setAttributeArray(
                                       _attributeLocationThirdVertexCoordinates,
                                       _thirdVertexCoordinates );

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    float resolutionX = _odfCartesianField.getResolutionX();
    float resolutionY = _odfCartesianField.getResolutionY();
    float resolutionZ = _odfCartesianField.getResolutionZ();

    for ( z = begin.z; z < end.z; ++z )
    {

      for ( y = begin.y; y < end.y; ++y )
      {

        for ( x = begin.x; x < end.x; ++x )
        {

          const gkg::MeshDeformation< float >* meshDeformation = 
                                         _odfCartesianField.getItem( x, y, z );

          if ( meshDeformation )
          {

            QVector4D translation( x * resolutionX, 
                                   y * resolutionY, 
                                   z * resolutionZ, 
                                   0.0 );
            _qGLShaderProgram->setUniformValue( _uniformLocationTranslation,
                                                translation );
            _qGLShaderProgram->setUniformValueArray(
                             _uniformLocationTransformation,
                             (GLfloat*)&meshDeformation->getDeformations()[ 0 ],
                             _outputOrientationCount,
                             1 );
            _glList->callList();

          }

        }

      }

    }

  }
  GKG_CATCH( "void gkg::ARealTimeOdfFieldObject::drawOrthogonalPlane( "
             "const gkg::Vector3d< int32_t >& begin, "
             "gconst gkg::Vector3d< int32_t >& end ) const" );

}


void gkg::ARealTimeOdfFieldObject::drawTransversalPlane(
                            const anatomist::SliceViewState* sliceVState ) const
{

  try
  {

    _qGLShaderProgram->setAttributeValue( _attributeLocationMeshScale,
                                          _meshScale );
    _qGLShaderProgram->setUniformValue( _uniformIsShaderedObject, 1 );

    _qGLShaderProgram->setAttributeArray(
                                  _attributeLocationSecondAndThirdVertexIndices,
                                  _secondAndThirdVertexIndices );
    _qGLShaderProgram->setAttributeArray( 
                                      _attributeLocationSecondVertexCoordinates,
                                      _secondVertexCoordinates );
    _qGLShaderProgram->setAttributeArray(
                                      _attributeLocationThirdVertexCoordinates, 
                                      _thirdVertexCoordinates );

    const aims::Quaternion* quaternion = sliceVState->orientation;
    gkg::Vector3d< double > resolution = _odfCartesianField.getResolution();
    float voxelSize = ( resolution.x + resolution.y + resolution.z ) / 6.0f;

    Point3df n = quaternion->transformInverse( Point3df( 0, 0, 1 ) );
    Point4df plane( n[ 0 ], n[ 1 ], n[ 2 ], -n.dot( sliceVState->position ) );
    float normalizationFactor = std::sqrt( n[ 0 ] * n[ 0 ] +
                                           n[ 1 ] * n[ 1 ] +
                                           n[ 2 ] * n[ 2 ] );
    int32_t sizeX = _odfCartesianField.getSizeX();
    int32_t sizeY = _odfCartesianField.getSizeY();
    int32_t sizeZ = _odfCartesianField.getSizeZ();
    int32_t x, y, z;

    for ( z = 0; z < sizeZ; ++z )
    {

      for ( y = 0; y < sizeY; ++y )
      {

        for ( x = 0; x < sizeX; ++x )
        {

          const gkg::MeshDeformation< float >* meshDeformation = 
                                         _odfCartesianField.getItem( x, y, z );

          if ( meshDeformation )
          {

            QVector4D translation( x * resolution.x, 
                                   y * resolution.y, 
                                   z * resolution.z, 
                                   0.0 );
            float result = std::abs( ( plane[ 0 ] * translation.x() ) +
                                     ( plane[ 1 ] * translation.y() ) +
                                     ( plane[ 2 ] * translation.z() ) +
                                     plane[ 3 ] ) / normalizationFactor;


            if ( result < voxelSize )
            {

              _qGLShaderProgram->setUniformValue( _uniformLocationTranslation,
                                                  translation );
              _qGLShaderProgram->setUniformValueArray(
                           _uniformLocationTransformation,
                           (GLfloat*)&(meshDeformation->getDeformations()[ 0 ]),
                           _outputOrientationCount,
                           1 );
              _glList->callList();

            }

          }

        }

      }

    }

  }
  GKG_CATCH( "void gkg::ARealTimeOdfFieldObject::drawTransversalPlane( "
             "const anatomist::SliceViewState* sliceVState ) const" );

}


void gkg::ARealTimeOdfFieldObject::updateShaderCode()
{

  try
  {

    if ( _qGLShader )
    {

      delete _qGLShader;
      delete _qGLShaderProgram;

    }

    _qGLShader = new QGLShader( QGLShader::Vertex );

    double minimumResolution = std::min( std::min( 
                                         _odfCartesianField.getResolutionX(),
                                         _odfCartesianField.getResolutionY() ),
                                    _odfCartesianField.getResolutionZ() ) / 2.0;

    _qGLShader->compileSourceCode( gkg::shaderCodeWithApproximatedNormals(
         gkg::StringConverter::toString( _outputOrientationCount ),
         gkg::StringConverter::toString( minimumResolution, 10, 3 ) ).c_str() );

    _qGLShaderProgram = new QGLShaderProgram();
    _qGLShaderProgram->addShader( _qGLShader );
    _qGLShaderProgram->link();

  }
  GKG_CATCH( "void gkg::ARealTimeOdfFieldObject::updateShaderCode()" );

}


void gkg::ARealTimeOdfFieldObject::setShaderAttributes()
{

  try
  {

    _qGLShaderProgram->bind();

    _attributeLocationMeshScale = _qGLShaderProgram->attributeLocation( 
                                                                  "meshScale" );
    _uniformIsShaderedObject = _qGLShaderProgram->uniformLocation( 
                                                           "isShaderedObject" );

    // approximate normal
    _attributeLocationSecondVertexCoordinates =
                         _qGLShaderProgram->attributeLocation( "coordinates2" );
    _attributeLocationThirdVertexCoordinates =
                         _qGLShaderProgram->attributeLocation( "coordinates3" );
    _attributeLocationSecondAndThirdVertexIndices =
                       _qGLShaderProgram->attributeLocation( "polygonIndices" );

    _uniformLocationTranslation = _qGLShaderProgram->uniformLocation(
                                                                "translation" );
    _uniformLocationTransformation = _qGLShaderProgram->uniformLocation(
                                                             "transformation" );
    _attributeLocationCurrentVertexIndices =
                                _qGLShaderProgram->attributeLocation( "index" );
    _qGLShaderProgram->release();

    if ( _glList )
    {

      delete _glList;

    }

    _glList = new anatomist::GLList();

  }
  GKG_CATCH( "void gkg::ARealTimeOdfFieldObject::setShaderAttributes()" );

}


void gkg::ARealTimeOdfFieldObject::getApproximatedNormals()
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

      i += 3;

    }

  }
  GKG_CATCH( "void ARealTimeOdfObject::getApproximatedNormals()" );

}


int32_t gkg::ARealTimeOdfFieldObject::registerClass()
{

  try
  {

    _classType = registerObjectType( "RealTimeODF" );

#ifdef GKG_DEBUG

    std::cout << "Real-time ODF registered : " << _classType << std::endl;

#endif

    return _classType;

  }
  GKG_CATCH( "int32_t gkg::ARealTimeOdfFieldObject::registerClass()" );

}
