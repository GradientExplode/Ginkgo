#include <gkg-anatomist-plugin-objects/AInteractiveBundleMapObject.h>
#include <gkg-anatomist-plugin-rendering-options/AInteractiveBundleMapRenderingOptionsWidget.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-processing-colorimetry/RandomColorGenerator.h>
#include <gkg-core-exception/Exception.h>


#include <anatomist/surface/glcomponent.h>

#include <anatomist/object/actions.h>
#include <anatomist/control/qObjTree.h>
#include <map>
#include <fstream>


//
// PARTIE A DEVELOPPER dans le constructeur de AInteractiveBundleMapObject
// -----------------------------------------------------------------------
//
// GLsizei pointCount = ( GLsizei )....;
// GLsizei segmentCount = ( GLsizei )....;
// 
// std::vector< GLfloat > pointsAndColors( pointCount );
// // a remplir
// 
// std::vector< GLuint > segmentIndices( 2U * segmentCount );
// // a remplir
// 
// 
// 
// 
// GLuint buffers[ 2 ];
// 
// 
// PARTIE A DEVELOPPER dans la methode glMakeBodyGLL()
// -----------------------------------------------------------------------
//
// glGenBuffers( 2, &buffers );
// 
// 
// glBindBuffer( GL_ARRAY_BUFFER, buffers[ 0 ] );
// glBufferData( GL_ARRAY_BUFFER,
//               pointsAndColors.size() * sizeof( GLfloat ),
//               &pointsAndColors[ 0 ],
//               GL_STATIC_DRAW );
// 
// 
// 
// 
// glBindBuffer( GL_ARRAY_BUFFER, buffers[ 1 ] );
// glBufferData( GL_ELEMENT_ARRAY_BUFFER,
//               segmentIndices.size() * sizeof( GLuint ),
//               &segmentIndices[ 0 ],
//               GL_STATIC_DRAW );
// 
// 
// glBindBuffer( GL_ARRAY_BUFFER, buffers[ 0 ] );
// glVertexPointer( 3, GL_FLOAT,
//                  6 * sizeof( GL_FLOAT ),
//                  ( GLfloat* )NULL + 0 );
// glColorPointer( 3, GL_FLOAT,
//                 6 * sizeof( GL_FLOAT ),
//                 ( GLfloat* )NULL + 3 );
// 
// 
// glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffers[ 1 ] );
// 
// glEnableClientState( GL_VERTEX_ARRAY );
// glEnableClientState( GL_COLOR_ARRAY );
// 
// glDrawElements( GL_LINES,
//                 2U * segmentCount,
//                 GL_UNSIGNED_INT, 0 );
// 
// glDisableClientState( GL_VERTEX_ARRAY );
// glDisableClientState( GL_NORMAL_ARRAY );



int32_t gkg::AInteractiveBundleMapObject::_classType =
                              gkg::AInteractiveBundleMapObject::registerClass();
Tree* gkg::AInteractiveBundleMapObject::_optionTree = 0;



gkg::AInteractiveBundleMapObject::AInteractiveBundleMapObject(
        const gkg::RCPointer< gkg::BundleMap< std::string > >& bundleMap,
        const gkg::RCPointer< std::vector< gkg::RGBComponent > >& bundleColors )
                                 : _bundleMap( bundleMap ),
                                   _bundleColors( bundleColors )
{

  try
  {

    _type = gkg::AInteractiveBundleMapObject::_classType;
    _boundingBox = _bundleMap->getBoundingBox();
    _fiberSubSamplingFactor = 1;
    _fiberLineWidth = 1.0;
    _fiberColorEncoding = 0;
    _fileNameScalarFeatureVolume = "";
    _fileNameScalarFeatureVolumeToBundleTransform3d = "";
    _lowerBound = -1e38;
    _upperBound = +1e38;

    this->GetMaterial().SetDiffuse( 0.15, 0.15, 0.15, 0.3 );

    // adding icon
    if ( QObjectTree::TypeNames.find( _classType ) == 
         QObjectTree::TypeNames.end() )
    {

      std::string
          iconPath = gkg::ConfigurationInfo::getInstance().getIconPath(
                                        "icon-interactive-fiber-clusters.png" );
      if ( !QObjectTree::TypeIcons[ _classType ].load( iconPath.c_str() ) )
      {

        QObjectTree::TypeIcons.erase( _classType );
        std::cerr << "icon " << iconPath << " not found!" << std::endl;

      }
      QObjectTree::TypeNames[ _classType ] = "INTERACTIVE_FIBER_CLUSTERS";

    }

  }
  GKG_CATCH( "gkg::AInteractiveBundleMapObject::AInteractiveBundleMapObject( "
             "const gkg::RCPointer< gkg::BundleMap< std::string > >& "
             "bundleMap, "
             "const gkg::RCPointer< std::vector< gkg::RGBComponent > >& "
             "bundleColors )" );

}


gkg::AInteractiveBundleMapObject::~AInteractiveBundleMapObject()
{

}


bool gkg::AInteractiveBundleMapObject::Is2DObject()
{

  try
  {

    return true;

  }
  GKG_CATCH( "bool gkg::AInteractiveBundleMapObject::Is2DObject()" );

}


bool gkg::AInteractiveBundleMapObject::Is3DObject()
{

  try
  {

    return true;

  }
  GKG_CATCH( "bool gkg::AInteractiveBundleMapObject::Is3DObject()" );

}


Tree* gkg::AInteractiveBundleMapObject::optionTree() const
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
      t2 = new Tree( true, "Save" );
      t2->setProperty( "callback",
                       &anatomist::ObjectActions::saveStatic );
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
      t2->setProperty(
                    "callback",
                    &gkg::AInteractiveBundleMapRenderingOptionsWidget::create );
      t->insert( t2 );

    }
    return( _optionTree );

  }
  GKG_CATCH( "Tree* gkg::AInteractiveBundleMapObject::optionTree() const" );

}


float gkg::AInteractiveBundleMapObject::MinX2D() const
{

  try
  {

    return _boundingBox.getLowerX();

  }
  GKG_CATCH( "float gkg::AInteractiveBundleMapObject::MinX2D() const" );

}


float gkg::AInteractiveBundleMapObject::MinY2D() const
{

  try
  {

    return _boundingBox.getLowerY();

  }
  GKG_CATCH( "float gkg::AInteractiveBundleMapObject::MinY2D() const" );

}


float gkg::AInteractiveBundleMapObject::MinZ2D() const
{

  try
  {

    return _boundingBox.getLowerZ();

  }
  GKG_CATCH( "float gkg::AInteractiveBundleMapObject::MinZ2D() const" );

}


float gkg::AInteractiveBundleMapObject::MaxX2D() const
{

  try
  {

    return _boundingBox.getUpperX();

  }
  GKG_CATCH( "float gkg::AInteractiveBundleMapObject::MaxX2D() const" );

}


float gkg::AInteractiveBundleMapObject::MaxY2D() const
{

  try
  {

    return _boundingBox.getUpperY();

  }
  GKG_CATCH( "float gkg::AInteractiveBundleMapObject::MaxY2D() const" );

}


float gkg::AInteractiveBundleMapObject::MaxZ2D() const
{

  try
  {

    return _boundingBox.getUpperZ();

  }
  GKG_CATCH( "float gkg::AInteractiveBundleMapObject::MaxZ2D() const" );

}


bool gkg::AInteractiveBundleMapObject::boundingBox( std::vector< float > & bmin,
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
  GKG_CATCH( "bool gkg::AInteractiveBundleMapObject::boundingBox( "
             "std::vector< float > & bmin, "
             "std::vector< float > & bmax ) const" );

}


int32_t gkg::AInteractiveBundleMapObject::getFiberSubSamplingFactor() const
{

  try
  {

    return _fiberSubSamplingFactor;

  }
  GKG_CATCH( "int32_t "
             "gkg::AInteractiveBundleMapObject::getFiberSubSamplingFactor() const" );

}


void 
gkg::AInteractiveBundleMapObject::setFiberSubSamplingFactor( int32_t value )
{

  try
  {

    if ( value < 1 )
    {

      throw std::runtime_error(
                    "fiber sub-sampling factor must be greater or equal to 1" );

    }
    _fiberSubSamplingFactor = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapObject::setFiberSubSamplingFactor( "
             "int32_t value )" );

}


float gkg::AInteractiveBundleMapObject::getFiberLineWidth() const
{

  try
  {

    return _fiberLineWidth;

  }
  GKG_CATCH( "float "
             "gkg::AInteractiveBundleMapObject::getFiberLineWidth() const" );

}


void 
gkg::AInteractiveBundleMapObject::setFiberLineWidth( float value )
{

  try
  {

    if ( value < 1.0 )
    {

      throw std::runtime_error(
                             "fiber line width must be greater or equal to 1" );

    }
    _fiberLineWidth = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapObject::setFiberLineWidth( "
             "float value )" );

}


int32_t gkg::AInteractiveBundleMapObject::getFiberColorEncoding() const
{

  try
  {

    return _fiberColorEncoding;

  }
  GKG_CATCH( "int32_t "
             "gkg::AInteractiveBundleMapObject::getFiberColorEncoding() const" );

}


const std::string& gkg::AInteractiveBundleMapObject::
                                          getFileNameScalarFeatureVolume() const
{

  try
  {

    return _fileNameScalarFeatureVolume;

  }
  GKG_CATCH( "const std::string& "
             "gkg::AInteractiveBundleMapObject::"
             "getFileNameScalarFeatureVolume() const" );

}


const std::string& 
gkg::AInteractiveBundleMapObject::
                       getFileNameScalarFeatureVolumeToBundleTransform3d() const
{

  try
  {

    return _fileNameScalarFeatureVolumeToBundleTransform3d;

  }
  GKG_CATCH( "const std::string& "
             "gkg::AInteractiveBundleMapObject::"
             "getFileNameScalarFeatureVolumeToBundleTransform3d() const" );

}


double gkg::AInteractiveBundleMapObject::getLowerBound() const
{

  try
  {

    return _lowerBound;

  }
  GKG_CATCH( "double gkg::AInteractiveBundleMapObject::getLowerBound() const" );

}


double gkg::AInteractiveBundleMapObject::getUpperBound() const
{

  try
  {

    return _upperBound;

  }
  GKG_CATCH( "double gkg::AInteractiveBundleMapObject::getUpperBound() const" );

}


void gkg::AInteractiveBundleMapObject::setFiberColorEncoding(
              int32_t colorEncodingType,
              const std::string& fileNameScalarFeatureVolume,
              const std::string& fileNameScalarFeatureVolumeToBundleTransform3d,
              double lowerBound,
              double upperBound )
{

  try
  {

    if ( ( colorEncodingType < 0 ) || ( colorEncodingType > 3 ) )
    {

      throw std::runtime_error( "fiber color encoding must be 0, 1, 2 or 3" );

    }
    _fiberColorEncoding = colorEncodingType;

    if ( _fiberColorEncoding == 3 )
    {

      bool hasChanged = false;
      if ( ( fileNameScalarFeatureVolume != 
             _fileNameScalarFeatureVolume ) ||
           ( fileNameScalarFeatureVolumeToBundleTransform3d != 
             _fileNameScalarFeatureVolumeToBundleTransform3d ) ||
           ( lowerBound != _lowerBound ) ||
           ( upperBound != _upperBound ) )
      {

        hasChanged = true;

      }
      _fileNameScalarFeatureVolume = fileNameScalarFeatureVolume;
      _fileNameScalarFeatureVolumeToBundleTransform3d = 
                                 fileNameScalarFeatureVolumeToBundleTransform3d;
      _lowerBound = lowerBound;
      _upperBound = upperBound;

      if ( hasChanged )
      {

        this->computeScalarFeatureVolumeBasedColors();

      }

    }

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapObject::setFiberColorEncoding( "
             "int32_t value )" );

}


bool gkg::AInteractiveBundleMapObject::render( anatomist::PrimList& primitiveList,
                                    const anatomist::ViewState& viewState )
{

  try
  {

    anatomist::GLComponent* glComponent = anatomist::GLComponent::glAPI();
    glComponent->glSetChanged( glBODY );

    return anatomist::AObject::render( primitiveList, viewState );

  }
  GKG_CATCH( "bool gkg::AFiberObject::render( "
             "anatomist::PrimList& primitiveList, "
             "const anatomist::ViewState& viewState )" );

}


bool gkg::AInteractiveBundleMapObject::glMakeBodyGLL(
                                       const anatomist::ViewState& /*viewState*/,
                                       const anatomist::GLList& glList ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // creating a new list
    ////////////////////////////////////////////////////////////////////////////

    glNewList( glList.item(), GL_COMPILE );

    glDisable(GL_LIGHTING);
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable( GL_COLOR_MATERIAL );


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
    // drawing fibers
    ////////////////////////////////////////////////////////////////////////////

    glLineWidth( _fiberLineWidth );

    glBegin( GL_LINES );

    gkg::Vector3d< float > color;
    gkg::BundleMap< std::string >::const_iterator
      b = _bundleMap->begin(),
      be = _bundleMap->end();
    std::vector< gkg::RGBComponent >::const_iterator
      c = _bundleColors->begin();
    std::vector< float >::const_iterator cPrime1;
    if ( _fiberColorEncoding == 3 )
    {

      cPrime1 = _scalarFeatureVolumeBasedColors->begin();

    }
    while ( b != be )
    {


      const gkg::RGBComponent& rgbComponent = *c;

      // in case of fiber colors corresponding to labels
      if ( _fiberColorEncoding == 0 )
      {

        red = ( float )rgbComponent.r / 255.0f;
        green = ( float )rgbComponent.g / 255.0f;
        blue = ( float )rgbComponent.b / 255.0f;

      }
      const gkg::BundleMap< std::string >::Bundle& bundle = b->second;

      std::vector< gkg::LightCurve3d< float > >::const_iterator
        f = bundle.begin(),
        fe = bundle.end();
      while ( f != fe )
      {

        const gkg::LightCurve3d< float >& fiber = *f;
        gkg::LightCurve3d< float >::const_iterator
          p1 = fiber.begin(),
          pe = fiber.end();
        gkg::LightCurve3d< float >::const_iterator p2 = p1;
        std::advance( p2, _fiberSubSamplingFactor );

        std::vector< float >::const_iterator cPrime2;
        if ( _fiberColorEncoding == 3 )
        {

           cPrime2 = cPrime1;
            std::advance( cPrime2, _fiberSubSamplingFactor );

        }

        while ( p2 != pe )
        {


          // in case fibers are colored using their local tangent
          if ( _fiberColorEncoding == 1 )
          {

            color = ( *p2 - *p1 ).absoluteCoordinates();
            color.normalize();
            red = color.x;
            green = color.y;
            blue = color.z;

          }
          else if ( _fiberColorEncoding == 3 )
          {

            red = *cPrime1;
            green = 0.0;
            blue = 1 - *cPrime1;

          }

          glColor4f( red, green, blue, opacity );
          glVertex3f( p1->x, p1->y, p1->z );

          if ( _fiberColorEncoding == 3 )
          {

            red = *cPrime2;
            green = 0.0;
            blue = 1 - *cPrime2;

          }
          glColor4f( red, green, blue, opacity );
          glVertex3f( p2->x, p2->y, p2->z );

          std::advance( p1, _fiberSubSamplingFactor );
          std::advance( p2, _fiberSubSamplingFactor );

          if ( _fiberColorEncoding == 3 )
          {

            std::advance( cPrime1, _fiberSubSamplingFactor );
            std::advance( cPrime2, _fiberSubSamplingFactor );

          }

        }

        ++ f;

      }

      ++ b;
      ++ c;

    }

    glEnd();


    ////////////////////////////////////////////////////////////////////////////
    // ending the list
    ////////////////////////////////////////////////////////////////////////////
    glEndList();

    return true;

  }
  GKG_CATCH( "bool gkg::AInteractiveBundleMapObject::glMakeBodyGLL( "
             "const anatomist::ViewState& viewState, "
             "const anatomist::GLList& glList ) const" );

}


////////////////////////////////////////////////////////////////////////////////
// static methods
////////////////////////////////////////////////////////////////////////////////

std::list< anatomist::AObject* > 
gkg::AInteractiveBundleMapObject::load(
                                    const std::string& fileName,
                                    anatomist::ObjectReader::PostRegisterList &,
                                    carto::Object /* options */ )
{

  try
  {

    std::string fileNameRoot = fileName.substr( 0, fileName.length() - 10U );
    std::string fileNameBundleMap = fileNameRoot + ".bundles";
    std::string fileNameHierarchy = fileNameRoot + ".hie";

    // reading bundle map
    gkg::RCPointer< gkg::BundleMap< std::string > >
      bundleMap( new gkg::BundleMap< std::string > );
    gkg::Reader::getInstance().read( fileNameBundleMap, *bundleMap );

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
    gkg::RandomColorGenerator randomColorGenerator( 0.85, 1.00 );

    // building bundle color map
    int32_t colorCount = bundleMap->getBundleCount();
    gkg::RCPointer< std::vector< RGBComponent > > 
      bundleColors( new std::vector< RGBComponent >( colorCount ) );
    gkg::BundleMap< std::string >::const_iterator
      b = bundleMap->begin(),
      be = bundleMap->end();
    std::vector< gkg::RGBComponent >::iterator c = bundleColors->begin();
    std::map< std::string, gkg::RGBComponent >::const_iterator color;
    while ( b != be )
    {

      if ( hierarchyFile.isExisting() )
      {

        color = colors.find( b->first );
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
      ++ b;
      ++ c;

    }

    // creating AInteractiveBundleMapObject object
    gkg::AInteractiveBundleMapObject*
      aBundleMapObject = new gkg::AInteractiveBundleMapObject( bundleMap,
                                                               bundleColors );
    std::list< anatomist::AObject* > objectList;
    objectList.push_back( aBundleMapObject );

    return objectList;

  }
  GKG_CATCH( "std::list< anatomist::AObject* > "
             "gkg::AInteractiveBundleMapObject::load( const std::string& fileName, "
             "anatomist::ObjectReader::PostRegisterList &, "
             "carto::Object /* options */ )" );

}


int32_t gkg::AInteractiveBundleMapObject::registerClass()
{

  try
  {

    // associating a new loader to the .clusters extension
    anatomist::ObjectReader::registerLoader(
                                       "iclusters",
                                       gkg::AInteractiveBundleMapObject::load );

    // creating a new class type
    _classType = registerObjectType( "INTERACTIVE_FIBER_CLUSTERS" );

    // loading the GKG plugin(s)
    gkg::PluginLoader::getInstance().load( false );

#ifdef GKG_DEBUG

    std::cout << "INTERACTIVE_FIBER_CLUSTERS loader registered : "
              << _classType << std::endl;

#endif

    return _classType;

  }
  GKG_CATCH( "int32_t gkg::AInteractiveBundleMapObject::registerClass()" );

}


int32_t gkg::AInteractiveBundleMapObject::getClassType()
{

  try
  {

    return _classType;

  }
  GKG_CATCH( "int32_t gkg::AInteractiveBundleMapObject::getClassType()" );

}


void gkg::AInteractiveBundleMapObject::computeScalarFeatureVolumeBasedColors()
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // reading the scalar feature volume
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< float > scalarFeatureVolume;
    gkg::TypedVolumeReaderProcess< float >
      typedVolumeReaderProcess( scalarFeatureVolume );
    typedVolumeReaderProcess.execute( _fileNameScalarFeatureVolume );

    gkg::Vector3d< double > resolution;
    scalarFeatureVolume.getResolution( resolution );

    ////////////////////////////////////////////////////////////////////////////
    // reading the 3D transformation
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::Transform3d< float > > transform3d;
    if ( !_fileNameScalarFeatureVolumeToBundleTransform3d.empty() )
    {

      try
      {

        gkg::RigidTransform3d< float >*
          rigidTransform3d = new gkg::RigidTransform3d< float >;
        std::ifstream 
          is( _fileNameScalarFeatureVolumeToBundleTransform3d.c_str() );
        rigidTransform3d->readTrm( is );
        is.close();
        transform3d.reset( rigidTransform3d );

      }
      catch ( std::exception& )
      {

        gkg::AffineTransform3d< float >*
          affineTransform3d = new gkg::AffineTransform3d< float >;
        std::ifstream 
          is( _fileNameScalarFeatureVolumeToBundleTransform3d.c_str() );
        affineTransform3d->readTrm( is );
        is.close();
        transform3d.reset( affineTransform3d );

      }

    }
    else
    {

      transform3d.reset( new gkg::IdentityTransform3d< float > );

    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating the color vector
    ////////////////////////////////////////////////////////////////////////////

    _scalarFeatureVolumeBasedColors.reset( new std::vector< float >(
                                           _bundleMap->getTotalPointCount() ) );


    ////////////////////////////////////////////////////////////////////////////
    // computing the color barycenters (between blue and red colors)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< float > pInScalarFeatureVolumeFrame;
    float value = 0.0f;
    gkg::BundleMap< std::string >::const_iterator
      b = _bundleMap->begin(),
      be = _bundleMap->end();
    std::vector< float >::iterator
      c = _scalarFeatureVolumeBasedColors->begin();
    while ( b != be )
    {

      const gkg::BundleMap< std::string >::Bundle& bundle = b->second;

      std::vector< gkg::LightCurve3d< float > >::const_iterator
        f = bundle.begin(),
        fe = bundle.end();
      while ( f != fe )
      {

        const gkg::LightCurve3d< float >& fiber = *f;
        gkg::LightCurve3d< float >::const_iterator
          p = fiber.begin(),
          pe = fiber.end();
        while ( p != pe )
        {

          transform3d->getInverse( *p, pInScalarFeatureVolumeFrame );
          gkg::PartialVolumingQuickResampler< float >::getInstance().resample(
            scalarFeatureVolume,
            0.0f,
            pInScalarFeatureVolumeFrame,
            value,
            &resolution );

          if ( value < _lowerBound )
          {

            *c = 0.0f;

          }
          else if ( value > _upperBound )
          {

            *c = 1.0f;

          }
          else
          {

            *c = ( value - _lowerBound ) / ( _upperBound - _lowerBound );

          }

          ++ p;
          ++ c;

        }

        ++ f;

      }

      ++ b;

    }

  }
  GKG_CATCH( "void gkg::AInteractiveBundleMapObject::"
             "computeScalarFeatureVolumeBasedColors()" );


}

