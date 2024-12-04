#include <gkg-graphics-X11-core/DataCursorImplementation.h>


gkg::DataCursorImplementation::DataCursorImplementation(
                                   gkg::DataCursor* dataCursor,
                                   int16_t xHot, int16_t yHot,
                                   const int32_t* pattern, const int32_t* mask )
                              : _target( dataCursor ),
                                _x( xHot ),
                                _y( yHot ),
                                _pattern( pattern ),
                                _mask( mask )
{
}


gkg::DataCursorImplementation::~DataCursorImplementation()
{
}
