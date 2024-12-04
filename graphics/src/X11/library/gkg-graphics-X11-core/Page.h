#ifndef _gkg_graphics_X11_core_Page_h_
#define _gkg_graphics_X11_core_Page_h_


#include <deque>
#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


// arbitrary placements
class Page : public Glyph
{

  public:

    struct Info
    {

      enum Status
      {

        Allocated = 0x01,
        Extended = 0x02,
        Positioned = 0x04,
        Hidden = 0x08

      };

      Info();

      Glyph* glyph;
      GeometryAllocation geometryAllocation;
      GeometryExtension geometryExtension;
      FloatCoordinate x;
      FloatCoordinate y;
      int32_t status;

    };

    Page( Glyph* background );
    virtual ~Page();

    FloatCoordinate getLeft() const;
    FloatCoordinate getRight() const;
    FloatCoordinate getBottom() const;
    FloatCoordinate getTop() const;
    FloatCoordinate getX() const;
    FloatCoordinate getY() const;

    void move( Glyph::Index index, FloatCoordinate x, FloatCoordinate y );
    void getLocation( Glyph::Index index,
                      FloatCoordinate& x, FloatCoordinate& y );
    void show( Glyph::Index index, bool showing );
    bool isShowing( Glyph::Index index ) const;

    virtual void request( GeometryRequisition& geometryRequisition ) const;
    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;
    virtual void print( Printer* printer,
                        const GeometryAllocation& geometryAllocation ) const;
    virtual void pick( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation,
                       int32_t depth,
                       Hit& hit );
    virtual void undraw();

    virtual void append( Glyph* other );
    virtual void prepend( Glyph* other );
    virtual void insert( Glyph::Index index, Glyph* other );
    virtual void remove( Glyph::Index index );
    virtual void replace( Glyph::Index index, Glyph* other );
    virtual void change( Glyph::Index index );

    virtual Index getCount() const;
    virtual Glyph* getComponent( Index index ) const;
    virtual void getGeometryAllotment(
                                   Index index,
                                   DimensionName dimensionName,
                                   GeometryAllotment& geometryAllotment ) const;

  private:

    std::deque< Info > _infos;
    Glyph* _background;
    Canvas* _canvas;
    GeometryAllocation _geometryAllocation;

};


}


#endif
