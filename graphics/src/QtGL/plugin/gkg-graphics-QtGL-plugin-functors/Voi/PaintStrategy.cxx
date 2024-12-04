#include <gkg-graphics-QtGL-plugin-functors/Voi/PaintStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>


gkg::VoiModeStrategy::ModeId gkg::VoiPaintStrategy::id()
{ 

  return gkg::VoiModeStrategy::PaintMode;
  
}


void gkg::VoiPaintStrategy::pressed( gkg::VoiDrawA& /* da */, gkg::VoiData& d, 
                                     gkg::Vector3d< int32_t >& p, int32_t t, 
                                     QPoint&, Qt::KeyboardModifiers modifiers )
{

  int32_t index = (int32_t)d.label()( p );

  if ( index )
  {

    if ( d.labelInfo().getShow( index ) )
    {

      d.setCurrentLabel( index );

    }

  }
  else
  {
    
    index = d.labelInfo().currentLabel();

    if ( ( index != -1 ) && d.labelInfo().getShow( index ) )
    {
  
      if ( ( modifiers & Qt::ControlModifier ) )
      {
  
        d.fillLabel( p, t, index );
    
      }
      else
      {

        d.setLabelPoint( p, t, index );

      }

    }

  }

}

void gkg::VoiPaintStrategy::erase( gkg::VoiDrawA&, gkg::VoiData& d, 
                                   gkg::Vector3d< int32_t >& p, int32_t t, 
                                   QPoint&, Qt::KeyboardModifiers modifiers )
{

  int32_t index = d.labelInfo().currentLabel();

  if ( ( index != -1 ) && d.labelInfo().getShow( index ) )
  {

    if ( ( modifiers & Qt::ControlModifier ) )
    {

      d.undoEvolution();

    }
    else
    {

      d.setLabelPoint( p, t, 0 );

    }

  }

}


void gkg::VoiPaintStrategy::moved( gkg::VoiDrawA& da, gkg::VoiData &d, 
                                   gkg::Vector3d< int32_t >& p, int32_t t, 
                                   QPoint& pt,
                                   Qt::KeyboardModifiers modifiers )
{

  if ( !( modifiers & Qt::ControlModifier ) )
  {

    pressed( da, d, p, t, pt, modifiers );

  }

}


QCursor gkg::VoiPaintStrategy::cursor()
{

  gkg::VoiCursor curs( gkg::VoiCursor::VoiPenCursor );

  return curs.cursor();

}
