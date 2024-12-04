#ifndef _gkg_graphics_X11_core_DataCursorImplementation_h_
#define _gkg_graphics_X11_core_DataCursorImplementation_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


class Display;
class Visual;
class DataCursor;


class DataCursorImplementation
{

  public:

    virtual ~DataCursorImplementation();

    virtual void makeCursor( Display* display, Visual* visual ) = 0;

  protected:

    DataCursorImplementation( DataCursor* dataCursor,
                              int16_t xHot, int16_t yHot,
                              const int32_t* pattern, const int32_t* mask );

    DataCursor* _target;

    int16_t _x;
    int16_t _y;
    const int32_t* _pattern;
    const int32_t* _mask;

};


}


#endif
