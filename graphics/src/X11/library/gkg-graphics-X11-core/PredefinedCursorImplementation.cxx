#include <gkg-graphics-X11-core/PredefinedCursorImplementation.h>


gkg::PredefinedCursorImplementation::PredefinedCursorImplementation(
                                        gkg::PredefinedCursor* predefinedCursor,
                                        int32_t code )
                                    : _target( predefinedCursor ),
                                      _code( code )
{
}


gkg::PredefinedCursorImplementation::~PredefinedCursorImplementation()
{
}


gkg::PredefinedCursor* gkg::PredefinedCursorImplementation::getTarget() const
{

  return _target;

}


