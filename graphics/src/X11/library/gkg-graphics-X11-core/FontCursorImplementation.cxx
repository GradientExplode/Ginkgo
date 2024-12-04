#include <gkg-graphics-X11-core/FontCursorImplementation.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <gkg-graphics-X11-core/Font.h>


gkg::FontCursorImplementation::FontCursorImplementation(
                                                 gkg::FontCursor* fontCursor,
                                                 const gkg::Font* font, 
                                                 int32_t pattern, int32_t mask )
                              : _target( fontCursor ),
                                _font( font ),
                                _pattern( pattern ),
                                _mask( mask )
{

  if ( _font )
  {

    _font->addReference();

  }

}


gkg::FontCursorImplementation::~FontCursorImplementation()
{

  if ( _font )
  {

    _font->removeReference();

  }

}
