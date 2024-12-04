#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/ActionCallback.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>
#include <gkg-graphics-X11-progressive/ProgressiveImage.h>
#include <iostream>
#include "SliceModel.h"



class ProgressiveApplication 
{

  public:

    ProgressiveApplication( gkg::ProgressiveImage* progressiveImage );

    void update();
    void quit();

  private:

    gkg::ProgressiveImage* _progressiveImage;

};


ProgressiveApplication::ProgressiveApplication(
                                       gkg::ProgressiveImage* progressiveImage )
                       : _progressiveImage( progressiveImage )
{
}


void ProgressiveApplication::update()
{

  _progressiveImage->update();

}


void ProgressiveApplication::quit()
{

  gkg::Session::getInstance().quit();

}


int main( int argc, char* argv[] )
{

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestProgressiveImage01", argc, argv );

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  gkg::ProgressiveModel*
    progressiveModel = new SliceModel;

  gkg::ProgressiveImage*
    progressiveImage = new gkg::ProgressiveImage( progressiveModel, 8, true );

  ProgressiveApplication*
    progressiveApplication = new ProgressiveApplication( progressiveImage );

  gkg::Glyph*
    layoutMarginImage = layoutFactory.createMargin( progressiveImage, 10 );

  gkg::ActionCallback< ProgressiveApplication >* actionCallbackUpdate =
    new gkg::ActionCallback< ProgressiveApplication >(
                                              progressiveApplication,
                                              &ProgressiveApplication::update );

  gkg::Glyph* 
    pushButtonUpdate = widgetFactory->createPushButton( "Update image",
                                                        actionCallbackUpdate );

  gkg::ActionCallback< ProgressiveApplication >* actionCallbackQuit =
    new gkg::ActionCallback< ProgressiveApplication >(
                                              progressiveApplication,
                                              &ProgressiveApplication::quit );

  gkg::Glyph*
    pushButtonQuit = widgetFactory->createPushButton( "Quit",
                                                      actionCallbackQuit );

  gkg::Glyph*
    layoutMarginUpdate = layoutFactory.createMargin( pushButtonUpdate, 10 );

  gkg::Glyph*
    layoutMarginQuit = layoutFactory.createMargin( pushButtonQuit, 10 );

  gkg::Glyph* hBox = layoutFactory.createHBox( layoutMarginUpdate,
                                               layoutMarginQuit );

  gkg::Glyph* vBox = layoutFactory.createVBox( layoutMarginImage,
                                               hBox );

  gkg::Glyph* insetFrame = widgetFactory->createInsetFrame( vBox ); 

  gkg::ApplicationWindow* 
    aWindow = new gkg::ApplicationWindow( insetFrame );

  int result = session.runWindow( aWindow );

  delete progressiveImage;

  return result;

}
