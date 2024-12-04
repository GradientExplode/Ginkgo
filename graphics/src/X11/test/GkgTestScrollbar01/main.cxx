// scroll bar and bounded value editor

#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/Background.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/ActionCallback.h>
#include <gkg-graphics-X11-core/TellTale.h>
#include <gkg-graphics-X11-core/Button.h>
#include <gkg-graphics-X11-core/Adjustable.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>
#include <iostream>
#include <cstdlib>


//
// class Application
//

class Application : public gkg::Observer
{

  public:

    Application( gkg::Adjustable* adjustable,
                 gkg::TellTaleState* tellTaleState );
    virtual ~Application();

    void printValue();

    virtual void update( const gkg::Observable* observable, void* arg = 0 );
    virtual void unregisterObservable( gkg::Observable* observable );

  private:

    gkg::Adjustable* _adjustable;
    gkg::TellTaleState* _continuous;

};


Application::Application( gkg::Adjustable* adjustable,
                          gkg::TellTaleState* tellTaleState ) 
            : _adjustable( adjustable ),
              _continuous( tellTaleState )
{

  adjustable->addObserver( gkg::DimensionX, this );

}


Application::~Application()
{

  if ( _adjustable )
  {

    _adjustable->removeObserver( gkg::DimensionX, this );

  }

}


void Application::printValue()
{

  std::cout << _adjustable->getCurrentLower( gkg::DimensionX ) << std::endl;

}


void Application::update( const gkg::Observable*, void* )
{

  if ( _continuous->test( gkg::TellTaleState::Chosen ) )
  {

    printValue();

  }

}


void Application::unregisterObservable( gkg::Observable* )
{

  _adjustable = 0;

}


//
// class BoundedValue
//

class BoundedValue : public gkg::Adjustable
{

  public:

    BoundedValue( gkg::FloatCoordinate lower, gkg::FloatCoordinate upper );
    ~BoundedValue();

    void setLowerBound( gkg::FloatCoordinate c );
    void setUpperBound( gkg::FloatCoordinate c );
    void setCurrentValue( gkg::FloatCoordinate value );
    void setScrollIncrement( gkg::FloatCoordinate c );
    void setPageIncrement( gkg::FloatCoordinate c );

    gkg::FloatCoordinate getLower( gkg::DimensionName d ) const;
    gkg::FloatCoordinate getUpper( gkg::DimensionName d ) const;
    gkg::FloatCoordinate getLength( gkg::DimensionName d ) const;
    gkg::FloatCoordinate getCurrentLower( gkg::DimensionName d ) const;
    gkg::FloatCoordinate getCurrentUpper( gkg::DimensionName d ) const;
    gkg::FloatCoordinate getCurrentLength( gkg::DimensionName d ) const;

    void scrollForward( gkg::DimensionName d );
    void scrollBackward( gkg::DimensionName d );
    void pageForward( gkg::DimensionName d );
    void pageBackward( gkg::DimensionName d );

    void scrollTo( gkg::DimensionName d, gkg::FloatCoordinate position );

  protected:

      BoundedValue();

  private:

    gkg::FloatCoordinate _currentValue;
    gkg::FloatCoordinate _lower;
    gkg::FloatCoordinate _span;
    gkg::FloatCoordinate _scrollIncrement;
    gkg::FloatCoordinate _pageIncrement;
};


BoundedValue::BoundedValue()
             : _scrollIncrement( 0 ),
               _pageIncrement( 0 )
{
}


BoundedValue::BoundedValue( gkg::FloatCoordinate lower,
                            gkg::FloatCoordinate upper )
{

  _lower = lower;
  _span = upper - lower;
  _scrollIncrement = _span * 0.04;
  _pageIncrement = _span * 0.4;
  _currentValue = ( lower + upper ) * 0.5;

}


BoundedValue::~BoundedValue()
{
}


void BoundedValue::setLowerBound( gkg::FloatCoordinate c )
{

  _lower = c;

}


void BoundedValue::setUpperBound( gkg::FloatCoordinate c )
{

  _span = c - _lower;

}


void BoundedValue::setCurrentValue( gkg::FloatCoordinate value )
{

  _currentValue = value;
  getConstrain( gkg::DimensionX, _currentValue );
  notify( gkg::DimensionX );
  notify( gkg::DimensionY );

}


void BoundedValue::setScrollIncrement( gkg::FloatCoordinate c )
{

  _scrollIncrement = c;

}


void BoundedValue::setPageIncrement( gkg::FloatCoordinate c )
{

  _pageIncrement = c;

}


gkg::FloatCoordinate BoundedValue::getLower( gkg::DimensionName ) const
{

  return _lower;

}

gkg::FloatCoordinate BoundedValue::getUpper( gkg::DimensionName ) const
{

  return _lower + _span;

}


gkg::FloatCoordinate BoundedValue::getLength( gkg::DimensionName ) const
{

  return _span;

}


gkg::FloatCoordinate 
BoundedValue::getCurrentLower( gkg::DimensionName ) const
{

  return _currentValue;

}


gkg::FloatCoordinate 
BoundedValue::getCurrentUpper( gkg::DimensionName ) const
{

  return _currentValue;

}


gkg::FloatCoordinate
BoundedValue::getCurrentLength( gkg::DimensionName ) const
{

  return 0;

}


void BoundedValue::scrollForward( gkg::DimensionName d )
{

  scrollTo( d, _currentValue + _scrollIncrement );

}


void BoundedValue::scrollBackward( gkg::DimensionName d )
{

  scrollTo( d, _currentValue - _scrollIncrement );

}


void BoundedValue::pageForward( gkg::DimensionName d )
{

  scrollTo( d, _currentValue + _pageIncrement );

}


void BoundedValue::pageBackward( gkg::DimensionName d )
{

  scrollTo( d, _currentValue - _pageIncrement );

}


void BoundedValue::scrollTo( gkg::DimensionName d,
                             gkg::FloatCoordinate position )
{

  gkg::FloatCoordinate p = position;
  getConstrain( d, p );
  if ( p != _currentValue )
  {

    _currentValue = p;
    notify( gkg::DimensionX );
    notify( gkg::DimensionY );

  }

}


//
// main procedure
//

int main( int argc, char* argv[] )
{

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgTestScrollbar01", argc, argv );

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

  gkg::Button*
    continuousButton = widgetFactory->createCheckBox( "Continuous", 0 );
  BoundedValue* boundedValue = new BoundedValue( 0.0, 100.0 );

  Application* application = new Application( boundedValue,
                                              continuousButton->getState() );

  gkg::ActionCallback< Application >* actionCallback =
    new gkg::ActionCallback< Application >( application,
                                               &Application::printValue );

  boundedValue->setCurrentValue( 50.0 );
  boundedValue->setScrollIncrement( 5 );
  boundedValue->setPageIncrement( 20 );

  gkg::Glyph* pushButton = widgetFactory->createPushButton( "Print value",
                                                            actionCallback );

  gkg::Glyph* hCenter1 = layoutFactory.createHCenter( pushButton );
  gkg::Glyph* glue = layoutFactory.createVGlue( 10 );
  gkg::Glyph* hCenter2 = layoutFactory.createHCenter( continuousButton );

  gkg::Glyph* vBox = layoutFactory.createVBox( hCenter1, glue, hCenter2 );

  gkg::Glyph* margin = layoutFactory.createMargin( vBox, 20 );

  gkg::Glyph* scrollBar = widgetFactory->createVScrollBar( boundedValue );
  gkg::Glyph* insetFrameSb = widgetFactory->createInsetFrame( scrollBar );

  gkg::Glyph* hBox = layoutFactory.createHBox( margin, insetFrameSb );

  gkg::Glyph* insetFrame = widgetFactory->createInsetFrame( hBox );

  gkg::ApplicationWindow* aWindow = new gkg::ApplicationWindow( insetFrame );

  int result = session.runWindow( aWindow );

  return result;

}
