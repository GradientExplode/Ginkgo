#include <gkg-graphics-X11-core/Placement.h>
#include <gkg-graphics-X11-core/Layout.h>
#include <gkg-graphics-X11-core/Printer.h>


static void place( gkg::Glyph* glyph,
                   const gkg::GeometryAllocation& given,
                   gkg::Layout* layout,
                   gkg::GeometryAllocation& result )
{

  gkg::GeometryRequisition geometryRequisition;
  glyph->request( geometryRequisition );
  layout->allocate( given, 1, &geometryRequisition, &result );

}


gkg::Placement::Placement( gkg::Glyph* body, gkg::Layout* layout )
               : gkg::MonoGlyph( body ),
                 _layout( layout )
{
}


gkg::Placement::~Placement()
{

  delete _layout;

}


void
gkg::Placement::request( gkg::GeometryRequisition& geometryRequisition) const
{

  gkg::Glyph* glyph = getBody();
  if ( glyph )
  {

    glyph->request( geometryRequisition );

  }
  _layout->request( 0, 0, geometryRequisition );

}



void
gkg::Placement::allocate( gkg::Canvas* canvas,
                          const gkg::GeometryAllocation& geometryAllocation,
                          gkg::GeometryExtension& geometryExtension )
{

  gkg::Glyph* glyph = getBody();
  if ( glyph )
  {

    gkg::GeometryAllocation g( geometryAllocation );
    place( glyph, geometryAllocation, _layout, g );
    glyph->allocate( canvas, g, geometryExtension );

  }

}    


void 
gkg::Placement::draw( gkg::Canvas* canvas,
                      const gkg::GeometryAllocation& geometryAllocation ) const
{

  gkg::Glyph* glyph = getBody();
  if ( glyph  )
  {

    gkg::GeometryAllocation g( geometryAllocation );
    place( glyph, geometryAllocation, _layout, g );
    glyph->draw( canvas, g );

  }

}


void 
gkg::Placement::print( gkg::Printer* printer,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  gkg::Glyph* glyph = getBody();
  if ( glyph  )
  {

    gkg::GeometryAllocation g( geometryAllocation );
    place( glyph, geometryAllocation, _layout, g );
    glyph->print( printer, g );

  }

}


void gkg::Placement::pick( gkg::Canvas* canvas,
                           const gkg::GeometryAllocation& geometryAllocation,
                           int32_t depth,
                           gkg::Hit& hit )
{

  gkg::Glyph* glyph = getBody();
  if ( glyph  )
  {

    gkg::GeometryAllocation g( geometryAllocation );
    place( glyph, geometryAllocation, _layout, g );
    glyph->pick( canvas, g, depth, hit );

  }

}
