#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_ScrollBars_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_ScrollBars_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/Strategy.h>
#include <gkg-processing-coordinates/Vector3d.h>

#include <QWidget>


class QLabel;
class QScrollBar;


namespace gkg
{


class VoiData;


class ScrollBars : public QWidget
{

  Q_OBJECT

  public:

    ScrollBars( QWidget* parent = 0 );

    void initialize( VoiData& data, VoiStrategy::StgyType type );
    void setSizes( int32_t windowWidth, int32_t windowHeight );

    void updateSlice( Vector3d< int32_t >& pt, VoiStrategy::StgyType type );
    void deltaSlice( int32_t delta );

  private slots:

    void changeSlice( int );
    void changeFrame( int );

  signals:

    void valueChanged( int );
    void frameChanged( int );

  private:

    QWidget* _parent;
    QLabel* _label;
    QLabel* _labelT;
    QScrollBar* _scrollBar;
    QScrollBar* _scrollBarT;
    int32_t _count;

};


}


#endif
