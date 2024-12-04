#ifndef _gkg_graphics_X11_core_MarginLayout_h_
#define _gkg_graphics_X11_core_MarginLayout_h_


#include <gkg-graphics-X11-core/Layout.h>


namespace gkg
{


class MarginLayout : public Layout
{

  public:

    MarginLayout( FloatCoordinate margin );
    MarginLayout( FloatCoordinate hMargin,
                  FloatCoordinate vMargin );
    MarginLayout( FloatCoordinate lMargin,
                  FloatCoordinate rMargin,
                  FloatCoordinate bMargin,
                  FloatCoordinate tMargin );
    MarginLayout( FloatCoordinate lMargin,
                  FloatCoordinate lStretch,
                  FloatCoordinate lShrink,
                  FloatCoordinate rMargin,
                  FloatCoordinate rStretch,
                  FloatCoordinate rShrink,
                  FloatCoordinate bMargin,
                  FloatCoordinate bStretch,
                  FloatCoordinate bShrink,
                  FloatCoordinate tMargin,
                  FloatCoordinate tStretch,
                  FloatCoordinate tShrink );
    virtual ~MarginLayout();

    virtual void request( Glyph::Index count,
                          const GeometryRequisition* geometryRequisitions,
                          GeometryRequisition& result );
    virtual void allocate( const GeometryAllocation& given,
                           Glyph::Index count,
                           const GeometryRequisition* geometryRequisitions,
	                   GeometryAllocation* result );

  private:

    static FloatCoordinate getSpan( FloatCoordinate span,
                                    const GeometryRequirement& total,
	                            FloatCoordinate natural,
                                    FloatCoordinate stretch,
                                    FloatCoordinate shrink );

    FloatCoordinate _lNatural, _lStretch, _lShrink;
    FloatCoordinate _rNatural, _rStretch, _rShrink;
    FloatCoordinate _bNatural, _bStretch, _bShrink;
    FloatCoordinate _tNatural, _tStretch, _tShrink;
    GeometryRequisition _geometryRequisition;

};


}


#endif
