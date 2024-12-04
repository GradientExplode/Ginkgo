#ifndef _gkg_graphics_X11_core_VRule_h_
#define _gkg_graphics_X11_core_VRule_h_


#include <gkg-graphics-X11-core/Rule.h>


namespace gkg
{


class VRule : public Rule
{

  public:

    VRule(  const Color* color, FloatCoordinate thickness );
    virtual ~VRule();

};


}


#endif
