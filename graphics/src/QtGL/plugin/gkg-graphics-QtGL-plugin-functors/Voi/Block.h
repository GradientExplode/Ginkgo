#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_Block_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_Block_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


class Block
{

  public:

    Block();
    virtual ~Block();

    int32_t getLabel();
    void setLabel( int32_t label );
    void setActive( bool show );
    bool isActive();

  private:

    int32_t _label;

};


}


#endif
