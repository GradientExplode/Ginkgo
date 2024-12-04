#ifndef _gkg_graphics_X11_core_DragZoneSink_h_
#define _gkg_graphics_X11_core_DragZoneSink_h_


#include <gkg-graphics-X11-core/DragZone.h>


namespace gkg
{


class DragZoneSinkImplementation;


// A drag zone sink publishes that a window can accept drag messages.  It
// also consumes any drag messages that do not fall in a drag zone.  This
// class could be eliminated and the code moved to X11WindowImplementation.cxx


class DragZoneSink : public DragZone
{

  public:

    DragZoneSink( Glyph* glyph );
    virtual ~DragZoneSink();

    virtual void drop( Event& event, const char* data, int32_t length );

    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;
    virtual void pick( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation,
                       int32_t depth,
                       Hit& hit );

    virtual bool event( Event& event );

    DragZoneSinkImplementation* getImplementation() const;

  private:

    DragZoneSinkImplementation* _dragZoneSinkImplementation;

};


}


#endif
