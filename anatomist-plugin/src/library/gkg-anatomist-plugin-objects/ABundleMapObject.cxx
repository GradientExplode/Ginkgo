#include <gkg-anatomist-plugin-objects/ABundleMapObject.h>
#include <gkg-anatomist-plugin-rendering-options/ABundleMapRenderingOptionsWidget.h>
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
#include <chrono>
#include <omp.h>


int32_t gkg::ABundleMapObject::_classType =
                                         gkg::ABundleMapObject::registerClass();
Tree* gkg::ABundleMapObject::_optionTree = 0;


gkg::ABundleMapObject::ABundleMapObject(
        const gkg::RCPointer< gkg::BundleMap< std::string > >& bundleMap,
        const gkg::RCPointer< std::vector< gkg::RGBComponent > >& bundleColors )
                      : _bundleMap( bundleMap ),
                        _bundleColors( bundleColors )
{

  try
  {
    
    _type = gkg::ABundleMapObject::_classType;
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
                                                    "icon-fiber-clusters.png" );
      if ( !QObjectTree::TypeIcons[ _classType ].load( iconPath.c_str() ) )
      {

        QObjectTree::TypeIcons.erase( _classType );
        std::cerr << "icon " << iconPath << " not found!" << std::endl;

      }
      QObjectTree::TypeNames[ _classType ] = "FIBER_CLUSTERS";

    } 


    std::set<std::string> mapLabels = _bundleMap->getLabels();
    //Set list of labels name
    setBundleLabels(mapLabels);

    std::set<std::string> labels= getBundleLabels();
    

    bool foundRight = false;
    bool foundLeft = false;
    classificationLabel = false;

    for (const std::string &label : mapLabels)
    {
        
        if (label.find("Right") == 0) {
            foundRight = true;
        } else if (label.find("Left") == 0) {
            foundLeft = true;
        }
        
        if (foundRight && foundLeft) {
          classificationLabel = true;
            break;
        }
    }
    
    if (classificationLabel)
    {
      rightLabels_states["Right Hemisphere"]= true;
      leftLabels_states["Left Hemisphere"] = true;
      interLabels_states["Inter Hemisphere"] = true;
      
      // Crée la map de label pour Right , left et inter
      for (const std::string& label : mapLabels) 
      {

            if (label.find("Right") != std::string::npos) 
            {

              rightLabels_states[label] = true;

            }
            else if (label.find("Left") != std::string::npos) 
            {

              leftLabels_states[label] = true;

            }
            else 
            {

              interLabels_states[label] = true;

            }
        }
    }
    else
    {
      everyLabels_states["Every Bundle Label"] = true;
      for (const std::string& label : mapLabels) 
      {
        everyLabels_states[label] = true;
      }
      
    }

  //Création d'une map de couleur en fonction des bundles
  _bundleColorMap = createBundleColorMap();


  }
  GKG_CATCH( "gkg::ABundleMapObject, ::ABundleMapObject( "
             "const gkg::RCPointer< gkg::BundleMap< std::string > >& "
             "bundleMap, "
             "const gkg::RCPointer< std::vector< gkg::RGBComponent > >& "
             "bundleColors )" );

}


std::map<std::string, gkg::RGBComponent>
                                 gkg::ABundleMapObject::createBundleColorMap()
{

    std::map<std::string, gkg::RGBComponent> bundleColorMap;

    if (_bundleMap == nullptr || _bundleColors == nullptr) 
    {
        return bundleColorMap; 
    }

    auto bundleIter = _bundleMap->begin();
    auto colorIter = _bundleColors->begin();

    while (bundleIter != _bundleMap->end() && colorIter != _bundleColors->end()) 
    {
        const std::string& bundleName = bundleIter->first;
        const gkg::RGBComponent& bundleColor = *colorIter;

        bundleColorMap[bundleName] = bundleColor;

        ++bundleIter;
        ++colorIter;
    }

    return bundleColorMap;
}

std::vector<gkg::showFibers> gkg::ABundleMapObject::processFibersShowVector()
                                                                           const
{
      std::vector<gkg::showFibers> visibleFibers;
      gkg::BundleMap<std::string>::const_iterator b = _bundleMap->begin();


      while (b != _bundleMap->end())
      {
        const gkg::BundleMap<std::string>::Bundle& bundle = b->second;
        std::vector<gkg::LightCurve3d<float>>::const_iterator f =
                                                              bundle.begin();
        std::vector<gkg::LightCurve3d<float>>::const_iterator fe =
                                                                  bundle.end();
        std::string bundlefirst = b->first;
        bool value = getBundleVisibility(bundlefirst);
        if (value)
        {
            while (f != fe)
            {
                const gkg::LightCurve3d<float>& fiber = *f;
                showFibers fiberAdd;
                fiberAdd.fiberPointer = &fiber; 
                fiberAdd.bundleLabel = bundlefirst;
                visibleFibers.push_back(fiberAdd);

                ++ f;
            }
        }
        ++b;
      }
  return visibleFibers;
}



std::vector<gkg::showFibers> gkg::ABundleMapObject::
                      processBundlesColorVisualisation(
                              std::vector<gkg::showFibers> visibleFibers) const
{
    // Parcourir toutes les fibres dans visibleFibers
    for (auto& fiber : visibleFibers)
    {
        const std::string& bundleLabel = fiber.bundleLabel;

        auto it = _bundleColorMap.find(bundleLabel);

        if (it != _bundleColorMap.end())
        {
            fiber.color = &(it->second);
        }
    }

    return visibleFibers;
}

gkg::Vector3d<float> gkg::ABundleMapObject::processScalarFeatureVolumeColor(
                                                             float cPrime) const
{
    // Calcul des composantes RGB en fonction de cPrime
    float red = cPrime;
    float green = 0.0;
    float blue = 1 - cPrime;

    // Création d'un vecteur de couleur RGB
    gkg::Vector3d<float> color(red, green, blue);

    // Retourner la couleur calculée
    return color;
}

gkg::Vector3d<float> gkg::ABundleMapObject::processLocalDirectionColor(
           const gkg::Vector3d<float>& p1, const gkg::Vector3d<float>& p2) const
{
    gkg::Vector3d<float> color = (p2 - p1).absoluteCoordinates();
    color.normalize();
    return color;
}

gkg::Vector3d<float> gkg::ABundleMapObject::processConstantMaterialColor() const
{
    const anatomist::Material& material = this->material();
    float red = ( float )material.Diffuse( 0 );
    float green = ( float )material.Diffuse( 1 );
    float blue = ( float )material.Diffuse( 2 );
    gkg::Vector3d<float> color(red,green,blue);
    return color;
}

void gkg::ABundleMapObject::streamizeVisualisation() const
{
  const anatomist::Material& material = this->material();
  float opacity_color = ( float )material.Diffuse( 3 );
  float red ;
  float green ;
  float blue ;
  auto fibersVector = processFibersShowVector();
  int nbThreads = omp_get_max_threads() - 1;

  //BundleColorVisualisation
  if(_fiberColorEncoding == 0)
  {
    fibersVector = processBundlesColorVisualisation(fibersVector);
// Loop through the vector of visible fibers and draw them in parallel
    #pragma omp parallel for schedule(static, 1) num_threads(nbThreads)
    for (size_t i = 0; i < fibersVector.size(); ++i)
    {
      const showFibers& fiberInfo = fibersVector[i ];
      const gkg::LightCurve3d<float>& fiber = *(fiberInfo.fiberPointer);

      auto p1 = fiber.begin();
      auto p2 = std::next(p1, _fiberSubSamplingFactor);

      while (p2 != fiber.end())
      {
        const gkg::RGBComponent& rgbComponent = *fiberInfo.color;
        red = (float)(rgbComponent.r) / 255.0f;
        green = (float)(rgbComponent.g) / 255.0f;
        blue = (float)(rgbComponent.b) / 255.0f;

        glColor4f(red, green, blue, opacity_color);
        

        glVertex3f(p1->x, p1->y, p1->z);
        glVertex3f(p2->x, p2->y, p2->z);

        std::advance(p1, _fiberSubSamplingFactor);
        std::advance(p2, _fiberSubSamplingFactor);
      }
    }    
  }

      //BundleColorVisualisation
    if(_fiberColorEncoding == 1)
    {
  // Loop through the vector of visible fibers and draw them in parallel
      #pragma omp parallel for schedule(static, 1) num_threads(nbThreads)
      for (size_t i = 0; i < fibersVector.size(); ++i)
      {
        const showFibers& fiberInfo = fibersVector[i];
        const gkg::LightCurve3d<float>& fiber = *(fiberInfo.fiberPointer);

        auto p1 = fiber.begin();
        auto p2 = std::next(p1, _fiberSubSamplingFactor);

        while (p2 != fiber.end())
        {
          gkg::Vector3d<float> color = processLocalDirectionColor(*p1, *p2);
          glColor4f(color.x, color.y, color.z, opacity_color);

          glVertex3f(p1->x, p1->y, p1->z);
          glVertex3f(p2->x, p2->y, p2->z);

          std::advance(p1, _fiberSubSamplingFactor);
          std::advance(p2, _fiberSubSamplingFactor);
        }
      }    
    }

      //BundleColorVisualisation
    if(_fiberColorEncoding == 2)
    {
      gkg::Vector3d<float> color = processConstantMaterialColor();
  // Loop through the vector of visible fibers and draw them in parallel
      #pragma omp parallel for schedule(static, 1) num_threads(nbThreads)
      for (size_t i = 0; i < fibersVector.size(); ++i)
      {
        const showFibers& fiberInfo = fibersVector[i];
        const gkg::LightCurve3d<float>& fiber = *(fiberInfo.fiberPointer);

        auto p1 = fiber.begin();
        auto p2 = std::next(p1, _fiberSubSamplingFactor);

        while (p2 != fiber.end())
        {
          glColor4f(color.x, color.y, color.z, opacity_color);

          glVertex3f(p1->x, p1->y, p1->z);
          glVertex3f(p2->x, p2->y, p2->z);

          std::advance(p1, _fiberSubSamplingFactor);
          std::advance(p2, _fiberSubSamplingFactor);
        }
      }    
    }

    if (_fiberColorEncoding == 3)
    {
      std::vector<float>::const_iterator cPrime1 =
                                       _scalarFeatureVolumeBasedColors->begin();

  // Boucle à travers le vecteur de fibres visibles et les dessiner en parallèle
      #pragma omp parallel for schedule(static, 1) num_threads(nbThreads)
      for (size_t i = 0; i < fibersVector.size(); ++i)
      {
          const showFibers& fiberInfo = fibersVector[i];
          const gkg::LightCurve3d<float>& fiber = *(fiberInfo.fiberPointer);

          auto p1 = fiber.begin();
          auto p2 = std::next(p1, _fiberSubSamplingFactor);

          while (p2 != fiber.end())
          {
      // Calculer la couleur basée sur le volume des caractéristiques scalaires
              gkg::Vector3d<float> color = 
                                      processScalarFeatureVolumeColor(*cPrime1);

              // Définir la couleur pour le rendu OpenGL
              glColor4f(color.x, color.y, color.z, opacity_color);


              glVertex3f(p1->x, p1->y, p1->z);
              glVertex3f(p2->x, p2->y, p2->z);

              std::advance(p1, _fiberSubSamplingFactor);
              std::advance(p2, _fiberSubSamplingFactor);

            // Avancez à l'élément suivant dans _scalarFeatureVolumeBasedColors
              std::advance(cPrime1, _fiberSubSamplingFactor);
          }
      }
    }     
}
gkg::ABundleMapObject::~ABundleMapObject()
{

}


bool gkg::ABundleMapObject::Is2DObject()
{

  try
  {

    return true;

  }
  GKG_CATCH( "bool gkg::ABundleMapObject::Is2DObject()" );

}  

bool gkg::ABundleMapObject::Is3DObject()
{

  try
  {

    return true;

  }
  GKG_CATCH( "bool gkg::ABundleMapObject::Is3DObject()" );

}


Tree* gkg::ABundleMapObject::optionTree() const
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
      t2->setProperty( "callback",
                       &gkg::ABundleMapRenderingOptionsWidget::create );
      t->insert( t2 );

    }
    return( _optionTree );

  }
  GKG_CATCH( "Tree* gkg::ABundleMapObject::optionTree() const" );

}


float gkg::ABundleMapObject::MinX2D() const
{

  try
  {

    return _boundingBox.getLowerX();

  }
  GKG_CATCH( "float gkg::ABundleMapObject::MinX2D() const" );

}


float gkg::ABundleMapObject::MinY2D() const
{

  try
  {

    return _boundingBox.getLowerY();

  }
  GKG_CATCH( "float gkg::ABundleMapObject::MinY2D() const" );

}


float gkg::ABundleMapObject::MinZ2D() const
{

  try
  {

    return _boundingBox.getLowerZ();

  }
  GKG_CATCH( "float gkg::ABundleMapObject::MinZ2D() const" );

}


float gkg::ABundleMapObject::MaxX2D() const
{

  try
  {

    return _boundingBox.getUpperX();

  }
  GKG_CATCH( "float gkg::ABundleMapObject::MaxX2D() const" );

}


float gkg::ABundleMapObject::MaxY2D() const
{

  try
  {

    return _boundingBox.getUpperY();

  }
  GKG_CATCH( "float gkg::ABundleMapObject::MaxY2D() const" );

}


float gkg::ABundleMapObject::MaxZ2D() const
{

  try
  {

    return _boundingBox.getUpperZ();

  }
  GKG_CATCH( "float gkg::ABundleMapObject::MaxZ2D() const" );

}


bool gkg::ABundleMapObject::boundingBox( std::vector< float > & bmin,
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
  GKG_CATCH( "bool gkg::ABundleMapObject::boundingBox( "
             "std::vector< float > & bmin, "
             "std::vector< float > & bmax ) const" );

}


int32_t gkg::ABundleMapObject::getFiberSubSamplingFactor() const
{

  try
  {

    return _fiberSubSamplingFactor;

  }
  GKG_CATCH( "int32_t "
             "gkg::ABundleMapObject::getFiberSubSamplingFactor() const" );

}


void 
gkg::ABundleMapObject::setFiberSubSamplingFactor( int32_t value )
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
             "gkg::ABundleMapObject::setFiberSubSamplingFactor( "
             "int32_t value )" );

}


float gkg::ABundleMapObject::getFiberLineWidth() const
{

  try
  {

    return _fiberLineWidth;

  }
  GKG_CATCH( "float "
             "gkg::ABundleMapObject::getFiberLineWidth() const" );

}


void 
gkg::ABundleMapObject::setFiberLineWidth( float value )
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
             "gkg::ABundleMapObject::setFiberLineWidth( "
             "float value )" );

}


int32_t gkg::ABundleMapObject::getFiberColorEncoding() const
{

  try
  {

    return _fiberColorEncoding;

  }
  GKG_CATCH( "int32_t "
             "gkg::ABundleMapObject::getFiberColorEncoding() const" );

}


const std::string& gkg::ABundleMapObject::getFileNameScalarFeatureVolume() const
{

  try
  {

    return _fileNameScalarFeatureVolume;

  }
  GKG_CATCH( "const std::string& "
             "gkg::ABundleMapObject::getFileNameScalarFeatureVolume() const" );

}


const std::string& 
gkg::ABundleMapObject::getFileNameScalarFeatureVolumeToBundleTransform3d() const
{

  try
  {

    return _fileNameScalarFeatureVolumeToBundleTransform3d;

  }
  GKG_CATCH( "const std::string& "
             "gkg::ABundleMapObject::"
             "getFileNameScalarFeatureVolumeToBundleTransform3d() const" );

}


double gkg::ABundleMapObject::getLowerBound() const
{

  try
  {

    return _lowerBound;

  }
  GKG_CATCH( "double gkg::ABundleMapObject::getLowerBound() const" );

}


double gkg::ABundleMapObject::getUpperBound() const
{

  try
  {

    return _upperBound;

  }
  GKG_CATCH( "double gkg::ABundleMapObject::getUpperBound() const" );

}

// Implémentation du setter pour la liste de labels
void gkg::ABundleMapObject::setBundleLabels
                                      (const std::set<std::string>& newLabels) {
 
    bundleLabels = newLabels;

}

// Implémentation du getter pour la liste de labels
const std::set<std::string>& gkg::ABundleMapObject::getBundleLabels() const {
    return bundleLabels;
}


void gkg::ABundleMapObject::setFiberColorEncoding(
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
             "gkg::ABundleMapObject::setFiberColorEncoding( "
             "int32_t value )" );

}


bool gkg::ABundleMapObject::render( anatomist::PrimList& primitiveList,
                                    const anatomist::ViewState& viewState )
{

  try{

    anatomist::GLComponent* glComponent = anatomist::GLComponent::glAPI();
    glComponent->glSetChanged( glBODY );

    return anatomist::AObject::render( primitiveList, viewState );

  }
  GKG_CATCH( "bool gkg::AFiberObject::render( "
             "anatomist::PrimList& primitiveList, "
             "const anatomist::ViewState& viewState )" );

}


bool gkg::ABundleMapObject::glMakeBodyGLL(
    const anatomist::ViewState& /*viewState*/,
    const anatomist::GLList& glList) const
{
    try
    {
        ////////////////////////////////////////////////////////////////////////
        // creating a new list
        ////////////////////////////////////////////////////////////////////////

        glNewList(glList.item(), GL_COMPILE);

        glDisable(GL_LIGHTING);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);

    

        ////////////////////////////////////////////////////////////////////////
        // drawing fibers
        ////////////////////////////////////////////////////////////////////////

        
        glLineWidth(_fiberLineWidth);


        glBegin(GL_LINES);

        streamizeVisualisation();

        glEnd();

        ////////////////////////////////////////////////////////////////////////
        // ending the list
        ////////////////////////////////////////////////////////////////////////
        glEndList();



        return true;
    }
    GKG_CATCH("bool gkg::ABundleMapObject::glMakeBodyGLL( "
              "const anatomist::ViewState& viewState, "
              "const anatomist::GLList& glList ) const");
}


////////////////////////////////////////////////////////////////////////////////
// static methods
////////////////////////////////////////////////////////////////////////////////

std::list< anatomist::AObject* > 
gkg::ABundleMapObject::load( const std::string& fileName,
                             anatomist::ObjectReader::PostRegisterList &,
                             carto::Object /* options */ )
{

  try
  {

    std::string fileNameRoot = fileName.substr( 0, fileName.length() - 9U );
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
//    int32_t colorIndex = 0;
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

//          *c = randomColorGenerator.getRandomRainbowColor( colorIndex,
//                                                           colorCount );
        *c = randomColorGenerator.getRandomColor();

        }

      }
      else
      {

//        *c = randomColorGenerator.getRandomRainbowColor( colorIndex,
//                                                         colorCount );
        *c = randomColorGenerator.getRandomColor();

      }
      ++ b;
      ++ c;
//      ++ colorIndex;

    }

    // creating ABundleMapObject object
    gkg::ABundleMapObject* aBundleMapObject = new gkg::ABundleMapObject(
                                                      bundleMap,
                                                      bundleColors );
    std::list< anatomist::AObject* > objectList;
    objectList.push_back( aBundleMapObject );

    return objectList;

  }
  GKG_CATCH( "std::list< anatomist::AObject* > "
             "gkg::ABundleMapObject::load( const std::string& fileName, "
             "anatomist::ObjectReader::PostRegisterList &, "
             "carto::Object /* options */ )" );

}


int32_t gkg::ABundleMapObject::registerClass()
{

  try
  {

    // associating a new loader to the .clusters extension
    anatomist::ObjectReader::registerLoader( "clusters",
                                             gkg::ABundleMapObject::load );

    // creating a new class type
    _classType = registerObjectType( "FIBER_CLUSTERS" );

    // loading the GKG plugin(s)
    gkg::PluginLoader::getInstance().load( false );

#ifdef GKG_DEBUG

    std::cout << "FIBER_CLUSTERS loader registered : "
              << _classType << std::endl;

#endif

    return _classType;

  }
  GKG_CATCH( "int32_t gkg::ABundleMapObject::registerClass()" );

}


int32_t gkg::ABundleMapObject::getClassType()
{

  try
  {

    return _classType;

  }
  GKG_CATCH( "int32_t gkg::ABundleMapObject::getClassType()" );

}


void gkg::ABundleMapObject::computeScalarFeatureVolumeBasedColors()
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
  GKG_CATCH( "void gkg::ABundleMapObject::"
             "computeScalarFeatureVolumeBasedColors()" );


}



void gkg::ABundleMapObject::setSplitLabels(std::map<std::string, 
                            bool>& newRightLabels,
                            std::map<std::string, bool>& newLeftLabels,
                            std::map<std::string, bool>& newInterLabels,
                            std::map<std::string, bool>& newEveryLabels) 
{

    rightLabels_states = newRightLabels;
    leftLabels_states = newLeftLabels;
    interLabels_states = newInterLabels;
    everyLabels_states = newEveryLabels;

}
// Getter pour rightLabels
std::map<std::string, bool>&  gkg::ABundleMapObject::getRightLabels_states()
{

    return rightLabels_states;

}

// Getter pour leftLabels
std::map<std::string, bool>&  gkg::ABundleMapObject::getLeftLabels_states()
{

    return leftLabels_states;

}

// Getter pour otherLabels
std::map<std::string, bool>&  gkg::ABundleMapObject::getInterLabels_states()
{

    return interLabels_states;

}

std::map<std::string, bool>&  gkg::ABundleMapObject::getEveryLabels_states()
{

    return everyLabels_states;

}
bool  gkg::ABundleMapObject::getClassificationLabel_states()
{

    return classificationLabel;

}

void gkg::ABundleMapObject::printCheckBoxStates() 
{

    // Affichage des résultats en utilisant les getters
    std::cout << "Right Labels:" << std::endl;
    for (const auto& pair : getRightLabels_states()) 
    {
  
      std::cout << pair.first << " (" << (pair.second ? "true" : "false")
                                                            << ")" << std::endl;
  
    }

    std::cout << "\nLeft Labels:" << std::endl;
    for (const auto& pair : getLeftLabels_states()) 
    {
  
      std::cout << pair.first << " (" << (pair.second ? "true" : "false") 
                                                            << ")" << std::endl;
  
    }

    std::cout << "\nOther Labels:" << std::endl;
    for (const auto& pair : getInterLabels_states()) 
    {
  
        std::cout << pair.first << " (" << (pair.second ? "true" : "false") 
                                                            << ")" << std::endl;
  
    }
}

bool gkg::ABundleMapObject::getBundleVisibility(std::string Key) const
{ 
  if(classificationLabel)
  {
    if (Key.find("Right") != std::string::npos) 
    {

      return rightLabels_states[Key];
    
    } 
    else if (Key.find("Left") != std::string::npos) 
    {
    
      return leftLabels_states[Key];
    
    }
    else 
    {
    
      return interLabels_states[Key];
    
    }
  }
  else
  {
    return everyLabels_states[Key];
  }
}


