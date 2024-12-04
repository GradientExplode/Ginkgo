#ifndef _gkg_graphics_X11_core_LRAutoScrollBox_h_
#define _gkg_graphics_X11_core_LRAutoScrollBox_h_


#include <gkg-graphics-X11-core/Patch.h>


namespace gkg
{


class Color;
class PolyGlyph;
class LRScrollBox;


class LRAutoScrollBox : public Patch
{

  public:

    LRAutoScrollBox( bool shrinkable );
    virtual ~LRAutoScrollBox();

    virtual void request( GeometryRequisition& geometryRequisition ) const;
    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );

    virtual bool isShown( Glyph::Index index ) const;
    virtual Glyph::Index getFirstShown() const;
    virtual Glyph::Index getLastShown() const;

    virtual void append( Glyph* other );
    virtual void prepend( Glyph* other );
    virtual void insert( Glyph::Index index, Glyph* other );
    virtual void remove( Glyph::Index index );
    virtual void replace( Glyph::Index index, Glyph* other );

    virtual Glyph::Index getCount() const;
    virtual Glyph* getComponent( Glyph::Index index ) const;

    virtual void setSmallScroll( FloatCoordinate floatCoordinate );
    virtual FloatCoordinate getSmallScroll() const;
    virtual void setLargeScroll( FloatCoordinate floatCoordinate );
    virtual FloatCoordinate getLargeScroll() const;

    virtual void scrollForward();
    virtual void scrollBackward();
    virtual void pageForward();
    virtual void pageBackward();

    virtual void scrollTo( FloatCoordinate lower );

  protected:

    void activateSlider( bool value );

    PolyGlyph* _vBox;
    LRScrollBox* _lrScrollBox;
    bool _sliderOn;

};


}


#endif
