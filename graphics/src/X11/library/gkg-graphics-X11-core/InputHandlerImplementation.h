#ifndef _gkg_graphics_X11_core_InputHandlerImplementation_h_
#define _gkg_graphics_X11_core_InputHandlerImplementation_h_


#include <deque>
#include <gkg-graphics-X11-core/EventHandler.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class InputHandler;
class Style;
class GeometryAllocationInfo;
class GeometryAllocTable;


class InputHandlerImplementation : public EventHandler
{

  public:

    InputHandlerImplementation( InputHandler* inputHandler, Style* style );
    virtual ~InputHandlerImplementation();

    InputHandler* getTarget() const;

    virtual void setParent( InputHandler* parent );
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

    virtual Glyph::Index getFocusItem() const;
    virtual InputHandler* getFocusHandler() const;
    virtual void undraw();

    virtual bool event( Event& event );

    virtual GeometryAllocationInfo& getInfo( Canvas* canvas,
                                 const GeometryAllocation& geometryAllocation );
    virtual GeometryAllocationInfo* getMostRecentInfo();
    virtual void reset();
    virtual void down(Event&);
    virtual void motion(Event&);
    virtual void up(Event&);
    virtual bool isInside( const Event& event,
                           const GeometryAllocationInfo& info );

  private:

    InputHandler* _target;
    Style* _style;
    std::deque< InputHandler* > _children;
    Glyph::Index _focusItem;
    InputHandler* _focusHandler;
    InputHandler* _parent;
    GeometryAllocTable* _geometryAllocations;
    bool _pressed;
    bool _recordedTime;
    Event::Button _button;
    uint32_t _clickTime;

    static uint32_t _threshold;

};


}


#endif

