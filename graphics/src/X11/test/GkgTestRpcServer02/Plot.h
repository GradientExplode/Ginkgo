#ifndef _gkg_graphics_X11_GkgTestRpcServer02_Plot_h
#define _gkg_graphics_X11_GkgTestRpcServer02_Plot_h


#include <gkg-graphics-X11-core/Glyph.h>



// Simple graph glyph

namespace gkg
{


class Brush;
class Color;


}


class Plot : public gkg::Glyph
{

  public:

    Plot( int32_t& count, float* x, float* y,
          const gkg::Color* color, const gkg::Brush* brush );

    void setPoints( int32_t& count, float* x, float* y);

    virtual void request( gkg::GeometryRequisition& geometryRequisition ) const;
    virtual void allocate( gkg::Canvas* canvas,
                           const gkg::GeometryAllocation& geometryAllocation,
                           gkg::GeometryExtension& geometryExtension );
    virtual void draw( gkg::Canvas* canvas,
                      const gkg::GeometryAllocation& geometryAllocation ) const;
    virtual void pick( gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation,
                       int32_t depth,
                       gkg::Hit& hit );

  protected:

    virtual ~Plot();

    int32_t* _count;
    float* _x;
    float* _y;

    const gkg::Color* _color;
    const gkg::Brush* _brush;

};


inline
void Plot::setPoints( int32_t& count, float* x, float* y )
{

  _count = &count;
  _x = x;
  _y = y;

}


#endif
