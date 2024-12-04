#include <gkg-anatomist-plugin-objects/AOdfFieldObject.h>
#include <gkg-anatomist-plugin-rendering-options/AOdfFieldRenderingOptionsWidget.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-processing-mesh/MeshFactory.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-mesh/MeshScaler_i.h>
#include <gkg-processing-container/CartesianField_i.h>
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-processing-coordinates/AntipodallySymmetricUniformOrientationSet.h>
#include <gkg-processing-mesh/ConvexHull_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/MaximumFilter_i.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-processing-algobase/BetweenOrEqualToFunction_i.h>
#include <gkg-processing-algobase/Thresholder_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>


#include <anatomist/surface/glcomponent.h>

#include <anatomist/object/actions.h>
#include <anatomist/control/qObjTree.h>
#include <anatomist/window/Window.h>
#include <aims/resampling/quaternion.h>
#include <map>
#include <fstream>
#include <GL/gl.h>



////////////////////////////////////////////////////////////////////////////////
// class OdfNormalComputationContext
////////////////////////////////////////////////////////////////////////////////

gkg::OdfNormalComputationContext::OdfNormalComputationContext(
          const std::vector< gkg::Vector3d< int32_t > >& sites,
          const gkg::CartesianField< std::vector< float > >& odfTransformations,
          const gkg::MeshMap< int32_t, float, 3U >& unitSphere,
          std::vector< std::vector< QVector3D > >& normalSets )
                                 : gkg::LoopContext< int64_t >(),
                                   _sites( sites ),
                                   _odfTransformations( odfTransformations ),
                                   _unitSphere( unitSphere ),
                                   _normalSets( normalSets )
{
}


gkg::OdfNormalComputationContext::~OdfNormalComputationContext()
{
}


void gkg::OdfNormalComputationContext::doIt( int64_t startIndex,
                                             int64_t countIndex )
{

  try
  {

    std::vector< std::vector< QVector3D > > localNormalSets( countIndex );
    gkg::MeshScaler< int32_t, float, 3U > meshScaler;

    int64_t index = 0;
    for ( index = 0; index < countIndex; index++ )
    {

      const std::vector< float >& 
        odfTransformations = *_odfTransformations.getItem(
                                                 _sites[ startIndex + index ] );

      gkg::MeshMap< int32_t, float, 3U > odfMeshMap( _unitSphere );
      meshScaler.scale( odfMeshMap, 0, odfTransformations, odfMeshMap );

      std::list< gkg::Vector3d< float > >
        normals = odfMeshMap.getNormals( 0 );

      std::list< QVector3D > normalsAsQVector3D;

      std::list< gkg::Vector3d< float > >::const_iterator
        n = normals.begin(),
        ne = normals.end();

      while ( n != ne )
      {

        normalsAsQVector3D.push_back( QVector3D( ( GLfloat )n->x,
                                                 ( GLfloat )n->y,
                                                 ( GLfloat )n->z ) );
                                                 
        ++ n;

      }

      localNormalSets[ index ] = std::vector< QVector3D >(
                                                     normalsAsQVector3D.begin(),
                                                     normalsAsQVector3D.end() );
    }


    lock();

    for ( index = 0; index < countIndex; index++ )
    {

      _normalSets[ startIndex + index ] = localNormalSets[ index ];


    }
    unlock();

  }
  GKG_CATCH( "void gkg::OdfNormalComputationContext::doIt( "
             "int32_t startIndex, "
             "int32_t countIndex )" );

}


////////////////////////////////////////////////////////////////////////////////
// class AOdfFIeldObject
////////////////////////////////////////////////////////////////////////////////

int32_t gkg::AOdfFieldObject::_classType =
                            gkg::AOdfFieldObject::registerClass();
Tree* gkg::AOdfFieldObject::_optionTree = 0;



gkg::AOdfFieldObject::AOdfFieldObject(
             const gkg::RCPointer< gkg::OdfCartesianField >& odfCartesianField )
                     : QObject(),
                       anatomist::SliceableObject(),
                       _odfCartesianField( odfCartesianField ),
                       _vertexCountPerOdf( 0 ),
                       _meshScale( 1.0f ),
                       _isAlreadyDisplayed( false ),
                       _sphereList( 0 )
{

  try
  {

    _type = gkg::AOdfFieldObject::_classType;


    const gkg::SiteMap< int32_t, int32_t >& 
      siteMap = _odfCartesianField->getSiteMap();


    //const gkg::TextureMap< gkg::OrientationDistributionFunction >& 
    //  textureMap = _odfCartesianField->getItems();

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
    this->computeOdfTranslations();

    // initializing iterator(s)
    //int32_t rank = _odfCartesianField->getSiteMap().getRank( 0 );
    //const std::list< gkg::Vector3d< int32_t > >&
    //  sites = siteMap.getSites( rank );
    
    //int32_t siteCount = ( int32_t )sites.size();

    this->setVertexCountPerOdf( 60 );
    this->setMeshScale( 1.0 );

    this->GetMaterial().SetDiffuse( 0.80, 0.80, 0.80, 1.0 );


    // adding icon
    if ( QObjectTree::TypeNames.find( _classType ) == 
         QObjectTree::TypeNames.end() )
    {

      std::string
          iconPath = gkg::ConfigurationInfo::getInstance().getIconPath(
                                            "icon-odf.png" );
      if ( !QObjectTree::TypeIcons[ _classType ].load( iconPath.c_str() ) )
      {

        QObjectTree::TypeIcons.erase( _classType );
        std::cerr << "icon " << iconPath << " not found!" << std::endl;

      }
      QObjectTree::TypeNames[ _classType ] = "ODF Cartesian Field";

    }

  }
  GKG_CATCH( "gkg::AOdfFieldObject::AOdfFieldObject( "
             "const gkg::RCPointer< gkg::OdfCartesianField >& "
             "odfCartesianField )" );

}


gkg::AOdfFieldObject::~AOdfFieldObject()
{

  if ( _sphereList )
  {

    delete _sphereList;

  }

}


bool gkg::AOdfFieldObject::Is2DObject()
{

  try
  {

    return true;

  }
  GKG_CATCH( "bool gkg::AOdfFieldObject::Is2DObject()" );

}


bool gkg::AOdfFieldObject::Is3DObject()
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool gkg::AOdfFieldObject::Is3DObject()" );

}


bool gkg::AOdfFieldObject::IsFusion2DAllowed()
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool gkg::AOdfFieldObject::Is3DObject()" );

}


Tree* gkg::AOdfFieldObject::optionTree() const
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
                       &gkg::AOdfFieldRenderingOptionsWidget::create );
      t->insert( t2 );

    }
    return( _optionTree );

  }
  GKG_CATCH( "Tree* gkg::AOdfFieldObject::optionTree() const" );

}


float gkg::AOdfFieldObject::MinX2D() const
{

  try
  {

    return _boundingBox.getLowerX() - ( float )_resolution.x / 2.0f;

  }
  GKG_CATCH( "float gkg::AOdfFieldObject::MinX2D() const" );

}


float gkg::AOdfFieldObject::MinY2D() const
{

  try
  {

    return _boundingBox.getLowerY() - ( float )_resolution.y / 2.0f;

  }
  GKG_CATCH( "float gkg::AOdfFieldObject::MinY2D() const" );

}


float gkg::AOdfFieldObject::MinZ2D() const
{

  try
  {

    return _boundingBox.getLowerZ() - ( float )_resolution.z / 2.0f;

  }
  GKG_CATCH( "float gkg::AOdfFieldObject::MinZ2D() const" );

}


float gkg::AOdfFieldObject::MaxX2D() const
{

  try
  {

    return _boundingBox.getUpperX() + ( float )_resolution.x / 2.0f;

  }
  GKG_CATCH( "float gkg::AOdfFieldObject::MaxX2D() const" );

}


float gkg::AOdfFieldObject::MaxY2D() const
{

  try
  {

    return _boundingBox.getUpperY() + ( float )_resolution.y / 2.0f;

  }
  GKG_CATCH( "float gkg::AOdfFieldObject::MaxY2D() const" );

}


float gkg::AOdfFieldObject::MaxZ2D() const
{

  try
  {

    return _boundingBox.getUpperZ() + ( float )_resolution.z / 2.0f;

  }
  GKG_CATCH( "float gkg::AOdfFieldObject::MaxZ2D() const" );

}


bool gkg::AOdfFieldObject::boundingBox(
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
  GKG_CATCH( "bool gkg::AOdfFieldObject::boundingBox( "
             "std::vector< float > & bmin, "
             "std::vector< float > & bmax ) const" );

}


int32_t gkg::AOdfFieldObject::getVertexCountPerOdf() const
{

  try
  {

    return _vertexCountPerOdf;

  }
  GKG_CATCH( "int32_t gkg::AOdfFieldObject::"
             "getVertexCountPerOdf() const" );

}


void gkg::AOdfFieldObject::setVertexCountPerOdf( int32_t vertexCount )
{

  try
  {


    bool changed = ( vertexCount != _vertexCountPerOdf );

    _vertexCountPerOdf = vertexCount;

    if ( changed )
    {

      // creating output orientation set
      _outputOrientationSet.reset(
         new gkg::AntipodallySymmetricUniformOrientationSet( 
                                                         _vertexCountPerOdf ) );

      // updating ODF cartesian field accordingly
      _odfCartesianField->setOutputOrientationSet(
                                                  _outputOrientationSet.get() );


      // building unit sphere for GL rendering
      _unitSphere.reset( new gkg::MeshMap< int32_t, float, 3U > );

      gkg::ConvexHull::getInstance().addConvexHull( 
                                       _outputOrientationSet->getOrientations(),
                                       0,
                                       *_unitSphere );

      // filling unit sphere vertex indices
      _unitSphereVertexIndices.resize( _vertexCountPerOdf );
      GLint i = 0;
      for( i = 0; i < ( GLint )_vertexCountPerOdf; i++ )
      {

        _unitSphereVertexIndices[ i ] = ( GLfloat )i;

      }

      // filling vertex coordinates
      _unitSphereVertexCoordinates.resize( 3U * _vertexCountPerOdf );

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

      // computing the ODF probabilities for the corresponding vertices
      this->computeOdfProbabilities();


      std::cout << "ODF probabilities computed" << std::endl;

      // computing the ODF transformations for the corresponding vertices
      this->computeOdfTransformations();

      std::cout << "ODF transformations computed" << std::endl;

      // computing the ODF transformations for the corresponding vertices
      this->computeOdfNormals();

      std::cout << "ODF normals computed" << std::endl;

      // updating the shader code
      if ( _isAlreadyDisplayed )
      {

        this->updateShader();
        std::cout << "shader updated" << std::endl;

      }

    }

  }
  GKG_CATCH( "void gkg::AOdfFieldObject::"
             "setVertexCountPerOdf( "
             "int32_t vertexCount )" );

}


float gkg::AOdfFieldObject::getMeshScale() const
{

  try
  {

    return _meshScale;

  }
  GKG_CATCH( "float gkg::AOdfFieldObject::"
             "getMeshScale() const" );

}


void gkg::AOdfFieldObject::setMeshScale( float meshScale )
{

  try
  {


    //bool changed = ( meshScale != _meshScale );

    _meshScale = meshScale;

    // we don't have to update the shader whe modifying the mesh scale
    // because it is an attribute of the shader that can be set as an
    // input of the shader

  }
  GKG_CATCH( "void gkg::AOdfFieldObject::"
             "setMeshScale( "
             "float meshScale )" );

}


bool gkg::AOdfFieldObject::render( anatomist::PrimList& primitiveList,
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
  GKG_CATCH( "bool gkg::AOdfFieldObject::render( "
             "anatomist::PrimList& primitiveList, "
             "const anatomist::ViewState& viewState )" );

}


bool gkg::AOdfFieldObject::glMakeBodyGLL(
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
    //float red = ( float )material.Diffuse( 0 );
    //float green = ( float )material.Diffuse( 1 );
    //float blue = ( float )material.Diffuse( 2 );
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

      _shaderProgram->setAttributeValue( _attributeLocationOpacity,
                                         opacity );
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

            if ( _odfCartesianField->getItem( x, y, z ) )
            {
                        
              _shaderProgram->setUniformValue(
                                        _uniformLocationTranslation,
                                        *_odfTranslations->getItem( x, y, z ) );

              _shaderProgram->setUniformValueArray( 
                                _uniformLocationTransformations,
                  &( ( _odfTransformations->getItem( x, y, z )->data() )[ 0 ] ),
                                _vertexCountPerOdf,
                                1 );

              _shaderProgram->setUniformValueArray(
                                        _uniformLocationNormals,
                                        _odfNormals->getItem( x, y, z )->data(),
                                        _vertexCountPerOdf );

              // calling the display list which renders the sphere
              _sphereList->callList();

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

            if ( _odfCartesianField->getItem( x, y, z ) )
            {

              _shaderProgram->setUniformValue(
                                        _uniformLocationTranslation,
                                        *_odfTranslations->getItem( x, y, z ) );

              _shaderProgram->setUniformValueArray( 
                                _uniformLocationTransformations,
                                _odfTransformations->getItem( x, y, z )->data(),
                                _vertexCountPerOdf,
                                1 );

              _shaderProgram->setUniformValueArray(
                                        _uniformLocationNormals,
                                        _odfNormals->getItem( x, y, z )->data(),
                                        _vertexCountPerOdf );

              // calling the display list which renders the sphere
              _sphereList->callList();

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

            if ( _odfCartesianField->getItem( x, y, z ) )
            {

              _shaderProgram->setUniformValue(
                                        _uniformLocationTranslation,
                                        *_odfTranslations->getItem( x, y, z ) );

              _shaderProgram->setUniformValueArray( 
                                _uniformLocationTransformations,
                                _odfTransformations->getItem( x, y, z )->data(),
                                _vertexCountPerOdf,
                                1 );

              _shaderProgram->setUniformValueArray(
                                        _uniformLocationNormals,
                                        _odfNormals->getItem( x, y, z )->data(),
                                        _vertexCountPerOdf );

              // calling the display list which renders the sphere
              _sphereList->callList();

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
  GKG_CATCH( "bool gkg::AOdfFieldObject::glMakeBodyGLL( "
             "const anatomist::ViewState& viewState, "
             "const anatomist::GLList& glList ) const" );

}


void gkg::AOdfFieldObject::computeOdfTranslations()
{

  try
  {

    // pointing to site(s)
    int32_t rank = _odfCartesianField->getSiteMap().getRank( 0 );
    const std::list< gkg::Vector3d< int32_t > >&
      sites = _odfCartesianField->getSiteMap().getSites( rank );

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
    _odfTranslations.reset( new gkg::CartesianField< QVector4D >(
                              _size.x, _size.y, _size.z,
                              _resolution,
                              _odfCartesianField->getSiteMap(),
                              textureMapOfTranslations,
                              0, 0, 0 ) );

  }
  GKG_CATCH( "void gkg::AOdfFieldObject::computeOdfTranslations()" );

}



void gkg::AOdfFieldObject::computeOdfProbabilities()
{

  try
  {

    // pointing to site(s)
    int32_t rank = _odfCartesianField->getSiteMap().getRank( 0 );
    const std::list< gkg::Vector3d< int32_t > >&
      sites = _odfCartesianField->getSiteMap().getSites( rank );

    // computing corresponding probability set(s)
    std::list< std::vector< float > > probabilitySets;
    std::list< gkg::Vector3d< int32_t > >::const_iterator
      s = sites.begin(),
      se = sites.end();
    while ( s != se )
    {

      probabilitySets.push_back(
        _odfCartesianField->getItem( *s )->getOrientationProbabilities() );
      ++ s;

    }

    // creating associated texture map
    gkg::TextureMap< std::vector< float > > textureMapOfProbabilities;
    textureMapOfProbabilities.addTextures( probabilitySets );

    // creating ODF probabilities Cartesian field 
    _odfProbabilities.reset( new gkg::CartesianField< std::vector< float > >(
                                               _size.x, _size.y, _size.z,
                                               _resolution,
                                               _odfCartesianField->getSiteMap(),
                                               textureMapOfProbabilities,
                                               0, 0, 0 ) );

  }
  GKG_CATCH( "void gkg::AOdfFieldObject::computeOdfProbabilities()" );

}


void gkg::AOdfFieldObject::computeOdfTransformations()
{

  try
  {

    // pointing to site(s)
    int32_t rank = _odfCartesianField->getSiteMap().getRank( 0 );
    const std::list< gkg::Vector3d< int32_t > >&
      sites = _odfCartesianField->getSiteMap().getSites( rank );

    // allocating min max filter(s)
    gkg::MinimumFilter< std::vector< float >, float > minimumFilter;
    gkg::MaximumFilter< std::vector< float >, float > maximumFilter;

    // computing corresponding transformations
    std::vector< float > transformations( _vertexCountPerOdf );
    float minimumProbability = 0.0;
    float maximumProbability = 0.0;
    std::list< std::vector< float > > transformationSets;
    std::list< gkg::Vector3d< int32_t > >::const_iterator
      s = sites.begin(),
      se = sites.end();
    while ( s != se )
    {

      const std::vector< float >& 
        orientationProbabilities = *_odfProbabilities->getItem( *s );

      minimumFilter.filter( orientationProbabilities, minimumProbability );
      maximumFilter.filter( orientationProbabilities, maximumProbability );

      gkg::Rescaler< std::vector< float >, std::vector< float > >
        rescaler( minimumProbability, maximumProbability,
                  0.0, 
                  1.0 );
      rescaler.rescale( orientationProbabilities, transformations );

      transformationSets.push_back( transformations );

      ++ s;

    }

    // creating associated texture map
    gkg::TextureMap< std::vector< float > > textureMapOfTransformations;
    textureMapOfTransformations.addTextures( transformationSets );

    // creating ODF transformation Cartesian field 
    _odfTransformations.reset( new gkg::CartesianField< std::vector< float > >(
                               _size.x, _size.y, _size.z,
                               _resolution,
                               _odfCartesianField->getSiteMap(),
                               textureMapOfTransformations,
                               0, 0, 0 ) );

  }
  GKG_CATCH( "void gkg::AOdfFieldObject::computeOdfTransformations()" );

}


void gkg::AOdfFieldObject::computeOdfNormals()
{

  try
  {

    // pointing to site(s)
    int32_t rank = _odfCartesianField->getSiteMap().getRank( 0 );
    const std::list< gkg::Vector3d< int32_t > >&
      sites = _odfCartesianField->getSiteMap().getSites( rank );

    
    std::vector< gkg::Vector3d< int32_t > >
      vectorOfSites( sites.begin(), sites.end() );

    // computing corresponding transformations
    std::vector< std::vector< QVector3D > > normalSets( vectorOfSites.size() );

    gkg::OdfNormalComputationContext  odfNormalComputationContext(
                                                           vectorOfSites,
                                                           *_odfTransformations,
                                                           *_unitSphere,
                                                           normalSets );
    gkg::ThreadedLoop< int64_t >
      threadedLoop( &odfNormalComputationContext,
                    0,                                   // start index
                    ( int64_t )vectorOfSites.size(),     // count
                    0 );                                 // parallel
    threadedLoop.launch();


    // creating associated texture map
    gkg::TextureMap< std::vector< QVector3D > >
      textureMapOfNormals;
    textureMapOfNormals.addTextures( normalSets );

    // creating ODF normal Cartesian field 
    _odfNormals.reset( new gkg::CartesianField< std::vector< QVector3D > >(
                               _size.x, _size.y, _size.z,
                               _resolution,
                               _odfCartesianField->getSiteMap(),
                               textureMapOfNormals,
                               0, 0, 0 ) );

  }
  GKG_CATCH( "void gkg::AOdfFieldObject::computeOdfNormals()" );

}


std::string gkg::AOdfFieldObject::getShaderSourceCode() const
{

  try
  {

    std::string vertexCountPerOdfString = gkg::StringConverter::toString(
                                                           _vertexCountPerOdf );
    std::string halfMinimumResolutionString = gkg::StringConverter::toString(
                                                _halfMinimumResolution, 10, 3 );

    std::string sourceCode;

    sourceCode +=
    "attribute float opacity;\n";
    sourceCode +=
    "attribute float meshScale;\n";
    sourceCode +=
    "uniform int isShaderedObject;\n";
    sourceCode +=
    "uniform vec4 translation;\n";
     sourceCode +=
    "uniform vec3 normals[ " + vertexCountPerOdfString + " ];\n";
    sourceCode +=
    "uniform float transformations[ " + vertexCountPerOdfString + " ];\n";
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
    "    vec4 v = vec4( gl_Vertex.x, gl_Vertex.y, gl_Vertex.z, opacity );\n";

    sourceCode +=
    "    v.x = abs( v.x );\n";
    sourceCode +=
    "    v.y = abs( v.y );\n";
    sourceCode +=
    "    v.z = abs( v.z );\n";


    sourceCode +=
    "    vec3 normal = normals[ int( unitSphereVertexIndex ) ];\n"
    "    vec3 normalizedNormal = gl_NormalMatrix * normal;\n"
    "    vec3 lightDirection = vec3( gl_LightSource[ 0 ].position );\n"
    "    float normalizedNormalDotLightDirection = dot( normalizedNormal, \n"
    "                                                   lightDirection );\n";
    sourceCode +=
    "    gl_FrontColor = normalizedNormalDotLightDirection * v;\n";
    sourceCode +=
    "    gl_Position.x = transformations[ int( unitSphereVertexIndex ) ] * \n"
    "                    gl_Vertex.x * halfMinimumResolution * meshScale;\n";
    sourceCode +=
    "    gl_Position.y = transformations[ int( unitSphereVertexIndex ) ] * \n"
    "                    gl_Vertex.y * halfMinimumResolution * meshScale;\n";
    sourceCode +=
    "    gl_Position.z = transformations[ int( unitSphereVertexIndex ) ] * \n"
    "                    gl_Vertex.z * halfMinimumResolution * meshScale;\n";
    sourceCode +=
    "    gl_Position.w = gl_Vertex.w;\n";
    sourceCode +=
    "    gl_Position = gl_ModelViewProjectionMatrix * \n"
    "                  ( gl_Position + translation );\n";
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
  GKG_CATCH( "void gkg::AOdfFieldObject::getShaderSourceCode( "
             "int32_t vertexCountPerOdf ) const" );

}


void gkg::AOdfFieldObject::updateShader()
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

    _attributeLocationOpacity =
      _shaderProgram->attributeLocation( "opacity" );
    _attributeLocationMeshScale =
      _shaderProgram->attributeLocation( "meshScale" );
    _uniformIsShaderedObject =
      _shaderProgram->uniformLocation( "isShaderedObject" );
    _uniformLocationTranslation = 
      _shaderProgram->uniformLocation( "translation" );
    _uniformLocationNormals = 
      _shaderProgram->uniformLocation( "normals" );
    _uniformLocationTransformations =
       _shaderProgram->uniformLocation( "transformations" );
    _attributeLocationUnitSphereVertexIndex =
       _shaderProgram->attributeLocation( "unitSphereVertexIndex" );

    _shaderProgram->release();

    if ( _sphereList )
    {

      delete _sphereList;

    }
    _sphereList = new anatomist::GLList();

  }
  GKG_CATCH( "void gkg::AOdfFieldObject::updateShader()" );

}


////////////////////////////////////////////////////////////////////////////////
// static methods
////////////////////////////////////////////////////////////////////////////////

std::list< anatomist::AObject* > 
gkg::AOdfFieldObject::load( const std::string& fileName,
                            anatomist::ObjectReader::PostRegisterList &,
                            carto::Object /* options */ )
{

  try
  {

    // building the names of the files to be uploaded
    std::string fileNameRoot = fileName.substr( 0, fileName.length() - 11U );
    std::string fileNameOdfTextureMap = fileNameRoot + ".texturemap";
    std::string fileNameOdfSiteMap = fileNameRoot + ".sitemap";


    gkg::SiteMap< int32_t, int32_t > odfSiteMap;
    gkg::Reader::getInstance().read( fileNameOdfSiteMap, odfSiteMap );


    gkg::TextureMap< gkg::OrientationDistributionFunction > odfTextureMap;
    gkg::Reader::getInstance().read( fileNameOdfTextureMap, odfTextureMap );

    // checking that the texture map is not empty
    gkg::TextureMap< gkg::OrientationDistributionFunction >::iterator
      o = odfTextureMap.begin(),
      oe = odfTextureMap.end();

    if ( o == oe )
    {

      throw std::runtime_error( "ODF Cartesian field is empty!" );

    }

    // checking for the basis type
    //gkg::OrientationDistributionFunction::BasisType
    //  basisType = o->second.getBasisType();


    // the ODF Cartesian field requires to provide an orientation set, so we
    // create a default 60 orientation  antipoldally symmetric uniform 
    // orientationn set
    gkg::AntipodallySymmetricUniformOrientationSet outputOrientationSet( 60 );


    // creating the ODF Cartesian field
    gkg::RCPointer< gkg::OdfCartesianField >
      odfCartesianField( new gkg::OdfCartesianField( odfSiteMap,
                                                     odfTextureMap,
                                                     outputOrientationSet ) );

    std::cout << "ODF count : "
              << odfCartesianField->getSiteMap().getSiteCount( 0 )
              << std::endl;

    // creating AOdfFieldObject object
    gkg::AOdfFieldObject* 
      aOdfFieldObject = new gkg::AOdfFieldObject( odfCartesianField );
    std::list< anatomist::AObject* > objectList;
    objectList.push_back( aOdfFieldObject );

    return objectList;

  }
  GKG_CATCH( "std::list< anatomist::AObject* > "
             "gkg::AOdfFieldObject::load( const std::string& fileName, "
             "anatomist::ObjectReader::PostRegisterList &, "
             "carto::Object /* options */ )" );

}


int32_t gkg::AOdfFieldObject::registerClass()
{

  try
  {

    // associating a new loader to the .texturemap extension
    anatomist::ObjectReader::registerLoader(
      "texturemap",
      gkg::AOdfFieldObject::load );

    // creating a new class type
    _classType = registerObjectType( "ODF_CARTESIAN_FIELD" );

    // loading the GKG plugin(s)
    gkg::PluginLoader::getInstance().load( false );

#ifdef GKG_DEBUG

    std::cout << "ODF_CARTESIAN_FIELD loader registered : "
              << _classType << std::endl;

#endif

    return _classType;

  }
  GKG_CATCH( "int32_t gkg::AOdfFieldObject::registerClass()" );

}


int32_t gkg::AOdfFieldObject::getClassType()
{

  try
  {

    return _classType;

  }
  GKG_CATCH( "int32_t gkg::AOdfFieldObject::getClassType()" );

}



