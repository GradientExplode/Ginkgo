#include <gkg-anatomist-plugin-objects/AMedusaGeometryObject.h>
#include <gkg-anatomist-plugin-rendering-options/AMedusaGeometryObjectRenderingOptionsWidget.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-processing-mesh/MeshFactory.h>
#include <gkg-processing-colorimetry/RandomColorGenerator.h>
#include <gkg-processing-colorimetry/RandomColorGenerator.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-exception/Exception.h>


#include <anatomist/surface/glcomponent.h>

#include <anatomist/object/actions.h>
#include <anatomist/control/qObjTree.h>
#include <map>
#include <fstream>
#include <GL/gl.h>


int32_t gkg::AMedusaGeometryObject::_classType =
                            gkg::AMedusaGeometryObject::registerClass();
Tree* gkg::AMedusaGeometryObject::_optionTree = 0;



gkg::AMedusaGeometryObject::AMedusaGeometryObject(
  const gkg::RCPointer< gkg::VirtualTissue >& virtualTissue,
  const gkg::RCPointer< std::vector< gkg::RGBComponent > >& populationColors )
                           : QObject(),
                             anatomist::SliceableObject(),
                             _virtualTissue( virtualTissue ),
                             _populationColors( populationColors )
{

  try
  {

    _type = gkg::AMedusaGeometryObject::_classType;
    _boundingBox = _virtualTissue->getFieldOfView();
    
    _vertexCountPerSphereAtom = 60;
    _fiberPopulationVisibility = true;
    _somaPopulationVisibility = true;
    _neuronPopulationVisibility = true;
    _dendritePopulationVisibility = true;
    _astrocytePopulationVisibility = true;
    _oligodendrocytePopulationVisibility = true;

    this->GetMaterial().SetDiffuse( 0.80, 0.80, 0.80, 0.5 );

    // adding icon
    if ( QObjectTree::TypeNames.find( _classType ) == 
         QObjectTree::TypeNames.end() )
    {

      std::string
          iconPath = gkg::ConfigurationInfo::getInstance().getIconPath(
                                            "icon-medusa-geometry.png" );
      if ( !QObjectTree::TypeIcons[ _classType ].load( iconPath.c_str() ) )
      {

        QObjectTree::TypeIcons.erase( _classType );
        std::cerr << "icon " << iconPath << " not found!" << std::endl;

      }
      QObjectTree::TypeNames[ _classType ] = "Medusa Geometry";

    }

  }
  GKG_CATCH( "gkg::AMedusaObject::AMedusaGeometryObject( "
             "const gkg::RCPointer< gkg::VirtualTissue >& virtualTissue, "
             "const gkg::RCPointer< std::vector< gkg::RGBComponent > >& "
             "populationColors )" );

}


gkg::AMedusaGeometryObject::~AMedusaGeometryObject()
{

}


bool gkg::AMedusaGeometryObject::Is2DObject()
{

  try
  {

    return true;

  }
  GKG_CATCH( "bool gkg::AMedusaGeometryObject::Is2DObject()" );

}


bool gkg::AMedusaGeometryObject::Is3DObject()
{

  try
  {

    return true;

  }
  GKG_CATCH( "bool gkg::AMedusaGeometryObject::Is3DObject()" );

}


Tree* gkg::AMedusaGeometryObject::optionTree() const
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
                    &gkg::AMedusaGeometryObjectRenderingOptionsWidget::create );
      t->insert( t2 );

    }
    return( _optionTree );

  }
  GKG_CATCH( "Tree* gkg::AMedusaGeometryObject::optionTree() const" );

}


float gkg::AMedusaGeometryObject::MinX2D() const
{

  try
  {

    return _boundingBox.getLowerX();

  }
  GKG_CATCH( "float gkg::AMedusaGeometryObject::MinX2D() const" );

}


float gkg::AMedusaGeometryObject::MinY2D() const
{

  try
  {

    return _boundingBox.getLowerY();

  }
  GKG_CATCH( "float gkg::AMedusaGeometryObject::MinY2D() const" );

}


float gkg::AMedusaGeometryObject::MinZ2D() const
{

  try
  {

    return _boundingBox.getLowerZ();

  }
  GKG_CATCH( "float gkg::AMedusaGeometryObject::MinZ2D() const" );

}


float gkg::AMedusaGeometryObject::MaxX2D() const
{

  try
  {

    return _boundingBox.getUpperX();

  }
  GKG_CATCH( "float gkg::AMedusaGeometryObject::MaxX2D() const" );

}


float gkg::AMedusaGeometryObject::MaxY2D() const
{

  try
  {

    return _boundingBox.getUpperY();

  }
  GKG_CATCH( "float gkg::AMedusaGeometryObject::MaxY2D() const" );

}


float gkg::AMedusaGeometryObject::MaxZ2D() const
{

  try
  {

    return _boundingBox.getUpperZ();

  }
  GKG_CATCH( "float gkg::AMedusaGeometryObject::MaxZ2D() const" );

}


bool gkg::AMedusaGeometryObject::boundingBox(
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
  GKG_CATCH( "bool gkg::AMedusaGeometryObject::boundingBox( "
             "std::vector< float > & bmin, "
             "std::vector< float > & bmax ) const" );

}


int32_t gkg::AMedusaGeometryObject::getVertexCountPerSphereAtom() const
{

  try
  {

    return _vertexCountPerSphereAtom;

  }
  GKG_CATCH( "int32_t gkg::AMedusaGeometryObject::"
             "getVertexCountPerSphereAtom() const" );

}


void gkg::AMedusaGeometryObject::setVertexCountPerSphereAtom(
                                                      int32_t vertexCount )
{

  try
  {

    if ( vertexCount < 12 )
    {

      _vertexCountPerSphereAtom =12;

    }
    else
    {

      _vertexCountPerSphereAtom = vertexCount;

    }

  }
  GKG_CATCH( "void gkg::AMedusaGeometryObject::"
             "setVertexCountPerSphereAtom( "
             "int32_t vertexCount )" );

}


bool gkg::AMedusaGeometryObject::getFiberPopulationVisibility() const
{

  try
  {

    return _fiberPopulationVisibility;

  }
  GKG_CATCH( "bool "
             "gkg::AMedusaGeometryObject::"
             "getFiberPopulationVisibility() const" );

}


void gkg::AMedusaGeometryObject::setFiberPopulationVisibility( bool value )
{

  try
  {

    _fiberPopulationVisibility = value;

  }
  GKG_CATCH( "void "
             "gkg::AMedusaGeometryObject::"
             "setFiberPopulationVisibility( bool value )" );

}


bool gkg::AMedusaGeometryObject::getSomaPopulationVisibility() const
{

  try
  {

    return _somaPopulationVisibility;

  }
  GKG_CATCH( "bool "
             "gkg::AMedusaGeometryObject::"
             "getSomaPopulationVisibility() const" );

}


void gkg::AMedusaGeometryObject::setSomaPopulationVisibility( bool value )
{

  try
  {

    _somaPopulationVisibility = value;

  }
  GKG_CATCH( "void "
             "gkg::AMedusaGeometryObject::"
             "setSomaPopulationVisibility( bool value )" );

}


bool gkg::AMedusaGeometryObject::getNeuronPopulationVisibility() const
{

  try
  {

    return _neuronPopulationVisibility;

  }
  GKG_CATCH( "bool "
             "gkg::AMedusaGeometryObject::"
             "getNeuronPopulationVisibility() const" );

}


void gkg::AMedusaGeometryObject::setNeuronPopulationVisibility( bool value )
{

  try
  {

    _neuronPopulationVisibility = value;

  }
  GKG_CATCH( "void "
             "gkg::AMedusaGeometryObject::"
             "setNeuronPopulationVisibility( bool value )" );

}


bool gkg::AMedusaGeometryObject::getDendritePopulationVisibility() const
{

  try
  {

    return _dendritePopulationVisibility;

  }
  GKG_CATCH( "bool "
             "gkg::AMedusaGeometryObject::"
             "getDendritePopulationVisibility() const" );

}


void gkg::AMedusaGeometryObject::setDendritePopulationVisibility( bool value )
{

  try
  {

    _dendritePopulationVisibility = value;

  }
  GKG_CATCH( "void "
             "gkg::AMedusaGeometryObject::"
             "setDendritePopulationVisibility( bool value )" );

}


bool gkg::AMedusaGeometryObject::getAstrocytePopulationVisibility() const
{

  try
  {

    return _astrocytePopulationVisibility;

  }
  GKG_CATCH( "bool "
             "gkg::AMedusaGeometryObject::"
             "getAstrocytePopulationVisibility() const" );

}


void gkg::AMedusaGeometryObject::setAstrocytePopulationVisibility( bool value )
{

  try
  {

    _astrocytePopulationVisibility = value;

  }
  GKG_CATCH( "void "
             "gkg::AMedusaGeometryObject::"
             "setAstrocytePopulationVisibility( bool value )" );

}


bool gkg::AMedusaGeometryObject::getOligodendrocytePopulationVisibility() const
{

  try
  {

    return _oligodendrocytePopulationVisibility;


  }
  GKG_CATCH( "bool "
             "gkg::AMedusaGeometryObject::"
             "getOligodendrocytePopulationVisibility() const" );

}


void gkg::AMedusaGeometryObject::setOligodendrocytePopulationVisibility(
                                                                  bool value )
{

  try
  {

    _oligodendrocytePopulationVisibility = value;

  }
  GKG_CATCH( "void "
             "gkg::AMedusaGeometryObject::"
             "setOligodendrocytePopulationVisibility( bool value )" );

}


bool gkg::AMedusaGeometryObject::render( anatomist::PrimList& primitiveList,
                                         const anatomist::ViewState& viewState )
{

  try
  {

    anatomist::GLComponent* glComponent = anatomist::GLComponent::glAPI();
    glComponent->glSetChanged( glBODY );

    return anatomist::AObject::render( primitiveList, viewState );

  }
  GKG_CATCH( "bool gkg::AMedusaGeometryObject::render( "
             "anatomist::PrimList& primitiveList, "
             "const anatomist::ViewState& viewState )" );

}


bool gkg::AMedusaGeometryObject::glMakeBodyGLL(
                                     const anatomist::ViewState& /*viewState*/,
                                     const anatomist::GLList& glList ) const
{

  try
  {


    gkg::NumericalAnalysisImplementationFactory* factory = 
       gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();
    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

    ////////////////////////////////////////////////////////////////////////////
    // creating a list for a single unit sphere
    ////////////////////////////////////////////////////////////////////////////

    RCPointer< MeshMap< int32_t, float, 3U > >
      meshMap = gkg::MeshFactory::getInstance().getSphere(
                  gkg::Vector3d< float >( 0.0, 0.0, 0.0 ),
                  1.0,
                  _vertexCountPerSphereAtom );

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

    gkg::VirtualTissue::const_iterator
      p = _virtualTissue->begin(),
      pe = _virtualTissue->end();
    while ( p != pe )
    {

      // getting access to the current population
      const gkg::Population& population = **p;

      // getting access to the current population id
      const int32_t& populationId = population.getId();

      // getting access to the current population typename
      std::string populationTypeName = population.getTypeName();

      if ( ( ( populationTypeName == "fiber-population" ) &&
             (_fiberPopulationVisibility == true ) ) ||
           ( ( populationTypeName == "astrocyte-population" ) &&
             (_astrocytePopulationVisibility == true ) ) ||
           ( ( populationTypeName == "oligodendrocyte-population" ) &&
             (_oligodendrocytePopulationVisibility == true ) ) ||
           ( ( populationTypeName == "dendrite-population" ) &&
             (_dendritePopulationVisibility == true ) ) ||
           ( ( populationTypeName == "soma-population" ) &&
             (_somaPopulationVisibility == true ) ) ||
           ( ( populationTypeName == "neuron-population" ) &&
             (_neuronPopulationVisibility == true ) ) )
      {

        // setting the color of the current population
        const gkg::RGBComponent& 
          rgbComponent = ( *_populationColors )[ populationId ];

        red = ( float )rgbComponent.r / 255.0;
        green = ( float )rgbComponent.g / 255.0;
        blue = ( float )rgbComponent.b / 255.0;

        // looping over the population cell(s)
        gkg::Population::const_iterator
          c = population.begin(),
          ce = population.end();
        while ( c != ce )
        {


          glColor4f( ( GLfloat )std::max( 
                                  std::min( factory->getUniformRandomNumber(
                                                   randomGenerator,
                                                   0.85 * red,
                                                   1.15 * red ), 1.0 ), 0.0 ),
                     ( GLfloat )std::max( 
                                  std::min( factory->getUniformRandomNumber(
                                                   randomGenerator,
                                                   0.85 * green,
                                                   1.15 * green ), 1.0 ), 0.0 ),
                     ( GLfloat )std::max( 
                                  std::min( factory->getUniformRandomNumber(
                                                   randomGenerator,
                                                   0.85 * blue,
                                                   1.15 * blue ), 1.0 ), 0.0 ),
                     opacity );

          //  getting access to the current cell
          const gkg::Cell& cell = **c;

          // looping over the cell atom(s)
          gkg::Cell::const_iterator
            a = cell.begin(),
            ae = cell.end();
          while ( a != ae )
          {

            // getting access to the current sphere atom
 
            // saving current MODELVIEW matrix to stack
            glPushMatrix();
           
            if ( ( *a )->getType() == 1U )
            {
            
              const gkg::SphereAtom* 
              sphereAtom = static_cast< const gkg::SphereAtom* >( *a );

              // collecting its center and radius
              gkg::Vector3d< float > center = sphereAtom->getCenter();
              float radius = sphereAtom->getRadius();


              // translating sphere to sphere atom center
              glTranslatef( center.x, center.y, center.z );

              // scaling the sphere to the adequate radius
              glScaled( ( GLdouble )radius,
                        ( GLdouble )radius,
                        ( GLdouble )radius );


            }
            else if ( ( *a )->getType() == 2U )
            {
            
              const gkg::EllipsoidAtom* 
              ellipsoidAtom = static_cast< const gkg::EllipsoidAtom* >( *a );
            
              // collecting its center, radii and rotation angles
              
              gkg::Vector3d< float > center;
              gkg::Vector3d< float > radii;
              gkg::Vector3d< float > rotationAngles;
              ellipsoidAtom->getStdParameters( center,
                                               radii,
                                               rotationAngles,
                                               false );
                                            
              //center = gkg::Vector3d< float >( 0.0, 0.0, 0.0 );
              //radii = gkg::Vector3d< float >( 12.0, 2.4, 1.0 );
              //rotationAngles = gkg::Vector3d< float >( 0.0, 0.0, 0.0 );
              //std::cout << center << " " << radii << " "
              //          << rotationAngles << std::endl;

              // translating sphere to ellipsoid atom center
              glTranslatef( center.x, center.y, center.z );

              // rotating the ellipsoid wrt to the x, y and z coordinates
              glRotatef( ( GLdouble )rotationAngles.z * 180.0 / M_PI,
                         0.0, 0.0, 1.0 );
              glRotatef( ( GLdouble )rotationAngles.y * 180.0 / M_PI,
                         0.0, 1.0, 0.0 );
              glRotatef( ( GLdouble )rotationAngles.x * 180.0 / M_PI,
                         1.0, 0.0, 0.0 );

              // scaling the sphere to the adequate ellipsoid radii
              glScaled( ( GLdouble )radii.x,
                        ( GLdouble )radii.y,
                        ( GLdouble )radii.z );



            }

            // calling the display list which renders the sphere
            glCallList( sphereList );
				
            // removing current MODELVIEW Matrix from stack
            glPopMatrix();

            ++ a;          

          }

          ++ c;

        }

      }

      ++ p;

    }


    glColor4f( ( GLfloat )0,
               ( GLfloat )0,
               ( GLfloat )0,
               1.0 );
    glLineWidth( 2.0f );

    glBegin( GL_LINES );

    glVertex3d( _boundingBox.getUpperX(), _boundingBox.getLowerY(), _boundingBox.getLowerZ() );
    glVertex3d( _boundingBox.getUpperX(), _boundingBox.getUpperY(), _boundingBox.getLowerZ() );

    glVertex3d( _boundingBox.getUpperX(), _boundingBox.getLowerY(), _boundingBox.getUpperZ() );
    glVertex3d( _boundingBox.getUpperX(), _boundingBox.getUpperY(), _boundingBox.getUpperZ() );

    glVertex3d( _boundingBox.getLowerX(), _boundingBox.getLowerY(), _boundingBox.getLowerZ() );
    glVertex3d( _boundingBox.getLowerX(), _boundingBox.getUpperY(), _boundingBox.getLowerZ() );

    glVertex3d( _boundingBox.getLowerX(), _boundingBox.getLowerY(), _boundingBox.getUpperZ() );
    glVertex3d( _boundingBox.getLowerX(), _boundingBox.getUpperY(), _boundingBox.getUpperZ() );

    glVertex3d( _boundingBox.getUpperX(), _boundingBox.getLowerY(), _boundingBox.getLowerZ() );
    glVertex3d( _boundingBox.getLowerX(), _boundingBox.getLowerY(), _boundingBox.getLowerZ() );

    glVertex3d( _boundingBox.getUpperX(), _boundingBox.getUpperY(), _boundingBox.getLowerZ() );
    glVertex3d( _boundingBox.getLowerX(), _boundingBox.getUpperY(), _boundingBox.getLowerZ() );

    glVertex3d( _boundingBox.getUpperX(), _boundingBox.getLowerY(), _boundingBox.getUpperZ() );
    glVertex3d( _boundingBox.getLowerX(), _boundingBox.getLowerY(), _boundingBox.getUpperZ() );

    glVertex3d( _boundingBox.getUpperX(), _boundingBox.getUpperY(), _boundingBox.getUpperZ() );
    glVertex3d( _boundingBox.getLowerX(), _boundingBox.getUpperY(), _boundingBox.getUpperZ() );

    glVertex3d( _boundingBox.getUpperX(), _boundingBox.getLowerY(), _boundingBox.getUpperZ() );
    glVertex3d( _boundingBox.getUpperX(), _boundingBox.getLowerY(), _boundingBox.getLowerZ() );

    glVertex3d( _boundingBox.getUpperX(), _boundingBox.getUpperY(), _boundingBox.getUpperZ() );
    glVertex3d( _boundingBox.getUpperX(), _boundingBox.getUpperY(), _boundingBox.getLowerZ() );

    glVertex3d( _boundingBox.getLowerX(), _boundingBox.getLowerY(), _boundingBox.getUpperZ() );
    glVertex3d( _boundingBox.getLowerX(), _boundingBox.getLowerY(), _boundingBox.getLowerZ() );

    glVertex3d( _boundingBox.getLowerX(), _boundingBox.getUpperY(), _boundingBox.getUpperZ() );
    glVertex3d( _boundingBox.getLowerX(), _boundingBox.getUpperY(), _boundingBox.getLowerZ() );

    glEnd();

    glEndList();

    return true;

  }
  GKG_CATCH( "bool gkg::AMedusaGeometryObject::glMakeBodyGLL( "
             "const anatomist::ViewState& viewState, "
             "const anatomist::GLList& glList ) const" );

}


////////////////////////////////////////////////////////////////////////////////
// static methods
////////////////////////////////////////////////////////////////////////////////

std::list< anatomist::AObject* > 
gkg::AMedusaGeometryObject::load( const std::string& fileName,
                                  anatomist::ObjectReader::PostRegisterList &,
                                  carto::Object /* options */ )
{

  try
  {

    // building the names of the files to be uploaded
    std::string fileNameRoot = fileName.substr( 0, fileName.length() - 15U );
    std::string fileNameVirtualTissue = fileNameRoot + ".medusageometry";
    std::string fileNameHierarchy = fileNameRoot + ".hie";

    // reading virtual tissue
    gkg::RCPointer< gkg::VirtualTissue >
      virtualTissue( new gkg::VirtualTissue );
    gkg::Reader::getInstance().read( fileNameVirtualTissue, *virtualTissue );

    // exploring hierarchy and collecting bundle colors
    std::map< std::string, gkg::RGBComponent > colors;

    gkg::File hierarchyFile( fileNameHierarchy );

    if ( hierarchyFile.isExisting() )
    {

      std::ifstream is( fileNameHierarchy.c_str() );

      std::string syntaxId;
      std::string label;
      gkg::RGBComponent rgbComponent;
      int32_t red = 0;
      int32_t green = 0;
      int32_t blue = 0;
      while ( !is.eof() )
      {

        is >> syntaxId;

        if ( syntaxId == "name" )
        {

          is >> label;
          is >> syntaxId;
          if ( syntaxId == "color" )
          {

            is >> red;
            is >> green;
            is >> blue;
            colors[ label ] = gkg::RGBComponent( ( uint8_t )red,
                                                 ( uint8_t )green,
                                                 ( uint8_t )blue ) ;

          }

        }

      }

      is.close();

    }

    // in case no hierarchy file found, randomly generating colors
    gkg::RandomColorGenerator randomColorGenerator( 0.7, 0.95 );

    // building population color map
    gkg::RCPointer< std::vector< RGBComponent > > 
      populationColors( new std::vector< RGBComponent >(
                           virtualTissue->getPopulationCount() ) );
    gkg::VirtualTissue::const_iterator
      p = virtualTissue->begin(),
      pe = virtualTissue->end();
    std::vector< gkg::RGBComponent >::iterator
      c = populationColors->begin();
    std::map< std::string, gkg::RGBComponent >::const_iterator color;
    while ( p != pe )
    {

      // getting access to the current population
      const gkg::Population& population = **p;
      const int32_t& populationId = population.getId();

      const std::string& 
        populationLabel = virtualTissue->getPopulationLabelFromId(
                                                                 populationId );

      if ( hierarchyFile.isExisting() )
      {

        color = colors.find( populationLabel );
        if ( color != colors.end() )
        {

          *c = color->second;

        }
        else
        {

          *c = randomColorGenerator.getRandomColor();

        }

      }
      else
      {

        *c = randomColorGenerator.getRandomColor();

      }
      ++ p;
      ++ c;

    }

    // creating AMedusaGeometryObject object
    gkg::AMedusaGeometryObject* 
      aMedusaGeometryObject = new gkg::AMedusaGeometryObject(
                                              virtualTissue,
                                              populationColors );
    std::list< anatomist::AObject* > objectList;
    objectList.push_back( aMedusaGeometryObject );

    return objectList;

  }
  GKG_CATCH( "std::list< anatomist::AObject* > "
             "gkg::AMedusaObject::load( const std::string& fileName, "
             "anatomist::ObjectReader::PostRegisterList &, "
             "carto::Object /* options */ )" );

}


int32_t gkg::AMedusaGeometryObject::registerClass()
{

  try
  {

    // associating a new loader to the .clusters extension
    anatomist::ObjectReader::registerLoader(
      "medusageometry",
      gkg::AMedusaGeometryObject::load );

    // creating a new class type
    _classType = registerObjectType( "MEDUSA_GEOMETRY" );

    // loading the GKG plugin(s)
    gkg::PluginLoader::getInstance().load( false );

#ifdef GKG_DEBUG

    std::cout << "MEDUSA_GEOMETRY loader registered : "
              << _classType << std::endl;

#endif

    return _classType;

  }
  GKG_CATCH( "int32_t gkg::AMedusaGeometryObject::registerClass()" );

}


int32_t gkg::AMedusaGeometryObject::getClassType()
{

  try
  {

    return _classType;

  }
  GKG_CATCH( "int32_t gkg::AMedusaGeometryObject::getClassType()" );

}



