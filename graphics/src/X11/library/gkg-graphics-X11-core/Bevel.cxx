#include <gkg-graphics-X11-core/Bevel.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Printer.h>
#include <gkg-graphics-X11-core/Window.h>
#include <cmath>


gkg::Bevel::Bevel( gkg::Glyph* glyph,
                   gkg::Bevel::Beveler beveler,
                   const gkg::Color* light,
                   const gkg::Color* medium,
                   const gkg::Color* dark,
                   gkg::FloatCoordinate thickness,
                   float xAlign,
                   float yAlign,
                   bool hMargin,
                   bool vMargin )
           : gkg::BevelFrame( glyph, thickness,
                              xAlign, yAlign,
                              hMargin, vMargin ),
             _beveler( beveler ),
             _light( light ),
             _medium( medium ),
             _dark( dark )
{

  if ( _light )
  {

    _light->addReference();

  }
  if ( _medium )
  {

    _medium->addReference();

  }
  if ( _dark )
  {

    _dark->addReference();

  }

}


gkg::Bevel::~Bevel()
{

  if ( _light )
  {

    _light->removeReference();

  }
  if ( _medium )
  {

    _medium->removeReference();

  }
  if ( _dark )
  {

    _dark->removeReference();

  }

}


void gkg::Bevel::drawFrame( gkg::Canvas* canvas,
                            const gkg::GeometryAllocation& geometryAllocation,
                            gkg::FloatCoordinate thickness ) const
{

  ( *_beveler )( canvas,
                 _light, _medium, _dark,
                 thickness,
                 geometryAllocation.getLeft(),
                 geometryAllocation.getBottom(),
                 geometryAllocation.getRight(),
                 geometryAllocation.getTop() );

}


void gkg::Bevel::drawRectangle( gkg::Canvas* canvas,
                                const gkg::Color* light,
                                const gkg::Color* medium,
                                const gkg::Color* dark,
                                gkg::FloatCoordinate thickness,
                                gkg::FloatCoordinate left,
                                gkg::FloatCoordinate bottom,
                                gkg::FloatCoordinate right,
                                gkg::FloatCoordinate top )
{

  // background
  if ( medium )
  {

    canvas->fillRectangle( left, bottom, right, top, medium );

  }

  gkg::FloatCoordinate leftInside = left + thickness;
  gkg::FloatCoordinate bottomInside = bottom + thickness;
  gkg::FloatCoordinate rightInside = right - thickness;
  gkg::FloatCoordinate topInside = top - thickness;

  // left edge
  canvas->newPath();
  canvas->moveTo( left, bottom );
  canvas->lineTo( left, top );
  canvas->lineTo( leftInside, top );
  canvas->lineTo( leftInside, bottom );
  canvas->closePath();
  canvas->fill( light );

  // top edge
  canvas->newPath();
  canvas->moveTo( leftInside, topInside );
  canvas->lineTo( leftInside, top );
  canvas->lineTo( right, top );
  canvas->lineTo( right, topInside );
  canvas->closePath();
  canvas->fill( light );

  // right edge
  canvas->newPath();
  canvas->moveTo( rightInside, bottomInside);
  canvas->lineTo( rightInside, topInside);
  canvas->lineTo( right, top);
  canvas->lineTo( right, bottom);

  // bottom edge (as part of same path)
  canvas->lineTo( left, bottom );
  canvas->lineTo( leftInside, bottomInside );
  canvas->lineTo( rightInside, bottomInside );
  canvas->closePath();
  canvas->fill( dark );

}


// A bit of a misnomer to call these arrows; they are really beveled
// triangles.  The only tricky part is dropping the bevel down and to the
// right of the original triangle, which requires finding delta x,y for
// a given thickness and matching the slope of the triangle.

void gkg::Bevel::drawLeftArrow( gkg::Canvas* canvas,
                                const gkg::Color* light,
                                const gkg::Color* medium,
                                const gkg::Color* dark,
                                gkg::FloatCoordinate thickness,
                                gkg::FloatCoordinate left,
                                gkg::FloatCoordinate bottom,
                                gkg::FloatCoordinate right,
                                gkg::FloatCoordinate top )
{

  gkg::FloatCoordinate centerY = ( bottom + top ) * 0.5;
  float slope = ( ( top - bottom ) / ( right - left ) ) * 0.5;
  float deltaX = thickness / sqrt( slope * slope + 1 );
  float deltaY = slope * deltaX;

  canvas->newPath();
  canvas->moveTo( left, centerY );
  canvas->lineTo( right, top );
  canvas->lineTo( right, bottom );
  canvas->closePath();
  canvas->fill( medium );

  canvas->newPath();
  canvas->moveTo( right - thickness, bottom + thickness );
  canvas->lineTo( right - thickness, top - thickness );
  canvas->lineTo( right, top );
  canvas->lineTo( right, bottom );
  canvas->closePath();
  canvas->fill( dark );

  canvas->newPath();
  canvas->moveTo( left, centerY );
  canvas->lineTo( left + thickness, centerY );
  canvas->lineTo( right - thickness, bottom + thickness );
  canvas->lineTo( right, bottom );
  canvas->closePath();
  canvas->fill( dark );

  canvas->newPath();
  canvas->moveTo( left + deltaX, centerY - deltaY );
  canvas->lineTo( left, centerY );
  canvas->lineTo( right, top);
  canvas->lineTo( right, top - thickness );
  canvas->closePath();
  canvas->fill( light );

}

void gkg::Bevel::drawRightArrow( gkg::Canvas* canvas,
                                 const gkg::Color* light,
                                 const gkg::Color* medium,
                                 const gkg::Color* dark,
                                 gkg::FloatCoordinate thickness,
                                 gkg::FloatCoordinate left,
                                 gkg::FloatCoordinate bottom,
                                 gkg::FloatCoordinate right,
                                 gkg::FloatCoordinate top )
{

  gkg::FloatCoordinate centerY = ( bottom + top ) * 0.5;
  float slope = ( ( top - bottom ) / ( right - left ) ) * 0.5;
  float deltaX = thickness / sqrt( slope * slope + 1 );
  float deltaY = slope * deltaX;

  canvas->newPath();
  canvas->moveTo( left, bottom );
  canvas->lineTo( left, top );
  canvas->lineTo( right, centerY );
  canvas->closePath();
  canvas->fill( medium );

  canvas->newPath();
  canvas->moveTo( left, bottom );
  canvas->lineTo( left + thickness, bottom + thickness );
  canvas->lineTo( right - thickness, centerY );
  canvas->lineTo( right, centerY );
  canvas->closePath();
  canvas->fill( dark );

  canvas->newPath();
  canvas->moveTo( left, bottom );
  canvas->lineTo( left, top );
  canvas->lineTo( left + thickness, top - thickness );
  canvas->lineTo( left + deltaX, bottom + deltaY );
  canvas->closePath();
  canvas->fill( light );

  canvas->newPath();
  canvas->moveTo( left + thickness, top - thickness );
  canvas->lineTo( left, top );
  canvas->lineTo( right, centerY );
  canvas->lineTo( right - deltaX, centerY - deltaY );
  canvas->closePath();
  canvas->fill( light );

}

void gkg::Bevel::drawUpArrow( gkg::Canvas* canvas,
                              const gkg::Color* light,
                              const gkg::Color* medium,
                              const gkg::Color* dark,
                              gkg::FloatCoordinate thickness,
                              gkg::FloatCoordinate left,
                              gkg::FloatCoordinate bottom,
                              gkg::FloatCoordinate right,
                              gkg::FloatCoordinate top )
{

  gkg::FloatCoordinate centerX = ( left + right ) * 0.5;
  float slope = 2 * ( ( top - bottom ) / ( right - left ) );
  float deltaX = thickness / sqrt( slope * slope + 1 );
  float deltaY = slope * deltaX;

  canvas->newPath();
  canvas->moveTo( left, bottom );
  canvas->lineTo( centerX, top );
  canvas->lineTo( right, bottom );
  canvas->closePath();
  canvas->fill( medium );

  canvas->newPath();
  canvas->moveTo( left, bottom );
  canvas->lineTo( right, bottom );
  canvas->lineTo( right - thickness, bottom + thickness );
  canvas->lineTo( left + thickness, bottom + thickness );
  canvas->closePath();
  canvas->fill( dark );

  canvas->newPath();
  canvas->moveTo( centerX, top - thickness );
  canvas->lineTo( centerX, top );
  canvas->lineTo( right, bottom);
  canvas->lineTo( right - thickness, bottom + thickness );
  canvas->closePath();
  canvas->fill( dark );

  canvas->newPath();
  canvas->moveTo( left, bottom );
  canvas->lineTo( left + deltaX, bottom );
  canvas->lineTo( centerX + deltaX, top - deltaY );
  canvas->lineTo( centerX, top );
  canvas->closePath();
  canvas->fill( light );

}


void gkg::Bevel::drawDownArrow( gkg::Canvas* canvas,
                                const gkg::Color* light,
                                const gkg::Color* medium,
                                const gkg::Color* dark,
                                gkg::FloatCoordinate thickness,
                                gkg::FloatCoordinate left,
                                gkg::FloatCoordinate bottom,
                                gkg::FloatCoordinate right,
                                gkg::FloatCoordinate top )
{

  gkg::FloatCoordinate centerX = ( left + right ) * 0.5;
  float slope = 2 * ( ( top - bottom ) / ( right - left ) );
  float deltaX = thickness / sqrt( slope * slope + 1 );
  float deltaY = slope * deltaX;

  canvas->newPath();
  canvas->moveTo( left, top );
  canvas->lineTo( right, top );
  canvas->lineTo( centerX, bottom );
  canvas->closePath();
  canvas->fill( medium );

  canvas->newPath();
  canvas->moveTo( centerX, bottom );
  canvas->lineTo( centerX, bottom + thickness );
  canvas->lineTo( right - thickness, top - thickness );
  canvas->lineTo( right, top );
  canvas->closePath();
  canvas->fill( dark );

  canvas->newPath();
  canvas->moveTo( left, top );
  canvas->lineTo( centerX, bottom );
  canvas->lineTo( centerX + deltaX, bottom + deltaY );
  canvas->lineTo( left + thickness, top - thickness );
  canvas->closePath();
  canvas->fill( light );

  canvas->newPath();
  canvas->moveTo( left, top );
  canvas->lineTo( right, top );
  canvas->lineTo( right - deltaX, top - deltaY );
  canvas->lineTo( left + thickness, top - thickness );
  canvas->closePath();
  canvas->fill( light );

}


void gkg::Bevel::drawDiamond( gkg::Canvas* canvas,
                              const gkg::Color* light,
                              const gkg::Color* medium,
                              const gkg::Color* dark,
                              gkg::FloatCoordinate thickness,
                              gkg::FloatCoordinate left,
                              gkg::FloatCoordinate bottom,
                              gkg::FloatCoordinate right,
                              gkg::FloatCoordinate top )
{

  gkg::FloatCoordinate xMid = ( left + right ) * 0.5;
  gkg::FloatCoordinate yMid = ( bottom + top ) * 0.5;
  gkg::FloatCoordinate leftInside = left + thickness;
  gkg::FloatCoordinate topInside = top - thickness;
  gkg::FloatCoordinate rightInside = right - thickness;
  gkg::FloatCoordinate bottomInside = bottom + thickness;

  // interior of diamond
  canvas->newPath();
  canvas->moveTo( left, yMid );
  canvas->lineTo( xMid, top );
  canvas->lineTo( right, yMid );
  canvas->lineTo( xMid, bottom );
  canvas->closePath();
  canvas->fill( medium );

  // lower half
  canvas->newPath();
  canvas->moveTo( left, yMid );
  canvas->lineTo( xMid, bottom );
  canvas->lineTo( right, yMid );
  canvas->lineTo( rightInside, yMid );
  canvas->lineTo( xMid, bottomInside );
  canvas->lineTo( leftInside, yMid );
  canvas->closePath();
  canvas->fill( dark );

  // upper half
  canvas->newPath();
  canvas->moveTo( left, yMid );
  canvas->lineTo( xMid, top );
  canvas->lineTo( right, yMid );
  canvas->lineTo( rightInside, yMid );
  canvas->lineTo( xMid, topInside );
  canvas->lineTo( leftInside, yMid );
  canvas->closePath();
  canvas->fill( light );

}
