#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/FontImplementation.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-processing-algobase/Math.h>



//
// class FontImplementationLut
//

gkg::FontImplementationLut::NameToKnownFontsHashTable 
gkg::FontImplementationLut::_fonts;


gkg::FontImplementationLut::FontImplementationLut( const std::string& name,
                                                   float scale )
                           : _name( name ),
                             _scale( scale ),
                             _entry( 0 )
{
}


gkg::FontImplementationLut::~FontImplementationLut()
{

  std::list< gkg::FontImplementation* >::iterator
    f = _fontImplementations.begin(), fe = _fontImplementations.end();
  while ( f != fe )
  {

    if ( *f )
    {

      ( *f )->addReference();

    }
    ++ f;

  }

}


void gkg::FontImplementationLut::remove( const gkg::Font* font )
{

  if ( _entry )
  {

    std::list< gkg::Font* >::iterator
      f = _entry->fonts.begin(), fe = _entry->fonts.end();
    while ( f != fe )
    {

      if ( *f == font )
      {
        _entry->fonts.erase( f );
        break;

      }
      ++ f;

    }
    if ( _entry->fonts.empty() && _entry->fontImplementations.empty() )
    {

      _fonts.erase( _fonts.find( _name ) );
      delete _entry;

    }
    _entry = 0;

  }

}


gkg::FontImplementation* 
gkg::FontImplementationLut::getImplementation( gkg::Display* display )
{

  gkg::FontImplementation* fontImpl = 0;

  std::list< gkg::FontImplementation* >::const_iterator
    f = _fontImplementations.begin(), fe = _fontImplementations.end();
  while ( f != fe )
  {

    fontImpl = *f;
    if ( fontImpl->getDisplay() == display )
    {

      return fontImpl;

    }
    ++ f;

  }


  gkg::KnownFonts* knownFonts = 0;
  gkg::FontImplementationLut::NameToKnownFontsHashTable::const_iterator 
    k = _fonts.find( _name );
  if ( k != _fonts.end() )
  {

    knownFonts = k->second;
    fontImpl = findImplementation( knownFonts->fontImplementations,
                                   display,
                                   _scale );
    if ( fontImpl )
    {

      attach( fontImpl );
      return fontImpl;

    }

  }

  fontImpl = gkg::getGUIImplementationFactory()->createFontImplementation(
                                                       display, _name, _scale );
  addNewImplementation( known( knownFonts, _name ), fontImpl );
  return fontImpl;

}


gkg::FontImplementation* 
gkg::FontImplementationLut::getDefaultImplementation()
{

  if ( !_fontImplementations.empty() )
  {

    return _fontImplementations.back();

  }
  return getImplementation(gkg::Session::getInstance().getDefaultDisplay() );

}


void gkg::FontImplementationLut::addNewImplementation(
                                             gkg::KnownFonts* knownFonts,
                                             gkg::FontImplementation* fontImpl )
{

  fontImpl->setEntry( knownFonts );
  knownFonts->fontImplementations.push_back( fontImpl );
  attach( fontImpl );

}


void gkg::FontImplementationLut::setEntry( gkg::KnownFonts* entry )
{

  _entry = entry;

}


void 
gkg::FontImplementationLut::attach( gkg::FontImplementation* fontImpl )
{

  _fontImplementations.push_back( fontImpl );
  if ( fontImpl )
  {

    fontImpl->addReference();

  }

}


const gkg::Font* gkg::FontImplementationLut::lookup( gkg::Display* display,
                                                     const std::string& name,
                                                     float scale )
{

  gkg::FontImplementation* fontImpl = 0;
  const gkg::Font* font = 0;
  gkg::KnownFonts* knownFonts = 0;
  gkg::FontImplementationLut::NameToKnownFontsHashTable::iterator
    k = _fonts.find( name );

  if ( k != _fonts.end() )
  {

    knownFonts = k->second;
    std::list< gkg::Font* >::iterator f = knownFonts->fonts.begin(),
                                         fe = knownFonts->fonts.end();
    while ( f != fe )
    {

      font = *f;
      if ( gkg::equal( font->getScale(), scale, 0.0001f ) )
      {

        return font;

      }
      ++ f;

    }

    fontImpl = findImplementation( knownFonts->fontImplementations, 
                                   display,
                                   scale );
    if ( fontImpl )
    {

      return newFont( name, scale, knownFonts, fontImpl );

    }

  }

  fontImpl = gkg::getGUIImplementationFactory()->createFontImplementation(
                                                         display, name, scale );
  knownFonts = known( knownFonts, name );
  font = newFont( name, scale, knownFonts, fontImpl );
  font->_fontImplementationLut->addNewImplementation( knownFonts,fontImpl );
  font->_fontImplementationLut->setEntry( knownFonts );
  return font;

}


gkg::FontImplementation*
gkg::FontImplementationLut::findImplementation(
               const std::list< gkg::FontImplementation* >& fontImplementations,
               Display* display, float scale )
{

  std::list< gkg::FontImplementation* >::const_iterator
    f = fontImplementations.begin(), fe = fontImplementations.end();
  while ( f != fe )
  {

    gkg::FontImplementation* fontImpl =  *f;
    if ( ( fontImpl->getDisplay() == display ) &&
         gkg::equal( fontImpl->getScale(), scale, 0.0001f ) )
    {

      return fontImpl;

    }
    ++ f;

  }
  return 0;

}


const gkg::Font* gkg::FontImplementationLut::newFont(
                                   const std::string& name,
                                   float scale,
                                   gkg::KnownFonts* knownFonts,
                                   gkg::FontImplementation* fontImplementation )
{

  gkg::Font* font = new gkg::Font( name, scale );
  font->_fontImplementationLut->attach( fontImplementation );
  knownFonts->fonts.push_back( font );
  return font;

}


gkg::KnownFonts* gkg::FontImplementationLut::known( gkg::KnownFonts* knownFonts,
                                                    const std::string& name )
{

  if ( knownFonts )
  {

    return knownFonts;

  }
  gkg::KnownFonts* newKnownFonts = new gkg::KnownFonts;
  _fonts[ name ] = newKnownFonts;
  return newKnownFonts;

}


//
// class Font
//

gkg::Font::Font( const std::string& name, float scale )
          : gkg::RCGUIObject(),
            _fontImplementationLut( new gkg::FontImplementationLut( name,
                                                                    scale ) )
{
}


gkg::Font::~Font()
{

  delete _fontImplementationLut;

}


void gkg::Font::cleanup()
{

  _fontImplementationLut->remove( this );

}


const std::string& gkg::Font::getName() const
{

  return _fontImplementationLut->getDefaultImplementation()->getName();

}


const std::string& gkg::Font::getEncoding() const
{

  return _fontImplementationLut->getDefaultImplementation()->getEncoding();

}


float gkg::Font::getSize() const
{

  return _fontImplementationLut->getDefaultImplementation()->getSize();

}


float gkg::Font::getScale() const
{

  return _fontImplementationLut->getDefaultImplementation()->getScale();

}


void gkg::Font::getFontBoundingBox( gkg::FontBoundingBox& bbox ) const
{

  _fontImplementationLut->getDefaultImplementation()->getFontBoundingBox( bbox);

}


void gkg::Font::getCharBoundingBox( int32_t c, 
                                    gkg::FontBoundingBox& bbox ) const
{

  _fontImplementationLut->getDefaultImplementation()->getCharBoundingBox( c,
                                                                         bbox );

}


void gkg::Font::getStringBoundingBox( const std::string& s,
                                      gkg::FontBoundingBox& bbox ) const
{

  _fontImplementationLut->getDefaultImplementation()->getStringBoundingBox( s,
                                                                         bbox );

}


gkg::FloatCoordinate gkg::Font::getWidth( int32_t c ) const
{

  return _fontImplementationLut->getDefaultImplementation()->getWidth( c );

}


gkg::FloatCoordinate gkg::Font::getWidth( const std::string& s ) const
{

  return _fontImplementationLut->getDefaultImplementation()->getWidth( s );

}


gkg::FloatCoordinate gkg::Font::getHeight() const
{

  gkg::FontBoundingBox bbox;
  _fontImplementationLut->getDefaultImplementation()->getFontBoundingBox( bbox);
  return bbox.ascent + bbox.descent;

}


int32_t gkg::Font::getIndex( const std::string& s, float offset,
                             bool between ) const
{

  return _fontImplementationLut->getDefaultImplementation()->getIndex( s,
                                                                       offset,
                                                                       between);

}


gkg::FontImplementation* 
gkg::Font::getImplementation( gkg::Display* display ) const
{

  return _fontImplementationLut->getImplementation( display );

}


const gkg::Font* gkg::Font::lookup( const std::string& name )
{

  return gkg::FontImplementationLut::lookup(
                      Session::getInstance().getDefaultDisplay(), name, 1.0f );

}


bool gkg::Font::exists( gkg::Display* display, const std::string& name )
{

  return gkg::FontImplementationLut::lookup( display, name, 1.0 ) != 0;

}


