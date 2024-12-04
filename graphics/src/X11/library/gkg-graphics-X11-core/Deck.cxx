#include <gkg-graphics-X11-core/Align.h>
#include <gkg-graphics-X11-core/Deck.h>
#include <gkg-graphics-X11-core/Hit.h>


gkg::Deck::Deck()
          : gkg::PolyGlyph(),
            _card( -1 ),
            _changed( true )
{
}


gkg::Deck::~Deck()
{
}


gkg::Glyph::Index gkg::Deck::getCard() const
{

  return _card;

}


void gkg::Deck::flipTo( gkg::Glyph::Index index )
{

  undraw();
  _card = index;

}


void 
gkg::Deck::request( gkg::GeometryRequisition& geometryRequisition ) const
{

  if ( _changed )
  {

    gkg::Glyph::Index count = getCount();
    gkg::GeometryRequisition*
      geometryRequisitions = new gkg::GeometryRequisition[ count ];
    for ( gkg::Glyph::Index i = 0; i < count; i++ )
    {

      gkg::Glyph* glyph = getComponent( i );
      if ( glyph )
      {

        glyph->request( geometryRequisitions[ i ] );

      }

    }
    gkg::Deck* d = ( gkg::Deck* )this;    // hack to be suppressed
    gkg::Align x( gkg::DimensionX );
    x.request( count, geometryRequisitions, d->_geometryRequisition );
    gkg::Align y( gkg::DimensionY );
    y.request( count, geometryRequisitions, d->_geometryRequisition );
    delete [] geometryRequisitions;
    d->_changed = false;

  }
  geometryRequisition = _geometryRequisition;

}


// When allocating a deck, we allocate the card (top component).
// We also make the extension cover the entire allocation;
// otherwise updating gets complicated in the presence of a flipTo.
// Usually, this won't hurt.
void gkg::Deck::allocate( gkg::Canvas* canvas,
                          const gkg::GeometryAllocation& geometryAllocation,
                          gkg::GeometryExtension& geometryExtension )
{

  _geometryAllocation = geometryAllocation;
  if ( ( _card >= 0 ) && ( _card < getCount() ) )
  {

    gkg::Glyph* glyph = getComponent( _card );
    if ( glyph )
    {

      glyph->allocate( canvas, geometryAllocation, geometryExtension);

    }
    geometryExtension.merge( canvas, geometryAllocation );

  }

}


void 
gkg::Deck::draw( gkg::Canvas* canvas,
                 const gkg::GeometryAllocation& geometryAllocation ) const
{

  if ( ( _card >= 0 ) && ( _card < getCount() ) )
  {

    gkg::Glyph* glyph = getComponent( _card );
    if ( glyph )
    {

      glyph->draw( canvas, geometryAllocation );

    }

  }

}


void 
gkg::Deck::print( gkg::Printer* printer,
                  const gkg::GeometryAllocation& geometryAllocation ) const
{

  if ( ( _card >= 0 ) && ( _card < getCount() ) )
  {

    gkg::Glyph* glyph = getComponent( _card );
    if ( glyph )
    {

      glyph->print( printer, geometryAllocation );

    }

  }

}


void gkg::Deck::pick( gkg::Canvas* canvas,
                      const gkg::GeometryAllocation& geometryAllocation,
                      int32_t depth,
                      gkg::Hit& hit )
{

  if ( ( _card >= 0 ) && ( _card < getCount() ) )
  {

    gkg::Glyph* glyph = getComponent( _card );
    if ( glyph )
    {

      hit.begin( depth, this, _card );
      glyph->pick( canvas, geometryAllocation, depth + 1, hit );
      hit.end();

    }

  }

}


void gkg::Deck::undraw()
{

  if ( ( _card >= 0 ) && ( _card < getCount() ) )
  {

    gkg::Glyph* glyph = getComponent( _card );
    if ( glyph )
    {

      glyph->undraw();

    }

  }

}


void gkg::Deck::modified( gkg::Glyph::Index )
{

  _changed = true;

}


void gkg::Deck::getGeometryAllotment(
                               gkg::Glyph::Index,
                               gkg::DimensionName dimensionName,
                               gkg::GeometryAllotment& geometryAllotment ) const
{

  geometryAllotment = _geometryAllocation.getAllotment( dimensionName );

}
