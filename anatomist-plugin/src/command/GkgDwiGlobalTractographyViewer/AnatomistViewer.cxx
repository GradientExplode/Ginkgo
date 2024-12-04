#include "AnatomistViewer.h"

#include <gkg-anatomist-plugin-server/AnatomistServer.h>
#include <gkg-core-exception/Exception.h>
#include <anatomist/commands/cWindowConfig.h>
#include <set>


gkg::AnatomistViewer::AnatomistViewer( int32_t windowWidth, 
                                       gkg::AnatomistViewer::Type type,
                                       const std::string& name,
                                       QWidget* parent )
                     : QWidget( parent ),
                       _windowWidth( windowWidth )
{

  try
  {

    _gridLayout = new QGridLayout();
    _gridLayout->setContentsMargins( 0, 0, 0, 0 );
    _gridLayout->setSpacing( 0 );

    std::string typeName;
    switch ( type )
    {

      case gkg::AnatomistViewer::Axial:

        typeName = "Axial";
        break;

      case gkg::AnatomistViewer::Sagittal:

        typeName = "Sagittal";
        break;

      case gkg::AnatomistViewer::Coronal:

        typeName = "Coronal";
        break;

      case gkg::AnatomistViewer::ThreeD:

        typeName = "3D";
        break;

    }

    QWidget* window = gkg::AnatomistServer::getInstance().createWindow(
                                                                 name,
                                                                 typeName,
                                                                 0,
                                                                 0,
                                                                 _windowWidth,
                                                                 _windowWidth );
    if ( window )
    {

      _gridLayout->addWidget( window, 0, 0 );

    }

  }
  GKG_CATCH( "gkg::AnatomistViewer::AnatomistViewer( "
             "int32_t windowWidth, "
             "gkg::AnatomistViewer::Type type, "
             "const std::string& name, "
             "QWidget* parent )" );

}


gkg::AnatomistViewer::~AnatomistViewer()
{
}


void gkg::AnatomistViewer::reset()
{

  try
  {

    gkg::AnatomistServer::getInstance().closeWindows();

  }
  GKG_CATCH( "void gkg::AnatomistViewer::reset()" );
}



void gkg::AnatomistViewer::addGlobalTractographyObject(
                     gkg::AGlobalTractographyObject* aGlobalTractographyObject )
{

  try
  {

    if ( anatomist::theProcessor && theAnatomist )
    {
  
      gkg::AnatomistServer::getInstance().addObject(
                                               aGlobalTractographyObject,
                                               "a_global_tractography_object" );

      std::set< std::string > objects;
      objects.insert( "a_global_tractography_object" );

      std::set< std::string > windows;

      windows.insert( "Spin glass and connections" );

      gkg::AnatomistServer::getInstance().addObjectToWindow( objects,
                                                             windows );
      
      anatomist::AObject* object =
        gkg::AnatomistServer::getInstance().getAnatomistObject(
                                               "a_global_tractography_object" );

      std::set< anatomist::AWindow* > sw;
      sw.insert( *( object->WinList().begin() ) );
      carto::Object p = carto::Object::value( carto::Dictionary() );
      p->setProperty( "cursor_visibility", 0 );
      anatomist::WindowConfigCommand
        *wc = new anatomist::WindowConfigCommand( sw, *p );
      anatomist::theProcessor->execute( wc );

    }

  }
  GKG_CATCH( "void gkg::AnatomistViewer::addGlobalTractographyObject( "
             "gkg::AGlobalTractographyObject* aGlobalTractographyObject )" );

}

