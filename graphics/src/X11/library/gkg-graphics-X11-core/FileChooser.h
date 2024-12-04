#ifndef _gkg_graphics_X11_core_FileChooser_h_
#define _gkg_graphics_X11_core_FileChooser_h_


#include <string>
#include <gkg-graphics-X11-core/Dialog.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>


namespace gkg
{


class FileChooser;
class FileChooserImplementation;


//
// class FileChooserAction
//

class FileChooserAction : public RCGUIObject 
{

  public:

    virtual void execute( FileChooser* fileChooser, bool accept );

  protected:

    FileChooserAction();
    virtual ~FileChooserAction();

};


//
// class FileChooserCallback< T >
//

template < class T >
class FileChooserCallback : public FileChooserAction
{

  public:

    typedef void ( T::*MemberFunction )( FileChooser*, bool );

    FileChooserCallback( T* object, MemberFunction function );
    virtual ~FileChooserCallback();

    virtual void execute( FileChooser* fileChooser, bool accept );

  private:

    T* _object;
    MemberFunction _function;

};


//
// class FileChooser : select a file
//

class FileChooser : public Dialog
{

  public:

    FileChooser( const std::string& directory,
                 Style* style,
                 FileChooserAction* fileChooserAction = 0 );
    virtual ~FileChooser();

    virtual const std::string& getSelected() const;
    virtual void reRead();
    virtual void dismiss( bool accept );

    FileChooserImplementation* getImplementation() const;

  private:

    FileChooserImplementation* _fileChooserImplementation;

};


}


//
// FileChooserCallback< T > implementation
//

template < class T >
inline
gkg::FileChooserCallback< T >::FileChooserCallback(
                        T* object,
                        gkg::FileChooserCallback< T >::MemberFunction function )
                              : gkg::FileChooserAction(),
                                _object( object ),
                                _function( function )
{
}


template < class T >
inline
gkg::FileChooserCallback< T >::~FileChooserCallback()
{
}


template < class T >
inline
void gkg::FileChooserCallback< T >::execute( gkg::FileChooser* fileChooser,
                                             bool accept )
{

  ( _object->*_function )( fileChooser, accept );

}


#endif
