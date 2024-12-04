#include <gkg-graphics-X11-core/FieldEditorImplementation.h>
#include <gkg-graphics-X11-core/FieldEditor.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/Brush.h>
#include <gkg-graphics-X11-core/Background.h>
#include <gkg-graphics-X11-core/Character.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Target.h>
#include <gkg-graphics-X11-core/PlanarFigure.h>
#include <gkg-graphics-X11-core/TextBuffer.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <iostream>


gkg::FieldEditorImplementation::FieldEditorImplementation(
                                      FieldEditor* fieldEditor,
                                      const std::string& sample,
                                      gkg::Style* style,
                                      gkg::FieldEditorAction* fieldEditorAction,
                                      bool validateNormalChar,
                                      float minimumWidth )
                               : _target( fieldEditor ),
                                 _textBuffer( sample ),
                                 _pointPosition( 0 ),
                                 _markPosition( 0 ),
                                 _fieldEditorAction( fieldEditorAction ),
                                 _validateNormalChar( validateNormalChar ),
                                 _minimumWidth( minimumWidth ),
                                 _cursorIsOn( false )

{

  style->findAttribute( "frameThickness", _frameThickness );
  makeBody();

}


gkg::FieldEditorImplementation::~FieldEditorImplementation()
{
}


gkg::FieldEditor* gkg::FieldEditorImplementation::getTarget() const
{

  return _target;

}


void gkg::FieldEditorImplementation::press( const gkg::Event& event )
{

  focusIn();
  _pointPosition = locate( event );
  if ( !event.shiftIsDown() )
  {

    _markPosition = _pointPosition;
    update();

  }

}


void gkg::FieldEditorImplementation::drag( const gkg::Event& event )
{

  focusIn();
  _pointPosition = locate( event );
  update();

}


void gkg::FieldEditorImplementation::keyStroke( const gkg::Event& event )
{

  focusIn();
  prepareUndo();

  char buffer[ 4 ];

  int32_t count = event.getMapKey( buffer, 4 );
  uint32_t keySym = event.getKeySym();

#if 0

  std::cout << "character=[ buffer=( ";
  std::cout << ( int32_t )*( (unsigned char*)&buffer[0] ) << ", ";
  std::cout << ( int32_t )*( (unsigned char*)&buffer[1] ) << ", ";
  std::cout << ( int32_t )*( (unsigned char*)&buffer[2] ) << ", ";
  std::cout << ( int32_t )*( (unsigned char*)&buffer[3] ) << " ); ";
  std::cout << "keysym=" << keySym << "; ";
  std::cout << "count=" << count << " ]" << std::endl;

#endif

  if ( count == 1 )
  {

    switch ( buffer[ 0 ] )
    {

      case '\001':  // Ctrl-A

        goToBeginningOfLine();
        break;

      case '\002':  // Ctrl-B

        goToBackwardChar();
        break;

      case '\004':  // Ctrl-D

        if ( _markPosition == _pointPosition )
        {

          deleteCharForward();

        }
        else
        {

          deleteRegion();

        }
        if ( _validateNormalChar && _fieldEditorAction )
        {

          _fieldEditorAction->accept( _target );

        }
        break;

      case '\005':  // Ctrl-E

        goToEndOfLine();
        break;

      case '\006':  // Ctrl-F
	    
        goToForwardChar();
        break;

      case '\007':  // Ctrl-G
      case '\033':  // Escape

        if ( _fieldEditorAction )
        {

          _fieldEditorAction->cancel( _target );

        }
        clearBuffer();
        break;

      case '\010':  // Ctrl-H ou DEL

        if ( _markPosition == _pointPosition )
        {

          deleteCharBackward();

        }
        else 
        {

          deleteRegion();

        }
        if ( _validateNormalChar && _fieldEditorAction )
        {

          _fieldEditorAction->accept( _target );

        }
        break;

      case '\177':  // SUPPR

        if ( _markPosition == _pointPosition ) 
        {

          deleteCharForward();

        }
        else
        {

          deleteRegion();

        }
        if ( _validateNormalChar && _fieldEditorAction )
        {

          _fieldEditorAction->accept( _target );

        }
        break;

      case '\013':  // Ctrl-K

        deleteToEol();
        if ( _validateNormalChar && _fieldEditorAction )
        {

          _fieldEditorAction->accept( _target );

        }
        break;

      case '\015':  //  Ctrl-M

        if ( _fieldEditorAction )
        {

          _fieldEditorAction->accept( _target );

        }
        break;

      case '\t':    // TAB

        focusOut();
        break;

      default:  // the "normal" case, insert a char, too bad it's buried here..
        
        if ( _markPosition != _pointPosition )
        {

          deleteRegion();

        }
        insertChar( buffer[ 0 ] );
        if ( _validateNormalChar && _fieldEditorAction )
        {

          _fieldEditorAction->accept( _target );

        }
        break;

    }

  }
  else if ( count == 0 )
  {

    switch ( keySym )
    {

      case 65361U: // LEFT ARROW

        goToBackwardChar();
        break;

      case 65363U: // RIGHT ARROW

        goToForwardChar();
        break;

      case 65360U: // BEGIN

        goToBeginningOfLine();
        break;

      case 65367U: // END

        goToEndOfLine();
        break;

    }

  }

}


void gkg::FieldEditorImplementation::focusIn()
{

  cursorOn();
  update();

}


void gkg::FieldEditorImplementation::focusOut()
{

  cursorOff();
  update();
  if ( _fieldEditorAction )
  {

    _fieldEditorAction->accept( _target );

  }

}


void gkg::FieldEditorImplementation::cursorOn()
{

  _cursorIsOn = true;

}


void gkg::FieldEditorImplementation::cursorOff()
{

  _cursorIsOn = false;

}


int32_t gkg::FieldEditorImplementation::locate( const Event& event )
{

  gkg::WidgetFactory* widgetFactory = 
    gkg::LookAndFeel::getInstance().getWidgetFactory();

  const gkg::GeometryAllocation& 
    geometryAllocation = _target->getGeometryAllocation();
  float x = event.getPointerX();
  int32_t loc = 0;
  float xOffset = geometryAllocation.getLeft() + _frameThickness;
  while ( loc < _textBuffer.getLength() )
  {

    xOffset += widgetFactory->getFont()->getWidth(
                                      _textBuffer.getText().substr( loc, 1 ) );
    if ( xOffset < x )
    {

      ++ loc;

    }
    else
    {

      break;

    }

  }
  return loc;

}


void gkg::FieldEditorImplementation::deleteCharForward()
{

  if ( _pointPosition < _textBuffer.getLength() )
  {

    _textBuffer.erase( _pointPosition, 1 );
    update();

  }

}


void gkg::FieldEditorImplementation::deleteCharBackward()
{

  if ( _pointPosition > 0 )
  {

    _pointPosition --;
    _markPosition = _pointPosition;
    _textBuffer.erase( _pointPosition, 1 );
    update();

  }

}


void gkg::FieldEditorImplementation::deleteRegion()
{

  if ( _markPosition > _pointPosition )
  {

    for ( int32_t i = _pointPosition; i < _markPosition; i++ )
    {

      _textBuffer.erase( _pointPosition, 1 );

    }
    _markPosition = _pointPosition;

  }
  else 
  {

    for ( int32_t i = _markPosition; i < _pointPosition; i++ )
    {
 
      _textBuffer.erase( _markPosition, 1 );

    }
    _pointPosition= _markPosition;

  }
  update();

}


void gkg::FieldEditorImplementation::deleteToEol()
{

  while ( _textBuffer.getLength() > _pointPosition )
  {

    _textBuffer.erase( _pointPosition, 1 );

  }
  update();

}


void gkg::FieldEditorImplementation::goToBeginningOfLine()
{

  _markPosition = _pointPosition = 0;
  update();

}


void gkg::FieldEditorImplementation::goToEndOfLine()
{

  _markPosition = _pointPosition = _textBuffer.getLength();
  update();

}


void gkg::FieldEditorImplementation::goToForwardChar()
{

  if ( _pointPosition < _textBuffer.getLength() )
  {

    _pointPosition ++;
    _markPosition = _pointPosition;
    update();

  }

}


void gkg::FieldEditorImplementation::goToBackwardChar()
{

  if ( _pointPosition > 0 )
  {

    _pointPosition --;
    _markPosition = _pointPosition;
    update();

  }

}


void gkg::FieldEditorImplementation::insertChar( char c )
{

  gkg::WidgetFactory* widgetFactory = 
    gkg::LookAndFeel::getInstance().getWidgetFactory();

  // check to see if this character would exceed the allocation
  const gkg::GeometryAllocation& 
    geometryAllocation = _target->getGeometryAllocation();

  float swidth = widgetFactory->getFont()->getWidth( _textBuffer.getText() );
  float cwidth = widgetFactory->getFont()->getWidth( std::string( &c, 1 ) );
  float fwidth = geometryAllocation.getRight() -
                 geometryAllocation.getLeft() -
                 _frameThickness * 2;
  if ( swidth + cwidth > fwidth )
  {

    std::cerr << "\007";
    return;

  }

  _pointPosition ++;
  _markPosition = _pointPosition;
  _textBuffer.insert( _pointPosition - 1, std::string( &c, 1 ) );
  update();

}


void gkg::FieldEditorImplementation::clearBuffer()
{

  _textBuffer.erase( 0, _textBuffer.getLength() );
  _markPosition = _pointPosition = 0;
  update();

}


void gkg::FieldEditorImplementation::update()
{

  makeBody();
  _target->redraw();

}


void gkg::FieldEditorImplementation::selectAll()
{

  _markPosition = 0;
  _pointPosition = _textBuffer.getLength();
  update();

}


const std::string& gkg::FieldEditorImplementation::getText() const
{

  return _textBuffer.getText();

}


void gkg::FieldEditorImplementation::setText( const std::string& text )
{

  _textBuffer.erase( _textBuffer.getBeginningOfText(),
                     _textBuffer.getLength() );
  _textBuffer.insert( 0, text );
  update();

}


void gkg::FieldEditorImplementation::undo()
{

  std::cerr << "\007";
  _textBuffer.erase( 0, _textBuffer.getLength() );
  _textBuffer.insert( 0, _oldText );
  _pointPosition = _oldPointPosition;
  _markPosition = _oldMarkPosition;
  update();

}


void gkg::FieldEditorImplementation::makeBody()
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  gkg::WidgetFactory* widgetFactory = 
    gkg::LookAndFeel::getInstance().getWidgetFactory();

  const gkg::Color* bgColor = widgetFactory->getBackground();

  gkg::PolyGlyph* label = layoutFactory.createHBox();
  gkg::PolyGlyph* body = layoutFactory.createOverlay();
  body->append( layoutFactory.createVCenter( label ) );

  label->append( layoutFactory.createHSpace( 2 ) );
  int32_t bar1 = std::min( _pointPosition, _markPosition );
  int32_t bar2 = std::max( _pointPosition, _markPosition );
  int32_t i = 0;
  gkg::Display* 
    display = gkg::Session::getInstance().getDefaultDisplay();
  int32_t fontPixelHeight = display->toPixelCoordinate(
                                        widgetFactory->getFont()->getHeight() );

  for ( ; i < bar1; i++ )
  {

    label->append( new gkg::Character( _textBuffer.getChar( i ),
                                          widgetFactory->getFont(),
                                          gkg::Color::lookup( display,
                                                                 "black" ) ) );

  }

  for ( ; i < bar2; i++ )
  {

    label->append( new gkg::Background(
                     new gkg::Character( _textBuffer.getChar( i ),
                                            widgetFactory->getFont(),
                                            bgColor ),
                     gkg::Color::lookup( display, "black" ) ) );

  }

  for ( ; i < _textBuffer.getLength(); i++ )
  {

    label->append( new gkg::Character( _textBuffer.getChar( i ),
                                          widgetFactory->getFont(),
                                          gkg::Color::lookup( display,
                                                                 "black" ) ) );

  }

  label->append( layoutFactory.createHGlue( 0, fil, fil ) );
  if ( _cursorIsOn )
  {

    gkg::PolyGlyph* cursor = layoutFactory.createHBox();
    body->append( layoutFactory.createVCenter( cursor ) );
    float currentOffset = widgetFactory->getFont()->getWidth(
                           _textBuffer.getText().substr( 0, _pointPosition ) );
    cursor->append( layoutFactory.createHSpace( currentOffset ) );
    cursor->append( new gkg::PlanarFigureLine( new gkg::Brush( 0 ),
                                               widgetFactory->getForeground(),
                                               0, 0, 0, 0,
                                               fontPixelHeight ) );
    cursor->append( layoutFactory.createHGlue( 0, fil, fil ) );

  }
  if ( _minimumWidth > 0.0 )
  {

    _target->setBody( widgetFactory->createInsetFrame( 
                        layoutFactory.createHNatural(
                          layoutFactory.createVFixed(
                            new gkg::Target( body,
                                             gkg::Target::PrimitiveHit ),
                            fontPixelHeight ),
                          _minimumWidth ) ) );

  }
  else
  {

    _target->setBody( widgetFactory->createInsetFrame(
                        layoutFactory.createVFixed(
                          new gkg::Target( body, gkg::Target::PrimitiveHit ),
                          fontPixelHeight ) ) );

  }

}


void gkg::FieldEditorImplementation::prepareUndo()
{

  _oldText = _textBuffer.getText();
  _oldPointPosition = _pointPosition;
  _oldMarkPosition = _markPosition;

}
