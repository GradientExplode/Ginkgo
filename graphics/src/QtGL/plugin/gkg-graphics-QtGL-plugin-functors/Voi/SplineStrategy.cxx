#include <gkg-graphics-QtGL-plugin-functors/Voi/SplineStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>


gkg::VoiSplineStrategy::VoiSplineStrategy()
                      : gkg::VoiModeStrategy()
{
}


gkg::VoiModeStrategy::ModeId gkg::VoiSplineStrategy::id()
{ 

  return gkg::VoiModeStrategy::SplineMode;
  
}

