#include <gkg-graphics-X11-core/DialogHandler.h>
#include <gkg-graphics-X11-core/Dialog.h>


gkg::DialogHandler::DialogHandler( gkg::Dialog* dialog )
                   : gkg::EventHandler(),
                     _dialog( dialog )
{
}


gkg::DialogHandler::~DialogHandler()
{
}


bool gkg::DialogHandler::event( Event& )
{

  _dialog->dismiss( false );
  return true;

}
