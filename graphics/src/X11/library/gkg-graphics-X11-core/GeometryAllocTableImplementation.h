#ifndef _gkg_graphics_X11_core_GeometryAllocTableImplementation_h_
#define _gkg_graphics_X11_core_GeometryAllocTableImplementation_h_


#include <deque>
#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class GeometryAllocationInfo;
class Canvas;
class GeometryAllocation;


// An allocation table is represented by a list of pointers to geometry
// allocation information objects.  The list is ordered by usage
// from least recently used to most recently used.  Whenever a lookup
// is done on the table that finds the information for a <canvas, allocation>
// pair, the pointer to the information is removed from its current position
// in the list and put at the end.  In the common case where the list
// has a single element, moving the found element is unnecessary.

class GeometryAllocTableImplementation
{

  public:

    GeometryAllocTableImplementation( Glyph::Index count,
                                      int32_t maximumGeometryAllocations );
    virtual ~GeometryAllocTableImplementation();

    virtual GeometryAllocationInfo*
      find( Canvas* canvas,const GeometryAllocation& geometryAllocation );
    virtual GeometryAllocationInfo*
      findSameSize( Canvas* canvas,
                    const GeometryAllocation& geometryAllocation,
                    FloatCoordinate& dx, FloatCoordinate& dy );
    virtual GeometryAllocationInfo*
      allocate( Canvas* canvas, const GeometryAllocation& geometryAllocation );
    virtual GeometryAllocationInfo* getMostRecent() const;
    virtual void flush();

    static bool equal( const GeometryAllocation& a1,
                       const GeometryAllocation& a2 );
    static bool sameSize( const GeometryAllotment& a1,
                          const GeometryAllotment& a2 );

  private:

    Glyph::Index _count;
    int32_t _maximumGeometryAllocations;
    std::deque< GeometryAllocationInfo* > _geometryAllocations;

    static const float _epsilon;

};


}


#endif
