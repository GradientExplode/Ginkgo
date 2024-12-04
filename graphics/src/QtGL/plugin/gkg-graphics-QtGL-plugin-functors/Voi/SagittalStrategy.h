#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_SagittalStrategy_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_SagittalStrategy_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/Strategy2D.h>


namespace gkg
{


class VoiSagittalStrategy : public Voi2DStrategy
{

  public:

    VoiSagittalStrategy();

    void updateData( VoiData& );
    void updateLabel( VoiData&, std::list< Vector3d< int32_t > >& );
    void updateSlice( VoiData&, Vector3d< int32_t >&, int32_t );
    void updateSlice( VoiData&, int32_t );
    void deltaSlice( VoiData&, int32_t );
    void setIndicator( QPainter* );

    VoiStrategy::StgyType type();

    int32_t getSlice();

  protected:

    void initialize( VoiData& );
    void copyRoi( VoiData& d, int32_t label );
    void pasteRoi( VoiData& d, int32_t label );

    Vector3d< int32_t > click2Point4D( QPoint&, int32_t* );
    QPoint point4D2QPoint( Vector3d< int32_t >&, int32_t );

};


}


#endif
