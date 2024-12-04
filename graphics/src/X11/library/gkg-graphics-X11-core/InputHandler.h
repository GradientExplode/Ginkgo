#ifndef _gkg_graphics_X11_core_InputHandler_h_
#define _gkg_graphics_X11_core_InputHandler_h_


#include <gkg-graphics-X11-core/MonoGlyph.h>


namespace gkg
{


class Style;
class Event;
class EventHandler;
class Transformer;
class InputHandlerImplementation;


// glyph that handles input
class InputHandler : public MonoGlyph
{

  public:

    InputHandler( Glyph* glyph, Style* style);
    virtual ~InputHandler();

    virtual EventHandler* getEventHandler() const;
    virtual InputHandler* getParent() const;
    virtual Style* getStyle() const;

    virtual void appendInputHandler( InputHandler* inputHandler );
    virtual void removeInputHandler( Glyph::Index index );
    virtual void removeAllInputHandlers();
    virtual Glyph::Index getInputHandlerCount() const;
    virtual InputHandler* getInputHandler( Glyph::Index index ) const;

    virtual void focus( InputHandler* inputHandler );
    virtual void nextFocus();
    virtual void previousFocus();

    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;
    virtual void pick( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation,
                       int32_t depth,
                       Hit& hit );
    virtual void undraw();

    virtual void move( const Event& event );
    virtual void press( const Event& event );
    virtual void drag( const Event& event );
    virtual void release( const Event& event );
    virtual void keyStroke( const Event& event );
    virtual void doubleClick( const Event& event );

    virtual InputHandler* focusIn();
    virtual void focusOut();

    virtual void geometryAllocationChanged(
                                 Canvas* canvas,
                                 const GeometryAllocation& geometryAllocation );
    virtual bool isInside( const Event& event );

    virtual Canvas* getCanvas() const;
    virtual const Transformer& getTransformer() const;
    virtual const GeometryAllocation& getGeometryAllocation() const;

    virtual void redraw() const;
    virtual void repick( int32_t depth, Hit& hit );

    InputHandlerImplementation* getImplementation() const;

  private:

    InputHandlerImplementation* _inputHandlerImplementation;

};


}


#endif
