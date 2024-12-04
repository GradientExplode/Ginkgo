#ifndef _gkg_graphics_X11_core_FileChooserImplementation_h_
#define _gkg_graphics_X11_core_FileChooserImplementation_h_


#include <gkg-core-cppext/StdInt.h>
#include <string>


namespace gkg
{


class FileChooser;
class Glyph;
class Style;
class FileChooserAction;
class FileBrowser;
class FieldEditor;
class FieldEditorAction;
class Directory;
class Action;


class FileChooserImplementation
{

  public:

    FileChooserImplementation( FileChooser* fileChooser,
                               const std::string& directory,
                               Style* style,
                               FileChooserAction* fileChooserAction  );
    virtual ~FileChooserImplementation();

    virtual FileChooser* getTarget() const;

    virtual const std::string& getSelected() const;
    virtual void reRead();
    virtual void dismiss( bool accept );


  private:

    void build();
    void clear();
    void load();
    FieldEditor* addFilter( Style* style,
                            const std::string& patternAttribute,
                            const std::string& defaultPattern,
                            const std::string& captionAttribute,
                            const std::string& defaultCaption,
                            Glyph* glyph,
                            FieldEditorAction* action );
    bool isFiltered( const std::string& name, FieldEditor* fieldEditor );
    void acceptBrowser();
    void cancelBrowser();
    void acceptEditor( FieldEditor* fieldEditor );
    void cancelEditor( FieldEditor* );
    void acceptFilter( FieldEditor* );
    bool chdir( const std::string& name );

    FileChooser* _target;
    std::string _name;
    FileBrowser* _fileBrowser;
    FieldEditor* _editor;
    FieldEditor* _filter;
    FieldEditor* _directoryFilter;
    int32_t* _filterMap;
    Directory* _directory;
    FileChooserAction* _fileChooserAction;
    std::string _selected;
    Style* _style;
    Action* _update;

};


}


#endif
