#include <gkg-graphics-X11-core/FileChooser.h>
#include <gkg-graphics-X11-core/FileChooserImplementation.h>


//
// class FileChooserAction
//

gkg::FileChooserAction::FileChooserAction()
                       : gkg::RCGUIObject()
{
}


gkg::FileChooserAction::~FileChooserAction()
{
}


void gkg::FileChooserAction::execute( FileChooser*, bool )
{
}


//
// class FileChooser
//

gkg::FileChooser::FileChooser( const std::string& directory,
                               gkg::Style* style,
                               gkg::FileChooserAction* fileChooserAction )
                 : gkg::Dialog( 0, style )
{

  _fileChooserImplementation = new gkg::FileChooserImplementation( this,
                                          directory, style, fileChooserAction );

}


gkg::FileChooser::~FileChooser()
{

  delete _fileChooserImplementation;

}


const std::string& gkg::FileChooser::getSelected() const
{

  return _fileChooserImplementation->getSelected();

}


void gkg::FileChooser::reRead()
{

  _fileChooserImplementation->reRead();

}


void gkg::FileChooser::dismiss( bool accept )
{

  this->gkg::Dialog::dismiss( accept );
  _fileChooserImplementation->dismiss( accept );

}


gkg::FileChooserImplementation* gkg::FileChooser::getImplementation() const
{

  return _fileChooserImplementation;

}



