#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_StatisticStrategy_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_StatisticStrategy_h_

#include <gkg-graphics-QtGL-plugin-functors/Voi/ModeStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Indicator.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/VoiStatistic.h>
#include <gkg-processing-coordinates/Vector3d.h>


namespace gkg
{


class VoiStatisticStrategy : public VoiModeStrategy
{

  public:

    VoiStatisticStrategy();

    VoiModeStrategy::ModeId id();
    void pressed( VoiDrawA&, VoiData&, 
                  Vector3d< int32_t >&, int32_t, QPoint&,
                  Qt::KeyboardModifiers modifiers = Qt::NoModifier );
    void setIndicator( QPainter*, QPoint& );

  private:

    VoiIndicator indicator;
    VoiStatistic stat;

};


}


#endif
