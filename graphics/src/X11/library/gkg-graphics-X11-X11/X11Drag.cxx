#include <gkg-graphics-X11-X11/X11Drag.h>
#include <gkg-graphics-X11-X11/X11DragAtoms.h>


bool gkg::X11Drag::isDrag( const XEvent& xEvent )
{

  return gkg::X11DragAtoms::getInstance().enter( xEvent ) ||
         gkg::X11DragAtoms::getInstance().motion( xEvent ) ||
         gkg::X11DragAtoms::getInstance().leave( xEvent ) || 
         gkg::X11DragAtoms::getInstance().drop( xEvent );

}

void gkg::X11Drag::locate( const XEvent& xEvent, int32_t& x, int32_t& y )
{

  x = ( int32_t )xEvent.xclient.data.l[ 0 ];
  y = ( int32_t )xEvent.xclient.data.l[ 1 ];

}

