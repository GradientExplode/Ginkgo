#include <gkg-graphics-X11-core/Cursor.h>
#include <gkg-graphics-X11-core/CursorImplementation.h>
#include <gkg-graphics-X11-core/DataCursor.h>
#include <gkg-graphics-X11-core/BitmapCursor.h>
#include <gkg-graphics-X11-core/FontCursor.h>
#include <gkg-graphics-X11-core/PredefinedCursor.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>


gkg::Cursor* gkg::Cursor::_default = 0;
gkg::Cursor* gkg::Cursor::_menu = 0;
gkg::Cursor* gkg::Cursor::_arrow = 0;
gkg::Cursor* gkg::Cursor::_crossHairs = 0;
gkg::Cursor* gkg::Cursor::_leftText = 0;
gkg::Cursor* gkg::Cursor::_rightText = 0;
gkg::Cursor* gkg::Cursor::_hourGlass = 0;
gkg::Cursor* gkg::Cursor::_upperLeft = 0;
gkg::Cursor* gkg::Cursor::_upperRight = 0;
gkg::Cursor* gkg::Cursor::_lowerLeft = 0;
gkg::Cursor* gkg::Cursor::_lowerRight = 0;
gkg::Cursor* gkg::Cursor::_none = 0;


gkg::Cursor::Cursor( const gkg::Color* foreground,
                     const gkg::Color* background )
{

  _cursorImplementation =
    gkg::getGUIImplementationFactory()->createCursorImplementation( this,
                                                     foreground, background );

}


gkg::Cursor::~Cursor()
{

  delete _cursorImplementation;

}


gkg::CursorImplementation* gkg::Cursor::getImplementation() const
{

  return _cursorImplementation;

}


void gkg::Cursor::initialize()
{

  _default = gkg::getGUIImplementationFactory()->createArrowCursor();
  _menu = gkg::getGUIImplementationFactory()->createMenuCursor();
  _arrow = _default;
  _crossHairs = gkg::getGUIImplementationFactory()->createCrossHairsCursor();
  _leftText = gkg::getGUIImplementationFactory()->createLeftTextCursor();
  _rightText = gkg::getGUIImplementationFactory()->createRightTextCursor();
  _hourGlass = gkg::getGUIImplementationFactory()->createHourGlassCursor();
  _upperLeft = gkg::getGUIImplementationFactory()->createUpperLeftCursor();
  _upperRight = gkg::getGUIImplementationFactory()->createUpperRightCursor();
  _lowerLeft = gkg::getGUIImplementationFactory()->createLowerLeftCursor();
  _lowerRight = gkg::getGUIImplementationFactory()->createLowerRightCursor();
  _none = gkg::getGUIImplementationFactory()->createNoneCursor();

}


gkg::Cursor* gkg::Cursor::getDefault()
{

  return _default;

}


gkg::Cursor* gkg::Cursor::getArrow()
{

  return _arrow;

}


gkg::Cursor* gkg::Cursor::getMenu()
{

  return _menu;

}


gkg::Cursor* gkg::Cursor::getCrossHairs()
{

  return _crossHairs;

}


gkg::Cursor* gkg::Cursor::getLeftText()
{

  return _leftText;

}


gkg::Cursor* gkg::Cursor::getRightText()
{

  return _rightText;

}


gkg::Cursor* gkg::Cursor::getHourGlass()
{

  return _hourGlass;

}


gkg::Cursor* gkg::Cursor::getUpperLeft()
{

  return _upperLeft;

}


gkg::Cursor* gkg::Cursor::getUpperRight()
{

  return _upperRight;

}


gkg::Cursor* gkg::Cursor::getLowerLeft()
{

  return _lowerLeft;

}


gkg::Cursor* gkg::Cursor::getLowerRight()
{

  return _lowerRight;

}


gkg::Cursor* gkg::Cursor::getNone()
{

  return _none;

}


