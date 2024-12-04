#include <gkg-graphics-QtGL-plugin-functors/Voi/DistanceStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>


gkg::VoiDistanceStrategy::VoiDistanceStrategy()
                        : gkg::VoiModeStrategy()
{
}


gkg::VoiDistanceStrategy::ModeId gkg::VoiDistanceStrategy::id()
{ 

  return gkg::VoiModeStrategy::DistanceMode;
  
}

