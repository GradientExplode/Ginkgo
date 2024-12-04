#include <gkg-graphics-QtGL-plugin-functors/Voi/CheckedHeader.h>

#include <QPainter>
#include <QMouseEvent>


gkg::CheckedHeader::CheckedHeader( Qt::Orientation orientation,
                                   QWidget* parent )
                  : QHeaderView( orientation, parent ),
                    isOn( false )
{
}


void gkg::CheckedHeader::paintSection( QPainter* painter,
                                       const QRect& rect,
                                       int32_t logicalIndex ) const
{

  painter->save();
  QHeaderView::paintSection( painter, rect, logicalIndex );  
  painter->restore();

  if ( logicalIndex == 0 )
  {

    QStyleOptionButton option;
    option.initFrom( this );
    option.rect = checkBoxRect( rect );
    option.state |= isOn ? QStyle::State_On : QStyle::State_Off;

    this->style()->drawControl( QStyle::CE_CheckBox, &option, painter );

  }
		
}


void gkg::CheckedHeader::mousePressEvent( QMouseEvent* event )
{

  if ( logicalIndexAt( event->pos() ) == 0 )
  {

    isOn = !isOn;
    this->updateSection( 0 );

    emit toggled( isOn );

  }
  else
  {

    QHeaderView::mousePressEvent( event );

  }

}


QRect gkg::CheckedHeader::checkBoxRect( const QRect& sourceRect ) const
{

  QStyleOptionButton checkBoxStyleOption;
  QRect checkBoxRect = style()->subElementRect( QStyle::SE_CheckBoxIndicator, 
                                                &checkBoxStyleOption );
  QPoint checkBoxPoint( sourceRect.x() + 3,
                        sourceRect.y() + sourceRect.height() / 2 
                                       - checkBoxRect.height() / 2 );

  return QRect( checkBoxPoint, checkBoxRect.size() );

}

