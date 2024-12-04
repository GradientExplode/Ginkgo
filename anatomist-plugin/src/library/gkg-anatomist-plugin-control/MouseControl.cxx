#include <gkg-anatomist-plugin-control/MouseControl.h>
#include <gkg-anatomist-plugin-control/MouseAction.h>

#include <anatomist/controler/control_d.h>


gkg::MouseControl::MouseControl( const std::string& name )
                 : anatomist::Control( 2, 
                                       QT_TRANSLATE_NOOP( "ControlledWindow", 
                                                          name ) )
{
}


gkg::MouseControl::MouseControl( const gkg::MouseControl& c )
                 : anatomist::Control( c )
{
}


anatomist::Control* gkg::MouseControl::creator()
{

  return new gkg::MouseControl();

}


void gkg::MouseControl::eventAutoSubscription( 
                                             anatomist::ActionPool* actionPool )
{

  mousePressButtonEventSubscribe( 
    Qt::LeftButton, 
    Qt::NoModifier, 
    MouseActionLinkOf< gkg::MouseAction >( 
                                         actionPool->action( "GkgMouseAction" ),
                                         &gkg::MouseAction::press ) );

}
