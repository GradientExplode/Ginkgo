#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_ContrastStrategy_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_ContrastStrategy_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/ModeStrategy.h>
#include <gkg-processing-coordinates/Vector3d.h>

#include <QPoint>


namespace gkg
{


class VoiContrastStrategy : public VoiModeStrategy
{

  public:

    VoiContrastStrategy();

    VoiModeStrategy::ModeId id();
    void pressed( VoiDrawA&, VoiData&, Vector3d< int32_t >&, int32_t, 
                  QPoint&, Qt::KeyboardModifiers modifiers = Qt::NoModifier );
    void moved( VoiDrawA&, VoiData&, Vector3d< int32_t >&, int32_t,
                QPoint&, Qt::KeyboardModifiers modifiers = Qt::NoModifier );

  private:

    double progress( double v, double d );

    QPoint posClick;
    float minLut;
    float maxLut;
    float scaling;
    float origin;

    double startW;
    double startL;

};


}


#endif
