#ifndef _gkg_dmri_connectivity_matrix_ConnectivityMatrixGauge_h_
#define _gkg_dmri_connectivity_matrix_ConnectivityMatrixGauge_h_


#include <gkg-communication-thread/ThreadGauge.h>


namespace gkg
{


class ConnectivityMatrixGauge : public ThreadGauge< int32_t >
{

  public:

    ConnectivityMatrixGauge( int32_t fiberCount );

    void add( int32_t value );
    void reset();

  private:

    void display();

    int32_t _count;
    int32_t _fiberCount;
    bool _fiberIndexAlreadyDisplayed;

};


}



#endif
