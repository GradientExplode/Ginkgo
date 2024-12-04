#include <gkg-graphics-QtGL-plugin-functors/Voi/SelectionStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>
#include <gkg-processing-container/Volume_i.h>

#include <QMainWindow>
#include <QToolTip>
#include <QPainter>
#include <QPoint>

#include <sstream>


gkg::VoiSelectionStrategy::VoiSelectionStrategy() 
                          : gkg::VoiModeStrategy()
{
}


gkg::VoiModeStrategy::ModeId gkg::VoiSelectionStrategy::id()
{

  return VoiModeStrategy::SelectionMode;
  
}


void gkg::VoiSelectionStrategy::pressed( gkg::VoiDrawA& da, gkg::VoiData& d, 
                                         gkg::Vector3d< int32_t >& p, 
                                         int32_t t, 
                                         QPoint& qp,
                                         Qt::KeyboardModifiers )
{

  float val = 0.0f;
  std::ostringstream msg;

  msg << "Position : (" << p.x << "," << p.y << "," << p.z << "," << t << ")";
  msg << std::endl;

  if ( d.dataLoaded() )
  {
  
    val = d.getBaseFusion().getT1()( p.x, p.y, p.z, t );
    
  }

  msg << "Value : " << val << std::endl;

  int32_t lab = (int32_t)d.label()( p.x, p.y, p.z );

  msg << "Label :" << lab;

  if ( lab && d.labelInfo().getShow( lab ) )
  {

    d.setCurrentLabel( lab );

  }

  QToolTip::showText( da.mapToGlobal( qp ), QString( msg.str().c_str() ), &da );

  d.notifySlice( p, t );
  
}


void gkg::VoiSelectionStrategy::moved( gkg::VoiDrawA& da, gkg::VoiData& d, 
                                       gkg::Vector3d< int32_t >& p, int32_t t, 
				       QPoint& pt,
                                       Qt::KeyboardModifiers modifiers )
{

  pressed( da, d, p, t, pt, modifiers );

}


void gkg::VoiSelectionStrategy::setIndicator( QPainter* painter, QPoint& pos )
{

  indicator.draw( painter, pos );

}
