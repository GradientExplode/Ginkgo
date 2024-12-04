#ifndef _gkg_graphics_X11_core_TBScrollBoxImplementation_h_
#define _gkg_graphics_X11_core_TBScrollBoxImplementation_h_


#include <deque>
#include <gkg-graphics-X11-core/Glyph.h>
#include <gkg-graphics-X11-core/Transformer.h>


namespace gkg
{


class TBScrollBox;
class Transformer;


class TBScrollBoxImplementation
{

  public:

    struct Info
    {

      Glyph* glyph;
      GeometryAllocation geometryAllocation;

    };

    TBScrollBoxImplementation( TBScrollBox* tbScrollBox,
                               bool shrinkable );
    virtual ~TBScrollBoxImplementation();

    TBScrollBox* getTarget() const;

    virtual void request( GeometryRequisition& geometryRequisition );
    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation );
    virtual void pick( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation,
                       int32_t depth,
                       Hit& hit );
    virtual void undraw();

    virtual void modified( Glyph::Index index );
    virtual bool isShown( Glyph::Index index );
    virtual Glyph::Index getFirstShown();
    virtual Glyph::Index getLastShown();
    virtual void getGeometryAllotment( Glyph::Index index,
                                       DimensionName dimensionName,
                                       GeometryAllotment& geometryAllotment );

    virtual Glyph::Index getStart() const;
    virtual Glyph::Index getEnd() const;

    void doScroll( DimensionName dimensionName,
                   Glyph::Index newStart,
                   Glyph::Index newEnd );
    void doRefresh();

  private:

    virtual void check( Canvas* canvas,
                        const GeometryAllocation& geometryAllocation );
    virtual void refresh();
    virtual void reallocate();
    virtual void redraw();
    virtual void undrawRange( Glyph::Index begin, Glyph::Index end );

    TBScrollBox* _target;
    bool _shrinkable;
    Glyph::Index _start;
    Glyph::Index _end;
    bool _changed;
    GeometryRequisition _geometryRequisition;
    Canvas* _canvas;
    Transformer _transformer;
    GeometryAllocation _geometryAllocation;
    GeometryExtension _geometryExtension;
    std::deque< Info > _visible;
    Info _partiallyVisible;

};


}


#endif
