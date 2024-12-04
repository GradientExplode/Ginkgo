#ifndef _gkg_graphics_X11_core_Image_h_
#define _gkg_graphics_X11_core_Image_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class Raster;


// displays a Raster
class Image : public Glyph
{

  public:

    Image( const Raster* raster );
    virtual ~Image();

    virtual void request( GeometryRequisition& geometryRequisition ) const;
    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;

  private:

    const Raster* _raster;

};


}


#endif
