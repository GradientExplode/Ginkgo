#include <gkg-graphics-QtGL-plugin-functors/Voi/Indicator.h>

#include <QPainter>


gkg::VoiIndicator::VoiIndicator()
{
}


gkg::VoiIndicator::~VoiIndicator()
{
}


void gkg::VoiIndicator::draw( QPainter* painter, QPoint& pos )
{

  painter->setRenderHint( QPainter::Antialiasing );
  int x = pos.x();
  int y = pos.y();
  painter->setPen( Qt::red );
  painter->drawLine( x + 8, y, x + 8, y + 14 );
  painter->drawLine( x + 1, y + 7, x + 15, y + 7 );
  painter->setPen( Qt::white );
  painter->drawLine( x + 8, y + 6 , x + 8, y + 8 );
  painter->drawLine( x + 7, y + 7, x + 9, y + 7 );

}
