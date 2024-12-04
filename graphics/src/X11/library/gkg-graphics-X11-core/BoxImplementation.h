#ifndef _gkg_graphics_X11_core_BoxImplementation_h_
#define _gkg_graphics_X11_core_BoxImplementation_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class Box;
class Layout;
class Canvas;
class GeometryAllocationInfo;
class GeometryAllocTable;


class BoxImplementation
{

  public:

    BoxImplementation( Box* box, Layout* layout );
    virtual ~BoxImplementation();

    Box* getTarget() const;

    virtual bool getRequested() const;
    virtual const GeometryRequisition& getGeometryRequisition() const;

    virtual void request();
    virtual GeometryAllocationInfo& 
      getInfo( Canvas* canvas, const GeometryAllocation& geometryAllocation,
               GeometryExtension& geometryExtension );
    virtual void offsetAllocate( GeometryAllocationInfo& info,
                                 FloatCoordinate dx, FloatCoordinate dy );
    virtual void fullAllocate( GeometryAllocationInfo& info );
    virtual void invalidate();

    virtual void undraw();
    virtual void getGeometryAllotment(
                                   Glyph::Index index,
                                   DimensionName dimensionName,
                                   GeometryAllotment& geometryAllotment ) const;

  private:

    Box* _target;

    Layout* _layout;
    bool _requested;
    GeometryRequisition _geometryRequisition;
    GeometryAllocTable* _geometryAllocations;

};


}


#endif
