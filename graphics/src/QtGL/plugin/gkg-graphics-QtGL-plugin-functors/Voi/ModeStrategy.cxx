#include <gkg-graphics-QtGL-plugin-functors/Voi/ModeStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>


#include <QPainter>


gkg::VoiModeStrategy::VoiModeStrategy()
{
}


gkg::VoiModeStrategy::~VoiModeStrategy()
{
}


gkg::VoiModeStrategy::ModeId gkg::VoiModeStrategy::id()
{

  return (gkg::VoiModeStrategy::ModeId)0;
  
}


void gkg::VoiModeStrategy::pressed( gkg::VoiDrawA&, gkg::VoiData&, 
                                    gkg::Vector3d< int32_t >&, int32_t, 
				    QPoint&, Qt::KeyboardModifiers )
{
}


void gkg::VoiModeStrategy::erase( gkg::VoiDrawA&, gkg::VoiData&, 
                                  gkg::Vector3d< int32_t >&, int32_t, 
                                  QPoint&, Qt::KeyboardModifiers )
{
}


void gkg::VoiModeStrategy::moved( gkg::VoiDrawA&, gkg::VoiData&, 
                                  gkg::Vector3d< int32_t >&, int32_t, 
				  QPoint&, Qt::KeyboardModifiers )
{
}


void gkg::VoiModeStrategy::released( gkg::VoiData& )
{
}


QCursor gkg::VoiModeStrategy::cursor()
{

#if QT_VERSION < 300
  return QCursor( ArrowCursor );
#else
  return QCursor( Qt::ArrowCursor );
#endif

}


void gkg::VoiModeStrategy::setIndicator( QPainter*, QPoint& )
{
}
