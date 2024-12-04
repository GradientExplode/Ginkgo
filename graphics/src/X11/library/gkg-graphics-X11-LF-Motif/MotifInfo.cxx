#include <gkg-graphics-X11-LF-Motif/MotifInfo.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/WidgetFactory.h>
#include <gkg-graphics-X11-core/Session.h>
#include <string>


gkg::MotifInfo::MotifInfo( gkg::Style* style, gkg::ColoringMode coloringMode )
               : _style( style ),
                 _coloringMode( coloringMode )
{

  if ( _style )
  {

    _style->addReference();

  }
  load();

}


gkg::MotifInfo::~MotifInfo()
{

  unload();
  if ( _style )
  {

    _style->removeReference();

  }

}


gkg::Style* gkg::MotifInfo::getStyle() const
{

  return _style;

}


gkg::ColoringMode gkg::MotifInfo::getColoringMode() const
{

  return _coloringMode;

}


gkg::FloatCoordinate gkg::MotifInfo::getThickness() const
{

  return _thickness;

}


float gkg::MotifInfo::getToggleScale() const
{

  return _toggleScale;

}


float gkg::MotifInfo::getRadioScale() const
{

  return _radioScale;

}


gkg::FloatCoordinate gkg::MotifInfo::getMoverSize() const
{

  return _moverSize;

}


gkg::FloatCoordinate gkg::MotifInfo::getSliderSize() const
{

  return _sliderSize;

}


const gkg::Color* gkg::MotifInfo::getFlat() const
{

  return _flat;

}


const gkg::Color* gkg::MotifInfo::getLight() const
{

  return _light;

}


const gkg::Color* gkg::MotifInfo::getDull() const
{

  return _dull;

}


const gkg::Color* gkg::MotifInfo::getDark() const
{

  return _dark;

}


const gkg::Color* gkg::MotifInfo::getGrayOut() const
{

  return _grayOut;

}


void gkg::MotifInfo::load()
{

  _style->findAttribute( "frameThickness", _thickness );
  _style->findAttribute( "toggleScale", _toggleScale );
  _style->findAttribute( "radioScale", _radioScale );
  _style->findAttribute( "moverSize", _moverSize );
  _style->findAttribute( "sliderSize", _sliderSize );

  if ( _coloringMode == gkg::Monochrome )
  {

    std::string value = "#000000";
    if ( !_style->findAttribute( "background", value ) )
    {

      _style->findAttribute( "Background", value );

    }
    const gkg::Color* color = gkg::Color::lookup(
                      gkg::Session::getInstance().getDefaultDisplay(), value );
    if ( !color )
    {

      color = new gkg::Color( 0.0, 0.0, 0.0, 1.0 );

    }
    _flat = color;
    _grayOut = new gkg::Color( *color, 0.5 );

    value = "#ffffff";
    if ( !_style->findAttribute( "foreground", value ) )
    {

      _style->findAttribute( "Foreground", value );

    }
    color = gkg::Color::lookup(
                      gkg::Session::getInstance().getDefaultDisplay(), value );
    if ( !color )
    {

      color = new gkg::Color( 1.0, 1.0, 1.0, 1.0 );

    }
    _dull = color;
    _light = new gkg::Color( *color, 0.5, gkg::Color::Xor );
    _dark = color;

    // set the flat style attribute explicitly; FieldEditor(s) depend on it
    gkg::Color* black = new gkg::Color( 0.0, 0.0, 0.0, 1.0 );
    if ( black )
    {

      black->addReference();

    }
    if ( !_flat->distinguished( black ) )
    {

      _style->addAttribute( "flat", "black" );

    }
    else
    {

      _style->addAttribute( "flat", "white" );

    }
    if ( black )
    {

      black->removeReference();

    }

  }
  else
  {

    std::string value;
    _style->findAttribute( "flat", value );
    const gkg::Color* color = gkg::Color::lookup( 
                      gkg::Session::getInstance().getDefaultDisplay(), value );
    if ( !color )
    {

      color = new gkg::Color( 0.7, 0.7, 0.7, 1.0 );

    }
    _flat = color;
    _light = color->getBrightness( 0.5 );
    _dull = color->getBrightness( -0.2 );
    _dark = color->getBrightness( -0.4 );
    _grayOut = new gkg::Color( *color, 0.2 );

  }
  if ( _flat )
  {

    _flat->addReference();

  }
  if ( _light )
  {

    _light->addReference();

  }
  if ( _dull )
  {

    _dull->addReference();

  }
  if ( _dark )
  {

    _dark->addReference();

  }
  if ( _grayOut )
  {

    _grayOut->addReference();

  }

}


void gkg::MotifInfo::unload()
{

  if ( _flat )
  {

    _flat->removeReference();

  }
  if ( _light )
  {

    _light->removeReference();

  }
  if ( _dull )
  {

    _dull->removeReference();

  }
  if ( _dark )
  {

    _dark->removeReference();

  }
  if ( _grayOut )
  {

    _grayOut->removeReference();

  }

}

