#include <gkg-graphics-X11-core/FontImplementation.h>
#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/Display.h>


//
// class FontImplementation
//

gkg::FontImplementation::FontImplementation( gkg::Display* display,
                                             const std::string& /*name*/,
                                             float scale )
                        : gkg::RCGUIObject(),
                          _display( display ),
                          _entry( 0 ),
                          _scale( scale ),
                          _unscaled( ( scale > 0.9999f ) &&
                                     ( scale < 1.0001 ) ),
                          _name( "" ),
                          _encoding( "" ),
                          _size( 0 )
{
}


gkg::FontImplementation::~FontImplementation()
{

  std::list< gkg::FontImplementation* >::iterator
    f = _entry->fontImplementations.begin(),
    fe = _entry->fontImplementations.end();
  while ( f != fe )
  {

    if ( *f == this )
    {

      _entry->fontImplementations.erase( f );
      break;

    }
    ++ f;

  }

}


gkg::Display* gkg::FontImplementation::getDisplay() const
{

  return _display;

}


const std::string& gkg::FontImplementation::getName() const
{

  return _name;

}


const std::string& gkg::FontImplementation::getEncoding() const
{

  return _encoding;

}


float gkg::FontImplementation::getSize() const
{

  return _size;

}


float gkg::FontImplementation::getScale() const
{

  return _scale;

}


bool gkg::FontImplementation::isUnscaled() const
{

  return _unscaled;

}


void gkg::FontImplementation::setEntry( gkg::KnownFonts* entry )
{

  _entry = entry;

}


