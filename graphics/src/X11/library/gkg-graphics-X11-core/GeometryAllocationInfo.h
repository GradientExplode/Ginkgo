#ifndef _gkg_graphics_X11_core_GeometryAllocationInfo_h_
#define _gkg_graphics_X11_core_GeometryAllocationInfo_h_


#include <gkg-graphics-X11-core/Transformer.h>
#include <gkg-graphics-X11-core/GeometryAllocation.h>
#include <gkg-graphics-X11-core/GeometryExtension.h>


namespace gkg
{


class Canvas;


class GeometryAllocationInfo
{

  public:

    GeometryAllocationInfo();
    virtual ~GeometryAllocationInfo();

    void setCanvas( Canvas* canvas );
    Canvas* getCanvas() const;

    void setGeometryAllocation( const GeometryAllocation& geometryAllocation );
    const GeometryAllocation& getGeometryAllocation() const;
    GeometryAllocation& getGeometryAllocation();

    void setTransformer( const Transformer& transformer );
    const Transformer& getTransformer() const;

    void setGeometryExtension( const GeometryExtension& geometryExtension );
    GeometryExtension& getGeometryExtension();
    const GeometryExtension& getGeometryExtension() const;

    void setComponentGeometryAllocations( 
                                      GeometryAllocation* geometryAllocations );
    GeometryAllocation* getComponentGeometryAllocations();

  private:

    Canvas* _canvas;
    Transformer _transformer;
    GeometryAllocation _geometryAllocation;
    GeometryExtension _geometryExtension;
    int32_t _componentCount;
    GeometryAllocation* _componentGeometryAllocations; 

};


}


#endif
