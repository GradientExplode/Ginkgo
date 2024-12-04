#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_LabelAxialStrategy_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_LabelAxialStrategy_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/LabelStrategy.h>


namespace gkg
{


class VoiLabelAxialStrategy : public VoiLabelStrategy
{

  public:

    VoiLabelAxialStrategy();

    VoiLabelStrategy::TypeId id();
    void doit( VoiData*, Volume< int16_t >&, Vector3d< int32_t >&, 
               int32_t, int32_t, std::list< Vector2d< int32_t > >& );
    void spline( VoiData*, gkg::Volume< int16_t >&,
                 std::list< QPoint >&, int32_t, int32_t );

};


}


#endif
