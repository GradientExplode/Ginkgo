#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_ProfileStrategy_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_ProfileStrategy_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/Strategy.h>
#include <gkg-processing-coordinates/Vector3d.h>

#include <QVector>
#include <QPointF>
#include <QString>


namespace gkg
{


class ProfileStrategy : public VoiStrategy
{

  public:

    ProfileStrategy();

    void paint( QPainter* painter, const QRectF& rect );

    void updateDataLoaded( VoiData& );
    void updateSlice( VoiData&, Vector3d< int32_t >&, int32_t );
    void updateFrame( VoiData&, int32_t );

    VoiStrategy::StgyType type();

  private:

    QVector< QPointF > _samples[ 4 ];
    QString _msg;

};


}


#endif
