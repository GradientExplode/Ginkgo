#include "AnatomistViewer.h"

#include <gkg-anatomist-plugin-server/AnatomistServer.h>
#include <gkg-core-exception/Exception.h>
#include <set>


gkg::AnatomistViewer::AnatomistViewer( int32_t windowWidth,
                                       QWidget* parent )
                     : QWidget( parent ),
                       _windowWidth( windowWidth ),
                       _grid1( false ),
                       _grid4( false )
{

  try
  {

    _grid4Layout[ 0 ][ 0 ] = 0;
    _grid4Layout[ 0 ][ 1 ] = 0;
    _grid4Layout[ 1 ][ 0 ] = 0;
    _grid4Layout[ 1 ][ 1 ] = 0;

    _grid2Layout = new QGridLayout();
    _grid2Layout->setContentsMargins( 0, 0, 0, 0 );
    _grid2Layout->setSpacing( 0 );

  }
  GKG_CATCH( "gkg::AnatomistViewer::AnatomistViewer( "
             "int32_t windowWidth, "
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

    _grid1 = false;
    _grid4 = false;

    _grid4Layout[ 0 ][ 0 ] = 0;
    _grid4Layout[ 0 ][ 1 ] = 0;
    _grid4Layout[ 1 ][ 0 ] = 0;
    _grid4Layout[ 1 ][ 1 ] = 0;

  }
  GKG_CATCH( "void gkg::AnatomistViewer::reset()" );
}



void gkg::AnatomistViewer::createWindow( gkg::AnatomistViewer::Type type,
                                         uint32_t division,
                                         uint32_t row,
                                         uint32_t column,
                                         const std::string& name )
{

  try
  {

    if ( ( column < division ) && ( row < division ) )
    {

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

      // control through layouts on the Anatomist windows' size
      if ( window )
      {

        if ( ( division == 1 ) && !_grid1 && !_grid4 )
        {
 
          _grid2Layout->addWidget( window, 0, 0 );
          _grid1 = true;

        }
        else if ( ( division == 2 ) && !_grid1 )
        {

          _grid2Layout->addWidget( window, row, column );

        }
        else if ( ( division == 4 ) && !_grid1 )
        {

          if ( !_grid4Layout[ row / 2 ][ column / 2 ] )
          {

            QGridLayout* layout = new QGridLayout();
            _grid4Layout[ row / 2 ][ column / 2 ] = layout;
            _grid2Layout->addLayout( layout, row / 2, column / 2 );
            _grid4 = true;

          }

          _grid4Layout[ row / 2 ][ column / 2 ]->addWidget( window,
                                                            row % 2,
                                                            column % 2 );

        }

      }

    }

  }
  GKG_CATCH( "void gkg::AnatomistViewer::createWindow( "
             "gkg::AnatomistViewer::Type type, "
             "uint32_t division, "
             "uint32_t row, "
             "uint32_t column, "
             "const std::string& name )" );

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

      if ( _grid1 )
      {

        windows.insert( "Spin glass and connections" );

      }
      else
      {

        windows.insert( "Spin glass and connections - Coronal" );
        windows.insert( "Spin glass and connections - Sagittal" );
        windows.insert( "Spin glass and connections - Axial" );
        windows.insert( "Spin glass and connections - 3D" );

      }

      gkg::AnatomistServer::getInstance().addObjectToWindow( objects,
                                                             windows );

    }

  }
  GKG_CATCH( "void gkg::AnatomistViewer::addGlobalTractographyObject( "
             "const gkg::RCPointer< gkg::AGlobalTractographyObject >& "
             "aGlobalTractographyObject )" );

}

