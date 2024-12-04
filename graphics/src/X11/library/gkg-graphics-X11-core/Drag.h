#ifndef _gkg_graphics_X11_core_Drag_h_
#define _gkg_graphics_X11_core_Drag_h_


#include <gkg-graphics-X11-core/MonoGlyph.h>


namespace gkg
{


// To create a dragable glyph:
// 1) create a derived class of Drag
// 2) implement the member functions dragGlyph and/or dragCursor.  dragGlyph
//    should return a glyph that will be dragged across the screen in a window.
//    dragCursor should return a cursor that will be dragged across the screen.
// 3) implement the member function dragData.  dragData should produce a
//    string to be sent to a drag zone
// 4) the dragType member function can be implemented to provide a string
//    that is sent to a drag zone when the drag zone is entered
// 5) caught, commit, and abort can be implemented to change the default
//    behavior.  the default behavior is that a middle button press starts a
//    drag, a chorded button press aborts, and a button release commits a drag.
// 6) dragOffset can be implemented to calculate a transformed offset from
//    the left-top corner of the dragGlyph.


class Cursor;
class Event;
class Hit;
class DragImplementation;


class Drag : public MonoGlyph
{

  public:

    Drag( Glyph* glyph );
    virtual ~Drag();

    virtual void setDragable( bool value );
    virtual bool isDragable() const;

    virtual Glyph* getDragGlyph() = 0;
    virtual Cursor* getDragCursor() = 0;
    virtual void getDragData( const char*& value, int32_t& length ) = 0;
    virtual void getDragType( const char*& value, int32_t& length );
    virtual void getDragOffset( Event& event, int32_t& dx, int32_t& dy );

    virtual bool caught( const Event& event ) const;
    virtual bool commit( const Event& event ) const;
    virtual bool abort( const Event& event ) const;

    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void pick( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation,
                       int32_t depth,
                       Hit& hit );

    DragImplementation* getImplementation() const;

  private:

    DragImplementation* _dragImplementation;

};


}


#endif
