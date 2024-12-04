#include <gkg-graphics-QtGL-plugin-functors/Voi/CylinderStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>


gkg::VoiCylinderStrategy::VoiCylinderStrategy()
                        : gkg::VoiModeStrategy()
{
}


gkg::VoiModeStrategy::ModeId gkg::VoiCylinderStrategy::id()
{ 

  return gkg::VoiModeStrategy::CylinderMode;
  
}


void gkg::VoiCylinderStrategy::pressed( gkg::VoiDrawA&, gkg::VoiData&, 
                                        gkg::Vector3d< int32_t >&, int32_t, 
                                        QPoint&, Qt::KeyboardModifiers )
{
}
