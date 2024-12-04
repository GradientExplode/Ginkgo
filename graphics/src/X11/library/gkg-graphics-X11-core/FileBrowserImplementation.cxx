#include <gkg-graphics-X11-core/FileBrowserImplementation.h>
#include <gkg-graphics-X11-core/FileBrowser.h>
#include <gkg-communication-dispatcher/IOCallback.h>
#include <gkg-communication-dispatcher/Dispatcher.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-core/TBScrollBox.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Window.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>


static gkg::FileBrowserImplementation::KeyInfo defaultKeyFunctions[] =
{

  { '\r',   "open",
    &gkg::FileBrowserImplementation::open           },
  { '\007', "cancel",
    &gkg::FileBrowserImplementation::cancel         },
  { '\033', "cancel",
    &gkg::FileBrowserImplementation::cancel         },
  { 'g',    "scroll-to-first",
    &gkg::FileBrowserImplementation::scrollToFirst  },
  { 'G',    "scroll-to-last",
    &gkg::FileBrowserImplementation::scrollToLast   },
  { 'a',    "select-all",
    &gkg::FileBrowserImplementation::selectAll      },
  { '\177', "unselect-all",
    &gkg::FileBrowserImplementation::unselectAll    },
  { '\010', "unselect-all",
    &gkg::FileBrowserImplementation::unselectAll    },
  { '\t',   "next-focus",
    &gkg::FileBrowserImplementation::nextFocus      },
  { 'p',    "select-previous",
    &gkg::FileBrowserImplementation::selectPrevious },
  { 'n',    "select-next",
    &gkg::FileBrowserImplementation::selectNext     },
  { '<',    "select-top",
    &gkg::FileBrowserImplementation::selectTop      },
  { '>',    "select-bottom",
    &gkg::FileBrowserImplementation::selectBottom   },
  { 'j',    "scroll-down",
    &gkg::FileBrowserImplementation::scrollDown     },
  { 'k',    "scroll-up",
    &gkg::FileBrowserImplementation::scrollUp       },
  { ' ',    "page-down",
    &gkg::FileBrowserImplementation::pageDown       },
  { 'b',    "page-up",
    &gkg::FileBrowserImplementation::pageUp         },
  { 'd',    "half-page-down",
    &gkg::FileBrowserImplementation::halfPageDown   },
  { 'u',    "half-page-up",
    &gkg::FileBrowserImplementation::halfPageUp     },
  { 0,      0,
    0                                                  }

};


gkg::FileBrowserImplementation::FileBrowserImplementation(
                                              gkg::FileBrowser* fileBrowser )
                               : _target( fileBrowser ),
                                 _selected( -1 ),
                                 _box( new gkg::TBScrollBox( true ) )
{

  const gkg::Font*
    font = gkg::LookAndFeel::getInstance().getWidgetFactory()->getFont();
  gkg::FontBoundingBox boundingBox;
  font->getFontBoundingBox( boundingBox );
  _scale = 1.0 / ( boundingBox.ascent + boundingBox.descent );
  _saveCursor = 0;
  for ( int32_t i = 0; i < FILE_BROWSER_KEYFUNCTION_SIZE; i++ )
  {

    _keyFunctions[ i ] = 0;

  }
  for ( gkg::FileBrowserImplementation::KeyInfo*
        k = &defaultKeyFunctions[ 0 ]; k->key != 0; k++ )
  {

    _keyFunctions[ ( int32_t )k->key ] = k->function;

  }
  _rateHandler = new gkg::IOCallback< gkg::FileBrowserImplementation >(
                       this,
                       &gkg::FileBrowserImplementation::setRateScrollTimer );
  gkg::Style* 
    style = gkg::LookAndFeel::getInstance().getWidgetFactory()->getStyle();
  int32_t milliseconds = 75;
  style->findAttribute( "scrollRate", milliseconds );
  _usecRate = 1000 * milliseconds;
  _target->setBody( _box );

}


gkg::FileBrowserImplementation::~FileBrowserImplementation()
{

  delete _rateHandler;

}


gkg::FileBrowser* gkg::FileBrowserImplementation::getTarget() const
{

  return _target;

}


void gkg::FileBrowserImplementation::press( const gkg::Event& event )
{

  gkg::Event::Button button = event.getPointerButton();
  gkg::Window* window = _target->getCanvas()->getWindow();
  switch ( button )
  {

    case gkg::Event::Left:

      _target->Browser::press( event );
      _mode = gkg::FileBrowserImplementation::Selecting;
      break;

    case gkg::Event::Middle:

      _mode = gkg::FileBrowserImplementation::GrabScrolling;
      _saveCursor = window->getCursor();
      _startScrollPointer = event.getPointerY();
      _startScrollPosition = _box->getCurrentLower( gkg::DimensionY );
      window->setCursor( 
        gkg::LookAndFeel::getInstance().getWidgetFactory()->getHandCursor() );
      break;

    case gkg::Event::Right:

      _mode = gkg::FileBrowserImplementation::RateScrolling;
      _saveCursor = window->getCursor();
      _startScrollPointer = event.getPointerY();
      _startScrollPosition = _box->getCurrentLower( gkg::DimensionY );
      break;

    default:

      break;

  }

}


void gkg::FileBrowserImplementation::drag( const gkg::Event& event )
{

  gkg::FloatCoordinate delta;
  gkg::Window* window = _target->getCanvas()->getWindow();
  switch ( _mode )
  {

    case gkg::FileBrowserImplementation::Selecting:

      _target->Browser::drag( event );
      break;

    case gkg::FileBrowserImplementation::GrabScrolling:

      delta = event.getPointerY() - _startScrollPointer;
      _box->scrollTo( gkg::DimensionY, _startScrollPosition - delta * _scale );
      break;

    case gkg::FileBrowserImplementation::RateScrolling:

      WidgetFactory*
        widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();
      _currentScrollPointer = event.getPointerY();
      if ( _currentScrollPointer > _startScrollPointer )
      {

        window->setCursor( widgetFactory->getUFastCursor() );

      }
      else
      {

        window->setCursor( widgetFactory->getDFastCursor() );

      }
      gkg::Dispatcher::getInstance().stopTimer( _rateHandler );
      setRateScrollTimer( 0, 0 );
      break;

  }

}


void gkg::FileBrowserImplementation::release( const gkg::Event& event )
{

  gkg::Window* window = _target->getCanvas()->getWindow();
  gkg::FloatCoordinate delta;
  switch ( _mode )
  {

    case gkg::FileBrowserImplementation::Selecting:

      _target->Browser::release( event );
      break;

    case gkg::FileBrowserImplementation::GrabScrolling:

      delta = event.getPointerY() - _startScrollPointer;
      _box->scrollTo( gkg::DimensionY, _startScrollPosition - delta * _scale );
      window->setCursor( _saveCursor );
      break;

    case gkg::FileBrowserImplementation::RateScrolling:

      gkg::Dispatcher::getInstance().stopTimer( _rateHandler );
      window->setCursor( _saveCursor );
      break;

  }

}


void gkg::FileBrowserImplementation::keyStroke( const gkg::Event& event )
{

  char c;
  if ( event.getMapKey( &c, 1 ) != 0 )
  {

    gkg::FileBrowserImplementation::KeyFunction 
      function = _keyFunctions[ ( int32_t )c ];
    if ( function )
    {

      ( this->*function )();

    }

  }

}


void gkg::FileBrowserImplementation::focusIn()
{

  if ( _selected == -1 )
  {

    selectTop();

  }
  else
  {

    _target->Browser::select( _selected );

  }

}


void gkg::FileBrowserImplementation::focusOut()
{

  _selected = _target->getSelected();

}


void gkg::FileBrowserImplementation::select( gkg::Glyph::Index index )
{

  _selected = index;

}



gkg::Adjustable* gkg::FileBrowserImplementation::getAdjustable() const
{

  return _box;

}


void gkg::FileBrowserImplementation::refresh()
{

  _box->scrollTo( gkg::DimensionY, ( gkg::FloatCoordinate )_box->getCount() );

}


void gkg::FileBrowserImplementation::setRateScrollTimer( int64_t, int64_t )
{

  gkg::FloatCoordinate delta = _currentScrollPointer - _startScrollPointer;
  _box->scrollTo( gkg::DimensionY,
                 _box->getCurrentLower( gkg::DimensionY ) + delta * _scale );
  gkg::Dispatcher::getInstance().startTimer( 0, _usecRate, _rateHandler );

}


void gkg::FileBrowserImplementation::open()
{

  gkg::Glyph::Index index = _target->getSelected();
  if ( ( index >= 0 ) && ( index < _target->getCount() ) )
  {

    _target->choose( index );

  }

}


void gkg::FileBrowserImplementation::cancel()
{

  _target->cancel();

}


void gkg::FileBrowserImplementation::scrollToFirst()
{

  _box->scrollTo( gkg::DimensionY, ( gkg::FloatCoordinate )_box->getCount() );

}


void gkg::FileBrowserImplementation::scrollToLast()
{

  _box->scrollTo( gkg::DimensionY, ( gkg::FloatCoordinate )0 );

}


void gkg::FileBrowserImplementation::selectAll()
{
}


void gkg::FileBrowserImplementation::unselectAll()
{

  _target->select( -1 );

}


void gkg::FileBrowserImplementation::nextFocus()
{

  _target->nextFocus();

}


void gkg::FileBrowserImplementation::selectPrevious()
{

  gkg::Glyph::Index index = _target->getSelected();
  if ( !_box->isShown( index ) )
  {

    _box->scrollTo( gkg::DimensionY,
                    ( gkg::FloatCoordinate )( _box->getCount() - index - 1 ) );

  }
  if ( index > 0 )
  {

    -- index;
    if ( !_box->isShown( index ) )
    {

      _box->scrollForward( gkg::DimensionY );

    }
    _target->select( index );

  }

}


void gkg::FileBrowserImplementation::selectNext()
{

  gkg::Glyph::Index index = _target->getSelected();
  if ( !_box->isShown( index ) )
  {

    _box->scrollTo( gkg::DimensionY,
                    ( gkg::FloatCoordinate )( _box->getCount() - index - 1 +
                    _box->getFirstShown() - _box->getLastShown() ) );

  }

  if ( index < _target->getCount() - 1 )
  {

    ++ index;
    if ( !_box->isShown( index ) )
    {

      _box->scrollBackward( gkg::DimensionY );

    }
    _target->select( index );

  }

}


void gkg::FileBrowserImplementation::selectTop()
{

  _target->select( _box->getFirstShown() );

}


void gkg::FileBrowserImplementation::selectBottom()
{

  _target->select( _box->getLastShown() );

}


void gkg::FileBrowserImplementation::scrollDown()
{

  _box->scrollForward( gkg::DimensionY );

}


void gkg::FileBrowserImplementation::scrollUp()
{

  _box->scrollBackward( gkg::DimensionY );

}


void gkg::FileBrowserImplementation::pageDown()
{

  _box->pageBackward( gkg::DimensionY );

}


void gkg::FileBrowserImplementation::pageUp()
{

  _box->pageForward( gkg::DimensionY );

}


void gkg::FileBrowserImplementation::halfPageDown()
{

  gkg::Glyph::Index n = _box->getLastShown() - _box->getFirstShown() + 1;
  gkg::Glyph::Index half = n >> 1;
  for ( gkg::Glyph::Index i = 0; i < half; i++ )
  {

    _box->scrollBackward( gkg::DimensionY );

  }

}


void gkg::FileBrowserImplementation::halfPageUp()
{

  gkg::Glyph::Index n = _box->getLastShown() - _box->getFirstShown() + 1;
  gkg::Glyph::Index half = n >> 1;
  for ( gkg::Glyph::Index i = 0; i < half; i++ )
  {

    _box->scrollForward( gkg::DimensionY );

  }

}
