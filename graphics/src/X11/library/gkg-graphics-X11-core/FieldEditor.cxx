#include <gkg-graphics-X11-core/FieldEditor.h>
#include <gkg-graphics-X11-core/FieldEditorImplementation.h>


//
// class FieldEditorAction
//

gkg::FieldEditorAction::FieldEditorAction()
                       : gkg::RCGUIObject()
{
}


gkg::FieldEditorAction::~FieldEditorAction()
{
}


void gkg::FieldEditorAction::accept( gkg::FieldEditor* )
{
}


void gkg::FieldEditorAction::cancel( gkg::FieldEditor* )
{
}


//
// class FieldEditor
//


gkg::FieldEditor::FieldEditor( const std::string& sample,
                               gkg::Style* style,
                               gkg::FieldEditorAction* fieldEditorAction,
                               bool validateNormalChar,
                               float minimumWidth )
                 : gkg::InputHandler( 0, style )
{

  _fieldEditorImplementation =
    new gkg::FieldEditorImplementation( this,
                                        sample,
                                        style,
                                        fieldEditorAction,
                                        validateNormalChar,
                                        minimumWidth );

}


gkg::FieldEditor::~FieldEditor()
{

  delete _fieldEditorImplementation;

}


void gkg::FieldEditor::press( const gkg::Event& event )
{

  _fieldEditorImplementation->press( event );

}


void gkg::FieldEditor::drag( const gkg::Event& event )
{

  _fieldEditorImplementation->drag( event );

}


void gkg::FieldEditor::keyStroke( const gkg::Event& event )
{

  _fieldEditorImplementation->keyStroke( event );

}


gkg::InputHandler* gkg::FieldEditor::focusIn()
{

  _fieldEditorImplementation->focusIn();
  return this->gkg::InputHandler::focusIn();

}


void gkg::FieldEditor::focusOut()
{

  _fieldEditorImplementation->focusOut();
  this->gkg::InputHandler::focusOut();

}


int32_t gkg::FieldEditor::locate( const gkg::Event& event )
{

  return _fieldEditorImplementation->locate( event );

}


void gkg::FieldEditor::deleteCharForward()
{

  _fieldEditorImplementation->deleteCharForward();

}


void gkg::FieldEditor::deleteCharBackward()
{

  _fieldEditorImplementation->deleteCharBackward();

}


void gkg::FieldEditor::deleteRegion()
{

  _fieldEditorImplementation->deleteRegion();

}


void gkg::FieldEditor::deleteToEol()
{

  _fieldEditorImplementation->deleteToEol();

}


void gkg::FieldEditor::goToBeginningOfLine()
{

  _fieldEditorImplementation->goToBeginningOfLine();

}


void gkg::FieldEditor::goToEndOfLine()
{

  _fieldEditorImplementation->goToEndOfLine();

}


void gkg::FieldEditor::goToForwardChar()
{

  _fieldEditorImplementation->goToForwardChar();

}


void gkg::FieldEditor::goToBackwardChar()
{

  _fieldEditorImplementation->goToBackwardChar();

}


void gkg::FieldEditor::insertChar( char c )
{

  _fieldEditorImplementation->insertChar( c );

}


void gkg::FieldEditor::clearBuffer()
{

  _fieldEditorImplementation->clearBuffer();

}


void gkg::FieldEditor::update()
{

  _fieldEditorImplementation->update();

}


void gkg::FieldEditor::selectAll()
{

  _fieldEditorImplementation->selectAll();

}


const std::string& gkg::FieldEditor::getText() const
{

  return _fieldEditorImplementation->getText();

}


void gkg::FieldEditor::setText( const std::string& text )
{

  return _fieldEditorImplementation->setText( text );

}


void gkg::FieldEditor::undo()
{

  _fieldEditorImplementation->undo();

}


gkg::FieldEditorImplementation* gkg::FieldEditor::getImplementation() const
{

  return _fieldEditorImplementation;

}
