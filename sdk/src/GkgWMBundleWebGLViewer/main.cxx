#include <gkg-communication-getopt/Application.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-dmri-io/ForceHardLinkWithGinkgoLibrary.h>
#include <gkg-processing-container/BoundingBox.h>
#include <string>
#include <cstdlib>

#include <Wt/WServer.h>
#include <Wt/WApplication.h>
#include <Wt/WEnvironment.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WImage.h>
#include <Wt/WGLWidget.h>


FORCE_HARD_LINK_WITH_CONNECTOMIST


////////////////////////////////////////////////////////////////////////////////
// shader codes
////////////////////////////////////////////////////////////////////////////////

std::string fragmentShaderSrc = std::string( 
"#ifdef GL_ES                                                                \n"
"                                                                            \n"
"  precision highp float;                                                    \n"
"                                                                            \n"
"#endif                                                                      \n"
"                                                                            \n"
"  varying vec3 vColor;                                                      \n"
"                                                                            \n"
"  void main( void )                                                         \n"
"  {                                                                         \n"
"                                                                            \n"
"    gl_FragColor = vec4( vColor.x, vColor.y, vColor.z, 1.0 );               \n"
"                                                                            \n"
"  }                                                                         \n"
 );


std::string vertexShaderSrc = std::string(
"  attribute vec3 aVertexPosition;                                           \n"
"  attribute vec3 aVertexColor;                                              \n"
"                                                                            \n"
"  uniform mat4 uMVMatrix; // [M]odel[V]iew matrix                           \n"
"  uniform mat4 uCMatrix;  // Client-side manipulated [C]amera matrix        \n"
"  uniform mat4 uPMatrix;  // Perspective [P]rojection matrix                \n"
"  uniform mat4 uNMatrix;  // [N]ormal transformation                        \n"
"  // uNMatrix is the transpose of the inverse of uCMatrix * uMVMatrix       \n"
"                                                                            \n"
"  varying vec3 vColor;                                                      \n"
"                                                                            \n"
"  void main( void )                                                         \n"
"  {                                                                         \n"
"                                                                            \n"
"    // computing the position of this vertex                                \n"
"    gl_Position = uPMatrix * uCMatrix * uMVMatrix *                         \n"
"                  vec4( aVertexPosition, 1.0 );                             \n"
"                                                                            \n"
"    // the color is corresponding to the direction                          \n"
"    vColor = aVertexColor;                                                  \n"
"  }                                                                         \n"
 );


////////////////////////////////////////////////////////////////////////////////
// class PaintWidget
////////////////////////////////////////////////////////////////////////////////


namespace gkg
{


class PaintWidget : public Wt::WGLWidget
{

  public:

    PaintWidget( const bool& useBinaryBuffers,
                 const gkg::BundleMap< std::string >& bundleMap );
  
    void initializeGL();
    void paintGL();
    void resizeGL( int width, int height );

    void setShaders( const std::string& vertexShader,
                     const std::string& fragmentShader );

  protected:

    gkg::Vector3d< float > getFOVCenter(
                                 gkg::BoundingBox< float >& boundingBox ) const;


    bool _useBinaryBuffers;
    bool _initialized;

    Wt::WGLWidget::JavaScriptMatrix4x4 _javaScriptMatrix4x4;

    std::string _vertexShader;
    std::string _fragmentShader;

    Wt::WGLWidget::Program _wtShaderProgram; 
    Wt::WGLWidget::AttribLocation _wtVertexPositionAttribute;
    Wt::WGLWidget::AttribLocation  _wtVertexColorAttribute;
    Wt::WGLWidget::UniformLocation _wtPositionMatrixUniformLocation;
    Wt::WGLWidget::UniformLocation _wtCMatrixUniformLocation;
    Wt::WGLWidget::UniformLocation _wtMVMatrixUniformLocation;
    Wt::WGLWidget::UniformLocation _wtNMatrixUniformLocation;

    Wt::WGLWidget::Buffer _wtObjectBuffer;

    std::vector< float > _data;

};


}


gkg::PaintWidget::PaintWidget( const bool& useBinaryBuffers,
                               const gkg::BundleMap< std::string >& bundleMap )
                 : Wt::WGLWidget(),
                   _useBinaryBuffers( useBinaryBuffers ),
                   _initialized( false )
{

  try
  {

    // converting the bundle map into an array
    int32_t totalSegmentCount = bundleMap.getTotalSegmentCount();
    _data.resize( totalSegmentCount * 2 * 2 * 3 * 4 );

    gkg::Vector3d< float > color;
    gkg::BundleMap< std::string >::const_iterator
      b = bundleMap.begin(),
      be = bundleMap.end();
    int32_t dataIndex = 0;
    while ( b != be )
    {

      const gkg::BundleMap< std::string >::Bundle& bundle = b->second;
      gkg::Curve3dMap< float >::const_iterator f = bundle.begin(),
                                               fe = bundle.end();

      while ( f != fe )
      {

        const gkg::LightCurve3d< float >& fiber = *f;

        typename gkg::LightCurve3d< float >::const_iterator
          p = fiber.begin(),
          pe = fiber.end();

        typename gkg::LightCurve3d< float >::const_iterator
          nextP = p + 1;

        if( p != pe )
        {

          while ( nextP != pe )
          {


            color = ( *nextP - *p ).absoluteCoordinates();
            color.normalize();

            _data[ dataIndex + 0 ] = p->x;
            _data[ dataIndex + 1 ] = p->y;
            _data[ dataIndex + 2 ] = p->z;

            _data[ dataIndex + 3 ] = color.x;
            _data[ dataIndex + 4 ] = color.y;
            _data[ dataIndex + 5 ] = color.z;

            _data[ dataIndex + 6 ] = nextP->x;
            _data[ dataIndex + 7 ] = nextP->y;
            _data[ dataIndex + 8 ] = nextP->z;

            _data[ dataIndex + 9 ] = color.x;
            _data[ dataIndex + 10 ] = color.y;
            _data[ dataIndex + 11 ] = color.z;

            ++ p;
            ++ nextP;
            dataIndex += 12;

          }

        }

        ++ f;

      }

      ++ b;

    }

    _javaScriptMatrix4x4 = Wt::WGLWidget::JavaScriptMatrix4x4();
    this->addJavaScriptMatrix4( _javaScriptMatrix4x4 );

  }
  GKG_CATCH( "gkg::PaintWidget::PaintWidget( "
             "const bool& useBinaryBuffers )" );

}


void gkg::PaintWidget::initializeGL()
{

  try
  {

    // computing the FOV center
    gkg::BoundingBox< float > boundingBox;
    gkg::Vector3d< float > fovCenter = this->getFOVCenter( boundingBox );

    // transform the world so that we look at the FOV center of the scene
    Wt::WMatrix4x4 worldTransform;
    worldTransform.lookAt( // camera position
                           ( double )fovCenter.x,
                           ( double )fovCenter.y,
                           ( double )fovCenter.z + 10,
                           // looking at
                           ( double )fovCenter.x,
                           ( double )fovCenter.y,
                           ( double )fovCenter.z,
                           // 'up' vector
                           0.0,
                           1.0,
                           0.0 );

    

    if ( !_initialized )
    {

      this->initJavaScriptMatrix4( _javaScriptMatrix4x4 );
      this->setJavaScriptMatrix4( _javaScriptMatrix4x4,
                                  worldTransform );

      // handling client-side mouse handler that modifies the world
      // transformation matrix; it works by specifying a FOV center and an up 
      // direction; mouse movements will allow the camera to be moved around 
      // the FOV center
      this->setClientSideLookAtHandler( _javaScriptMatrix4x4,
                                        // center point
                                        ( double )fovCenter.x,
                                        ( double )fovCenter.y,
                                        ( double )fovCenter.z,
                                        // upward direction
                                        0.0,
                                        1.0,
                                        0.0,
                                        // speed factor(s)
                                        0.005,
                                        0.005 );

      // Alternative: this installs a client-side mouse handler that allows
      // to 'walk' around: go forward, backward, turn left, turn right, ...
      //setClientSideWalkHandler(jsMatrix_, 0.05, 0.005);
      _initialized = true;

    }

    // loading a simple shader
    Wt::WGLWidget::Shader 
      fragmentShader = this->createShader( FRAGMENT_SHADER );
    this->shaderSource( fragmentShader, _fragmentShader);
    this->compileShader(fragmentShader);

    Wt::WGLWidget::Shader 
      vertexShader = this->createShader( VERTEX_SHADER );
    this->shaderSource( vertexShader, _vertexShader );
    this->compileShader( vertexShader );

    _wtShaderProgram = this->createProgram();
    this->attachShader( _wtShaderProgram, vertexShader );
    this->attachShader( _wtShaderProgram, fragmentShader );
    this->linkProgram( _wtShaderProgram );
    this->useProgram( _wtShaderProgram );

    // extracting the references to the attributes from the shader
    _wtVertexPositionAttribute =
      this->getAttribLocation( _wtShaderProgram, "aVertexPosition" );
    this->enableVertexAttribArray( _wtVertexPositionAttribute );
    _wtVertexColorAttribute   =
      this->getAttribLocation( _wtShaderProgram, "aVertexColor" );
    this->enableVertexAttribArray( _wtVertexColorAttribute );

    // extracting the references to the uniforms from the shader
    _wtPositionMatrixUniformLocation  = 
      this->getUniformLocation( _wtShaderProgram, "uPMatrix" );
    _wtCMatrixUniformLocation  = 
      this->getUniformLocation( _wtShaderProgram, "uCMatrix" );
    _wtMVMatrixUniformLocation = 
      this->getUniformLocation( _wtShaderProgram, "uMVMatrix" );
    _wtNMatrixUniformLocation  = 
      this->getUniformLocation( _wtShaderProgram, "uNMatrix" );

    // creating a Vertex Buffer Object (VBO) and load all polygon's data
    // (points, colors) into it; in this case we use one VBO that contains
    // all data (6 per point: vx, vy, vz, nx, ny, nz); alternatively you
    // can use multiple VBO's (e.g. one VBO for normals, one for points,
    // one for texture coordinates).
    // note that if you use indexed buffers, you cannot have indexes
    // larger than 65K, due to the limitations of WebGL.
    _wtObjectBuffer = this->createBuffer();
    this->bindBuffer( ARRAY_BUFFER, _wtObjectBuffer );

    // embedding the buffer directly in the JavaScript stream if 
    // _useBinaryBuffers is false; alternatively transfer the array directly as
    // a binary data resource if _useBinaryBuffers is true.
    this->bufferDatafv( ARRAY_BUFFER,
                        _data.begin(), _data.end(),
                        STATIC_DRAW,
                        _useBinaryBuffers );

    // setting the clear color to a transparent background
    this->clearColor( 0, 0, 0, 0 );

    // resetting Z-buffer, enabling Z-buffering
    this->clearDepth( 1 );
    this->enable( DEPTH_TEST );
    this->depthFunc( LEQUAL );


  }
  GKG_CATCH( "void gkg::PaintWidget::initializeGL()" );

}


void gkg::PaintWidget::paintGL()
{

  try
  {

    // clearing color an depth buffers
    this->clear( COLOR_BUFFER_BIT | DEPTH_BUFFER_BIT);

    // configure the shader: setting the uniforms

    // setting the camera transformation to the value of a client-side JS matrix
    this->uniformMatrix4( _wtCMatrixUniformLocation, _javaScriptMatrix4x4 );

    // a model matrix is used to move the model around; here, we let it to id
    Wt::WMatrix4x4 modelMatrix;
    this->uniformMatrix4( _wtMVMatrixUniformLocation, modelMatrix );

    // in desktop OpenGL, a shader has the gl_NormalMatrix matrix available in 
    // the shader language, a matrix that is used to transform normals to e.g. 
    // implement proper Phong shading (google will help you to find a detailed
    // explanation of why you need it). It is the transposed inverse of the 
    // model view matrix; unfortunately, this matrix is not available in WebGL,
    // so if you want to do phong shading, you must calculate it yourself;
    // Wt provides methods to calculate the transposed inverse of a matrix,
    // when client-side JS matrices are involved; here, we inverse-transpose
    // the product of the client-side camera matrix and the model matrix.
    this->uniformMatrix4( _wtNMatrixUniformLocation,
                           ( _javaScriptMatrix4x4 * modelMatrix ).inverted().
                                                                 transposed() );

    // configuring the shaders: set the attributes.
    // attributes are 'variables' within a shader: they vary for every point
    // that has to be drawn. All are stored in one VBO.
    this->bindBuffer( ARRAY_BUFFER, _wtObjectBuffer );

    // configure the vertex attributes
    this->vertexAttribPointer(
                         _wtVertexPositionAttribute,
                         3,         // every vertex has an x, y and z component
                         FLOAT,     // they are floats
                         false,     // do not normalize the vertices
                         2 * 3 * 4, // stride: first byte of the next vertex 
                                    // vx, vy, vz, nx, ny, nz
                         0 );       // offset position of the first vertex

    this->vertexAttribPointer(
                         _wtVertexColorAttribute,
                         3,
                         FLOAT,
                         false,
                         2 * 3 * 4,  //  stride: jump to the next normal
                         3 * 4 );    // offset: first normal after first vertex

    // drawing all the triangles
    this->drawArrays( LINES, 0, _data.size() / 6U );

  }
  GKG_CATCH( "void gkg::PaintWidget::paintGL()" );

}


void gkg::PaintWidget::resizeGL( int width, int height )
{

  try
  {

    // setting the viewport size
    this->viewport( 0, 0, width, height );

    // setting the projection matrix to some fixed values
    Wt::WMatrix4x4 projectionMatrix;
    projectionMatrix.perspective( 45,
                                  ( ( double )width ) / height,
                                  1,
                                  40 );
    this->uniformMatrix4( _wtPositionMatrixUniformLocation, projectionMatrix );

  }
  GKG_CATCH( "void gkg::PaintWidget::resizeGL( int width, int height )" );

}


void gkg::PaintWidget::setShaders( const std::string& vertexShader,
                                   const std::string& fragmentShader )
{

  try
  {

    _vertexShader = vertexShader;
    _fragmentShader = fragmentShader;

  }
  GKG_CATCH( "void gkg::PaintWidget::setShaders( "
             "const std::string& vertexShader, "
             "const std::string& fragmentShader )" );

}


gkg::Vector3d< float > gkg::PaintWidget::getFOVCenter(
                                  gkg::BoundingBox< float >& boundingBox ) const
{

  try
  {

    float minX = _data[ 0 ];
    float minY = _data[ 1 ];
    float minZ = _data[ 2 ];
    float maxX = _data[ 0 ];
    float maxY = _data[ 1 ];
    float maxZ = _data[ 2 ];

    std::vector< float>::size_type i = 0U;
    std::vector< float>::size_type count = _data.size() / 6U;

    for ( i = 0U; i < count; i++ )
    {

      // along X coordinate
      if ( _data[ 6 * i + 0 ] < minX )
      {

        minX = _data[ 6 * i + 0 ];

      }
      if ( _data[ 6 * i + 0 ] > maxX )
      {

        maxX = _data[ 6 * i + 0 ];

      }

      // along Y coordinate
      if ( _data[ 6 * i + 1 ] < minY )
      {

        minY = _data[ 6 * i + 1 ];

      }
      if ( _data[ 6 * i + 1 ] > maxY )
      {

        maxY = _data[ 6 * i + 1 ];

      }

      // along Z coordinate
      if ( _data[ 6 * i + 2 ] < minZ )
      {

        minZ = _data[ 6 * i + 2 ];

      }
      if ( _data[ 6 * i + 2 ] > maxZ )
      {

        maxZ = _data[ 6 * i + 2 ];

      }

    }

    boundingBox.setLowerX( minX );
    boundingBox.setLowerY( minY );
    boundingBox.setLowerZ( minZ );

    boundingBox.setUpperX( maxX );
    boundingBox.setUpperY( maxY );
    boundingBox.setUpperZ( maxZ );

    gkg::Vector3d< float > fieldOfViewCenter( ( minX + maxX ) / 2.0f,
                                              ( minY + maxY ) / 2.0f,
                                              ( minZ + maxZ ) / 2.0f );

    return fieldOfViewCenter;

  }
  GKG_CATCH( "gkg::Vector3d< float > "
             "gkg::PaintWidget::getFOVCenter( "
             "gkg::BoundingBox< float >& boundingBox ) const" );

}


////////////////////////////////////////////////////////////////////////////////
// class WMBundleWebGLViewer
////////////////////////////////////////////////////////////////////////////////

namespace gkg
{


class WMBundleWebGLViewer : public Wt::WApplication
{

  public:

    WMBundleWebGLViewer( const Wt::WEnvironment& wtEnvironment );

  private:

    void updateShaders();

    Wt::WContainerWidget* _wtContainerWidget;
    gkg::PaintWidget* _paintWidget;
    gkg::BundleMap< std::string > _bundleMap;

};


}


gkg::WMBundleWebGLViewer::WMBundleWebGLViewer(
                                         const Wt::WEnvironment& wtEnvironment )
                         : Wt::WApplication( wtEnvironment )
{

  try
  {


    std::cout << "gkg::WMBundleWebGLViewer::1" << std::endl;

    // adding title in the window bar
    this->setTitle( "WM Bundle WegGL Viewer" );

    // reading data
    std::string fileNameBundleMap = std::string( std::getenv( "HOME" ) ) +
                                    "/Git/gkg/sdk/src/GkgWMBundleWebGLViewer/"
                                    "example.bundles";
//    if ( verbose )
//    {

      std::cout << "reading '" << fileNameBundleMap << "' : " << std::flush;

//    }
    
    gkg::Reader::getInstance().read( fileNameBundleMap, _bundleMap );
//    if ( verbose )
//    {

      std::cout << "done" << std::endl;

//    }


    // resetting paint widget to avoid pb during startup
    _paintWidget = 0;


    std::cout << "gkg::WMBundleWebGLViewer::2" << std::endl;

    // adding container for GL widget
    _wtContainerWidget = this->root()->addWidget(
                             Wt::cpp14::make_unique< Wt::WContainerWidget >() );

    std::cout << "gkg::WMBundleWebGLViewer::3" << std::endl;

    _wtContainerWidget->resize( 800, 800 );

    std::cout << "gkg::WMBundleWebGLViewer::4" << std::endl;

    _wtContainerWidget->setInline( false );

    std::cout << "gkg::WMBundleWebGLViewer::5" << std::endl;

    this->updateShaders();

    std::cout << "gkg::WMBundleWebGLViewer::6" << std::endl;

  }
  GKG_CATCH( "gkg::WMBundleWebGLViewer::WMBundleWebGLViewer( "
             "const Wt::WEnvironment& wtEnvironment )" );

}


void gkg::WMBundleWebGLViewer::updateShaders()
{

  try
  {

    // check if binary buffers are enabled i.e. if the application url is 
    // "GkgWMBundleWebGLViewer" on localhost:8080, use this to enable binary 
    // buffers: localhost:8080/webgl?binaryBuffers query given URL arguments...
    Wt::Http::ParameterValues httpParameterValues = 
      Wt::WApplication::instance()->environment().getParameterValues(
                                                              "binaryBuffers" );
    bool useBinaryBuffers = !httpParameterValues.empty();

    // reset the current _paintWidget
    if ( _paintWidget )
    {

      _paintWidget->removeFromParent();

    }
    _paintWidget = _wtContainerWidget->addWidget( 
               Wt::cpp14::make_unique< gkg::PaintWidget >( useBinaryBuffers,
                                                           this->_bundleMap ) );

    _paintWidget->resize( 800, 800 );
    _paintWidget->setShaders( vertexShaderSrc,
                              fragmentShaderSrc );

    //_paintWidget->setAlternativeContent(
    //                            cpp14::make_unique<WImage>( "nowebgl.png" ) );

  }
  GKG_CATCH( "void gkg::WMBundleWebGLViewer::updateShaders()" );

}


////////////////////////////////////////////////////////////////////////////////
// createApplication() singleton function
////////////////////////////////////////////////////////////////////////////////


std::unique_ptr< Wt::WApplication > 
createApplication( const Wt::WEnvironment& wtEnvironment )
{

  try
  {

    return Wt::cpp14::make_unique< gkg::WMBundleWebGLViewer >( wtEnvironment );

  }
  GKG_CATCH( "std::unique_ptr< Wt::WApplication > "
             "createApplication( const Wt::WEnvironment& wtEnvironment )" );

}


////////////////////////////////////////////////////////////////////////////////
// main()
////////////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {


    gkg::PluginLoader::getInstance().load();

//    std::string fileNameBundleMap;
//    bool verbose = false;
//    gkg::Application application( argc, argv,
//                                  "White matter bundle WebGL viewer" );
//    application.addSingleOption( "-i",
//                                 "Input bundle map file name",
//                                 fileNameBundleMap );
//    application.addSingleOption( "-verbose",
//                                 "Show as much information as possible",
//                                 verbose, 
//                                 true );
//    application.initialize();

    Wt::WServer wtServer( argc, argv, WTHTTP_CONFIGURATION );


    wtServer.addEntryPoint( Wt::EntryPointType::Application,
                            &createApplication );
    wtServer.run();

  }
  GKG_CATCH_COMMAND( result );

  return result;

}

