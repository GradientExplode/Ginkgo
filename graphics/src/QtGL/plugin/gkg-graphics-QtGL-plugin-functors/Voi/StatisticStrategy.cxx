#include <gkg-graphics-QtGL-plugin-functors/Voi/StatisticStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>
#include <gkg-processing-container/Volume_i.h>

#include <QMainWindow>
#include <QToolTip>
#include <QPainter>
#include <QPoint>

#include <sstream>


gkg::VoiStatisticStrategy::VoiStatisticStrategy() 
                          : gkg::VoiModeStrategy()
{
}


gkg::VoiModeStrategy::ModeId gkg::VoiStatisticStrategy::id()
{ 

  return gkg::VoiModeStrategy::StatisticMode;
  
}


void gkg::VoiStatisticStrategy::pressed( gkg::VoiDrawA& da, gkg::VoiData& d, 
                                         gkg::Vector3d< int32_t >& p, 
                                         int32_t t, 
				         QPoint& qp, Qt::KeyboardModifiers )
{

  std::ostringstream msg;

  msg << "Position : (" << p.x << "," << p.y << "," << p.z << "," << t << ")";

  if ( d.dataLoaded() )
  {
  
    float val = (float)d.getBaseFusion().getT1()( p.x, p.y, p.z, t );
    int32_t lab = (int32_t)d.label()( p );

    msg << std::endl;
    msg << "Intensity : " << val << std::endl;
    msg << "Label : " << lab << std::endl;
    msg << "Name : " << d.labelInfo().getName( lab ) << std::endl;

    if ( stat.empty() )
    {

      stat.compute( d );

    }

    gkg::VoiStatistic::Stat statistic = stat.statistic( lab, t );
    msg << "Statistics :" << std::endl;
    msg << "   mean : " << statistic.mean << std::endl;
    msg << "   stddev : " << statistic.sigma << std::endl;
    msg << "   volume : " << statistic.volume << " mm3" << std::endl;
    msg << "   surface : " << statistic.surface << " mm2";
  
  }

  QToolTip::showText( da.mapToGlobal( qp ), QString( msg.str().c_str() ), &da );

  d.notifySlice( p, t );

}


void gkg::VoiStatisticStrategy::setIndicator( QPainter* painter, QPoint& pos )
{

  indicator.draw( painter, pos );

}
