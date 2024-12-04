#include <gkg-graphics-X11-core/PopupWindowImplementation.h>


gkg::PopupWindowImplementation::PopupWindowImplementation(
                                                 gkg::PopupWindow* popupWindow )
                               : _target( popupWindow )
{
}


gkg::PopupWindowImplementation::~PopupWindowImplementation()
{
}


gkg::PopupWindow* gkg::PopupWindowImplementation::getTarget() const
{

  return _target;

}
