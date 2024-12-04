#ifndef _gkg_graphics_X11_core_HRule_h_
#define _gkg_graphics_X11_core_HRule_h_


#include <gkg-graphics-X11-core/Rule.h>


namespace gkg
{


class HRule : public Rule
{

  public:

    HRule(  const Color* color, FloatCoordinate thickness );
    virtual ~HRule();

};


}


#endif
