#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_Indicator_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_Indicator_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>

#include <QPoint>


class QPainter;


namespace gkg
{


class VoiIndicator
{

  public:

    VoiIndicator();
    virtual ~VoiIndicator();

    void draw( QPainter* painter, QPoint& pos );

};


}


#endif
