#ifndef _gkg_graphics_X11_core_GeometryAllocTable_h_
#define _gkg_graphics_X11_core_GeometryAllocTable_h_


#include <gkg-graphics-X11-base/Coordinates.h>
#include <gkg-graphics-X11-core/Glyph.h>
#include <gkg-graphics-X11-core/GeometryAllocation.h>
#include <gkg-graphics-X11-core/GeometryAllocationInfo.h>


namespace gkg
{


class GeometryAllocTableImplementation;


// common information for < canvas, geometryAllocation > pairs
class GeometryAllocTable
{

  public:

    GeometryAllocTable( Glyph::Index count = 1,
                        int32_t maximumGeometryAllocations = 5 );
    virtual ~GeometryAllocTable();

    virtual GeometryAllocationInfo*
      find( Canvas* canvas,const GeometryAllocation& geometryAllocation ) const;
    virtual GeometryAllocationInfo*
      findSameSize( Canvas* canvas,
                    const GeometryAllocation& geometryAllocation,
                    FloatCoordinate& dx, FloatCoordinate& dy ) const;
    virtual GeometryAllocationInfo*
      allocate( Canvas* canvas, const GeometryAllocation& geometryAllocation );
    virtual GeometryAllocationInfo* getMostRecent() const;
    virtual void flush();

    GeometryAllocTableImplementation* getImplementation() const;

  private:

    GeometryAllocTableImplementation* _geometryAllocTableImplementation;

};


}


#endif
