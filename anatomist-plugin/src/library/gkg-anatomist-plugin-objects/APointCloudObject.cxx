#include <gkg-anatomist-plugin-objects/APointCloudObject.h>
#include <gkg-anatomist-plugin-rendering-options/APointCloudObjectRenderingOptionsWidget.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-processing-mesh/MeshFactory.h>
#include <gkg-core-io/TextFileLineCount.h>
#include <gkg-core-exception/Exception.h>


#include <anatomist/surface/glcomponent.h>

#include <anatomist/object/actions.h>
#include <anatomist/control/qObjTree.h>
#include <map>
#include <fstream>
#include <GL/gl.h>


int32_t gkg::APointCloudObject::_classType =
                            gkg::APointCloudObject::registerClass();
Tree* gkg::APointCloudObject::_optionTree = 0;



gkg::APointCloudObject::APointCloudObject(
  const gkg::RCPointer< std::list< gkg::Vector3d< float > > >& points,
  const gkg::RCPointer< std::list< gkg::Vector3d< float > > >& colors )
                       : QObject(),
                         anatomist::SliceableObject(),
                         _points( points ),
                         _colors( colors )
{

  try
  {

    _type = gkg::APointCloudObject::_classType;

    float lowerX = 1e38;
    float lowerY = 1e38;
    float lowerZ = 1e38;
    float upperX = -1e38;
    float upperY = -1e38;
    float upperZ = -1e38;
    std::list< gkg::Vector3d< float > >::const_iterator
      p = _points->begin(),
      pe = _points->end();
    while ( p != pe )
    {

      if ( p->x < lowerX )
      {

        lowerX = p->x;

      }
      if ( p->y < lowerY )
      {

        lowerY = p->y;

      }
      if ( p->z < lowerZ )
      {

        lowerZ = p->z;

      }

      if ( p->x > upperX )
      {

        upperX = p->x;

      }
      if ( p->y > upperY )
      {

        upperY = p->y;

      }
      if ( p->z > upperZ )
      {

        upperZ = p->z;

      }

      ++ p;

    }
    _boundingBox.setLowerX( lowerX );
    _boundingBox.setLowerY( lowerY );
    _boundingBox.setLowerZ( lowerZ );
    _boundingBox.setUpperX( upperX );
    _boundingBox.setUpperY( upperY );
    _boundingBox.setUpperZ( upperZ );

    _sphereVertexCount = 60;
    _sphereRadius = 0.25;

    this->GetMaterial().SetDiffuse( 0.80, 0.80, 0.80, 0.5 );

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
      QObjectTree::TypeNames[ _classType ] = "Point Cloud";

    }

  }
  GKG_CATCH( "gkg::AMedusaObject::APointCloudObject( "
             "const gkg::RCPointer< std::list< gkg::Vector3d< float > > >& "
             " points )" );

}


gkg::APointCloudObject::~APointCloudObject()
{

}


bool gkg::APointCloudObject::Is2DObject()
{

  try
  {

    return true;

  }
  GKG_CATCH( "bool gkg::APointCloudObject::Is2DObject()" );

}


bool gkg::APointCloudObject::Is3DObject()
{

  try
  {

    return true;

  }
  GKG_CATCH( "bool gkg::APointCloudObject::Is3DObject()" );

}


Tree* gkg::APointCloudObject::optionTree() const
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
                    &gkg::APointCloudObjectRenderingOptionsWidget::create );
      t->insert( t2 );

    }
    return( _optionTree );

  }
  GKG_CATCH( "Tree* gkg::APointCloudObject::optionTree() const" );

}


float gkg::APointCloudObject::MinX2D() const
{

  try
  {

    return _boundingBox.getLowerX();

  }
  GKG_CATCH( "float gkg::APointCloudObject::MinX2D() const" );

}


float gkg::APointCloudObject::MinY2D() const
{

  try
  {

    return _boundingBox.getLowerY();

  }
  GKG_CATCH( "float gkg::APointCloudObject::MinY2D() const" );

}


float gkg::APointCloudObject::MinZ2D() const
{

  try
  {

    return _boundingBox.getLowerZ();

  }
  GKG_CATCH( "float gkg::APointCloudObject::MinZ2D() const" );

}


float gkg::APointCloudObject::MaxX2D() const
{

  try
  {

    return _boundingBox.getUpperX();

  }
  GKG_CATCH( "float gkg::APointCloudObject::MaxX2D() const" );

}


float gkg::APointCloudObject::MaxY2D() const
{

  try
  {

    return _boundingBox.getUpperY();

  }
  GKG_CATCH( "float gkg::APointCloudObject::MaxY2D() const" );

}


float gkg::APointCloudObject::MaxZ2D() const
{

  try
  {

    return _boundingBox.getUpperZ();

  }
  GKG_CATCH( "float gkg::APointCloudObject::MaxZ2D() const" );

}


bool gkg::APointCloudObject::boundingBox(
                                        std::vector< float > & bmin,
                                        std::vector< float > & bmax ) const
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


    return true;

  }
  GKG_CATCH( "bool gkg::APointCloudObject::boundingBox( "
             "std::vector< float > & bmin, "
             "std::vector< float > & bmax ) const" );

}


int32_t gkg::APointCloudObject::getSphereVertexCount() const
{

  try
  {

    return _sphereVertexCount;

  }
  GKG_CATCH( "int32_t gkg::APointCloudObject::"
             "getSphereVertexCount() const" );

}


void gkg::APointCloudObject::setSphereVertexCount( int32_t vertexCount )
{

  try
  {

    if ( vertexCount < 12 )
    {

      _sphereVertexCount = 12;

    }
    else
    {

      _sphereVertexCount = vertexCount;

    }

  }
  GKG_CATCH( "void gkg::APointCloudObject::"
             "setSphereVertexCount( "
             "int32_t vertexCount )" );

}


float gkg::APointCloudObject::getSphereRadius() const
{

  try
  {

    return _sphereRadius;

  }
  GKG_CATCH( "float gkg::APointCloudObject::"
             "getSphereRadius() const" );

}


void gkg::APointCloudObject::setSphereRadius( float radius )
{

  try
  {

    if ( radius < 0.0f )
    {

      _sphereRadius = 0.25;

    }
    else
    {

      _sphereRadius = radius;

    }

  }
  GKG_CATCH( "void gkg::APointCloudObject::"
             "setSphereRadius( "
             "float radius )" );

}


bool gkg::APointCloudObject::render( anatomist::PrimList& primitiveList,
                                      const anatomist::ViewState& viewState )
{

  try
  {

    anatomist::GLComponent* glComponent = anatomist::GLComponent::glAPI();
    glComponent->glSetChanged( glBODY );

    return anatomist::AObject::render( primitiveList, viewState );

  }
  GKG_CATCH( "bool gkg::APointCloudObject::render( "
             "anatomist::PrimList& primitiveList, "
             "const anatomist::ViewState& viewState )" );

}


bool gkg::APointCloudObject::glMakeBodyGLL(
                                     const anatomist::ViewState& /*viewState*/,
                                     const anatomist::GLList& glList ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // creating a list for a single unit sphere
    ////////////////////////////////////////////////////////////////////////////

    RCPointer< MeshMap< int32_t, float, 3U > >
      meshMap = gkg::MeshFactory::getInstance().getSphere(
                  gkg::Vector3d< float >( 0.0, 0.0, 0.0 ),
                  _sphereRadius,
                  _sphereVertexCount );

    GLuint sphereList = glGenLists( 1 );

    glNewList( sphereList, GL_COMPILE );

    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable( GL_LIGHTING );
    glEnable( GL_COLOR_MATERIAL );
    glEnable( GL_LIGHT0 );

    glBegin( GL_TRIANGLES );

    const std::list< gkg::Polygon< 3U > >&
      triangles = meshMap->polygons.getPolygons( 0 );

    const std::list< gkg::Vector3d< float > >&
      vertexList = meshMap->vertices.getSites( 0 );

    const std::list< gkg::Vector3d< float > >&
      normalList = meshMap->getNormals( 0 );

    std::vector< gkg::Vector3d< float > > vertices( vertexList.begin(),
                                                    vertexList.end() );
    std::vector< gkg::Vector3d< float > > normals( normalList.begin(),
                                                   normalList.end() );

    int32_t vertexIndex = 0;
    std::list< gkg::Polygon< 3U > >::const_iterator
      t = triangles.begin(),
      te = triangles.end();
    while ( t != te )
    {

      for ( vertexIndex = 0; vertexIndex < 3; vertexIndex++ )
      {

        const Vector3d< float >& 
          normal = normals[ t->indices[ vertexIndex ] ];

        glNormal3f( normal.x, normal.y, normal.z );

        const Vector3d< float >& 
          vertex = vertices[ t->indices[ vertexIndex ] ];
        glVertex3f( vertex.x, vertex.y, vertex.z );
                  
      }
      ++ t;

    }

    glEnd();

    glDisable( GL_LIGHT0 );
    glDisable( GL_COLOR_MATERIAL );
    glDisable( GL_LIGHTING );

    glEndList();


    ////////////////////////////////////////////////////////////////////////////
    // collecting transparency
    ////////////////////////////////////////////////////////////////////////////

    const anatomist::Material& material = this->material();
    // by default, fibers are colored using the default material
    float red = ( float )material.Diffuse( 0 );
    float green = ( float )material.Diffuse( 1 );
    float blue = ( float )material.Diffuse( 2 );
    float opacity = ( float )material.Diffuse( 3 );

    ////////////////////////////////////////////////////////////////////////////
    // drawing population(s)
    ////////////////////////////////////////////////////////////////////////////
    glNewList( glList.item(), GL_COMPILE );


    if ( _colors.isNull() )
    {

      glColor4f( ( GLfloat )red,
                 ( GLfloat )green,
                 ( GLfloat )blue,
                 ( GLfloat )opacity );

      std::list< gkg::Vector3d< float > >::const_iterator
        p = _points->begin(),
        pe = _points->end();
      while ( p != pe )
      {

        // saving current MODELVIEW matrix to stack
        glPushMatrix();

        // translation sphere to sphere atom center
        glTranslatef( p->x, p->y, p->z );

        // calling the display list which renders the sphere
        glCallList( sphereList );
 
        // removing current MODELVIEW Matrix from stack
        glPopMatrix();

        ++ p;

      }

    }
    else
    {

     std::list< gkg::Vector3d< float > >::const_iterator
        p = _points->begin(),
        pe = _points->end();
      std::list< gkg::Vector3d< float > >::const_iterator
        c = _colors->begin();
      while ( p != pe )
      {

        // adding the adequate color
        glColor4f( ( GLfloat )c->x,
                   ( GLfloat )c->y,
                   ( GLfloat )c->z,
                   ( GLfloat )opacity );

        // saving current MODELVIEW matrix to stack
        glPushMatrix();

        // translation sphere to sphere atom center
        glTranslatef( p->x, p->y, p->z );

        // calling the display list which renders the sphere
        glCallList( sphereList );
 
        // removing current MODELVIEW Matrix from stack
        glPopMatrix();

        ++ c;
        ++ p;

      }

    }

    glEndList();

    return true;

  }
  GKG_CATCH( "bool gkg::APointCloudObject::glMakeBodyGLL( "
             "const anatomist::ViewState& viewState, "
             "const anatomist::GLList& glList ) const" );

}


////////////////////////////////////////////////////////////////////////////////
// static methods
////////////////////////////////////////////////////////////////////////////////

std::list< anatomist::AObject* > 
gkg::APointCloudObject::load( const std::string& fileName,
                              anatomist::ObjectReader::PostRegisterList &,
                              carto::Object /* options */ )
{

  try
  {

    // building the names of the files to be uploaded
    std::string fileNameRoot = fileName.substr( 0, fileName.length() - 11U );
    std::string fileNamePointCloud = fileNameRoot + ".pointcloud";

    // reading virtual tissue
    gkg::RCPointer< std::list< gkg::Vector3d< float > > >
      points( new std::list< gkg::Vector3d< float > > );
    gkg::RCPointer< std::list< gkg::Vector3d< float > > >
      colors;

    gkg::File pointCloudFile( fileNamePointCloud );

    if ( pointCloudFile.isExisting() )
    {

      
      std::vector< int32_t > itemCounts = gkg::getStringItemCountPerLine(
                                                           fileNamePointCloud );

      int32_t itemCount = itemCounts[ 0 ];

      std::cout << "item count=" << itemCount << std::endl;
      std::ifstream is( fileNamePointCloud.c_str() );

      if ( itemCount == 3 )
      {

        gkg::Vector3d< float > point;
        while ( !is.eof() )
        {

          is >> point.x >> point.y >> point.z;
          points->push_back( point );

        }

      }
      else if ( itemCount == 6 )
      {

        colors.reset( new std::list< gkg::Vector3d< float > > );
        gkg::Vector3d< float > point;
        gkg::Vector3d< float > color;
        while ( !is.eof() )
        {

          is >> point.x >> point.y >> point.z;
          is >> color.x >> color.y >> color.z;
          points->push_back( point );
          colors->push_back( color );

        }

      }
      else
      {

        throw std::runtime_error(
                  "item count per line of the point cloud file is not 3 or 6" );

      }

      is.close();

    }

    std::cout << "point count : " << points->size() << std::endl;
    if ( !colors.isNull() )
    {

      std::cout << "color count : " << colors->size() << std::endl;

    }

    // creating APointCloudObject object
    gkg::APointCloudObject* 
      aPointCloudObject = new gkg::APointCloudObject( points, colors );
    std::list< anatomist::AObject* > objectList;
    objectList.push_back( aPointCloudObject );

    return objectList;

  }
  GKG_CATCH( "std::list< anatomist::AObject* > "
             "gkg::AMedusaObject::load( const std::string& fileName, "
             "anatomist::ObjectReader::PostRegisterList &, "
             "carto::Object /* options */ )" );

}


int32_t gkg::APointCloudObject::registerClass()
{

  try
  {

    // associating a new loader to the .clusters extension
    anatomist::ObjectReader::registerLoader(
      "pointcloud",
      gkg::APointCloudObject::load );

    // creating a new class type
    _classType = registerObjectType( "POINT_CLOUD" );

    // loading the GKG plugin(s)
    gkg::PluginLoader::getInstance().load( false );

#ifdef GKG_DEBUG

    std::cout << "POINT_CLOUD loader registered : "
              << _classType << std::endl;

#endif

    return _classType;

  }
  GKG_CATCH( "int32_t gkg::APointCloudObject::registerClass()" );

}


int32_t gkg::APointCloudObject::getClassType()
{

  try
  {

    return _classType;

  }
  GKG_CATCH( "int32_t gkg::APointCloudObject::getClassType()" );

}



