#include <gkg-graphics-QtGL-plugin-functors/Voi/FillStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>


gkg::VoiFillStrategy::VoiFillStrategy() 
                     : VoiModeStrategy()
{
}


gkg::VoiModeStrategy::ModeId gkg::VoiFillStrategy::id()
{ 

  return gkg::VoiModeStrategy::FillingMode;
  
}


void gkg::VoiFillStrategy::pressed( gkg::VoiDrawA&, gkg::VoiData& d, 
                                    gkg::Vector3d< int32_t >& p, int32_t t, 
				    QPoint&, Qt::KeyboardModifiers )
{

  int32_t index = d.labelInfo().currentLabel();

  if ( ( index != -1 ) && d.labelInfo().getShow( index ) )
  {
  
    d.fillLabel( p, t, index );
    
  }
  
}


void gkg::VoiFillStrategy::erase( gkg::VoiDrawA&, gkg::VoiData& d, 
                                  gkg::Vector3d< int32_t >&, int32_t,
				  QPoint&, Qt::KeyboardModifiers )
{

  int32_t index = d.labelInfo().currentLabel();

  if ( ( index != -1 ) && d.labelInfo().getShow( index ) )
  {

    d.undoEvolution();

  }
  
}


QCursor gkg::VoiFillStrategy::cursor()
{

  gkg::VoiCursor curs( gkg::VoiCursor::VoiBucketCursor );

  return curs.cursor();

}
