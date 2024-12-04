#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_PaintStrategy_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_PaintStrategy_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/ModeStrategy.h>
#include <gkg-processing-coordinates/Vector3d.h>

#include <QPoint>


namespace gkg
{


class VoiPaintStrategy : public VoiModeStrategy
{

  public:

    VoiModeStrategy::ModeId id();
    QCursor cursor();
    void pressed( VoiDrawA&, VoiData&, Vector3d< int32_t >&, int32_t,
                  QPoint&, Qt::KeyboardModifiers modifiers = Qt::NoModifier );
    void erase( VoiDrawA&, VoiData&, Vector3d< int32_t >&, int32_t,
                QPoint&, Qt::KeyboardModifiers modifiers = Qt::NoModifier );
    void moved( VoiDrawA&, VoiData&, Vector3d< int32_t >&, int32_t,
                QPoint&, Qt::KeyboardModifiers modifiers = Qt::NoModifier );

};


}


#endif
