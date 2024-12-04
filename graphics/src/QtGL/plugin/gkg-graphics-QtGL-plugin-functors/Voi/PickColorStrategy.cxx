#include <gkg-graphics-QtGL-plugin-functors/Voi/PickColorStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>
#include <gkg-processing-container/Volume_i.h>


gkg::VoiPickColorStrategy::VoiPickColorStrategy() 
                          : gkg::VoiModeStrategy()
{
}


gkg::VoiModeStrategy::ModeId gkg::VoiPickColorStrategy::id()
{ 

  return gkg::VoiModeStrategy::PickingColorMode;
  
}


void gkg::VoiPickColorStrategy::pressed( gkg::VoiDrawA&, gkg::VoiData& d, 
                                         gkg::Vector3d< int32_t >& p, int32_t, 
                                         QPoint&, Qt::KeyboardModifiers )
{

  int16_t label = d.label()( p );

  if ( d.labelInfo().getShow( label ) )
  {

    d.setCurrentLabel( label );

  }

}


QCursor gkg::VoiPickColorStrategy::cursor()
{

  gkg::VoiCursor curs( gkg::VoiCursor::VoiPipetteCursor );

  return curs.cursor();

}
