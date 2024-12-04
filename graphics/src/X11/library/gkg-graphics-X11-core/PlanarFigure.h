#ifndef _gkg_graphics_X11_core_PlanarFigure_h_
#define _gkg_graphics_X11_core_PlanarFigure_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class Brush;
class Color;


// planar figures
class PlanarFigure : public Glyph 
{

  public:

    virtual void request( GeometryRequisition& geometryRequisition ) const;
    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;

  protected:

    PlanarFigure( const Brush* brush,
                  const Color* stroke,
                  const Color* fill,
                  bool closed,
                  bool curved,
                  int32_t coords );
    virtual ~PlanarFigure();

    void addPoint( FloatCoordinate x, FloatCoordinate y );
    void addCurve( FloatCoordinate x, FloatCoordinate y,
                   FloatCoordinate x1, FloatCoordinate y1,
                   FloatCoordinate x2, FloatCoordinate y2 );
    void bSplineMoveTo( FloatCoordinate x, FloatCoordinate y,
                        FloatCoordinate x1, FloatCoordinate y1,
                        FloatCoordinate x2, FloatCoordinate y2 );
    void bSplineCurveTo( FloatCoordinate x, FloatCoordinate y,
                         FloatCoordinate x1, FloatCoordinate y1,
                         FloatCoordinate x2, FloatCoordinate y2 );

    const Brush* _brush;
    const Color* _stroke;
    const Color* _fill;

    bool _closed;
    bool _curved;
    int32_t _count;
    FloatCoordinate* _x;
    FloatCoordinate* _y;

    FloatCoordinate _xMin;
    FloatCoordinate _xMax;
    FloatCoordinate _yMin;
    FloatCoordinate _yMax;

};


class PlanarFigureLine : public PlanarFigure
{

  public:

    PlanarFigureLine( const Brush* brush,
                      const Color* stroke,
                      const Color* fill,
                      FloatCoordinate x1, FloatCoordinate y1,
                      FloatCoordinate x2, FloatCoordinate y2 );

  protected:

    virtual ~PlanarFigureLine();

};


class PlanarFigureRectangle : public PlanarFigure
{

  public:

    PlanarFigureRectangle( const Brush* brush,
                           const Color* stroke,
                           const Color* fill,
                           FloatCoordinate left,
                           FloatCoordinate bottom,
                           FloatCoordinate right,
                           FloatCoordinate top );

  protected:

    virtual ~PlanarFigureRectangle();

};


class PlanarFigureCircle : public PlanarFigure
{

  public:

    PlanarFigureCircle( const Brush* brush,
                        const Color* stroke,
                        const Color* fill,
                        FloatCoordinate x, FloatCoordinate y,
                        FloatCoordinate radius );

  protected:

    virtual ~PlanarFigureCircle();

};


class PlanarFigureEllipse : public PlanarFigure
{

  public:

    PlanarFigureEllipse( const Brush* brush,
                         const Color* stroke,
                         const Color* fill,
                         FloatCoordinate x, FloatCoordinate y,
                         FloatCoordinate radiusX, FloatCoordinate radiusY );

  protected:

    virtual ~PlanarFigureEllipse();

};


class PlanarFigureOpenBSpline : public PlanarFigure
{

  public:

    PlanarFigureOpenBSpline( const Brush* brush,
                             const Color* stroke,
                             const Color* fill,
                             FloatCoordinate* x,
                             FloatCoordinate* y,
                             int32_t count );

  protected:

    virtual ~PlanarFigureOpenBSpline();

};


class PlanarFigureClosedBSpline : public PlanarFigure
{

  public:

    PlanarFigureClosedBSpline( const Brush* brush,
                               const Color* stroke,
                               const Color* fill,
                               FloatCoordinate* x,
                               FloatCoordinate* y,
                               int32_t count );

  protected:

    virtual ~PlanarFigureClosedBSpline();

};


class PlanarFigurePolyline : public PlanarFigure
{

  public:

    PlanarFigurePolyline( const Brush* brush,
                          const Color* stroke,
                          const Color* fill,
                          FloatCoordinate* x,
                          FloatCoordinate* y,
                          int32_t count );

  protected:

    virtual ~PlanarFigurePolyline();

};


class PlanarFigurePolygon : public PlanarFigure
{

  public:

    PlanarFigurePolygon( const Brush* brush,
                         const Color* stroke,
                         const Color* fill,
                         FloatCoordinate* x,
                         FloatCoordinate* y,
                         int32_t count );

  protected:

    virtual ~PlanarFigurePolygon();

};


}


#endif
