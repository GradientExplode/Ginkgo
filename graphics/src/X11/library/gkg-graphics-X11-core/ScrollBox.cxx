#include <gkg-graphics-X11-core/ScrollBox.h>


gkg::ScrollBox::ScrollBox()
               : gkg::PolyGlyph()
{
}


gkg::ScrollBox::~ScrollBox()
{
}


bool gkg::ScrollBox::isShown( gkg::Glyph::Index index ) const
{

  return ( index >= 0 ) && ( index < getCount() );

}


gkg::Glyph::Index gkg::ScrollBox::getFirstShown() const
{

  return 0;

}


gkg::Glyph::Index gkg::ScrollBox::getLastShown() const
{

  return getCount() - 1;

}
