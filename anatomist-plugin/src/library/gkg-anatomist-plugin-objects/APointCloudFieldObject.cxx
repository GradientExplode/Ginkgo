#include <gkg-anatomist-plugin-objects/APointCloudFieldObject.h>
#include <gkg-anatomist-plugin-rendering-options/APointCloudFieldObjectRenderingOptionsWidget.h>
#include <gkg-dmri-io/DefaultAsciiItemWriterOfOdf_i.h>
#include <gkg-dmri-io/DefaultBinaryItemWriterOfOdf_i.h>
#include <gkg-dmri-io/DefaultBSwapItemWriterOfOdf_i.h>
#include <gkg-dmri-io/DefaultAsciiItemReaderOfOdf_i.h>
#include <gkg-dmri-io/DefaultBinaryItemReaderOfOdf_i.h>
#include <gkg-core-io/DefaultBSwapItemReaderOfStdVector_i.h>
#include <gkg-core-io/DefaultAsciiItemWriterOfStdVector_i.h>
#include <gkg-core-io/DefaultBinaryItemWriterOfStdVector_i.h>
#include <gkg-core-io/DefaultBSwapItemWriterOfStdVector_i.h>
#include <gkg-core-io/DefaultAsciiItemReaderOfStdVector_i.h>
#include <gkg-core-io/DefaultBinaryItemReaderOfStdVector_i.h>
#include <gkg-core-io/DefaultBSwapItemReaderOfStdVector_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-processing-mesh/MeshFactory.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-io/TextureMapAnalyzer_i.h>
#include <gkg-processing-io/TextureMapDiskFormat_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/CartesianField_i.h>
#include <gkg-processing-coordinates/AntipodallySymmetricUniformOrientationSet.h>
#include <gkg-processing-mesh/ConvexHull_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/Exception.h>


#include <anatomist/surface/glcomponent.h>

#include <anatomist/object/actions.h>
#include <anatomist/control/qObjTree.h>
#include <anatomist/window/Window.h>
#include <aims/resampling/quaternion.h>
#include <map>
#include <fstream>
#include <GL/gl.h>




template class gkg::TextureMapAnalyzer< std::vector< gkg::Vector3d< float > > >;
template class gkg::DiskFormatFactory<
                     gkg::TextureMap< std::vector< gkg::Vector3d< float > > > >;
template class gkg::TextureMapDiskFormat<
                                        std::vector< gkg::Vector3d< float > > >;

RegisterTextureAnalyzer( std::vector< gkg::Vector3d< float > >,
                         gkg_odf_gibbs_sampling );
RegisterTextureDiskFormat( std::vector< gkg::Vector3d< float > >,
                           gkg_odf_gibbs_sampling );



////////////////////////////////////////////////////////////////////////////////
// class APointCloudFieldObject
////////////////////////////////////////////////////////////////////////////////

int32_t gkg::APointCloudFieldObject::_classType =
                                   gkg::APointCloudFieldObject::registerClass();
Tree* gkg::APointCloudFieldObject::_optionTree = 0;



gkg::APointCloudFieldObject::APointCloudFieldObject(
                          const gkg::RCPointer<
                                  gkg::CartesianField<
                                    std::vector< gkg::Vector3d< float > > > >&
                                                      pointCloudCartesianField )
                            : QObject(),
                              anatomist::SliceableObject(),
                              _pointCloudCartesianField(
                                                     pointCloudCartesianField ),
                              _vertexCountPerSphere( 0 ),
                              _pointRadius( 0.05f ),
                              _meshScale( 1.0f ),
                              _isAlreadyDisplayed( false ),
                              _sphereList( 0 )
{

  try
  {

    _type = gkg::APointCloudFieldObject::_classType;


    const gkg::SiteMap< int32_t, int32_t >& 
      siteMap = _pointCloudCartesianField->getSiteMap();

    _size.x = 1;
    if ( siteMap.getHeader().hasAttribute( "sizeX" ) )
    {

      siteMap.getHeader().getAttribute( "sizeX", _size.x );

    }
    _size.y = 1;
    if ( siteMap.getHeader().hasAttribute( "sizeY" ) )
    {

      siteMap.getHeader().getAttribute( "sizeY", _size.y );

    }
    _size.z = 1;
    if ( siteMap.getHeader().hasAttribute( "sizeZ" ) )
    {

      siteMap.getHeader().getAttribute( "sizeZ", _size.z );

    }
    _resolution.x = 1.0;
    if ( siteMap.getHeader().hasAttribute( "resolutionX" ) )
    {

      siteMap.getHeader().getAttribute( "resolutionX", _resolution.x );

    }
    _resolution.y = 1.0;
    if ( siteMap.getHeader().hasAttribute( "resolutionY" ) )
    {

      siteMap.getHeader().getAttribute( "resolutionY", _resolution.y );

    }
    _resolution.z = 1.0;
    if ( siteMap.getHeader().hasAttribute( "resolutionZ" ) )
    {

      siteMap.getHeader().getAttribute( "resolutionZ", _resolution.z );

    }
    _halfMinimumResolution = std::min( std::min( _resolution.x,
                                                 _resolution.y ),
                                       _resolution.z ) / 2.0; 
    _offset.x = 1;
    if ( siteMap.getHeader().hasAttribute( "offsetX" ) )
    {

      siteMap.getHeader().getAttribute( "offsetX", _offset.x );

    }
    _offset.y = 1;
    if ( siteMap.getHeader().hasAttribute( "offsetY" ) )
    {

      siteMap.getHeader().getAttribute( "offsetY", _offset.y );

    }
    _offset.z = 1;
    if ( siteMap.getHeader().hasAttribute( "offsetZ" ) )
    {

      siteMap.getHeader().getAttribute( "offsetZ", _offset.z );

    }

    _scaling = ( float )std::min( std::min( _resolution.x, _resolution.y ),
                                  _resolution.z ) / 2.0f;

    _boundingBox = gkg::BoundingBox< float >( 
                      _offset.x * _resolution.x,
                      ( _offset.x + _size.x - 1 ) * _resolution.x,
                      _offset.x * _resolution.x,
                      ( _offset.y + _size.y - 1 ) * _resolution.y,
                      _offset.x * _resolution.x,
                      ( _offset.z + _size.z - 1 ) * _resolution.z );


    std::cout << "boundingBox=( "
              << _boundingBox.getLowerX() << ", "
              << _boundingBox.getLowerY() << ", "
              << _boundingBox.getLowerZ() << " ) -> ( "
              << _boundingBox.getUpperX() << ", "
              << _boundingBox.getUpperY() << ", "
              << _boundingBox.getUpperZ() << " )"
              << std::endl;


    // computing translations
    this->computePointCloudTranslations();

    std::cout << "computePointCloudTranslations done" << std::endl;

    // setting default sphere parameters
    this->setVertexCountPerSphere( 60 );
    this->setPointRadius( 0.05 );
    this->setMeshScale( 1.0 );


    // setting default point cloud color
    this->GetMaterial().SetDiffuse( 0.80, 0.80, 0.80, 1.0 );


    // adding icon
    if ( QObjectTree::TypeNames.find( _classType ) == 
         QObjectTree::TypeNames.end() )
    {

      std::string
          iconPath = gkg::ConfigurationInfo::getInstance().getIconPath(
                                            "icon-point-cloud.png" );
      if ( !QObjectTree::TypeIcons[ _classType ].load( iconPath.c_str() ) )
      {

        QObjectTree::TypeIcons.erase( _classType );
        std::cerr << "icon " << iconPath << " not found!" << std::endl;

      }
      QObjectTree::TypeNames[ _classType ] = "Point Cloud Cartesian Field";

    }


    std::cout << "end of APointCloudFieldObject constructor" << std::endl;

  }
  GKG_CATCH( "gkg::APointCloudFieldObject::APointCloudFieldObject( "
             "const gkg::RCPointer< gkg::CartesianField< "
             "std::vector< gkg::Vector3d< float > > > >& "
             "pointCloudCartesianField )" );

}


gkg::APointCloudFieldObject::~APointCloudFieldObject()
{

  if ( _sphereList )
  {

    delete _sphereList;

  }

}


bool gkg::APointCloudFieldObject::Is2DObject()
{

  try
  {

    return true;

  }
  GKG_CATCH( "bool gkg::APointCloudFieldObject::Is2DObject()" );

}


bool gkg::APointCloudFieldObject::Is3DObject()
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool gkg::APointCloudFieldObject::Is3DObject()" );

}


bool gkg::APointCloudFieldObject::IsFusion2DAllowed()
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool gkg::APointCloudFieldObject::Is3DObject()" );

}


Tree* gkg::APointCloudFieldObject::optionTree() const
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
      t2->setProperty( "callback",
                       &anatomist::ObjectActions::fileReload );
      t->insert( t2 );
      t2 = new Tree( true, "Rename object" );
      t2->setProperty( "callback",
                       &anatomist::ObjectActions::renameObject );
      t->insert( t2 );

      t = new Tree( true, "Color" );
      _optionTree->insert( t );
      t2 = new Tree( true, "Material" );
      t2->setProperty( "callback",
                       &anatomist::ObjectActions::colorMaterial );
      t->insert( t2 );

      t = new Tree( true, "Referential" );
      _optionTree->insert( t );
      t2 = new Tree( true, "Load" );
      t2->setProperty( "callback",
                       &anatomist::ObjectActions::referentialLoad );
      t->insert( t2 );
      t = new Tree( true, "Render" );
      _optionTree->insert( t );
      t2 = new Tree( true, "Options" );
      t2->setProperty( "callback",
                   &gkg::APointCloudFieldObjectRenderingOptionsWidget::create );
      t->insert( t2 );

    }
    return( _optionTree );

  }
  GKG_CATCH( "Tree* gkg::APointCloudFieldObject::optionTree() const" );

}


float gkg::APointCloudFieldObject::MinX2D() const
{

  try
  {

    return _boundingBox.getLowerX() - ( float )_resolution.x / 2.0f;

  }
  GKG_CATCH( "float gkg::APointCloudFieldObject::MinX2D() const" );

}


float gkg::APointCloudFieldObject::MinY2D() const
{

  try
  {

    return _boundingBox.getLowerY() - ( float )_resolution.y / 2.0f;

  }
  GKG_CATCH( "float gkg::APointCloudFieldObject::MinY2D() const" );

}


float gkg::APointCloudFieldObject::MinZ2D() const
{

  try
  {

    return _boundingBox.getLowerZ() - ( float )_resolution.z / 2.0f;

  }
  GKG_CATCH( "float gkg::APointCloudFieldObject::MinZ2D() const" );

}


float gkg::APointCloudFieldObject::MaxX2D() const
{

  try
  {

    return _boundingBox.getUpperX() + ( float )_resolution.x / 2.0f;

  }
  GKG_CATCH( "float gkg::APointCloudFieldObject::MaxX2D() const" );

}


float gkg::APointCloudFieldObject::MaxY2D() const
{

  try
  {

    return _boundingBox.getUpperY() + ( float )_resolution.y / 2.0f;

  }
  GKG_CATCH( "float gkg::APointCloudFieldObject::MaxY2D() const" );

}


float gkg::APointCloudFieldObject::MaxZ2D() const
{

  try
  {

    return _boundingBox.getUpperZ() + ( float )_resolution.z / 2.0f;

  }
  GKG_CATCH( "float gkg::APointCloudFieldObject::MaxZ2D() const" );

}


bool gkg::APointCloudFieldObject::boundingBox(
                                        std::vector< float > & bmin,
                                        std::vector< float > & bmax ) const
{

  try
  {

    bmin.resize( 3 );
    bmax.resize( 3 );

    bmin[ 0 ] = _boundingBox.getLowerX() - ( float )_resolution.x / 2.0f;
    bmin[ 1 ] = _boundingBox.getLowerY() - ( float )_resolution.y / 2.0f;
    bmin[ 2 ] = _boundingBox.getLowerZ() - ( float )_resolution.z / 2.0f;

    bmax[ 0 ] = _boundingBox.getUpperX() + ( float )_resolution.x / 2.0f;
    bmax[ 1 ] = _boundingBox.getUpperY() + ( float )_resolution.y / 2.0f;
    bmax[ 2 ] = _boundingBox.getUpperZ() + ( float )_resolution.z / 2.0f;


    return true;

  }
  GKG_CATCH( "bool gkg::APointCloudFieldObject::boundingBox( "
             "std::vector< float > & bmin, "
             "std::vector< float > & bmax ) const" );

}


int32_t gkg::APointCloudFieldObject::getVertexCountPerSphere() const
{

  try
  {

    return _vertexCountPerSphere;

  }
  GKG_CATCH( "int32_t gkg::APointCloudFieldObject::"
             "getVertexCountPerSphere() const" );

}


void gkg::APointCloudFieldObject::setVertexCountPerSphere( int32_t vertexCount )
{

  try
  {


    bool changed = ( vertexCount != _vertexCountPerSphere );

    _vertexCountPerSphere = vertexCount;

    if ( changed )
    {

      // creating output orientation set
      _outputOrientationSet.reset(
         new gkg::AntipodallySymmetricUniformOrientationSet( 
                                                      _vertexCountPerSphere ) );


      // building unit sphere for GL rendering
      _unitSphere.reset( new gkg::MeshMap< int32_t, float, 3U > );

      gkg::ConvexHull::getInstance().addConvexHull( 
                                       _outputOrientationSet->getOrientations(),
                                       0,
                                       *_unitSphere );

      // filling unit sphere vertex indices
      _unitSphereVertexIndices.resize( _vertexCountPerSphere );
      GLint i = 0;
      for( i = 0; i < ( GLint )_vertexCountPerSphere; i++ )
      {

        _unitSphereVertexIndices[ i ] = ( GLfloat )i;

      }

      // filling vertex coordinates
      _unitSphereVertexCoordinates.resize( 3U * _vertexCountPerSphere );

      std::list< gkg::Vector3d< float > >::const_iterator
        v = _unitSphere->vertices.getSites( 0 ).begin(),
        ve = _unitSphere->vertices.getSites( 0 ).end();
      std::vector< GLfloat >::iterator 
        vc = _unitSphereVertexCoordinates.begin();
      while ( v != ve )
      {

        *vc = ( GLfloat )v->x;
        ++ vc;

        *vc = ( GLfloat )v->y;
        ++ vc;

        *vc = ( GLfloat )v->z;
        ++ vc;

        ++ v;

      }


      // filling vertex normals
      _unitSphereVertexNormals.resize( _vertexCountPerSphere );

      std::list< gkg::Vector3d< float > > 
        normals = _unitSphere->getNormals( 0 );


      std::list< gkg::Vector3d< float > >::const_iterator
        n = normals.begin(),
        ne = normals.end();
      std::vector< QVector3D >::iterator 
        vn = _unitSphereVertexNormals.begin();
      while ( n != ne )
      {

        *vn = QVector3D( ( GLfloat )n->x,
                         ( GLfloat )n->y,
                         ( GLfloat )n->z );
                         
        ++ vn;
        ++ n;

      }


      // filling vertex indices
      _unitSpherePolygonIndices.resize(
                           3U * _unitSphere->polygons.getPolygons( 0 ).size() );

      std::list< gkg::Polygon< 3U > >::const_iterator
        p = _unitSphere->polygons.getPolygons( 0 ).begin(),
        pe = _unitSphere->polygons.getPolygons( 0 ).end();
      std::vector< GLuint >::iterator 
        vi = _unitSpherePolygonIndices.begin();
      while ( p != pe )
      {

        *vi = ( GLuint )p->indices[ 0 ];
        ++ vi;

        *vi = ( GLuint )p->indices[ 1 ];
        ++ vi;

        *vi = ( GLuint )p->indices[ 2 ];
        ++ vi;

        ++ p;

      }

      // updating the shader code
      if ( _isAlreadyDisplayed )
      {

        this->updateShader();
        std::cout << "shader updated" << std::endl;

      }

    }

  }
  GKG_CATCH( "void gkg::APointCloudFieldObject::"
             "setVertexCountPerOdf( "
             "int32_t vertexCount )" );

}


float gkg::APointCloudFieldObject::getPointRadius() const
{

  try
  {

    return _pointRadius;

  }
  GKG_CATCH( "float gkg::APointCloudFieldObject::"
             "getPointRadius() const" );

}


void gkg::APointCloudFieldObject::setPointRadius( float pointRadius )
{

  try
  {


    //bool changed = ( pointRadius != _pointRadius );

    _pointRadius = pointRadius;

    // we don't have to update the shader when modifying the point radius
    // because it is an attribute of the shader that can be set as an
    // input of the shader

  }
  GKG_CATCH( "void gkg::APointCloudFieldObject::"
             "setPointRadius( "
             "float pointRadius )" );

}


float gkg::APointCloudFieldObject::getMeshScale() const
{

  try
  {

    return _meshScale;

  }
  GKG_CATCH( "float gkg::APointCloudFieldObject::"
             "getMeshScale() const" );

}


void gkg::APointCloudFieldObject::setMeshScale( float meshScale )
{

  try
  {


    //bool changed = ( meshScale != _meshScale );

    _meshScale = meshScale;

    // we don't have to update the shader when modifying the mesh scale
    // because it is an attribute of the shader that can be set as an
    // input of the shader

  }
  GKG_CATCH( "void gkg::APointCloudFieldObject::"
             "setMeshScale( "
             "float meshScale )" );

}


bool gkg::APointCloudFieldObject::render( anatomist::PrimList& primitiveList,
                                   const anatomist::ViewState& viewState )
{

  try
  {

    _isAlreadyDisplayed = true;


    if ( _shader.isNull() )
    {

      // updating the shader code
      this->updateShader();

    }

    if ( !_sphereList->item() )
    {

      _sphereList->generate();

    }

    anatomist::GLComponent* glComponent = anatomist::GLComponent::glAPI();
    glComponent->glSetChanged( glBODY );

    return anatomist::AObject::render( primitiveList, viewState );

  }
  GKG_CATCH( "bool gkg::APointCloudFieldObject::render( "
             "anatomist::PrimList& primitiveList, "
             "const anatomist::ViewState& viewState )" );

}


bool gkg::APointCloudFieldObject::glMakeBodyGLL(
                                     const anatomist::ViewState& viewState,
                                     const anatomist::GLList& glList ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // detecting whether it is an orthogonale plane and draw items
    ////////////////////////////////////////////////////////////////////////////

    const anatomist::SliceViewState* sliceVState = viewState.sliceVS();

    Point3df position = viewState.window->getPosition();

    const aims::Quaternion* quaternion = sliceVState->orientation;
    Point4df quaternionVector = quaternion->vector();

    float OneOverSquareRootOfTwo = ( float )( 1.0 / std::sqrt( 2.0 ) );
    Point4df perpendicularPlaneToXAxis( -0.5f, -0.5f, -0.5f, 0.5f );
    Point4df perpendicularPlaneToYAxis( OneOverSquareRootOfTwo, 0.0f, 0.0f,
                                        OneOverSquareRootOfTwo );
    Point4df perpendicularPlaneToZAxis( 0.0f, 0.0f, 0.0f, 1.0f );

    ////////////////////////////////////////////////////////////////////////////
    // collecting transparency
    ////////////////////////////////////////////////////////////////////////////
 
    const anatomist::Material& material = this->material();
    float red = ( float )material.Diffuse( 0 );
    float green = ( float )material.Diffuse( 1 );
    float blue = ( float )material.Diffuse( 2 );
    float opacity = ( float )material.Diffuse( 3 );
 

    ////////////////////////////////////////////////////////////////////////////
    // enabling GL vertex array
    ////////////////////////////////////////////////////////////////////////////

    glEnableClientState( GL_VERTEX_ARRAY );

    _shaderProgram->enableAttributeArray(
                                      _attributeLocationUnitSphereVertexIndex );

    glVertexPointer( 3, GL_FLOAT, 0, _unitSphereVertexCoordinates.data() );


    ////////////////////////////////////////////////////////////////////////////
    // generating the GL list corresponding to a plane
    ////////////////////////////////////////////////////////////////////////////

    glNewList( glList.item(), GL_COMPILE );


    bool isBinded = _shaderProgram->bind();
    if ( isBinded )
    {

      _shaderProgram->setAttributeArray(
                                        _attributeLocationUnitSphereVertexIndex,
                                        _unitSphereVertexIndices.data(),
                                        1 );

      _shaderProgram->setAttributeValue( _attributeLocationRed,
                                         red );
      _shaderProgram->setAttributeValue( _attributeLocationGreen,
                                         green );
      _shaderProgram->setAttributeValue( _attributeLocationBlue,
                                         blue );
      _shaderProgram->setAttributeValue( _attributeLocationOpacity,
                                         opacity );
      _shaderProgram->setAttributeValue( _attributeLocationPointRadius,
                                         _pointRadius );
      _shaderProgram->setAttributeValue( _attributeLocationMeshScale,
                                         _meshScale );
      _shaderProgram->setUniformValue( _uniformIsShaderedObject, 1 );

      //bool isOrthogonal = false;
      if ( quaternionVector == perpendicularPlaneToZAxis )
      {

        //isOrthogonal = true;

        int32_t z = ( int32_t )( ( position[ 2 ] - _boundingBox.getLowerZ() ) /
                                 _resolution.z + 0.5 );

        int32_t x = 0;
        int32_t y = 0;
        for ( y = 0; y < _size.y; y++ )
        {

          for ( x = 0; x < _size.x; x++ )
          {

            const std::vector< gkg::Vector3d< float > >* pointCloud =
              _pointCloudCartesianField->getItem( x, y, z );
            if ( pointCloud )
            {

              std::vector< gkg::Vector3d< float > >::const_iterator
                p = pointCloud->begin(),
                pe = pointCloud->end();
              while ( p != pe )
              {

              
                _shaderProgram->setUniformValue(
                                 _uniformLocationTranslation,
                                 *_pointCloudTranslations->getItem( x, y, z ) );

                _shaderProgram->setUniformValue( _uniformLocationPoint,
                                                 QVector4D( ( GLfloat )p->x,
                                                            ( GLfloat )p->y,
                                                            ( GLfloat )p->z,
                                                            ( GLfloat )0.0 ) );

                _shaderProgram->setUniformValueArray(
                                        _uniformLocationNormals,
                                        _unitSphereVertexNormals.data(),
                                        _vertexCountPerSphere );

                // calling the display list which renders the sphere
                _sphereList->callList();

                ++ p;

              }

            }

          }

        }

      }
      else if ( ( ( quaternionVector[ 0 ] - perpendicularPlaneToYAxis[ 0 ] ) *
                  ( quaternionVector[ 0 ] - perpendicularPlaneToYAxis[ 0 ] ) +
                  ( quaternionVector[ 1 ] - perpendicularPlaneToYAxis[ 1 ] ) *
                  ( quaternionVector[ 1 ] - perpendicularPlaneToYAxis[ 1 ] ) +
                  ( quaternionVector[ 2 ] - perpendicularPlaneToYAxis[ 2 ] ) *
                  ( quaternionVector[ 2 ] - perpendicularPlaneToYAxis[ 2 ] ) )
                < 1e-10 )
      {

        //isOrthogonal = true;

        int32_t y = ( int32_t )( ( position[ 1 ] - _boundingBox.getLowerY() ) /
                                 _resolution.y + 0.5 );

        int32_t x = 0;
        int32_t z = 0;
        for ( z = 0; z < _size.z; z++ )
        {

          for ( x = 0; x < _size.x; x++ )
          {

            const std::vector< gkg::Vector3d< float > >* pointCloud =
              _pointCloudCartesianField->getItem( x, y, z );
            if ( pointCloud )
            {

              std::vector< gkg::Vector3d< float > >::const_iterator
                p = pointCloud->begin(),
                pe = pointCloud->end();
              while ( p != pe )
              {

              
                _shaderProgram->setUniformValue(
                                 _uniformLocationTranslation,
                                 *_pointCloudTranslations->getItem( x, y, z ) );

                _shaderProgram->setUniformValue( _uniformLocationPoint,
                                                 QVector4D( ( GLfloat )p->x,
                                                            ( GLfloat )p->y,
                                                            ( GLfloat )p->z,
                                                            ( GLfloat )0.0 ) );

                _shaderProgram->setUniformValueArray(
                                        _uniformLocationNormals,
                                        _unitSphereVertexNormals.data(),
                                        _vertexCountPerSphere );

                // calling the display list which renders the sphere
                _sphereList->callList();

                ++ p;

              }

            }

          }

        }

      }
      else if ( quaternionVector == perpendicularPlaneToXAxis )
      {

        //isOrthogonal = true;

        int32_t x = ( int32_t )( ( position[ 0 ] - _boundingBox.getLowerX() ) /
                                 _resolution.x + 0.5 );

        int32_t y = 0;
        int32_t z = 0;
        for ( z = 0; z < _size.z; z++ )
        {

          for ( y = 0; y < _size.y; y++ )
          {

            const std::vector< gkg::Vector3d< float > >* pointCloud =
              _pointCloudCartesianField->getItem( x, y, z );
            if ( pointCloud )
            {

              std::vector< gkg::Vector3d< float > >::const_iterator
                p = pointCloud->begin(),
                pe = pointCloud->end();
              while ( p != pe )
              {

              
                _shaderProgram->setUniformValue(
                                 _uniformLocationTranslation,
                                 *_pointCloudTranslations->getItem( x, y, z ) );

                _shaderProgram->setUniformValue( _uniformLocationPoint,
                                                 QVector4D( ( GLfloat )p->x,
                                                            ( GLfloat )p->y,
                                                            ( GLfloat )p->z,
                                                            ( GLfloat )0.0 ) );

                _shaderProgram->setUniformValueArray(
                                        _uniformLocationNormals,
                                        _unitSphereVertexNormals.data(),
                                        _vertexCountPerSphere );

                // calling the display list which renders the sphere
                _sphereList->callList();

                ++ p;

              }

            }

          }

        }

      }

      _shaderProgram->setUniformValue( _uniformIsShaderedObject, 0 );

      _shaderProgram->release();

    }


    // drawing bounding box with black lines
    glColor4f( ( GLfloat )0,
               ( GLfloat )0,
               ( GLfloat )0,
               1.0 );
    glLineWidth( 1.0f );

    glBegin( GL_LINES );

    glVertex3d( _boundingBox.getUpperX(),
                _boundingBox.getLowerY(),
                _boundingBox.getLowerZ() );
    glVertex3d( _boundingBox.getUpperX(),
                _boundingBox.getUpperY(),
                _boundingBox.getLowerZ() );

    glVertex3d( _boundingBox.getUpperX(),
                _boundingBox.getLowerY(),
                _boundingBox.getUpperZ() );
    glVertex3d( _boundingBox.getUpperX(),
                _boundingBox.getUpperY(),
                _boundingBox.getUpperZ() );

    glVertex3d( _boundingBox.getLowerX(),
                _boundingBox.getLowerY(),
                _boundingBox.getLowerZ() );
    glVertex3d( _boundingBox.getLowerX(),
                _boundingBox.getUpperY(),
                _boundingBox.getLowerZ() );

    glVertex3d( _boundingBox.getLowerX(),
                _boundingBox.getLowerY(),
                _boundingBox.getUpperZ() );
    glVertex3d( _boundingBox.getLowerX(),
                _boundingBox.getUpperY(),
                _boundingBox.getUpperZ() );

    glVertex3d( _boundingBox.getUpperX(),
                _boundingBox.getLowerY(),
                _boundingBox.getLowerZ() );
    glVertex3d( _boundingBox.getLowerX(),
                _boundingBox.getLowerY(),
                _boundingBox.getLowerZ() );

    glVertex3d( _boundingBox.getUpperX(),
                _boundingBox.getUpperY(),
                _boundingBox.getLowerZ() );
    glVertex3d( _boundingBox.getLowerX(),
                _boundingBox.getUpperY(),
                _boundingBox.getLowerZ() );

    glVertex3d( _boundingBox.getUpperX(),
                _boundingBox.getLowerY(),
                _boundingBox.getUpperZ() );
    glVertex3d( _boundingBox.getLowerX(),
                _boundingBox.getLowerY(),
                _boundingBox.getUpperZ() );

    glVertex3d( _boundingBox.getUpperX(),
                _boundingBox.getUpperY(),
                _boundingBox.getUpperZ() );
    glVertex3d( _boundingBox.getLowerX(),
                _boundingBox.getUpperY(),
                _boundingBox.getUpperZ() );

    glVertex3d( _boundingBox.getUpperX(),
                _boundingBox.getLowerY(),
                _boundingBox.getUpperZ() );
    glVertex3d( _boundingBox.getUpperX(),
                _boundingBox.getLowerY(),
                _boundingBox.getLowerZ() );

    glVertex3d( _boundingBox.getUpperX(),
                _boundingBox.getUpperY(),
                _boundingBox.getUpperZ() );
    glVertex3d( _boundingBox.getUpperX(),
                _boundingBox.getUpperY(),
                _boundingBox.getLowerZ() );

    glVertex3d( _boundingBox.getLowerX(),
                _boundingBox.getLowerY(),
                _boundingBox.getUpperZ() );
    glVertex3d( _boundingBox.getLowerX(),
                _boundingBox.getLowerY(),
                _boundingBox.getLowerZ() );

    glVertex3d( _boundingBox.getLowerX(),
                _boundingBox.getUpperY(),
                _boundingBox.getUpperZ() );
    glVertex3d( _boundingBox.getLowerX(),
                _boundingBox.getUpperY(),
                _boundingBox.getLowerZ() );

    glEnd();


    glEndList();


    ////////////////////////////////////////////////////////////////////////////
    // creating a list for a single unit sphere
    ////////////////////////////////////////////////////////////////////////////

    glNewList( _sphereList->item(), GL_COMPILE );

    glDrawElements( GL_TRIANGLES, _unitSpherePolygonIndices.size(),
                    GL_UNSIGNED_INT, _unitSpherePolygonIndices.data() );

    glEndList();


    ////////////////////////////////////////////////////////////////////////////
    // disabling GL vertex array
    ////////////////////////////////////////////////////////////////////////////

    glDisableClientState( GL_VERTEX_ARRAY );

    return true;


  }
  GKG_CATCH( "bool gkg::APointCloudFieldObject::glMakeBodyGLL( "
             "const anatomist::ViewState& viewState, "
             "const anatomist::GLList& glList ) const" );

}


void gkg::APointCloudFieldObject::computePointCloudTranslations()
{

  try
  {

    // pointing to site(s)
    int32_t rank = _pointCloudCartesianField->getSiteMap().getRank( 0 );
    const std::list< gkg::Vector3d< int32_t > >&
      sites = _pointCloudCartesianField->getSiteMap().getSites( rank );

    // computing corresponding translation(s)
    std::list< QVector4D > translations;
    std::list< gkg::Vector3d< int32_t > >::const_iterator
      s = sites.begin(),
      se = sites.end();
    while ( s != se )
    {

      translations.push_back( QVector4D( ( GLfloat )s->x * _resolution.x,
                                         ( GLfloat )s->y * _resolution.y,
                                         ( GLfloat )s->z * _resolution.z,
                                         ( GLfloat )0.0 ) );
      ++ s;

    }

    // creating associated texture map
    gkg::TextureMap< QVector4D > textureMapOfTranslations;
    textureMapOfTranslations.addTextures( translations );

    // creating ODF translation Cartesian field 
    _pointCloudTranslations.reset( new gkg::CartesianField< QVector4D >(
                                        _size.x, _size.y, _size.z,
                                        _resolution,
                                        _pointCloudCartesianField->getSiteMap(),
                                        textureMapOfTranslations,
                                        0, 0, 0 ) );

  }
  GKG_CATCH( "void gkg::APointCloudFieldObject::computeOdfTranslations()" );

}



std::string gkg::APointCloudFieldObject::getShaderSourceCode() const
{

  try
  {

    std::string vertexCountPerSphereString = gkg::StringConverter::toString(
                                                        _vertexCountPerSphere );
    std::string halfMinimumResolutionString = gkg::StringConverter::toString(
                                                _halfMinimumResolution, 10, 3 );

    std::string sourceCode;

    sourceCode +=
    "attribute float red;\n";
    sourceCode +=
    "attribute float green;\n";
    sourceCode +=
    "attribute float blue;\n";
    sourceCode +=
    "attribute float opacity;\n";
    sourceCode +=
    "attribute float pointRadius;\n";
    sourceCode +=
    "attribute float meshScale;\n";
    sourceCode +=
    "uniform int isShaderedObject;\n";
    sourceCode +=
    "uniform vec4 translation;\n";
    sourceCode +=
    "uniform vec4 point;\n";
     sourceCode +=
    "uniform vec3 normals[ " + vertexCountPerSphereString + " ];\n";
    sourceCode +=
    "attribute float unitSphereVertexIndex;\n\n";
    sourceCode +=
    "void main( void )\n";
    sourceCode +=
    "{\n";
    sourceCode +=
    "  if ( isShaderedObject == 1 )\n";
    sourceCode +=
    "  {\n";
    sourceCode +=
    "    float halfMinimumResolution = float(" + 
                                          halfMinimumResolutionString + " );\n";
    sourceCode +=
    "    vec4 v = vec4( red, green, blue, opacity );\n";

    sourceCode +=
    "    vec4 scaledPoint = point * halfMinimumResolution * meshScale;\n";

    sourceCode +=
    "    vec3 normal = normals[ int( unitSphereVertexIndex ) ];\n"
    "    vec3 normalizedNormal = gl_NormalMatrix * normal;\n"
    "    vec3 lightDirection = vec3( gl_LightSource[ 0 ].position );\n"
    "    float normalizedNormalDotLightDirection = dot( normalizedNormal, \n"
    "                                                   lightDirection );\n";
    sourceCode +=
    "    gl_FrontColor = normalizedNormalDotLightDirection * v;\n";
    sourceCode +=
    "    gl_Position.x = gl_Vertex.x * halfMinimumResolution * pointRadius * meshScale;\n";
    sourceCode +=
    "    gl_Position.y = gl_Vertex.y * halfMinimumResolution * pointRadius * meshScale;\n";
    sourceCode +=
    "    gl_Position.z = gl_Vertex.z * halfMinimumResolution * pointRadius * meshScale;\n";
    sourceCode +=
    "    gl_Position.w = gl_Vertex.w;\n";
    sourceCode +=
    "    gl_Position = gl_ModelViewProjectionMatrix * \n"
    "                  ( gl_Position + translation + scaledPoint );\n";
    sourceCode +=
    "  }\n";
    sourceCode +=
    "  else\n";
    sourceCode +=
    "  {\n";
    sourceCode +=
    "    gl_FrontColor = gl_Color;\n";
    sourceCode +=
    "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n";
    sourceCode +=
    "  }\n";
    sourceCode +=
    "}\n";


    return sourceCode;

  }
  GKG_CATCH( "void gkg::APointCloudFieldObject::getShaderSourceCode( "
             "int32_t vertexCountPerOdf ) const" );

}


void gkg::APointCloudFieldObject::updateShader()
{

  try
  {

    std::cout << std::endl << std::endl
              << "-----------------------" << std::endl;
    std::cout << this->getShaderSourceCode().c_str()
              << "-----------------------" << std::endl;
    _shader.reset( new QGLShader( QGLShader::Vertex ) );
    _shader->compileSourceCode( this->getShaderSourceCode().c_str() );


    _shaderProgram.reset( new QGLShaderProgram() );
    _shaderProgram->addShader( _shader.get() );
    _shaderProgram->link();

    _shaderProgram->bind();

    _attributeLocationRed =
      _shaderProgram->attributeLocation( "red" );
    _attributeLocationGreen =
      _shaderProgram->attributeLocation( "green" );
    _attributeLocationBlue =
      _shaderProgram->attributeLocation( "blue" );
    _attributeLocationOpacity =
      _shaderProgram->attributeLocation( "opacity" );
    _attributeLocationPointRadius =
      _shaderProgram->attributeLocation( "pointRadius" );
    _attributeLocationMeshScale =
      _shaderProgram->attributeLocation( "meshScale" );
    _uniformIsShaderedObject =
      _shaderProgram->uniformLocation( "isShaderedObject" );
    _uniformLocationTranslation = 
      _shaderProgram->uniformLocation( "translation" );
    _uniformLocationPoint = 
      _shaderProgram->uniformLocation( "point" );
    _uniformLocationNormals = 
      _shaderProgram->uniformLocation( "normals" );
    _attributeLocationUnitSphereVertexIndex =
       _shaderProgram->attributeLocation( "unitSphereVertexIndex" );

    _shaderProgram->release();

    if ( _sphereList )
    {

      delete _sphereList;

    }
    _sphereList = new anatomist::GLList();

  }
  GKG_CATCH( "void gkg::APointCloudFieldObject::updateShader()" );

}


////////////////////////////////////////////////////////////////////////////////
// static methods
////////////////////////////////////////////////////////////////////////////////

std::list< anatomist::AObject* > 
gkg::APointCloudFieldObject::load( const std::string& fileName,
                            anatomist::ObjectReader::PostRegisterList &,
                            carto::Object /* options */ )
{

  try
  {

    // building the names of the files to be uploaded
    std::string fileNameRoot = fileName.substr( 0, fileName.length() - 27U );
    std::string fileNamePointCloudTextureMap = fileNameRoot + ".texturemap";
    std::string fileNamePointCloudSiteMap = fileNameRoot + ".sitemap";


    gkg::SiteMap< int32_t, int32_t > pointCloudSiteMap;
    gkg::Reader::getInstance().read( fileNamePointCloudSiteMap,
                                     pointCloudSiteMap );


    gkg::TextureMap< std::vector< gkg::Vector3d< float > > >
      pointCloudTextureMap;
    gkg::Reader::getInstance().read( fileNamePointCloudTextureMap,
                                     pointCloudTextureMap );

    // checking that the texture map is not empty
    gkg::TextureMap< std::vector< gkg::Vector3d< float > > >::iterator
      pc = pointCloudTextureMap.begin(),
      pce = pointCloudTextureMap.end();

    if ( pc == pce )
    {

      throw std::runtime_error( "Point Cloud Cartesian field is empty!" );

    }

    // creating the ODF Cartesian field
    gkg::RCPointer<
      gkg::CartesianField< std::vector< gkg::Vector3d< float > > > >
      pointCloudCartesianField( 
        new gkg::CartesianField< std::vector< Vector3d< float > > >(
                                                       pointCloudSiteMap,
                                                       pointCloudTextureMap ) );

    std::cout << "Point Cloud count : "
              << pointCloudCartesianField->getSiteMap().getSiteCount( 0 )
              << std::endl;

    // creating APointCloudFieldObject object
    gkg::APointCloudFieldObject* 
      aPointCloudFieldObject = new gkg::APointCloudFieldObject(
                                                     pointCloudCartesianField );
    std::list< anatomist::AObject* > objectList;
    objectList.push_back( aPointCloudFieldObject );

    return objectList;

  }
  GKG_CATCH( "std::list< anatomist::AObject* > "
             "gkg::APointCloudFieldObject::load( const std::string& fileName, "
             "anatomist::ObjectReader::PostRegisterList &, "
             "carto::Object /* options */ )" );

}


int32_t gkg::APointCloudFieldObject::registerClass()
{

  try
  {

    // associating a new loader to the .texturemap extension
    anatomist::ObjectReader::registerLoader(
      "texturemap_of_point_clouds",
      gkg::APointCloudFieldObject::load );

    // creating a new class type
    _classType = registerObjectType( "POINT_CLOUD_CARTESIAN_FIELD" );

    // loading the GKG plugin(s)
    gkg::PluginLoader::getInstance().load( false );

#ifdef GKG_DEBUG

    std::cout << "POINT_CLOUD_CARTESIAN_FIELD loader registered : "
              << _classType << std::endl;

#endif

    return _classType;

  }
  GKG_CATCH( "int32_t gkg::APointCloudFieldObject::registerClass()" );

}


int32_t gkg::APointCloudFieldObject::getClassType()
{

  try
  {

    return _classType;

  }
  GKG_CATCH( "int32_t gkg::APointCloudFieldObject::getClassType()" );

}



