#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/WidgetFactoryImplementation.h>
#include <iostream>
#include <cstdlib>


gkg::LookAndFeel::LookAndFeel()
                 : _widgetFactory( 0 ),
                   _dialogFactory( 0 ),
                   _ready( false )
{
}


gkg::LookAndFeel::~LookAndFeel()
{

  _widgetFactoryAccessors.clear();
  _dialogFactoryAccessors.clear();

}


gkg::WidgetFactory* gkg::LookAndFeel::getWidgetFactory()
{

  if ( !_ready )
  {

    selectFactories();

  }
  return _widgetFactory;

}


gkg::DialogFactory* gkg::LookAndFeel::getDialogFactory()
{

  if ( !_ready )
  {

    selectFactories();

  }
  return _dialogFactory;

}


bool gkg::LookAndFeel::registerFactories(
                 const std::string& lookName,
                 gkg::LookAndFeel::WidgetFactoryAccessor widgetFactoryAccessor,
                 gkg::LookAndFeel::DialogFactoryAccessor dialogFactoryAccessor )
{

  if ( widgetFactoryAccessor && dialogFactoryAccessor )
  {

    std::map< std::string,
              gkg::LookAndFeel::WidgetFactoryAccessor >::const_iterator
      w = _widgetFactoryAccessors.find( lookName );
    std::map< std::string,
              gkg::LookAndFeel::DialogFactoryAccessor >::const_iterator
      d = _dialogFactoryAccessors.find( lookName );

    if ( ( w == _widgetFactoryAccessors.end() ) &&
         ( d == _dialogFactoryAccessors.end() ) )
    {

      _widgetFactoryAccessors[ lookName ] = widgetFactoryAccessor;
      _dialogFactoryAccessors[ lookName ] = dialogFactoryAccessor;
      return true;

    }
    else
    {

      std::cerr << "gkg: look and feel " << lookName << " already exists"
                << std::endl;
      return false;

    }

  }
  return false;

}


void gkg::LookAndFeel::selectFactories()
{

  std::string lookAndFeel;

  if ( gkg::Session::getInstance().getStyle()->findAttribute(
                                                "look_and_feel", lookAndFeel ) )
  {

    if ( select( lookAndFeel ) )
    {

      // everything is ok and L&F was found; exit normally
      _ready = true;
      return;

    }

  }

  // L&F may not have been specified in style or may not have been found;
  // look for default Motif L&F:
  if ( select( "Motif" ) )
  {

    _ready = true;
    return;

  }

  std::cerr << "gkg:: no look and feel found : problem!" << std::endl;
  ::abort();

}


bool gkg::LookAndFeel::select( const std::string& lookName )
{

  std::map< std::string,
            gkg::LookAndFeel::WidgetFactoryAccessor >::const_iterator
    w = _widgetFactoryAccessors.find( lookName );
  std::map< std::string,
            gkg::LookAndFeel::DialogFactoryAccessor >::const_iterator
    d = _dialogFactoryAccessors.find( lookName );

  if ( ( w != _widgetFactoryAccessors.end() ) &&
       ( d != _dialogFactoryAccessors.end() ) )
  {

    _widgetFactory = &( *w->second )();
    _dialogFactory = &( *d->second )();
    selectColoringMode();
    _widgetFactory->styleChanged( 
                              _widgetFactory->getImplementation()->getStyle() );
    return true;

  }
  else
  {

    std::cerr << "gkg: look and feel " << lookName << " does not exist"
              << std::endl;

  }

  return false;

}


void gkg::LookAndFeel::selectColoringMode()
{

  gkg::ColoringMode coloringMode = gkg::Polychrome;

  if ( gkg::Session::getInstance().getStyle()->getValueIsOn("monochrome") )
  {

    coloringMode = gkg::Monochrome;

  }

  // if system does not allow coloring, default to monochrome:
  const gkg::Color* c1 = new gkg::Color( 0.0, 0.0, 0.0, 1.0 );
  if ( c1 )
  {

    c1->addReference();

  }
  const gkg::Color* c2 = new gkg::Color( 1.0, 1.0, 1.0, 1.0 );
  if ( c2 )
  {

    c2->addReference();

  }
  const gkg::Color* c3 = new gkg::Color( 0.5, 0.5, 0.5, 1.0 );
  if ( c3 )
  {

    c3->addReference();

  }
  if ( !c3->distinguished( c1 ) || !c3->distinguished( c2 ) )
  {

    coloringMode = gkg::Monochrome;

  }
  if ( c1 )
  {

    c1->removeReference();

  }
  if ( c2 )
  {

    c2->removeReference();

  }
  if ( c3 )
  {

    c3->removeReference();

  }

  _widgetFactory->setColoringMode( coloringMode );

}
