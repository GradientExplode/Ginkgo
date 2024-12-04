#ifndef _gkg_graphics_X11_X11_Xext_h_
#define _gkg_graphics_X11_X11_Xext_h_


extern "C"
{


#include <gkg-graphics-X11-X11/Xdefs.h>


#define XShapeQueryExtension    cc_XShapeQueryExtension
#define XShapeQueryVersion      cc_XShapeQueryVersion
#define XShapeCombineRegion     cc_XShapeCombineRegion
#define XShapeCombineRectangles cc_XShapeCombineRectangles
#define XShapeCombineMask       cc_XShapeCombineMask
#define XShapeCombineShape      cc_XShapeCombineShape
#define XShapeOffsetShape       cc_XShapeOffsetShape
#define XShapeQueryExtents      cc_XShapeQueryExtents
#define XShapeSelectInput       cc_XShapeSelectInput
#define XShapeInputSelected     cc_XShapeInputSelected
#define XShapeGetRectangles     cc_XShapeGetRectangles

#include <X11/extensions/Xext.h>
#include <X11/extensions/shape.h>

#undef XShapeQueryExtension
#undef XShapeQueryVersion
#undef XShapeCombineRegion
#undef XShapeCombineRectangles
#undef XShapeCombineMask
#undef XShapeCombineShape
#undef XShapeOffsetShape
#undef XShapeQueryExtents
#undef XShapeSelectInput
#undef XShapeInputSelected
#undef XShapeGetRectangles

Bool XShapeQueryExtension( Display*, int32_t*, int32_t* );
Status XShapeQueryVersion( Display*, int32_t*, int32_t* );
void XShapeCombineRegion( Display*, XWindow, int32_t, int32_t, int32_t, 
                          Region, int32_t );
void XShapeCombineRectangles( Display*, XWindow, int32_t, int32_t, int32_t, 
                              XRectangle*, int32_t, int32_t, int32_t );
void XShapeCombineMask( Display*, XWindow, int32_t, int32_t, int32_t, 
                        Pixmap, int32_t );
void XShapeCombineShape( Display*, XWindow, int32_t, int32_t, int32_t, 
                         XWindow, int32_t, int32_t );
void XShapeOffsetShape( Display*, XWindow, int32_t, int32_t, int32_t );
Status XShapeQueryExtents( Display*, XWindow,
                           Bool*, int32_t*, int32_t*, uint32_t*, uint32_t*,
                           Bool*, int32_t*, int32_t*, uint32_t*, uint32_t* );
void XShapeSelectInput( Display*, XWindow, Bool );
uint32_t XShapeInputSelected( Display*, XWindow );
XRectangle* XShapeGetRectangles( Display*, XWindow, int32_t, int32_t*, 
                                 int32_t* );

#include <gkg-gui-X11/Xundefs.h>


}


#endif
