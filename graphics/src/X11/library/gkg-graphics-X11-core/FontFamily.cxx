#include <gkg-graphics-X11-core/FontFamily.h>
#include <gkg-graphics-X11-core/FontFamilyImplementation.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>


gkg::FontFamily::FontFamily( const std::string& familyName )
                : _familyName( familyName )
{
}


gkg::FontFamily::~FontFamily()
{

  std::map< gkg::Display*, gkg::FontFamilyImplementation* >::iterator
    f = _fontFamilyImplementations.begin(),
    fe = _fontFamilyImplementations.end();
  while ( f != fe )
  {

    delete f->second;
    ++ f;

  }
  _fontFamilyImplementations.clear();

}


bool gkg::FontFamily::getFont( int32_t size,  const std::string& style,
                               std::string& name, float& scale )
{

  return getImplementation( Session::getInstance().getDefaultDisplay()
                           )->getFont( size, style, name, scale );

}


bool gkg::FontFamily::getFont( int32_t size, std::string& name, float& scale )
{

  return getFont( size, "", name, scale );

}


gkg::FontFamilyImplementation* 
gkg::FontFamily::getImplementation( gkg::Display* display )
{


  std::map< gkg::Display*,
            gkg::FontFamilyImplementation* >::const_iterator
    f = _fontFamilyImplementations.find( display ); 

  if ( f == _fontFamilyImplementations.end() )
  {

    gkg::FontFamilyImplementation* fontFamilyImpl = 
      gkg::getGUIImplementationFactory()->createFontFamilyImplementation( this,
                                                         display, _familyName );
    _fontFamilyImplementations[ display ] = fontFamilyImpl;
    return fontFamilyImpl;

  }
  else
  {

    return f->second;

  }

}
