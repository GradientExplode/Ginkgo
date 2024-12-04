#include "AGlobalTractographyObject.h"
#include <gkg-core-exception/Exception.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-cppext/CUniStd.h>
#include <anatomist/object/actions.h>
#include <gkg-anatomist-plugin-server/AnatomistServer.h>
#include <gkg-processing-transform/Rotation3dFunctions.h>
#include <gkg-processing-algobase/Math.h>



int32_t gkg::AGlobalTractographyObject::_classType = 
                                gkg::AGlobalTractographyObject::registerClass();
Tree* gkg::AGlobalTractographyObject::_optionTree = 0;


gkg::AGlobalTractographyObject::AGlobalTractographyObject( 
         const gkg::RCPointer< gkg::SpinGlassAndConnectionMap >& 
                                                     spinGlassAndConnectionMap )
                               : anatomist::SliceableObject(),
                                 _spinGlassAndConnectionMap(
                                                     spinGlassAndConnectionMap )
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

  cleanup();

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

    return _spinGlassAndConnectionMap->getLowerX();

  }
  GKG_CATCH( "float gkg::AGlobalTractographyObject::MinX2D() const" );

}


float gkg::AGlobalTractographyObject::MinY2D() const
{

  try
  {

    return _spinGlassAndConnectionMap->getLowerY();

  }
  GKG_CATCH( "float gkg::AGlobalTractographyObject::MinY2D() const" );

}


float gkg::AGlobalTractographyObject::MinZ2D() const
{

  try
  {

    return _spinGlassAndConnectionMap->getLowerZ();

  }
  GKG_CATCH( "float gkg::AGlobalTractographyObject::MinZ2D() const" );

}


float gkg::AGlobalTractographyObject::MaxX2D() const
{

  try
  {

    return _spinGlassAndConnectionMap->getUpperX();

  }
  GKG_CATCH( "float gkg::AGlobalTractographyObject::MaxX2D() const" );

}


float gkg::AGlobalTractographyObject::MaxY2D() const
{

  try
  {

    return _spinGlassAndConnectionMap->getUpperY();

  }
  GKG_CATCH( "float gkg::AGlobalTractographyObject::MaxY2D() const" );

}


float gkg::AGlobalTractographyObject::MaxZ2D() const
{

  try
  {

    return _spinGlassAndConnectionMap->getUpperZ();

  }
  GKG_CATCH( "float gkg::AGlobalTractographyObject::MaxZ2D() const" );

}


bool gkg::AGlobalTractographyObject::boundingBox( Point3df& lowerCorner,
                                                  Point3df& upperCorner ) const
{

  try
  {

    lowerCorner[ 0 ] = _spinGlassAndConnectionMap->getLowerX();
    lowerCorner[ 1 ] = _spinGlassAndConnectionMap->getLowerY();
    lowerCorner[ 2 ] = _spinGlassAndConnectionMap->getLowerZ();

    upperCorner[ 0 ] = _spinGlassAndConnectionMap->getUpperX();
    upperCorner[ 1 ] = _spinGlassAndConnectionMap->getUpperY();
    upperCorner[ 2 ] = _spinGlassAndConnectionMap->getUpperZ();

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

      gkg::Vector3d< float > secondAxis;
      gkg::Vector3d< float > thirdAxis;

      std::list< gkg::RCPointer< gkg::SpinGlass > > spinGlasses;
      _spinGlassAndConnectionMap->getValidSpinGlasses( spinGlasses );

      std::list< gkg::RCPointer< gkg::SpinGlass > >::const_iterator
        s = spinGlasses.begin();
      int32_t i = 0;

      for ( i = 0; i < validSpinGlassCount; i++, s++ )
      {

        const gkg::Vector3d< float >& orientation = ( *s )->getOrientation();

        gkg::getRandomOrthonormalTriederFromFirstAxis( orientation,
                                                       secondAxis,
                                                       thirdAxis );

        float length = ( *s )->getLength();

        // setting the backward extremity of the spin glass
        const gkg::Vector3d< float >&
          backwardExtremity = ( *s )->getExtremity( gkg::SpinGlass::Backward );

        glColor3f( std::fabs( orientation.x ),
                   std::fabs( orientation.y ),
                   std::fabs( orientation.z ) );
        glVertex3f( backwardExtremity.x,
                    backwardExtremity.y,
                    backwardExtremity.z );

        // setting the forward extremity of the spin glass
        const gkg::Vector3d< float >&
          forwardExtremity = ( *s )->getExtremity( gkg::SpinGlass::Forward );

        glColor3f( std::fabs( orientation.x ),
                   std::fabs( orientation.y ),
                   std::fabs( orientation.z ) );
        glVertex3f( forwardExtremity.x,
                    forwardExtremity.y,
                    forwardExtremity.z );

        // creating an arrow to represent the orientation of the spin glass
        glColor3f( std::fabs( orientation.x ),
                   std::fabs( orientation.y ),
                   std::fabs( orientation.z ) );
        glVertex3f( forwardExtremity.x,
                    forwardExtremity.y,
                    forwardExtremity.z );

        glColor3f( std::fabs( orientation.x ),
                   std::fabs( orientation.y ),
                   std::fabs( orientation.z ) );
        glVertex3f( forwardExtremity.x - 
                    ( orientation.x - secondAxis.x ) * 0.2 * length,
                    forwardExtremity.y - 
                    ( orientation.y - secondAxis.y ) * 0.2 * length,
                    forwardExtremity.z - 
                    ( orientation.z - secondAxis.z ) * 0.2 * length );

        glColor3f( std::fabs( orientation.x ),
                   std::fabs( orientation.y ),
                   std::fabs( orientation.z ) );
        glVertex3f( forwardExtremity.x,
                    forwardExtremity.y,
                    forwardExtremity.z );

        glColor3f( std::fabs( orientation.x ),
                   std::fabs( orientation.y ),
                   std::fabs( orientation.z ) );
        glVertex3f( forwardExtremity.x - 
                    ( orientation.x + secondAxis.x ) * 0.2 * length,
                    forwardExtremity.y - 
                    ( orientation.y + secondAxis.y ) * 0.2 * length,
                    forwardExtremity.z - 
                    ( orientation.z + secondAxis.z ) * 0.2 * length );

      }

    }

    glEnd();


    ////////////////////////////////////////////////////////////////////////////
    // drawing spin glass connections
    ////////////////////////////////////////////////////////////////////////////
    glLineWidth( 3 );
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
      int32_t i = 0;
      for ( i = 0; i < validSpinGlassConnectionCount; i++, c++ )
      {

        // setting the first extremity of the spin glass connection
        const gkg::Vector3d< float >& firstExtremity = 
                                                    ( *c )->getFirstExtremity();
        //const gkg::RCPointer< gkg::SpinGlass >& firstSpinGlass =
        //                                              ( *c )->getSpinGlass1();
        const gkg::SpinGlass::ExtremityType& firstExtremityType =
                                           ( *c )->getSpinGlassExtremityType1();

        if ( firstExtremityType == gkg::SpinGlass::Forward )
        {

          glColor3f( 0.2, 0.2, 0.2 );
          glVertex3f( firstExtremity.x/* -
                      0.1 * firstSpinGlass->getOrientation().x*/,
                      firstExtremity.y/* -
                      0.1 * firstSpinGlass->getOrientation().y*/,
                      firstExtremity.z/* -
                      0.1 * firstSpinGlass->getOrientation().z*/ );

        }
        else
        {

          glColor3f( 0.2, 0.2, 0.2 );
          glVertex3f( firstExtremity.x/* +
                      0.1 * firstSpinGlass->getOrientation().x*/,
                      firstExtremity.y/* +
                      0.1 * firstSpinGlass->getOrientation().y*/,
                      firstExtremity.z/* +
                      0.1 * firstSpinGlass->getOrientation().z*/ );

        }

        // setting the second extremity of the spin glass connection
        const gkg::Vector3d< float >& secondExtremity = 
                                                   ( *c )->getSecondExtremity();
        //const gkg::RCPointer< gkg::SpinGlass >& secondSpinGlass =
        //                                              ( *c )->getSpinGlass2();
        const gkg::SpinGlass::ExtremityType& secondExtremityType =
                                           ( *c )->getSpinGlassExtremityType2();

        if ( secondExtremityType == gkg::SpinGlass::Forward )
        {

          glColor3f( 0.2, 0.2, 0.2 );
          glVertex3f( secondExtremity.x/* -
                      0.1 * secondSpinGlass->getOrientation().x*/,
                      secondExtremity.y/* -
                      0.1 * secondSpinGlass->getOrientation().y*/,
                      secondExtremity.z/* -
                      0.1 * secondSpinGlass->getOrientation().z*/ );

        }
        else
        {

          glColor3f( 0.2, 0.2, 0.2 );
          glVertex3f( secondExtremity.x/* +
                      0.1 * secondSpinGlass->getOrientation().x*/,
                      secondExtremity.y/* +
                      0.1 * secondSpinGlass->getOrientation().y*/,
                      secondExtremity.z/* +
                      0.1 * secondSpinGlass->getOrientation().z*/ );
        }

      }

    }

    glEnd();
    glLineWidth( 1 );

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

    const gkg::Vector3d< double >& roiResolution =
                                 _spinGlassAndConnectionMap->getRoiResolution();

    float minX = MinX2D();
    float maxX = MaxX2D();// - ( float )roiResolution.x;

    float minY = MinY2D();
    float maxY = MaxY2D();// - ( float )roiResolution.y;

    float minZ = MinZ2D();
    float maxZ = MaxZ2D();// - ( float )roiResolution.z;

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
    // drawing the current voxel and highlightening the chosen spin glass
    ////////////////////////////////////////////////////////////////////////////

    // highlightening the chosen spin glass
    if ( validSpinGlassCount && !_spinGlassAndConnectionMap.isNull() )
    {

      const gkg::RCPointer< gkg::SpinGlass >&
        currentSpinGlass = _spinGlassAndConnectionMap->getCurrentSpinGlass();

      if ( !currentSpinGlass.isNull() )
      {

        glLineWidth( 3 );
        glBegin( GL_LINES );

        const gkg::Vector3d< float >&
          currentSpinGlassPosition = currentSpinGlass->getPosition();
        const gkg::Vector3d< float >&
          currentSpinGlassOrientation = currentSpinGlass->getOrientation();
        float currentSpinGlassLength = currentSpinGlass->getLength();

        gkg::Vector3d< float > secondAxis;
        gkg::Vector3d< float > thirdAxis;
        gkg::getRandomOrthonormalTriederFromFirstAxis(
                                                    currentSpinGlassOrientation,
                                                    secondAxis,
                                                    thirdAxis );

        // setting the backward extremity of the current spin glass
        const gkg::Vector3d< float >&
          currentSpinGlassBackwardExtremity =
                     currentSpinGlass->getExtremity( gkg::SpinGlass::Backward );

        glColor3f( std::fabs( currentSpinGlassOrientation.x ),
                   std::fabs( currentSpinGlassOrientation.y ),
                   std::fabs( currentSpinGlassOrientation.z ) );
        glVertex3f( currentSpinGlassBackwardExtremity.x,
                    currentSpinGlassBackwardExtremity.y,
                    currentSpinGlassBackwardExtremity.z );

        // setting the forward extremity of the current spin glass
        const gkg::Vector3d< float >&
          currentSpinGlassForwardExtremity =
                      currentSpinGlass->getExtremity( gkg::SpinGlass::Forward );

        glColor3f( std::fabs( currentSpinGlassOrientation.x ),
                   std::fabs( currentSpinGlassOrientation.y ),
                   std::fabs( currentSpinGlassOrientation.z ) );
        glVertex3f( currentSpinGlassForwardExtremity.x,
                    currentSpinGlassForwardExtremity.y,
                    currentSpinGlassForwardExtremity.z );

        // creating an arrow to represent the orientation of the current spin
        // glass
        glColor3f( std::fabs( currentSpinGlassOrientation.x ),
                   std::fabs( currentSpinGlassOrientation.y ),
                   std::fabs( currentSpinGlassOrientation.z ) );
        glVertex3f( currentSpinGlassForwardExtremity.x,
                    currentSpinGlassForwardExtremity.y,
                    currentSpinGlassForwardExtremity.z );

        glColor3f( std::fabs( currentSpinGlassOrientation.x ),
                   std::fabs( currentSpinGlassOrientation.y ),
                   std::fabs( currentSpinGlassOrientation.z ) );
        glVertex3f( currentSpinGlassForwardExtremity.x - 
                    ( currentSpinGlassOrientation.x - secondAxis.x ) *
                    0.2 * currentSpinGlassLength,
                    currentSpinGlassForwardExtremity.y - 
                    ( currentSpinGlassOrientation.y - secondAxis.y ) *
                    0.2 * currentSpinGlassLength,
                    currentSpinGlassForwardExtremity.z - 
                    ( currentSpinGlassOrientation.z - secondAxis.z ) *
                    0.2 * currentSpinGlassLength );

        glColor3f( std::fabs( currentSpinGlassOrientation.x ),
                   std::fabs( currentSpinGlassOrientation.y ),
                   std::fabs( currentSpinGlassOrientation.z ) );
        glVertex3f( currentSpinGlassForwardExtremity.x,
                    currentSpinGlassForwardExtremity.y,
                    currentSpinGlassForwardExtremity.z );

        glColor3f( std::fabs( currentSpinGlassOrientation.x ),
                   std::fabs( currentSpinGlassOrientation.y ),
                   std::fabs( currentSpinGlassOrientation.z ) );
        glVertex3f( currentSpinGlassForwardExtremity.x - 
                    ( currentSpinGlassOrientation.x + secondAxis.x ) *
                    0.2 * currentSpinGlassLength,
                    currentSpinGlassForwardExtremity.y - 
                    ( currentSpinGlassOrientation.y + secondAxis.y ) *
                    0.2 * currentSpinGlassLength,
                    currentSpinGlassForwardExtremity.z - 
                    ( currentSpinGlassOrientation.z + secondAxis.z ) *
                    0.2 * currentSpinGlassLength );

        glEnd();

        // drawing the corresponding voxel
        glBegin( GL_LINES );
        glColor3f( 0.0, 0.0, 0.8 );

        windowType = gkg::AnatomistServer::Unknown;
        clippingDistance = 0.0;

        gkg::AnatomistServer::getInstance().getViewStateInformation(
                                                             viewState,
                                                             position,
                                                             windowType,
                                                             clippingDistance );

        // computing the current voxel according to the current spin glass
        gkg::Vector3d< int32_t > currentVoxel;
        currentVoxel.x = gkg::round( currentSpinGlassPosition.x /
                                     roiResolution.x );
        currentVoxel.y = gkg::round( currentSpinGlassPosition.y /
                                     roiResolution.y );
        currentVoxel.z = gkg::round( currentSpinGlassPosition.z /
                                     roiResolution.z );

        gkg::Vector3d< float > lowerBoundary;
        gkg::Vector3d< float > upperBoundary;

        // setting the lower and upper boundaries of the voxel bounding box
        lowerBoundary.x = currentVoxel.x * ( float )roiResolution.x -
                          ( float )( roiResolution.x / 2.0 );
        lowerBoundary.y = currentVoxel.y * ( float )roiResolution.y -
                          ( float )( roiResolution.y / 2.0 );
        lowerBoundary.z = currentVoxel.z * ( float )roiResolution.z -
                          ( float )( roiResolution.z / 2.0 );

        upperBoundary.x = lowerBoundary.x + ( float )roiResolution.x;
        upperBoundary.y = lowerBoundary.y + ( float )roiResolution.y;
        upperBoundary.z = lowerBoundary.z + ( float )roiResolution.z;

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

    }


    ////////////////////////////////////////////////////////////////////////////
    // ending the list
    ////////////////////////////////////////////////////////////////////////////
    glEndList();

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


