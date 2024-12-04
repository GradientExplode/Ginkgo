#include <gkg-anatomist-plugin-server/AnatomistServer.h>
#include <gkg-anatomist-plugin-control/MouseEvent.h>
#include <gkg-anatomist-plugin-control/MouseAction.h>
#include <gkg-anatomist-plugin-control/MouseControl.h>
#include <gkg-core-exception/Exception.h>

#include <anatomist/application/Anatomist.h>
#include <anatomist/commands/cCreateWindow.h>
#include <anatomist/commands/cCloseWindow.h>
#include <anatomist/commands/cDeleteAll.h>
#include <anatomist/commands/cRemoveObject.h>
#include <anatomist/commands/cAddObject.h>
#include <anatomist/commands/cCamera.h>
#include <anatomist/commands/cSetObjectPalette.h>
#include <anatomist/commands/cFusionObjects.h>
#include <anatomist/commands/cFusion2DParams.h>
#include <anatomist/commands/cLoadTransformation.h>
#include <anatomist/commands/cAssignReferential.h>
#include <anatomist/commands/cSetControl.h>
#include <anatomist/window3D/window3D.h>
#include <anatomist/mobject/Fusion2D.h>
#include <anatomist/controler/controldictionary.h>
#include <anatomist/controler/controlmanager.h>
#include <anatomist/controler/actiondictionary.h>
#include <anatomist/window/controlledWindow.h>
#include <anatomist/config/version.h>

#include <vector>
#include <set>


gkg::AnatomistServer::AnatomistServer()
                    : gkg::Singleton< gkg::AnatomistServer >()
{

  try
  {

    std::vector< const char * > argv;
    argv.push_back( "anatomist" );
    argv.push_back( "-b" );
    int32_t argc = argv.size();

    new anatomist::Anatomist( argc, &argv[ 0 ], "GKG Anatomist Plugin" );

    if ( theAnatomist )
    {

      theAnatomist->initialize();

    }

  }
  GKG_CATCH( "gkg::AnatomistServer::AnatomistServer() "
             ": gkg::Singleton< gkg::AnatomistServer >()" );

}


gkg::AnatomistServer::~AnatomistServer()
{
}


QWidget* gkg::AnatomistServer::createWindow(
                                           const std::string& name,
                                           const std::string& type, 
                                           int32_t x0, 
                                           int32_t y0,
                                           int32_t width,
                                           int32_t height,
                                           gkg::AnatomistServer::Option option )
{

  try
  {

    if ( theAnatomist )
    {

      carto::Object anatomistOption = carto::Object::value(
                                                          carto::Dictionary() );

      if ( option & gkg::AnatomistServer::NoDecoration )
      {

        anatomistOption->setProperty( "no_decoration", 
                                      carto::Object::value( 1 ) );

      }

      if ( option & gkg::AnatomistServer::NoBorder )
      {

        anatomistOption->setProperty( "wflags", 
                                      carto::Object::value( 
                                               (int)Qt::FramelessWindowHint ) );

      }

      std::vector< int32_t > geometry;
      geometry.push_back( x0 );
      geometry.push_back( y0 );
      geometry.push_back( width );
      geometry.push_back( height );
    
      anatomist::CreateWindowCommand* command =
              new anatomist::CreateWindowCommand( type, -1, 0, geometry,
                                                  0, 0, 0, 0, anatomistOption );
      _processor.execute( command );

      QAWindow* window = dynamic_cast< QAWindow* >( command->createdWindow() );

      if ( window )
      {

        _windows.insert( std::make_pair( name, window ) );

        return window;

      }

    }

    return NULL;

  }
  GKG_CATCH( "QWidget* gkg::AnatomistServer::createWindow( "
             "QWidget* parent, "
             "const std::string& name, "
             "const std::string& type, "
             "int32_t x0, "
             "int32_t y0, "
             "int32_t width, "
             "int32_t height, "
             "gkg::AnatomistServer::Option option )" );

}


void gkg::AnatomistServer::muteAxial( const std::string& WindowName )
{

  try
  {

    std::map< std::string, QAWindow* >::iterator
      w = _windows.find( WindowName );

    if ( w != _windows.end() )
    {

      AWindow3D* aWindow3D = dynamic_cast< AWindow3D* >( w->second );

      if ( aWindow3D )
      {

        aWindow3D->muteAxial();

      }

    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::muteAxial( "
             "const std::string& WindowName )" );

}


void gkg::AnatomistServer::muteSagittal( const std::string& WindowName )
{

  try
  {

    std::map< std::string, QAWindow* >::iterator
      w = _windows.find( WindowName );

    if ( w != _windows.end() )
    {

      AWindow3D* aWindow3D = dynamic_cast< AWindow3D* >( w->second );

      if ( aWindow3D )
      {

        aWindow3D->muteSagittal();

      }

    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::muteSagittal( "
             "const std::string& WindowName )" );

}


void gkg::AnatomistServer::muteCoronal( const std::string& WindowName )
{

  try
  {

    std::map< std::string, QAWindow* >::iterator
      w = _windows.find( WindowName );

    if ( w != _windows.end() )
    {

      AWindow3D* aWindow3D = dynamic_cast< AWindow3D* >( w->second );

      if ( aWindow3D )
      {

        aWindow3D->muteCoronal();

      }

    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::muteCoronal( "
             "const std::string& WindowName )" );

}


void gkg::AnatomistServer::mute3D( const std::string& WindowName )
{

  try
  {

    std::map< std::string, QAWindow* >::iterator
      w = _windows.find( WindowName );

    if ( w != _windows.end() )
    {

      AWindow3D* aWindow3D = dynamic_cast< AWindow3D* >( w->second );

      if ( aWindow3D )
      {

        aWindow3D->mute3D();

      }

    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::mute3D( "
             "const std::string& WindowName )" );

}


void gkg::AnatomistServer::closeWindows()
{

  try
  {

    if ( theAnatomist && theAnatomist->getWindows().size() )
    {

      anatomist::CloseWindowCommand* command =
        new anatomist::CloseWindowCommand( theAnatomist->getWindows() );
      _processor.execute( command );
      _windows.clear();
 
    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::closeWindows()" );

}


void gkg::AnatomistServer::addObjectsToViews()
{

  try
  {

    if ( theAnatomist &&
         theAnatomist->getObjects().size() &&
         theAnatomist->getWindows().size() )
    {
 
      anatomist::AddObjectCommand* command =
        new anatomist::AddObjectCommand( theAnatomist->getObjects(),
                                         theAnatomist->getWindows() );
      _processor.execute( command );

    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::addObjectsToViews()" );

}


void gkg::AnatomistServer::updateViews( bool adjustPalette )
{

  try
  {

    if ( theAnatomist && theAnatomist->getObjects().size() )
    {

      std::set< anatomist::AObject* > objects = theAnatomist->getObjects();
      std::set< anatomist::AObject* >::iterator o  = objects.begin(),
                                                oe = objects.end();

      while ( o != oe )
      {

        if ( *o && theAnatomist->hasObject( *o ) )
        {

          anatomist::GLComponent* glComponent = (*o)->glAPI();

          if ( glComponent )
          {

            glComponent->glSetTexImageChanged( true, 0 );
            glComponent->glSetChanged( anatomist::GLComponent::glBODY, true );

          }
          else
          {

            (*o)->setChanged();

          }

          if ( (*o)->type() == anatomist::AObject::VOLUME )
          {

            (*o)->SetExtrema();

            if ( adjustPalette )
            {

              (*o)->adjustPalette();

            }

          }

          (*o)->notifyObservers( *o );

        }

        ++o;

      }

    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::updateViews( bool adjustPalette )" );

}


void gkg::AnatomistServer::removeAllObjects()
{

  try
  {

    if ( theAnatomist && theAnatomist->getObjects().size() )
    {

      size_t n = 0;
      std::set< anatomist::AObject* > objects;
      std::set< anatomist::AObject* >::iterator o, oe;
 
      do
      {
 
        n = 0;
        objects = theAnatomist->getObjects();
        o = objects.begin();
        oe = objects.end();
 
        while ( o != oe )
        {
 
          if ( theAnatomist->hasObject( *o ) && (*o)->CanBeDestroyed() )
          {

            n += theAnatomist->destroyObject( *o );

          }
 
          ++o;
 
        }
 
      }
      while ( n && !objects.empty() );

      theAnatomist->Refresh();
      _objects.clear();

    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::removeAllObjects()" );

}


void gkg::AnatomistServer::removeAllObjectsAndWindows()
{

  try
  {

    if ( theAnatomist )
    {

      anatomist::DeleteAllCommand* command = new anatomist::DeleteAllCommand;
      _processor.execute( command );
      _objects.clear();
      _windows.clear();

    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::removeAllObjectsAndWindows()" );

}


void gkg::AnatomistServer::adjustCamera( float posX,
                                         float posY,
                                         float posZ,
                                         float posT )
{

  try
  {

    if ( theAnatomist && theAnatomist->getWindows().size() )
    {

      Point4df curpos( posX, posY, posZ, posT );
      anatomist::CameraCommand* command =
          new anatomist::CameraCommand( theAnatomist->getWindows(), 
                                        0, 0, 0,  0, false, &curpos );
      _processor.execute( command );

    }

  }
  GKG_CATCH( "gkg::AnatomistServer::adjustCamera( "
             "float posX, float posY, float posZ, float posT" );

}


void gkg::AnatomistServer::setPalette( const std::string name,
                                       bool min1flg, float min1,
                                       bool max1flg, float max1,
                                       bool absmode )
{

  try
  {

    if ( theAnatomist )
    {

      setPalette( theAnatomist->getObjects(), name, min1flg, min1,
                  max1flg, max1, absmode );

    }

  }
  GKG_CATCH( "gkg::AnatomistServer::setPalette( "
             "const std::string name, "
             "bool min1flg, float min1, "
             "bool max1flg, float max1, bool absmode )" );

}


void gkg::AnatomistServer::setPalette( std::set< std::string > obj,
                                       const std::string name,
                                       bool min1flg, float min1,
                                       bool max1flg, float max1,
                                       bool absmode )
{

  try
  {

    std::set< anatomist::AObject* > setObj;
    std::set< std::string >::iterator
      o = obj.begin(),
      oe = obj.end();

    while ( o != oe )
    {

      std::map< std::string, anatomist::AObject* >::iterator
        ao = _objects.find( *o );

      if ( ao != _objects.end() )
      {

        setObj.insert( ao->second );

      }

      ++o;

    }

    setPalette( setObj, name, min1flg, min1, max1flg, max1, absmode );

  }
  GKG_CATCH( "void gkg::AnatomistServer::setPalette( "
             "std::set< std::string >& obj, "
             "const std::string name, "
             "bool min1flg, float min1, "
             "bool max1flg, float max1, bool absmode )" );

}


void gkg::AnatomistServer::setPalette( std::set< anatomist::AObject* > obj,
                                       const std::string name,
                                       bool min1flg, float min1,
                                       bool max1flg, float max1,
                                       bool absmode )
{

  try
  {

    if ( theAnatomist )
    {

      anatomist::SetObjectPaletteCommand* command =
                          new anatomist::SetObjectPaletteCommand( obj, name,
                                                                  min1flg, min1,
                                                                  max1flg, max1,
                                                                  "",
                                                                  false, 0,
                                                                  false, 1,
                                                                  "",
                                                                  false,
                                                                  0.5,
                                                                  "",
                                                                  absmode );
      _processor.execute( command );

    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::setPalette( "
             "std::set< anatomist::AObject* >& obj, "
             "const std::string name, "
             "bool min1flg, float min1, "
             "bool max1flg, float max1, bool absmode )" );

}


bool gkg::AnatomistServer::addObjectToWindow( std::set< std::string > obj,
                                              std::set< std::string > win )
{

  try
  {

    if ( obj.empty() || win.empty() || _objects.empty() || _windows.empty() )
    {

      return false;

    }

    std::set< anatomist::AObject* > setObj;
    std::set< std::string >::iterator
      o = obj.begin(),
      oe = obj.end();

    while ( o != oe )
    {

      std::map< std::string, anatomist::AObject* >::iterator
        ao = _objects.find( *o );

      if ( ao != _objects.end() )
      {

        setObj.insert( ao->second );

      }

      ++o;

    }

    std::set< anatomist::AWindow* > setWin;
    std::set< std::string >::iterator
      w = win.begin(),
      we = win.end();

    while ( w != we )
    {

      std::map< std::string, QAWindow* >::iterator
        aw = _windows.find( *w );

      if ( aw != _windows.end() )
      {

        setWin.insert( aw->second );

      }

      ++w;

    }

    anatomist::AddObjectCommand* command =
                              new anatomist::AddObjectCommand( setObj, setWin );
    _processor.execute( command );

    return true;

  }
  GKG_CATCH( "bool gkg::AnatomistServer::addObjectToWindow( "
             "std::set< std::string > obj, std::set< std::string > win" );
}


void gkg::AnatomistServer::addObject( anatomist::AObject* aObject,
                                      const std::string& name )
{

  try
  {

    if ( !aObject )
    {
    
      throw std::runtime_error( "object pointer is null" );
    
    }
  
    if ( name.empty() )
    {
    
      throw std::runtime_error( "object name must be specified" );
    
    }
  
    if ( theAnatomist && aObject )
    {
  
      _objects.insert( std::make_pair( name, aObject ) );

      if ( aObject->type() == anatomist::AObject::VOLUME )
      {

        aObject->SetExtrema();
        aObject->adjustPalette();

      }

      theAnatomist->registerObject( aObject, 0 );

    }
  
  }
  GKG_CATCH( "void gkg::AnatomistServer::addObject( "
             "anatomist::AObject* aObject, "
             "const std::string& name )" );

}


void gkg::AnatomistServer::removeObject( const std::string& name )
{

  try
  {
  
    if ( name.empty() )
    {
    
      throw std::runtime_error( "object name must be specified" );
    
    }

    if ( theAnatomist )
    {

      std::map< std::string, anatomist::AObject* >::iterator
        o = _objects.find( name );

      if ( o != _objects.end() )
      {

        theAnatomist->unregisterObject( o->second );
        _objects.erase( o );

      }

    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::removeObject( "
             "const std::string& name )" );

}


void gkg::AnatomistServer::fusion2D( const std::string& vol1,
                                     const std::string& vol2,
                                     const std::string& method,
                                     const std::string& name )
{

  try
  {

    if ( !vol1.empty() && !vol2.empty() && !method.empty() && !name.empty() )
    {

      std::map< std::string, anatomist::AObject* >::iterator
        o1 = _objects.find( vol1 ),
        o2 = _objects.find( vol2 ),
        oe = _objects.end();

      if ( ( o1 != oe ) && ( o2 != oe ) )
      {

        std::vector< anatomist::AObject* > volumesFusion;

        volumesFusion.push_back( o1->second );
        volumesFusion.push_back( o2->second );

        anatomist::FusionObjectsCommand* command =
                             new anatomist::FusionObjectsCommand( volumesFusion,
                                                                  method, 
                                                                  -1, 
                                                                  false );
        _processor.execute( command );

        anatomist::AObject* object = command->createdObject();

        if ( object )
        {

          _objects.insert( std::make_pair( name, object ) );

        }

      }

    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::fusion2D( "
             "const std::string& vol1, const std::string& vol2, "
             "const std::string& method, const std::string& name )" );

}


void gkg::AnatomistServer::fusion2DParams( const std::string& obj,
                                           float rate, 
                                           int32_t mode )
{

  try
  {

    std::map< std::string, anatomist::AObject* >::iterator
      o = _objects.find( obj );

    if ( o != _objects.end() )
    {

      anatomist::Fusion2D* fusion = dynamic_cast< anatomist::Fusion2D* >( 
                                                                    o->second );

      if ( fusion )
      {

        anatomist::Fusion2DParamsCommand* command =
                     new anatomist::Fusion2DParamsCommand( fusion, mode, rate );
        _processor.execute( command );

      }

    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::Fusion2DParams( "
             "const std::string& obj, float rate, int32_t mode" );

}


void gkg::AnatomistServer::createReferential( const std::string& name )
{

  try
  {

    if ( !name.empty() )
    {

      anatomist::Referential* referential = new anatomist::Referential();

      if ( referential )
      {

        _referentials.insert( std::make_pair( name, referential ) );

      }

    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::createReferential( "
             "const std::string& name )" );

}


void gkg::AnatomistServer::applyTransformation( std::set< std::string > oOrg,
                                                std::set< std::string > wOrg,
                                                const std::string& refOrg,
                                                std::set< std::string > oDst,
                                                std::set< std::string > wDst,
                                                const std::string& refDest,
                                                const float matrix[ 4 ][ 3 ] )
{

  try
  {

    // Info: matrix is the transformation matrix in this form:
    // [ tx  ty  tz  ]
    // [ r00 r01 r02 ]
    // [ r10 r11 r12 ]
    // [ r20 r21 r22 ]

    std::set< anatomist::AObject* > setObjOrg;
    std::set< anatomist::AObject* > setObjDst;
    std::set< std::string >::iterator
      o = oOrg.begin(),
      oe = oOrg.end();

    while ( o != oe )
    {

      std::map< std::string, anatomist::AObject* >::iterator
        ao = _objects.find( *o );

      if ( ao != _objects.end() )
      {

        setObjOrg.insert( ao->second );

      }

      ++o;

    }

    o = oDst.begin(),
    oe = oDst.end();

    while ( o != oe )
    {

      std::map< std::string, anatomist::AObject* >::iterator
        ao = _objects.find( *o );

      if ( ao != _objects.end() )
      {

        setObjDst.insert( ao->second );

      }

      ++o;

    }

    std::set< anatomist::AWindow* > setWinOrg;
    std::set< anatomist::AWindow* > setWinDst;
    std::set< std::string >::iterator
      w = wOrg.begin(),
      we = wOrg.end();

    while ( w != we )
    {

      std::map< std::string, QAWindow* >::iterator
        aw = _windows.find( *w );

      if ( aw != _windows.end() )
      {

        setWinOrg.insert( aw->second );

      }

      ++w;

    }

    w = wDst.begin(),
    we = wDst.end();

    while ( w != we )
    {

      std::map< std::string, QAWindow* >::iterator
        aw = _windows.find( *w );

      if ( aw != _windows.end() )
      {

        setWinDst.insert( aw->second );

      }

      ++w;

    }

    anatomist::Referential* referentialOrg = 0;
    anatomist::Referential* referentialDst = 0;
    std::map< std::string, anatomist::Referential* >::iterator
      rOrg = _referentials.find( refOrg ),
      rDst = _referentials.find( refDest ),
      re = _referentials.end();

    if ( rOrg != re )
    {

      referentialOrg = rOrg->second;

    }

    if ( rDst != re )
    {

      referentialDst = rDst->second;

    }

    applyTransformation( setObjOrg, setWinOrg, referentialOrg,
                         setObjDst, setWinDst, referentialDst,
                         matrix );

  }
  GKG_CATCH( "void gkg::AnatomistServer::applyTransformation( "
             "std::set< std::string > oOrg, "
             "std::set< std::string > wOrg, "
             "const std::string& refOrg, "
             "std::set< std::string > oDst, "
             "std::set< std::string > wDst, "
             "const std::string& refDest, "
             "const float matrix[ 4 ][ 3 ]" );

}


void gkg::AnatomistServer::applyTransformation( std::set< std::string > oOrg,
                                                const std::string& refOrg,
                                                std::set< std::string > oDst,
                                                const std::string& refDest,
                                                const float matrix[ 4 ][ 3 ] )
{

  try
  {

    std::set< anatomist::AObject* > setObjOrg;
    std::set< anatomist::AObject* > setObjDst;
    std::set< anatomist::AWindow* > setWinOrg;
    std::set< anatomist::AWindow* > setWinDst;
    std::set< std::string >::iterator
      o = oOrg.begin(),
      oe = oOrg.end();

    while ( o != oe )
    {

      std::map< std::string, anatomist::AObject* >::iterator
        ao = _objects.find( *o );

      if ( ao != _objects.end() )
      {

        const std::set< anatomist::AWindow* >& windows = ao->second->WinList();

        setObjOrg.insert( ao->second );
        setWinOrg.insert( windows.begin(), windows.end() );

      }

      ++o;

    }

    o = oDst.begin(),
    oe = oDst.end();

    while ( o != oe )
    {

      std::map< std::string, anatomist::AObject* >::iterator
        ao = _objects.find( *o );

      if ( ao != _objects.end() )
      {

        const std::set< anatomist::AWindow* >& windows = ao->second->WinList();

        setObjDst.insert( ao->second );
        setWinDst.insert( windows.begin(), windows.end() );

      }

      ++o;

    }

    anatomist::Referential* referentialOrg = 0;
    anatomist::Referential* referentialDst = 0;
    std::map< std::string, anatomist::Referential* >::iterator
      rOrg = _referentials.find( refOrg ),
      rDst = _referentials.find( refDest ),
      re = _referentials.end();

    if ( rOrg != re )
    {

      referentialOrg = rOrg->second;

    }

    if ( rDst != re )
    {

      referentialDst = rDst->second;

    }

    applyTransformation( setObjOrg, setWinOrg, referentialOrg,
                         setObjDst, setWinDst, referentialDst,
                         matrix );

  }
  GKG_CATCH( "void gkg::AnatomistServer::applyTransformation( "
             "std::set< std::string > oOrg, const std::string& refOrg, "
             "std::set< std::string > oDst, const std::string& refDest, "
             "const float matrix[ 4 ][ 3 ] )" );

}


void gkg::AnatomistServer::applyTransformation(
                                           std::set< anatomist::AObject* > oOrg,
                                           std::set< anatomist::AWindow* > wOrg,
                                           anatomist::Referential* refOrg,
                                           std::set< anatomist::AObject* > oDst,
                                           std::set< anatomist::AWindow* > wDst,
                                           anatomist::Referential* refDest,
                                           const float matrix[ 4 ][ 3 ] )
{

  try
  {

    // Info: matrix is the transformation matrix in this form:
    // [ tx  ty  tz  ]
    // [ r00 r01 r02 ]
    // [ r10 r11 r12 ]
    // [ r20 r21 r22 ]

    if ( theAnatomist )
    {

      anatomist::AssignReferentialCommand* cAssignRef1 =
        new anatomist::AssignReferentialCommand( refOrg, oOrg, wOrg );
      _processor.execute( cAssignRef1 );

      anatomist::AssignReferentialCommand* cAssignRef2 =
        new anatomist::AssignReferentialCommand( refDest, oDst, wDst );
      _processor.execute( cAssignRef2 );

      anatomist::LoadTransformationCommand* cLoadTransform =
        new anatomist::LoadTransformationCommand( matrix, refOrg, refDest );
      _processor.execute( cLoadTransform );

    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::applyTransformation( "
             "std::set< anatomist::AObject* > oOrg, "
             "std::set< anatomist::AWindow* > wOrg, "
             "anatomist::Referential* refOrg, "
             "std::set< anatomist::AObject* > oDst, "
             "std::set< anatomist::AWindow* > wDst, "
             "anatomist::Referential* refDest, "
             "const float matrix[ 4 ][ 3 ] )" );

}


void gkg::AnatomistServer::assignReferential( const std::string& object,
                                              const std::string& referential )

{

  try
  {

    std::map< std::string, anatomist::AObject* >::iterator
      o = _objects.find( object );

    if ( o != _objects.end() )
    {

      std::map< std::string, anatomist::Referential* >::iterator
        r = _referentials.find( referential );

      if ( r != _referentials.end() )
      {

        const std::set< anatomist::AWindow* >& windows = o->second->WinList();
        std::set< anatomist::AWindow* >::iterator
          w = windows.begin(),
          we = windows.end();

        while ( w != we )
        {

          (*w)->setReferential( r->second );
          ++w;

        }

      }

    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::assignReferential( "
             "const std::string& object, const std::string& referential )" );

}


void gkg::AnatomistServer::unregisterReferentials()
{

  try
  {

    std::map< std::string, anatomist::Referential* >::iterator
      r = _referentials.begin(),
      re = _referentials.end();

    while ( r != re )
    {

      delete r->second;
      ++r;

    }

    _referentials.clear();

  }
  GKG_CATCH( "void gkg::AnatomistServer::unregisterReferentials()" );

}


anatomist::AObject* gkg::AnatomistServer::getAnatomistObject( 
                                                 const std::string& name ) const
{

  try
  {

    if ( !name.empty() )
    {

      std::map< std::string, anatomist::AObject* >::const_iterator
        o = _objects.find( name );

      if ( o != _objects.end() )
      {

        return o->second;

      }

    }

    return 0;

  }
  GKG_CATCH( "anatomist::AObject* gkg::AnatomistServer::getAnatomistObject( "
             "const std::string& name ) const" );

}


bool gkg::AnatomistServer::hasObject( const std::string& name ) const
{

  try
  {

    if ( !name.empty() && theAnatomist )
    {

      std::map< std::string, anatomist::AObject* >::const_iterator
        o = _objects.find( name );

      if ( o != _objects.end() )
      {

        return theAnatomist->hasObject( o->second );

      }

    }

    return false;

  }
  GKG_CATCH( "bool gkg::AnatomistServer::hasObject( "
             "const std::string& name ) const" );

}


void gkg::AnatomistServer::registerMouseEvent( gkg::MouseEvent* event )
{

  try
  {

    if ( event )
    {

      gkg::MouseAction ma;
      anatomist::ActionDictionary::instance()->addAction( 
                                                   ma.name(), 
                                                   &gkg::MouseAction::creator );

      gkg::MouseControl mc;
      anatomist::ControlDictionary::instance()->addControl( 
                                                    mc.name(), 
                                                    &gkg::MouseControl::creator,
                                                    mc.priority() );
      anatomist::ControlManager::instance()->addControl( "QAGLWidget3D", "", 
                                                         mc.name() );

      gkg::MouseAction* mouseAction = dynamic_cast< gkg::MouseAction* >(
      anatomist::ActionDictionary::instance()->getActionInstance( ma.name() ) );

      if ( mouseAction )
      {

        mouseAction->setMouseEvent( event );

      }

    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::registerMouseEvent( "
             "gkg::MouseEvent* event )" );

}


void gkg::AnatomistServer::setControl( const std::string& name, 
                                       const std::set< std::string >& windows )
{

  try
  {

    if ( theAnatomist && windows.size() )
    {

      std::set< anatomist::AWindow* > setWin;
      std::set< std::string >::iterator
        w = windows.begin(),
        we = windows.end();

      while ( w != we )
      {

        std::map< std::string, QAWindow* >::iterator
          aw = _windows.find( *w );

        if ( aw != _windows.end() )
        {

          setWin.insert( aw->second );

        }

        ++w;

      }

      anatomist::SetControlCommand* command =
                               new anatomist::SetControlCommand( setWin, name );
      _processor.execute( command );
 
    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::setControl( const std::string& name, "
             "const std::set< std::string >& windows )" );

}


gkg::Vector3d< float > gkg::AnatomistServer::getCurrentPosition(
                                                const std::string& referential )
{

  try
  {

    gkg::Vector3d< float > pos( 0.0f, 0.0f, 0.0f );
    std::map< std::string, anatomist::Referential* >::iterator
      r = _referentials.find( referential );

    if ( r != _referentials.end() && theAnatomist )
    {

      Point3df p = theAnatomist->lastPosition( r->second );
      pos.x = p[ 0 ];
      pos.y = p[ 1 ];
      pos.z = p[ 2 ];

    }

    return pos;

  }
  GKG_CATCH( "gkg::Vector3d< float > gkg::AnatomistServer::getCurrentPosition( "
             "const std::string& referential )" );

}


void gkg::AnatomistServer::focusView( const std::string& name )
{

  try
  {

    std::map< std::string, QAWindow* >::iterator
      w = _windows.find( name );

    if ( w != _windows.end() )
    {

      AWindow3D* aWindow3D = dynamic_cast< AWindow3D* >( w->second );

      if ( aWindow3D )
      {

        aWindow3D->focusView();

      }

    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::focusView( "
             "const std::string& name )" );

}


void gkg::AnatomistServer::activateDoubleClippingMode( const std::string& name )
{

  try
  {

    std::map< std::string, QAWindow* >::iterator
      w = _windows.find( name );

    if ( w != _windows.end() )
    {

      AWindow3D* aWindow3D = dynamic_cast< AWindow3D* >( w->second );

      if ( aWindow3D )
      {

        aWindow3D->setClipMode( AWindow3D::Double );

      }

    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::activateDoubleClippingMode( "
             "const std::string& name )" );

}


void gkg::AnatomistServer::setClippingDistance( const std::string& name,
                                                float value )
{

  try
  {

    std::map< std::string, QAWindow* >::iterator
      w = _windows.find( name );

    if ( w != _windows.end() )
    {

      AWindow3D* aWindow3D = dynamic_cast< AWindow3D* >( w->second );

      if ( aWindow3D )
      {

        aWindow3D->setClipDistance( value );

      }

    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::setClippingDistance( "
             "const std::string& name, float value )" );

}


void gkg::AnatomistServer::getViewStateInformation(
                                   const anatomist::ViewState& viewState,
                                   gkg::Vector3d< float >& position,
                                   gkg::AnatomistServer::WindowType& windowType,
                                   float& clippingDistance  ) const
{

  try
  {

    position.x = 0.0;
    position.y = 0.0;
    position.z = 0.0;
    windowType = gkg::AnatomistServer::Unknown;
    clippingDistance = 0.0;

    AWindow3D* aWindow3D = dynamic_cast< AWindow3D* >( viewState.window );
    if ( aWindow3D )
    {

#if ( ANATOMIST_VERSION_MAJOR >= 5 ) || \
    ( ( ANATOMIST_VERSION_MAJOR == 4 ) && \
      ( ANATOMIST_VERSION_MINOR >= 5 ) && \
      ( ANATOMIST_VERSION_TINY >= 0 ) )
      Point3df p = aWindow3D->getPosition();
#else
      Point3df p = aWindow3D->GetPosition();
#endif
      position.x = p[ 0 ];
      position.y = p[ 1 ];
      position.z = p[ 2 ];

      switch ( (AWindow3D::ViewType)aWindow3D->type() )
      {

        case AWindow3D::Axial:

          windowType = gkg::AnatomistServer::Axial;
          break;

        case AWindow3D::Sagittal:

          windowType = gkg::AnatomistServer::Sagittal;
          break;

        case AWindow3D::Coronal:

          windowType = gkg::AnatomistServer::Coronal;
          break;

        case AWindow3D::ThreeD:

          windowType = gkg::AnatomistServer::ThreeD;
          break;

        default:

          windowType = gkg::AnatomistServer::Unknown;

      }
      clippingDistance = aWindow3D->clipDistance();

    }

  }
  GKG_CATCH( "void gkg::AnatomistServer::getViewStateInformation( "
             "const anatomist::ViewState& viewState, "
             "gkg::Vector3d< float >& position, "
             "gkg::AnatomistServer::WindowType& windowType, "
             "float& clippingDistance  ) const" );

}
