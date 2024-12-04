#include <gkg-graphics-X11-LF-Motif/MotifDialogFactory.h>


gkg::MotifDialogFactory::MotifDialogFactory()
{
}


gkg::MotifDialogFactory::~MotifDialogFactory()
{
}


gkg::FieldEditor* 
gkg::MotifDialogFactory::createFieldEditor( const std::string& sample,
                                            gkg::Style* style,
                                            gkg::FieldEditorAction* action,
                                            bool validateNormalChar,
                                            float minimumWidth ) const
{

  return new gkg::FieldEditor( sample,
                               style,
                               action,
                               validateNormalChar,
                               minimumWidth );

}


gkg::FileChooser* 
gkg::MotifDialogFactory::createFileChooser(
                                          const std::string& dir,
                                          gkg::Style* style,
                                          gkg::FileChooserAction* action ) const
{

  return new gkg::FileChooser( dir, style, action );

}

