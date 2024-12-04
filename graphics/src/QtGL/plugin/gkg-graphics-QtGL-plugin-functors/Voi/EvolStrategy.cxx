#include <gkg-graphics-QtGL-plugin-functors/Voi/EvolStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>


gkg::VoiEvolutionStrategy::VoiEvolutionStrategy() : VoiModeStrategy()
{
}


gkg::VoiModeStrategy::ModeId gkg::VoiEvolutionStrategy::id()
{

  return gkg::VoiModeStrategy::EvolutionMode;
  
}


void gkg::VoiEvolutionStrategy::pressed( gkg::VoiDrawA&, gkg::VoiData& d, 
                                         gkg::Vector3d< int32_t >& p, int32_t t, 
				         QPoint&, Qt::KeyboardModifiers )
{

  int32_t index = d.labelInfo().currentLabel();

  if ( ( index != -1 ) && d.labelInfo().getShow( index ) )
  {
  
    d.evolutionLabel( p, t, index );
    
  }
  
}


void gkg::VoiEvolutionStrategy::erase( gkg::VoiDrawA&, gkg::VoiData& d, 
                                       gkg::Vector3d< int32_t >&, int32_t, 
				       QPoint&, Qt::KeyboardModifiers )
{

  int32_t index = d.labelInfo().currentLabel();

  if ( ( index != -1 ) && d.labelInfo().getShow( index ) )
  {

    d.undoEvolution();

  }

}


QCursor gkg::VoiEvolutionStrategy::cursor()
{

  gkg::VoiCursor curs( gkg::VoiCursor::VoiMagicWandCursor );

  return curs.cursor();
  
}
