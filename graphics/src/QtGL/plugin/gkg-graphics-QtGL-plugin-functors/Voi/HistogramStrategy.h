#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_HistogramStrategy_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_HistogramStrategy_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/Strategy.h>


namespace gkg
{


class HistogramStrategy : public VoiStrategy
{

  public:

    HistogramStrategy();

    void paint( QPainter* painter, const QRectF& rect );

    void updateDataLoaded( VoiData& );

    VoiStrategy::StgyType type();

  private:

    std::vector< double > _histogram;

};


}


#endif
