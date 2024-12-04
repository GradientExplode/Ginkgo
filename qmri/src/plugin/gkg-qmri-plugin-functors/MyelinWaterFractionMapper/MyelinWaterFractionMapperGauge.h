#ifndef _gkg_qmri_plugin_functors_MyelinWaterFractionMapper_MyelinWaterFractionMapperGauge_h_
#define _gkg_qmri_plugin_functors_MyelinWaterFractionMapper_MyelinWaterFractionMapperGauge_h_


#include <gkg-communication-thread/ThreadGauge.h>


namespace gkg
{


class MyelinWaterFractionMapperGauge : public ThreadGauge< int32_t >
{

  public:

    MyelinWaterFractionMapperGauge( int32_t maxCount );

    void add( int32_t value );
    void reset();

  private:

    void display();

    int32_t _count;
    int32_t _maxCount;
    bool _siteIndexAlreadyDisplayed;

};


}


#endif
