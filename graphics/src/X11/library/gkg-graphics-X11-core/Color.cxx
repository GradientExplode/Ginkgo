#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/ColorImplementation.h>
#include <gkg-graphics-X11-core/Visual.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/DisplayImplementation.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>


//
// class Color
//

gkg::Color::Color( gkg::FloatColorIntensity r,
                   gkg::FloatColorIntensity g,
                   gkg::FloatColorIntensity b,
                   float alpha,
                   gkg::Color::Operation operation )
           : gkg::RCGUIObject(),
             _colorImplementationLut( new gkg::ColorImplementationLut(
                                                   r, g, b, alpha, operation ) )
{
}


gkg::Color::Color( const gkg::Color& other,
                   float alpha,
                   gkg::Color::Operation operation )
           : gkg::RCGUIObject()
{

  _colorImplementationLut = new gkg::ColorImplementationLut(
                                      other._colorImplementationLut->getRed(),
                                      other._colorImplementationLut->getGreen(),
                                      other._colorImplementationLut->getBlue(),
                                      alpha,
                                      operation );

}


gkg::Color::~Color()
{

  delete _colorImplementationLut;

}


gkg::FloatColorIntensity gkg::Color::getRed() const
{

  return _colorImplementationLut->getRed();

}


gkg::FloatColorIntensity gkg::Color::getGreen() const
{

  return _colorImplementationLut->getGreen();

}


gkg::FloatColorIntensity gkg::Color::getBlue() const
{

  return _colorImplementationLut->getBlue();

}


float gkg::Color::getAlpha() const
{

  return _colorImplementationLut->getAlpha();

}


gkg::Color::Operation gkg::Color::getOperation() const
{

  return _colorImplementationLut->getOperation();

}


const std::string& gkg::Color::getName() const
{

  return _colorImplementationLut->getName();

}


gkg::Display* gkg::Color::getDisplay() const
{

  return _colorImplementationLut->getDisplay();

}


bool gkg::Color::distinguished( gkg::Display* display,
                                const gkg::Color* color ) const
{

  gkg::Visual* visual = display->getImplementation()->getDefaultVisual();
  return getImplementation( visual )->distinguished( color );

}


void gkg::Color::getIntensities( gkg::Display* display,
                                 gkg::FloatColorIntensity& r,
                                 gkg::FloatColorIntensity& g,
                                 gkg::FloatColorIntensity& b ) const
{

  gkg::Visual* visual = display->getImplementation()->getDefaultVisual();
  getImplementation( visual )->getIntensities( r, g, b );

}


bool gkg::Color::distinguished( const gkg::Color* color ) const
{

  return distinguished( gkg::Session::getInstance().getDefaultDisplay(),
                        color );

}


void gkg::Color::getIntensities( gkg::FloatColorIntensity& r,
                                 gkg::FloatColorIntensity& g,
                                 gkg::FloatColorIntensity& b ) const
{

  getIntensities( gkg::Session::getInstance().getDefaultDisplay(),
                  r, g, b );

}


gkg::Color* gkg::Color::getBrightness( float adjust ) const
{

  gkg::FloatColorIntensity r, g, b;
  getIntensities( r, g, b );
  if ( adjust >= 0 )
  {

    r += ( 1 - r ) * adjust;
    g += ( 1 - g ) * adjust;
    b += ( 1 - b ) * adjust;

  }
  else
  {

    float f = adjust + 1.0f;
    r *= f;
    g *= f;
    b *= f;

  }
  return new gkg::Color( r, g, b );

}


gkg::ColorImplementation* 
gkg::Color::getImplementation( gkg::Visual* visual ) const
{

  return _colorImplementationLut->getImplementation( visual );

}


const gkg::Color* 
gkg::Color::lookup( gkg::Display* display, const std::string& name )
{

  return gkg::ColorImplementationLut::lookup( display, name );

}


//
// class ColorImplementationLut
//

gkg::ColorImplementationLut::NameToColorHashTable 
gkg::ColorImplementationLut::_colors;


gkg::ColorImplementationLut::ColorImplementationLut(
                                               gkg::FloatColorIntensity red,
                                               gkg::FloatColorIntensity green,
                                               gkg::FloatColorIntensity blue,
                                               float alpha,
                                               gkg::Color::Operation operation )
                            : _red( red ),
                              _green( green ),
                              _blue( blue ),
                              _alpha( alpha ),
                              _operation( operation ),
                              _name( "" ),
                              _display( 0 )
{
}


gkg::ColorImplementationLut::~ColorImplementationLut()
{

  if ( _display )
  {

    _colors.erase( _colors.find( gkg::ColorEntry( _display, _name ) ) );

  }

  std::list< gkg::ColorImplementation* >::iterator
    c = _colorImplementations.begin(), ce = _colorImplementations.end();
  while ( c != ce )
  {

    delete *c;
    ++ c;

  }
  _colorImplementations.clear();

}


gkg::FloatColorIntensity gkg::ColorImplementationLut::getRed() const
{

  return _red;

}


gkg::FloatColorIntensity gkg::ColorImplementationLut::getGreen() const
{

  return _green;

}


gkg::FloatColorIntensity gkg::ColorImplementationLut::getBlue() const
{

  return _blue;

}


float gkg::ColorImplementationLut::getAlpha() const
{

  return _alpha;

}


gkg::Color::Operation gkg::ColorImplementationLut::getOperation() const
{

  return _operation;

}


const std::string& gkg::ColorImplementationLut::getName() const
{

  return _name;

}


gkg::Display* gkg::ColorImplementationLut::getDisplay() const
{

  return _display;

}


void gkg::ColorImplementationLut::setName( const std::string& name )
{

  _name = name;

}


void gkg::ColorImplementationLut::setDisplay( gkg::Display* display )
{

  _display = display;

}


gkg::ColorImplementation* 
gkg::ColorImplementationLut::getImplementation( gkg::Visual* visual )
{

  gkg::ColorImplementation* colorImpl = 0;
  std::list< ColorImplementation* >::iterator
    c = _colorImplementations.begin(), ce = _colorImplementations.end(); 

  while ( c != ce )
  {

    colorImpl = *c;
    if ( colorImpl->getVisual() == visual )
    {

      return colorImpl;

    }
    ++ c;

  }

  colorImpl = gkg::getGUIImplementationFactory()->createColorImplementation( 
                              visual, _red, _green, _blue, _alpha, _operation );
  _colorImplementations.push_back( colorImpl );
  return colorImpl;

}


const gkg::Color* 
gkg::ColorImplementationLut::lookup( gkg::Display* display,
                                     const std::string& name )
{


  gkg::ColorEntry colorEntry( display, name );

  gkg::ColorImplementationLut::NameToColorHashTable::const_iterator
    c = _colors.find( colorEntry );

  if ( c != _colors.end() )
  {

    return c->second;

  }

  gkg::FloatColorIntensity r, g, b;
  if ( display->getImplementation()->getDefaultVisual()->findColor( name,
                                                                    r, g, b ) )
  {

    gkg::Color* color = new gkg::Color( r, g, b );
    color->_colorImplementationLut->setName( name );
    color->_colorImplementationLut->setDisplay( display );
    _colors[ colorEntry ] = color;
    return color;

  }
  return 0;

}


