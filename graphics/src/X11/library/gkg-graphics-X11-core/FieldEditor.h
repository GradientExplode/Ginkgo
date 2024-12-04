#ifndef _gkg_graphics_X11_core_FieldEditor_h_
#define _gkg_graphics_X11_core_FieldEditor_h_


#include <string>
#include <gkg-graphics-X11-core/InputHandler.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>


namespace gkg
{


class FieldEditor;
class FieldEditorImplementation;
class Style;


//
// class FieldEditorAction
//

class FieldEditorAction : public RCGUIObject
{

  public:

    virtual void accept( FieldEditor* fieldEditor );
    virtual void cancel( FieldEditor* fieldEditor );

  protected:

    FieldEditorAction();
    virtual ~FieldEditorAction();

};


//
// class FieldEditorCallback< T >
//

template < class T >
class FieldEditorCallback : public FieldEditorAction
{

  public:

    typedef void ( T::*MemberFunction )( FieldEditor* );

    FieldEditorCallback( T* object,
                         MemberFunction accept,
                         MemberFunction cancel );
    virtual ~FieldEditorCallback();

    virtual void accept( FieldEditor* fieldEditor );
    virtual void cancel( FieldEditor* fieldEditor );

  private:

    T* _object;
    MemberFunction _accept;
    MemberFunction _cancel;

};


//
// class FieldEditor
//

class FieldEditor : public InputHandler
{

  public:

    FieldEditor( const std::string& sample,
                 Style* style,
                 FieldEditorAction* fieldEditorAction = 0,
                 bool validateNormalChar = false,
                 float minimumWidth = 0.0 );
    virtual ~FieldEditor();

    virtual void press( const Event& event );
    virtual void drag( const Event& event );
    virtual void keyStroke( const Event& event );

    virtual InputHandler* focusIn();
    virtual void focusOut();

    virtual int32_t locate( const Event& event );

    // editing functions
    virtual void deleteCharForward();
    virtual void deleteCharBackward();
    virtual void deleteRegion();
    virtual void deleteToEol();
    virtual void goToBeginningOfLine();
    virtual void goToEndOfLine();
    virtual void goToForwardChar();
    virtual void goToBackwardChar();
    virtual void insertChar( char c );
    virtual void clearBuffer();
    virtual void update();
    virtual void selectAll();

    virtual const std::string& getText() const;
    virtual void setText( const std::string& text );

    virtual void undo();

    FieldEditorImplementation* getImplementation() const;

  private:

    FieldEditorImplementation* _fieldEditorImplementation;

};


}


//
// FieldEditorCallback< T > implementation
//

template < class T >
inline
gkg::FieldEditorCallback< T >::FieldEditorCallback(
                       T* object,
                       gkg::FieldEditorCallback< T >::MemberFunction theAccept,
                       gkg::FieldEditorCallback< T >::MemberFunction theCancel )
                              : _object( object ),
                                _accept( theAccept ),
                                _cancel( theCancel )
{
}


template < class T >
inline
gkg::FieldEditorCallback< T >::~FieldEditorCallback()
{
} 


template < class T >
inline
void gkg::FieldEditorCallback< T >::accept( gkg::FieldEditor* fieldEditor )
{

   ( _object->*_accept )( fieldEditor );

}


template < class T >
inline
void gkg::FieldEditorCallback< T >::cancel( gkg::FieldEditor* fieldEditor )
{

  ( _object->*_cancel )( fieldEditor );

}


#endif
