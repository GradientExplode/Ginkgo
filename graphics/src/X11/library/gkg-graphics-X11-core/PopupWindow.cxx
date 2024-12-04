#include <gkg-graphics-X11-core/PopupWindow.h>
#include <gkg-graphics-X11-core/PopupWindowImplementation.h>
#include <gkg-graphics-X11-core/Glyph.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>



gkg::PopupWindow::PopupWindow( gkg::Glyph* glyph )
                 : gkg::Window( glyph )
{

  _popupWindowImplementation = 
    gkg::getGUIImplementationFactory()->createPopupWindowImplementation( this );

}


gkg::PopupWindow::~PopupWindow()
{

  delete _popupWindowImplementation;

}


void gkg::PopupWindow::setAttributes()
{

  _popupWindowImplementation->setAttributes();

}


gkg::PopupWindowImplementation*
gkg::PopupWindow::getImplementation() const
{

  return _popupWindowImplementation;

}
