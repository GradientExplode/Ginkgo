#ifndef _gkg_graphics_X11_core_TBScrollBox_h_
#define _gkg_graphics_X11_core_TBScrollBox_h_


#include <gkg-graphics-X11-core/ScrollBox.h>


namespace gkg
{


class TBScrollBoxImplementation;


// top -> bottom scroll box
class TBScrollBox : public ScrollBox
{

  public:

    TBScrollBox( bool shrinkable );
    virtual ~TBScrollBox();

    virtual void request( GeometryRequisition& geometryRequisition ) const;
    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;
    virtual void pick( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation,
                       int32_t depth,
                       Hit& hit );
    virtual void undraw();

    virtual void modified( Glyph::Index index );
    virtual bool isShown( Glyph::Index index ) const;
    virtual Glyph::Index getFirstShown() const;
    virtual Glyph::Index getLastShown() const;
    virtual void getGeometryAllotment(
                                   Index index,
                                   DimensionName dimensionName,
                                   GeometryAllotment& geometryAllotment ) const;

    virtual FloatCoordinate getLower( DimensionName d ) const;
    virtual FloatCoordinate getUpper( DimensionName d ) const;
    virtual FloatCoordinate getLength( DimensionName d ) const;
    virtual FloatCoordinate getCurrentLower( DimensionName d ) const;
    virtual FloatCoordinate getCurrentUpper( DimensionName d ) const;
    virtual FloatCoordinate getCurrentLength( DimensionName d ) const;

    virtual void scrollForward( DimensionName dimensionName );
    virtual void scrollBackward( DimensionName dimensionName );
    virtual void pageForward( DimensionName dimensionName );
    virtual void pageBackward( DimensionName dimensionName );

    virtual void scrollTo( DimensionName dimensionName, FloatCoordinate lower );

    TBScrollBoxImplementation* getImplementation() const;

  private:

    void scrollBy( DimensionName dimensionName, int32_t offset );
    void doScroll( DimensionName dimensionName,
                   Glyph::Index newStart, Glyph::Index newEnd );

    TBScrollBoxImplementation* _tbScrollBoxImplementation;

};


}


#endif
