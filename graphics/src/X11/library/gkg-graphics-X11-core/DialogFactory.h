#ifndef _gkg_graphics_X11_core_DialogFactory_h_
#define _gkg_graphics_X11_core_DialogFactory_h_


#include <string>
#include <gkg-graphics-X11-core/FieldEditor.h>
#include <gkg-graphics-X11-core/FileChooser.h>


namespace gkg
{


// object for creating common dialog boxes
class DialogFactory
{

  public:

    virtual ~DialogFactory();

    virtual FieldEditor* 
              createFieldEditor( const std::string& sample,
                                 Style* style,
                                 FieldEditorAction* action,
                                 bool validateNormalChar,
                                 float minimumWidth ) const = 0;
    virtual FileChooser* 
              createFileChooser( const std::string& dir,
                                 Style* style,
                                 FileChooserAction* action ) const = 0;

  protected:

    DialogFactory();

};


}


#endif
