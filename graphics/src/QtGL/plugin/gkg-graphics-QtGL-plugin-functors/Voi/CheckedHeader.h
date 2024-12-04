#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_CheckedHeader_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_CheckedHeader_h_


#include <QHeaderView>


class QPainter;
class QMouseEvent;


namespace gkg
{


class CheckedHeader : public QHeaderView
{

  Q_OBJECT

  public:
 
    CheckedHeader( Qt::Orientation orientation, QWidget* parent = 0 );

  signals:

    void toggled( bool checked );

  protected:

    void paintSection( QPainter* painter, const QRect& rect,
                       int32_t logicalIndex ) const;
    void mousePressEvent( QMouseEvent* event );

  private:

    QRect checkBoxRect( const QRect& sourceRect ) const;

    bool isOn;

};


}


#endif

