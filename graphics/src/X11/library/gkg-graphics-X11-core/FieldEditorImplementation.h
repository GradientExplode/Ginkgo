#ifndef _gkg_graphics_X11_core_FieldEditorImplementation_h_
#define _gkg_graphics_X11_core_FieldEditorImplementation_h_


#include <gkg-graphics-X11-core/TextBuffer.h>


namespace gkg
{


class FieldEditor;
class FieldEditorAction;
class Style;
class Event;


class FieldEditorImplementation
{

  public:

    FieldEditorImplementation( FieldEditor* fieldEditor,
                               const std::string& sample,
                               Style* style,
                               FieldEditorAction* fieldEditorAction,
                               bool validateNormalChar,
                               float minimumWidth );
    ~FieldEditorImplementation();

    FieldEditor* getTarget() const;

    void press( const Event& event );
    void drag( const Event& event );
    void keyStroke( const Event& event );

    void focusIn();
    void focusOut();

    void cursorOn();
    void cursorOff();

    int32_t locate( const Event& event );

    void deleteCharForward();
    void deleteCharBackward();
    void deleteRegion();
    void deleteToEol();
    void goToBeginningOfLine();
    void goToEndOfLine();
    void goToForwardChar();
    void goToBackwardChar();
    void insertChar( char c );
    void clearBuffer();
    void update();
    void selectAll();

    const std::string& getText() const;
    void setText( const std::string& text );

    void undo();

  private:

    void makeBody();
    void prepareUndo();

    FieldEditor* _target;

    TextBuffer _textBuffer;
    int32_t _pointPosition;
    int32_t _markPosition;
    FieldEditorAction* _fieldEditorAction;
    bool _validateNormalChar;
    float _minimumWidth;
    bool _cursorIsOn;
    float _frameThickness;
    
    std::string _oldText;
    int32_t _oldPointPosition;
    int32_t _oldMarkPosition;

};


}


#endif
