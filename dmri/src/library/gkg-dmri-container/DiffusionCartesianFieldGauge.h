#ifndef _gkg_dmri_container_DiffusionCartesianFieldGauge_h_
#define _gkg_dmri_container_DiffusionCartesianFieldGauge_h_


#include <gkg-communication-thread/ThreadGauge.h>


namespace gkg
{


class DiffusionCartesianFieldGauge : public ThreadGauge< int32_t >
{

  public:

    DiffusionCartesianFieldGauge( int32_t maxCount );

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
