#ifndef _gkg_graphics_X11_core_DragZone_h_
#define _gkg_graphics_X11_core_DragZone_h_


#include <gkg-graphics-X11-core/MonoGlyph.h>


namespace gkg
{


// To create a drag zone glyph:
//   1) create a derived class of DragZone
//   2) implement the member function drop; drop will be called when a drag
//      glyph is dropped in a DragZone glyph.
//   3) enter, motion, and leave can be implemented to indicate when a drop
//      would be sent to a drag zone.


class DragZoneImplementation;
class Event;


class DragZone : public MonoGlyph
{

  public:

    DragZone( Glyph* glyph );
    virtual ~DragZone();

    virtual void setSensitive( bool sensitive );
    virtual bool isSensitive() const;

    virtual void enter( Event& event, const char* type, int32_t length );
    virtual void motion( Event& event );
    virtual void leave( Event& event );

    virtual void drop( Event& event, const char* data, int32_t length ) = 0;

    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void pick( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation,
                       int32_t depth,
                       Hit& hit );

    DragZoneImplementation* getImplementation() const;

  private:

    DragZoneImplementation* _dragZoneImplementation;

};


}


#endif
