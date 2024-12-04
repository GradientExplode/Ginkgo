#include <gkg-graphics-X11-core/BitmapCursorImplementation.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <gkg-graphics-X11-core/Bitmap.h>


gkg::BitmapCursorImplementation::BitmapCursorImplementation(
                                                gkg::BitmapCursor* bitmapCursor,
                                                const gkg::Bitmap* pattern,
                                                const gkg::Bitmap* mask )
                                : _target( bitmapCursor ),
                                  _pattern( pattern ),
                                  _mask( mask )
{

  if ( _pattern )
  {

    _pattern->addReference();

  }
  if ( _mask )
  {

    _mask->addReference();

  }

}


gkg::BitmapCursorImplementation::~BitmapCursorImplementation()
{

  if ( _pattern )
  {

    _pattern->removeReference();

  }
  if ( _mask )
  {

    _mask->removeReference();

  }

}
