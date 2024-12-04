#include <gkg-graphics-X11-core/GeometryAllocationInfo.h>


gkg::GeometryAllocationInfo::GeometryAllocationInfo()
{
}


gkg::GeometryAllocationInfo::~GeometryAllocationInfo()
{

  if ( _componentGeometryAllocations )
  {

    delete [] _componentGeometryAllocations;

  }

}


void gkg::GeometryAllocationInfo::setCanvas( gkg::Canvas* canvas )
{

  _canvas = canvas;

}


gkg::Canvas* gkg::GeometryAllocationInfo::getCanvas() const
{

  return _canvas;

}


void gkg::GeometryAllocationInfo::setGeometryAllocation(
                             const gkg::GeometryAllocation& geometryAllocation )
{

  _geometryAllocation = geometryAllocation;

}


const gkg::GeometryAllocation& 
gkg::GeometryAllocationInfo::getGeometryAllocation() const
{

  return _geometryAllocation;

}


gkg::GeometryAllocation& 
gkg::GeometryAllocationInfo::getGeometryAllocation()
{

  return _geometryAllocation;

}


void gkg::GeometryAllocationInfo::setTransformer(
                                           const gkg::Transformer& transformer )
{

  _transformer = transformer;

}


const gkg::Transformer& 
gkg::GeometryAllocationInfo::getTransformer() const
{

  return _transformer;

}


void gkg::GeometryAllocationInfo::setGeometryExtension(
                               const gkg::GeometryExtension& geometryExtension )
{

  _geometryExtension = geometryExtension;

}

gkg::GeometryExtension& 
gkg::GeometryAllocationInfo::getGeometryExtension()
{

  return _geometryExtension;

}


const gkg::GeometryExtension& 
gkg::GeometryAllocationInfo::getGeometryExtension() const
{

  return _geometryExtension;

}


void gkg::GeometryAllocationInfo::setComponentGeometryAllocations(
                                  gkg::GeometryAllocation* geometryAllocations )
{

  _componentGeometryAllocations = geometryAllocations;

}


gkg::GeometryAllocation* 
gkg::GeometryAllocationInfo::getComponentGeometryAllocations()
{

  return _componentGeometryAllocations;

}
