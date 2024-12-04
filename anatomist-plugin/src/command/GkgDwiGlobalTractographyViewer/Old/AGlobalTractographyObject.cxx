#include "AGlobalTractographyObject.h"
#include <gkg-dmri-global-tractography/AnatomicalPriors.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-io/StringConverter.h>
#include <anatomist/object/actions.h>
#include <gkg-anatomist-plugin-server/AnatomistServer.h>



int32_t gkg::AGlobalTractographyObject::_classType = 
                                gkg::AGlobalTractographyObject::registerClass();
Tree* gkg::AGlobalTractographyObject::_optionTree = 0;


gkg::AGlobalTractographyObject::AGlobalTractographyObject( 
         const gkg::RCPointer< gkg::SpinGlassAndConnectionMap >& 
                                                     spinGlassAndConnectionMap,
         const gkg::RCPointer< gkg::GlobalTractographyOptimizationInformation >&
                                     globalTractographyOptimizationInformation )
                               : anatomist::SliceableObject(),
                                 _spinGlassAndConnectionMap(
                                                    spinGlassAndConnectionMap ),
                                 _globalTractographyOptimizationInformation(
                                     globalTractographyOptimizationInformation )
{

  try
  {

    // adding icon
    _type = _classType;
    _referential = 0;

    /*anatomist::AObjectPalette* palette = new anatomist::AObjectPalette( 
                           theAnatomist->palettes().find( "Rainbow1-fusion" ) );
    _palette = new AimsData< AimsRGBA >( palette->colors()->clone() );

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

    _material.SetDiffuseA( 0.01f * float( opacity ) );*/

  }
  GKG_CATCH( "gkg::AGlobalTractographyObject::AGlobalTractographyObject( "
             "const gkg::RCPointer< gkg::SpinGlassAndConnectionMap >& "
             "spinGlassAndConnectionMap )" );

}


gkg::AGlobalTractographyObject::~AGlobalTractographyObject()
{

  try
  {

    cleanup();

  }
  GKG_CATCH( "gkg::AGlobalTractographyObject::~AGlobalTractographyObject()" );

}


bool gkg::AGlobalTractographyObject::Is2DObject()
{

  try
  {

    return true;

  }
  GKG_CATCH( "bool gkg::AGlobalTractographyObject::Is2DObject()" );

}


bool gkg::AGlobalTractographyObject::Is3DObject()
{

  try
  {

    return true;

  }
  GKG_CATCH( "bool gkg::AGlobalTractographyObject::Is3DObject()" );

}


Tree* gkg::AGlobalTractographyObject::optionTree() const
{

  try
  {

    if ( !_optionTree )
    {

      _optionTree = new Tree( true, "option tree" );

      Tree* t;
      Tree* t2;

      // color menu
      Tree* tree = new Tree( true, "Color" );
      _optionTree->insert( tree );

      Tree* tree2 = new Tree( true, "Spin glass material" );
      tree2->setProperty( "callback",
                          &anatomist::ObjectActions::colorMaterial );
      tree->insert( tree2 );

      tree2 = new Tree( true, "Spin glass connection material" );
      tree2->setProperty( "callback",
                          &anatomist::ObjectActions::colorMaterial );
      tree->insert( tree2 );

      // referential menu
      tree = new Tree( true, "Referential" );
      _optionTree->insert( tree );
      tree2 = new Tree( true, "Load" );
      tree2->setProperty( "callback",
                          &anatomist::ObjectActions::referentialLoad );
      tree->insert( tree2 );

    }

    return _optionTree;

  }
  GKG_CATCH( "Tree* gkg::AGlobalTractographyObject::optionTree() const" );

}


float gkg::AGlobalTractographyObject::MinX2D() const
{

  try
  {

    if ( _spinGlassAndConnectionMap->getAnatomicalPriors().isNull() )
    {

      return 0.0;

    }
    return _spinGlassAndConnectionMap->getAnatomicalPriors()->getLowerX();

  }
  GKG_CATCH( "float gkg::AGlobalTractographyObject::MinX2D() const" );

}



float gkg::AGlobalTractographyObject::MinY2D() const
{

  try
  {

    if ( _spinGlassAndConnectionMap->getAnatomicalPriors().isNull() )
    {

      return 0.0;

    }
    return _spinGlassAndConnectionMap->getAnatomicalPriors()->getLowerY();

  }
  GKG_CATCH( "float gkg::AGlobalTractographyObject::MinY2D() const" );

}



float gkg::AGlobalTractographyObject::MinZ2D() const
{

  try
  {

    if ( _spinGlassAndConnectionMap->getAnatomicalPriors().isNull() )
    {

      return 0.0;

    }
    return _spinGlassAndConnectionMap->getAnatomicalPriors()->getLowerZ();

  }
  GKG_CATCH( "float gkg::AGlobalTractographyObject::MinZ2D() const" );

}


float gkg::AGlobalTractographyObject::MaxX2D() const
{

  try
  {

    if ( _spinGlassAndConnectionMap->getAnatomicalPriors().isNull() )
    {

      return 0.0;

    }
    return _spinGlassAndConnectionMap->getAnatomicalPriors()->getUpperX();

  }
  GKG_CATCH( "float gkg::AGlobalTractographyObject::MaxX2D() const" );

}



float gkg::AGlobalTractographyObject::MaxY2D() const
{

  try
  {

    if ( _spinGlassAndConnectionMap->getAnatomicalPriors().isNull() )
    {

      return 0.0;

    }
    return _spinGlassAndConnectionMap->getAnatomicalPriors()->getUpperY();

  }
  GKG_CATCH( "float gkg::AGlobalTractographyObject::MaxY2D() const" );

}



float gkg::AGlobalTractographyObject::MaxZ2D() const
{

  try
  {

    if ( _spinGlassAndConnectionMap->getAnatomicalPriors().isNull() )
    {

      return 0.0;

    }
    return _spinGlassAndConnectionMap->getAnatomicalPriors()->getUpperZ();

  }
  GKG_CATCH( "float gkg::AGlobalTractographyObject::MaxZ2D() const" );

}


bool gkg::AGlobalTractographyObject::boundingBox( Point3df& lowerCorner,
                                                  Point3df& upperCorner ) const
{

  try
  {

    if ( _spinGlassAndConnectionMap->getAnatomicalPriors().isNull() )
    {

      lowerCorner[ 0 ] = 0.0;
      lowerCorner[ 1 ] = 0.0;
      lowerCorner[ 2 ] = 0.0;

      upperCorner[ 0 ] = 0.0;
      upperCorner[ 1 ] = 0.0;
      upperCorner[ 2 ] = 0.0;

    }
    else
    {

      lowerCorner[ 0 ] =
                 _spinGlassAndConnectionMap->getAnatomicalPriors()->getLowerX();
      lowerCorner[ 1 ] =
                 _spinGlassAndConnectionMap->getAnatomicalPriors()->getLowerY();
      lowerCorner[ 2 ] =
                 _spinGlassAndConnectionMap->getAnatomicalPriors()->getLowerZ();

      upperCorner[ 0 ] =
                 _spinGlassAndConnectionMap->getAnatomicalPriors()->getUpperX();
      upperCorner[ 1 ] =
                 _spinGlassAndConnectionMap->getAnatomicalPriors()->getUpperY();
      upperCorner[ 2 ] =
                 _spinGlassAndConnectionMap->getAnatomicalPriors()->getUpperZ();

    }

    if ( lowerCorner[ 0 ] == upperCorner[ 0 ] )
    {

      upperCorner[ 0 ] = lowerCorner[ 0 ] + 1;

    }
    if ( lowerCorner[ 1 ] == upperCorner[ 1 ] )
    {

      upperCorner[ 1 ] = lowerCorner[ 1 ] + 1;

    }
    if ( lowerCorner[ 2 ] == upperCorner[ 2 ] )
    {

      upperCorner[ 2 ] = lowerCorner[ 2 ] + 1;

    }

    return true;

  }
  GKG_CATCH( "bool gkg::AGlobalTractographyObject::boundingBox( "
             "Point3df& lowerCorner,"
             "Point3df& upperCorner ) const" );

}


bool gkg::AGlobalTractographyObject::glMakeBodyGLL(
                                         const anatomist::ViewState& viewState,
                                         const anatomist::GLList& glList ) const
{

  try
  {

    std::cout << "start of "
              << "gkg::AGlobalTractographyObject::"
              << "glMakeBodyGLL()"
              << std::endl;

    ////////////////////////////////////////////////////////////////////////////
    // creating a new list
    ////////////////////////////////////////////////////////////////////////////
    glNewList( glList.item(), GL_COMPILE );

    glDisable(GL_LIGHTING);
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable( GL_COLOR_MATERIAL );

    ////////////////////////////////////////////////////////////////////////////
    // drawing spin glasses
    ////////////////////////////////////////////////////////////////////////////
    glBegin( GL_LINES );

    int32_t validSpinGlassCount =
                           _spinGlassAndConnectionMap->getValidSpinGlassCount();

    if ( validSpinGlassCount )
    {

      std::list< gkg::RCPointer< gkg::SpinGlass > > spinGlasses;
      _spinGlassAndConnectionMap->getValidSpinGlasses( spinGlasses );

      std::list< gkg::RCPointer< gkg::SpinGlass > >::const_iterator
        s = spinGlasses.begin();
      uint32_t i = 0;

      for ( i = 0; i < validSpinGlassCount; i++, s++ )
      {

        const gkg::Vector3d< float >& color = ( *s )->getOrientation();

        // setting the backward extremity of the spin glass
        const gkg::Vector3d< float >& backwardExtremity = 
                     ( *s )->getExtremity( gkg::SpinGlass::Backward );

        glColor3f( std::fabs( color.x ),
                   std::fabs( color.y ),
                   std::fabs( color.z ) );
        glVertex3f( backwardExtremity.x,
                    backwardExtremity.y,
                    backwardExtremity.z );

        // setting the forward extremity of the spin glass
        const gkg::Vector3d< float >& forwardExtremity = 
                     ( *s )->getExtremity( gkg::SpinGlass::Forward );

        glColor3f( std::fabs( color.x ),
                   std::fabs( color.y ),
                   std::fabs( color.z ) );
        glVertex3f( forwardExtremity.x,
                    forwardExtremity.y,
                    forwardExtremity.z );

      }

    }

    glEnd();


    ////////////////////////////////////////////////////////////////////////////
    // drawing spin glass connections
    ////////////////////////////////////////////////////////////////////////////
    glBegin( GL_LINES );

    int32_t validSpinGlassConnectionCount = _spinGlassAndConnectionMap->
                                             getValidSpinGlassConnectionCount();

    if ( validSpinGlassConnectionCount )
    {

      std::list< gkg::RCPointer< gkg::SpinGlassConnection > >
                                                           spinGlassConnections;
      _spinGlassAndConnectionMap->getValidSpinGlassConnections(
                                                         spinGlassConnections );

      std::list< gkg::RCPointer< gkg::SpinGlassConnection > >::const_iterator
        c = spinGlassConnections.begin();
      uint32_t i = 0;
      for ( i = 0; i < validSpinGlassConnectionCount; i++, c++ )
      {

        // setting the first extremity of the spin glass connection
        const gkg::Vector3d< float >& firstExtremity = 
                                                    ( *c )->getFirstExtremity();

        glColor3f( 0.2, 0.2, 0.2 );
        glVertex3f( firstExtremity.x,
                    firstExtremity.y,
                    firstExtremity.z );

        // setting the second extremity of the spin glass connection
        const gkg::Vector3d< float >& secondExtremity = 
                                                   ( *c )->getSecondExtremity();

        glColor3f( 0.2, 0.2, 0.2 );
        glVertex3f( secondExtremity.x,
                    secondExtremity.y,
                    secondExtremity.z );

      }

    }

    glEnd();

    ////////////////////////////////////////////////////////////////////////////
    // drawing bounding box
    ////////////////////////////////////////////////////////////////////////////
    glBegin( GL_LINES );

    glColor3f( 0.3, 0.3, 0.3 );

    gkg::Vector3d< float > position;
    gkg::AnatomistServer::WindowType windowType = gkg::AnatomistServer::Unknown;
    float clippingDistance = 0.0;

    gkg::AnatomistServer::getInstance().getViewStateInformation(
                                                             viewState,
                                                             position,
                                                             windowType,
                                                             clippingDistance );

    float minX = MinX2D();
    float maxX = MaxX2D();

    float minY = MinY2D();
    float maxY = MaxY2D();

    float minZ = MinZ2D();
    float maxZ = MaxZ2D();

    if ( windowType == gkg::AnatomistServer::Axial )
    {

      minZ = position.z - clippingDistance;
      maxZ = position.z + clippingDistance;

    }
    else if ( windowType == gkg::AnatomistServer::Sagittal )
    {

      minX = position.x - clippingDistance;
      maxX = position.x + clippingDistance;

    }
    else if ( windowType == gkg::AnatomistServer::Coronal )
    {

      minY = position.y - clippingDistance;
      maxY = position.y + clippingDistance;

    }

    glVertex3f( minX, minY, minZ );
    glVertex3f( maxX, minY, minZ );

    glVertex3f( minX, maxY, minZ );
    glVertex3f( maxX, maxY, minZ );

    glVertex3f( minX, minY, minZ );
    glVertex3f( minX, maxY, minZ );

    glVertex3f( maxX, minY, minZ );
    glVertex3f( maxX, maxY, minZ );

    glVertex3f( minX, minY, maxZ );
    glVertex3f( maxX, minY, maxZ );

    glVertex3f( minX, maxY, maxZ );
    glVertex3f( maxX, maxY, maxZ );

    glVertex3f( minX, minY, maxZ );
    glVertex3f( minX, maxY, maxZ );

    glVertex3f( maxX, minY, maxZ );
    glVertex3f( maxX, maxY, maxZ );

    glVertex3f( minX, minY, minZ );
    glVertex3f( minX, minY, maxZ );

    glVertex3f( minX, maxY, minZ );
    glVertex3f( minX, maxY, maxZ );

    glVertex3f( maxX, minY, minZ );
    glVertex3f( maxX, minY, maxZ );

    glVertex3f( maxX, maxY, minZ );
    glVertex3f( maxX, maxY, maxZ );

    glEnd();

    ////////////////////////////////////////////////////////////////////////////
    // drawing chosen voxel bounding box
    ////////////////////////////////////////////////////////////////////////////
    if ( !_globalTractographyOptimizationInformation->getChosenVoxels().empty()
         &&
         _globalTractographyOptimizationInformation->isLastChosenVoxelValid() )
    {

      glBegin( GL_LINES );

      glColor3f( 0.8, 0.4, 0.0 );

      windowType = gkg::AnatomistServer::Unknown;
      clippingDistance = 0.0;

      gkg::AnatomistServer::getInstance().getViewStateInformation(
                                                             viewState,
                                                             position,
                                                             windowType,
                                                             clippingDistance );

      gkg::Vector3d< float > lowerBoundary;
      gkg::Vector3d< float > upperBoundary;
      _spinGlassAndConnectionMap->getAnatomicalPriors()->getVoxelBoundaries(
               _globalTractographyOptimizationInformation->getLastChosenVoxel(),
               lowerBoundary,
               upperBoundary );

      minX = lowerBoundary.x;
      maxX = upperBoundary.x;

      minY = lowerBoundary.y;
      maxY = upperBoundary.y;

      minZ = lowerBoundary.z;
      maxZ = upperBoundary.z;

      if ( windowType == gkg::AnatomistServer::Axial )
      {

        minZ = position.z - clippingDistance;
        maxZ = position.z + clippingDistance;

      }
      else if ( windowType == gkg::AnatomistServer::Sagittal )
      {

        minX = position.x - clippingDistance;
        maxX = position.x + clippingDistance;

      }
      else if ( windowType == gkg::AnatomistServer::Coronal )
      {

        minY = position.y - clippingDistance;
        maxY = position.y + clippingDistance;

      }

      glVertex3f( minX, minY, minZ );
      glVertex3f( maxX, minY, minZ );

      glVertex3f( minX, maxY, minZ );
      glVertex3f( maxX, maxY, minZ );

      glVertex3f( minX, minY, minZ );
      glVertex3f( minX, maxY, minZ );

      glVertex3f( maxX, minY, minZ );
      glVertex3f( maxX, maxY, minZ );

      glVertex3f( minX, minY, maxZ );
      glVertex3f( maxX, minY, maxZ );

      glVertex3f( minX, maxY, maxZ );
      glVertex3f( maxX, maxY, maxZ );

      glVertex3f( minX, minY, maxZ );
      glVertex3f( minX, maxY, maxZ );

      glVertex3f( maxX, minY, maxZ );
      glVertex3f( maxX, maxY, maxZ );

      glVertex3f( minX, minY, minZ );
      glVertex3f( minX, minY, maxZ );

      glVertex3f( minX, maxY, minZ );
      glVertex3f( minX, maxY, maxZ );

      glVertex3f( maxX, minY, minZ );
      glVertex3f( maxX, minY, maxZ );

      glVertex3f( maxX, maxY, minZ );
      glVertex3f( maxX, maxY, maxZ );

      glEnd();

    }

    ////////////////////////////////////////////////////////////////////////////
    // ending the list
    ////////////////////////////////////////////////////////////////////////////
    glEndList();

    std::cout << "end of "
              << "gkg::AGlobalTractographyObject::"
              << "glMakeBodyGLL()"
              << std::endl;

    return true;

  }
  GKG_CATCH( "bool gkg::AGlobalTractographyObject::glMakeBodyGLL( "
             "const anatomist::ViewState& viewState, "
             "const anatomist::GLList& glList ) const" );

}


int32_t gkg::AGlobalTractographyObject::registerClass()
{

  try
  {

    _classType = registerObjectType( "GlobalTractography" );

    std::cout << "GlobalTractography registered : " << _classType << std::endl;

    return _classType;

  }
  GKG_CATCH( "int32_t gkg::AGlobalTractographyObject::registerClass()" );

}


int32_t gkg::AGlobalTractographyObject::getClassType()
{

  try
  {

    return _classType;

  }
  GKG_CATCH( "int32_t gkg::AGlobalTractographyObject::getClassType()" );

}


