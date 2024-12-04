#ifndef _gkg_graphics_X11_LF_Motif_MotifDialogFactory_h_
#define _gkg_graphics_X11_LF_Motif_MotifDialogFactory_h_


#include <gkg-graphics-X11-core/DialogFactory.h>
#include <gkg-core-pattern/SingletonAccessor.h>


namespace gkg
{


// object for creating common Motif dialog boxes
class MotifDialogFactory : public DialogFactory,
                           public SingletonAccessor< MotifDialogFactory,
                                                     DialogFactory >
{


  public:

    ~MotifDialogFactory();

    FieldEditor* createFieldEditor( const std::string& sample,
                                    Style* style,
                                    FieldEditorAction* action,
                                    bool validateNormalChar,
                                    float minimumWidth ) const;
    FileChooser* createFileChooser( const std::string& dir,
                                    Style* style,
                                    FileChooserAction* action ) const;

  protected:

    friend class Singleton< MotifDialogFactory >;

    MotifDialogFactory();

};


}


#endif
