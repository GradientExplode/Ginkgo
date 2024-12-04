#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_RoiCache_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_RoiCache_h_


#include <gkg-processing-coordinates/Vector3d.h>

#include <list>


namespace gkg
{


class RoiCache
{

  public:

    RoiCache();
    virtual ~RoiCache();

    void setLabel( int32_t label );
    bool isValid( int32_t label );
    bool empty();
    void add( Vector3d< int32_t >& roiPoint );
    std::list< Vector3d< int32_t > >& getRoi();
    void clear();

  private:

    std::list< Vector3d< int32_t > > _roi;
    int32_t _label;

};


}


#endif
