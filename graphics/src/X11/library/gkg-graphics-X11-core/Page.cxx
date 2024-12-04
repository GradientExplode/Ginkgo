#include <gkg-graphics-X11-core/Page.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Hit.h>
#include <gkg-graphics-X11-core/Printer.h>


static const float epsilon = 0.1;


//
// struct gkg::Page::Info
//

gkg::Page::Info::Info()
                   : glyph( 0 ),
                     x( 0 ),
                     y( 0 ),
                     status( 0 )
{
}


//
// class Page
//

gkg::Page::Page( gkg::Glyph* background )
          : gkg::Glyph(),
            _background( background ),
            _canvas( 0 )
{

  if ( _background )
  {

    _background->addReference();

  }

}


gkg::Page::~Page()
{

  std::deque< gkg::Page::Info >::iterator i = _infos.begin(),
                                          ie = _infos.end();
  while ( i != ie )
  {

    if ( ( *i ).glyph )
    {

      ( *i ).glyph->removeReference();

    }
    ++ i;

  }
  _infos.clear();
  if ( _background )
  {

    _background->removeReference();

  }

}


gkg::FloatCoordinate gkg::Page::getLeft() const
{

  return _geometryAllocation.getLeft();

}


gkg::FloatCoordinate gkg::Page::getRight() const
{

  return _geometryAllocation.getRight();

}


gkg::FloatCoordinate gkg::Page::getBottom() const
{

  return _geometryAllocation.getBottom();

}


gkg::FloatCoordinate gkg::Page::getTop() const
{

  return _geometryAllocation.getTop();

}


gkg::FloatCoordinate gkg::Page::getX() const
{

  return _geometryAllocation.getX();

}


gkg::FloatCoordinate gkg::Page::getY() const
{

  return _geometryAllocation.getY();

}


void gkg::Page::move( gkg::Glyph::Index index,
                      gkg::FloatCoordinate x,
                      gkg::FloatCoordinate y )
{

  gkg::Page::Info& info = _infos[ index ];
  if ( ( info.status & gkg::Page::Info::Allocated ) && 
       ( ( info.x != x ) || ( info.y != y ) ) )
  {

    gkg::GeometryExtension& geometryExtension = info.geometryExtension;
    gkg::GeometryAllocation& geometryAllocation = info.geometryAllocation;
    gkg::GeometryAllotment&
      ax = geometryAllocation.getAllotment( gkg::DimensionX );
    gkg::GeometryAllotment&
      ay = geometryAllocation.getAllotment( gkg::DimensionY );
    gkg::FloatCoordinate newX = _geometryAllocation.getX() + x;
    gkg::FloatCoordinate newY = _geometryAllocation.getY() + y;
    gkg::GeometryAllotment newAx( newX, ax.getSpan(), ax.getAlignment() );
    gkg::GeometryAllotment newAy( newY, ay.getSpan(), ay.getAlignment() );
    geometryAllocation.setAllotment( gkg::DimensionX, newAx );
    geometryAllocation.setAllotment( gkg::DimensionY, newAy );
    if ( _canvas )
    {

      _canvas->setDamage( geometryExtension );

    }
    geometryExtension.clear();
    info.glyph->allocate( _canvas, geometryAllocation, geometryExtension );
    if ( _canvas )
    {

      _canvas->setDamage( geometryExtension );

    }

  }
  info.x = x;
  info.y = y;

}


void gkg::Page::getLocation( gkg::Glyph::Index index,
                             gkg::FloatCoordinate& x,
                             gkg::FloatCoordinate& y )
{

  gkg::Page::Info& info = _infos[ index ];
  x = info.x;
  y = info.y;

}


void gkg::Page::show( gkg::Glyph::Index index, bool showing )
{

  gkg::Page::Info& info = _infos[ index ];
  if ( ( ( info.status & gkg::Page::Info::Hidden ) == 0 ) != showing )
  {

    if ( _canvas )
    {

      _canvas->setDamage( info.geometryExtension );

    }
    if ( showing )
    {

      info.status &= ~gkg::Page::Info::Hidden;

    }
    else
    {

      info.status |= gkg::Page::Info::Hidden;

    }

  }

}


bool gkg::Page::isShowing( gkg::Glyph::Index index ) const
{

  return ( _infos[ index ].status & gkg::Page::Info::Hidden ) == 0;

}


void 
gkg::Page::request( gkg::GeometryRequisition& geometryRequisition ) const
{

  if ( _background )
  {

    _background->request( geometryRequisition );

  }

}


void gkg::Page::allocate( gkg::Canvas* canvas,
                         const gkg::GeometryAllocation& geometryAllocation,
                         gkg::GeometryExtension& geometryExtension )
{

  _canvas = canvas;
  _geometryAllocation = geometryAllocation;
  if ( _background )
  {

    _background->allocate( canvas, geometryAllocation, geometryExtension );

  }

  gkg::Glyph::Index count = ( gkg::Glyph::Index )_infos.size();
  for ( gkg::Glyph::Index index = 0; index < count; index++ )
  {

    gkg::Page::Info& info = _infos[ index ];
    if ( info.glyph )
    {

      gkg::GeometryAllocation& a = info.geometryAllocation;
      gkg::GeometryExtension& e = info.geometryExtension;
      gkg::GeometryRequisition r;
      info.glyph->request( r );
      gkg::GeometryAllotment
        ax = gkg::GeometryAllotment(
                           geometryAllocation.getX() + info.x,
                           r.getRequirement( gkg::DimensionX ).getNatural(),
                           r.getRequirement( gkg::DimensionX ).getAlignment() );
      gkg::GeometryAllotment
        ay = gkg::GeometryAllotment(
                           geometryAllocation.getY() + info.y,
                           r.getRequirement( gkg::DimensionY ).getNatural(),
                           r.getRequirement( gkg::DimensionY ).getAlignment() );
      if ( !( info.status & gkg::Page::Info::Allocated ) ||
           !ax.isEqualTo( a.getAllotment( gkg::DimensionX ), epsilon ) ||
           !ay.isEqualTo( a.getAllotment( gkg::DimensionY ), epsilon ) )
      {

        if ( canvas && ( info.status & gkg::Page::Info::Extended ) )
        {

          canvas->setDamage( e );

        }
        a.setAllotment( gkg::DimensionX, ax );
        a.setAllotment( gkg::DimensionY, ay );
        e.clear();
        info.glyph->allocate( canvas, a, e );
        if ( canvas )
        {

          canvas->setDamage( e );

        }

      }
      info.status |= gkg::Page::Info::Allocated |
                     gkg::Page::Info::Extended;
      geometryExtension.merge( e );

    }

  }

}


void gkg::Page::draw( gkg::Canvas* canvas,
                      const gkg::GeometryAllocation& geometryAllocation ) const
{

  if ( _background )
  {

    _background->draw( canvas, geometryAllocation );

  }
  gkg::Glyph::Index count = ( gkg::Glyph::Index )_infos.size();
  for ( gkg::Glyph::Index index = 0; index < count; index++ )
  {

    const gkg::Page::Info& info = _infos[ index ];
    if ( info.glyph && !( info.status & gkg::Page::Info::Hidden ) )
    {

      const gkg::GeometryAllocation& a = info.geometryAllocation;
      const gkg::GeometryExtension& e = info.geometryExtension;
      if ( canvas->isDamaged( e ) )
      {

        info.glyph->draw( canvas, a );

      }

    }

  }

}


void gkg::Page::print( gkg::Printer* printer,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  if ( _background )
  {

    _background->print( printer, geometryAllocation );

  }
  gkg::Glyph::Index count = ( gkg::Glyph::Index )_infos.size();
  for ( gkg::Glyph::Index index = 0; index < count; index++ )
  {

    const gkg::Page::Info& info = _infos[ index ];
    if ( info.glyph && !( info.status & gkg::Page::Info::Hidden ) )
    {

      const gkg::GeometryAllocation& a = info.geometryAllocation;
      const gkg::GeometryExtension& e = info.geometryExtension;
      if ( printer->isDamaged( e ) )
      {

        info.glyph->print( printer, a );

      }

    }

  }

}


void gkg::Page::pick( gkg::Canvas* canvas,
                      const gkg::GeometryAllocation& geometryAllocation,
                      int32_t depth,
                      gkg::Hit& hit )
{

  if ( _background )
  {

    _background->pick( canvas, geometryAllocation, depth, hit );

  }
  gkg::Glyph::Index count = ( gkg::Glyph::Index )_infos.size();
  for ( gkg::Glyph::Index index = 0; index < count; index++ )
  {

    gkg::Page::Info& info = _infos[ index ];
    if ( info.glyph && !( info.status & gkg::Page::Info::Hidden ) )
    {

      gkg::GeometryAllocation& a = info.geometryAllocation;
      if ( ( hit.getRight() >= a.getLeft() ) &&
           ( hit.getLeft() < a.getRight() ) &&
           ( hit.getTop() >= a.getBottom() ) &&
           ( hit.getBottom() < a.getTop() ) )
      {

        hit.begin( depth, this, index );
        info.glyph->pick( canvas, a, depth + 1, hit );
        hit.end();

      }

    }

  }

}


void gkg::Page::undraw()
{

  if ( _background )
  {

    _background->undraw();

  }
  gkg::Glyph::Index count = ( gkg::Glyph::Index )_infos.size();
  for ( gkg::Glyph::Index index = 0; index < count; index++ )
  {

    gkg::Page::Info& info = _infos[ index ];
    gkg::Glyph* glyph = info.glyph;
    if ( glyph )
    {

      glyph->undraw();

    }

  }
  _canvas = 0;

}


void gkg::Page::append( gkg::Glyph* other )
{

  gkg::Page::Info info;
  info.glyph = other;
  info.status = 0;
  info.x = 0;
  info.y = 0;
  _infos.push_back( info );
  if ( other )
  {

    other->addReference();

  }

}


void gkg::Page::prepend( gkg::Glyph* other )
{

  gkg::Page::Info info;
  info.glyph = other;
  info.status = 0;
  info.x = 0;
  info.y = 0;
  _infos.push_front( info );
  if ( other )
  {

    other->addReference();

  }

}


void gkg::Page::insert( gkg::Glyph::Index index, gkg::Glyph* other )
{

  gkg::Page::Info info;
  info.glyph = other;
  info.status = 0;
  info.x = 0;
  info.y = 0;
  _infos.insert( _infos.begin() + index, info );
  if ( other )
  {

    other->addReference();

  }

}


void gkg::Page::remove( gkg::Glyph::Index index )
{

  gkg::Page::Info& info = _infos[ index ];
  if ( _canvas && ( info.status & gkg::Page::Info::Allocated ) )
  {

    _canvas->setDamage( info.geometryExtension );

  }
  if ( info.glyph )
  {

    info.glyph->removeReference();

  }
  _infos.erase( _infos.begin() + index );

}


void gkg::Page::replace( gkg::Glyph::Index index, gkg::Glyph* other )
{

  gkg::Page::Info& info = _infos[ index ];
  if ( _canvas && ( info.status & gkg::Page::Info::Allocated ) )
  {

    _canvas->setDamage( info.geometryExtension );

  }
  if ( other )
  {

    other->addReference();

  }
  if ( info.glyph )
  {

    info.glyph->removeReference();

  }
  info.glyph = other;
  info.status &= ~( gkg::Page::Info::Allocated );

}


void gkg::Page::change( gkg::Glyph::Index index )
{

  gkg::Page::Info& info = _infos[ index ];
  info.status &= ~( gkg::Page::Info::Allocated );

}


gkg::Glyph::Index gkg::Page::getCount() const
{

  return ( gkg::Glyph::Index )_infos.size();

}


gkg::Glyph* gkg::Page::getComponent( gkg::Glyph::Index index ) const
{

  return _infos[ index ].glyph;

}


void gkg::Page::getGeometryAllotment(
                               gkg::Glyph::Index index,
                               gkg::DimensionName dimensionName,
                               gkg::GeometryAllotment& geometryAllotment ) const
{

  geometryAllotment =
               _infos[ index ].geometryAllocation.getAllotment( dimensionName );

}


