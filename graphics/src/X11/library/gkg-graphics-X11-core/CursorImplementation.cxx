#include <gkg-graphics-X11-core/CursorImplementation.h>
#include <gkg-graphics-X11-core/Cursor.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Color.h>


gkg::CursorImplementation::CursorImplementation(
                                           gkg::Cursor* cursor,
                                           const gkg::Color* foreground,
                                           const gkg::Color* background )
                             : _target( cursor ),
                               _foreground( foreground ),
                               _background( background ),
                               _display( 0 )
{

  if ( _foreground )
  {

    _foreground->addReference();

  }
  if ( _background )
  {

    _background->addReference();

  }

}


gkg::CursorImplementation::~CursorImplementation()
{

  if ( _foreground )
  {

    _foreground->removeReference();

  }
  if ( _background )
  {

    _background->removeReference();

  }

}


gkg::Cursor* gkg::CursorImplementation::getTarget() const
{

  return _target;

}


const gkg::Color* 
gkg::CursorImplementation::makeColor( gkg::Display* display,
                                      gkg::Style* style,
                                      const std::string& str1,
                                      const std::string& str2,
                                      const std::string& str3,
                                      const std::string& defaultValue )
{

  const gkg::Color* color = 0;
  std::string value;
  if ( style->findAttribute( str1, value ) ||
       style->findAttribute( str2, value ) ||
       style->findAttribute( str3, value ) )
  {

    color = gkg::Color::lookup( display, value );

  }
  if ( !color )
  {

    color = gkg::Color::lookup( display, defaultValue );

  }
  if ( color )
  {

    color->addReference();

  }
  return color;

}


const gkg::Color* gkg::CursorImplementation::getForeground() const
{

  return _foreground;

}


const gkg::Color* gkg::CursorImplementation::getBackground() const
{

  return _background;

}


