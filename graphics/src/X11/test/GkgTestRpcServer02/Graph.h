#ifndef _gkg_graphics_X11_GkgTestRpcServer02_Graph_h
#define _gkg_graphics_X11_GkgTestRpcServer02_Graph_h


#include <string>
#include <gkg-graphics-X11-core/InputHandler.h>
#include <gkg-graphics-X11-core/XYMarker.h>


class Axis;
class Plot;

namespace gkg
{


class Color;
class Patch;


}


class Graph : public gkg::InputHandler
{

  public:

    Graph( float w, float h,
           float xBegin, float xEnd, 
           float yBegin, float yEnd,
           const gkg::Color* color,
           const std::string& symbol );

    void addPoint( gkg::FloatCoordinate x, gkg::FloatCoordinate y );

    virtual void request( gkg::GeometryRequisition& geometryRequisition ) const;
    virtual void allocate( gkg::Canvas* canvas,
                           const gkg::GeometryAllocation& geometryAllocation,
                           gkg::GeometryExtension& geometryExtension );
    virtual void press( const gkg::Event& event );

  protected:

    virtual ~Graph();
    virtual void mark( int32_t );

  protected:

    bool _init;
    int32_t _current;
    int32_t _count;
    gkg::FloatCoordinate* _x;
    gkg::FloatCoordinate* _y;
    float _w, _h;
    float _xRange, _yRange;
    float _xOrigin, _yOrigin;
    Plot* _plot;
    gkg::XYMarker* _marker;
    Axis* _xAxis;
    gkg::Patch* _xPatch;
    Axis* _yAxis;
    gkg::Patch* _yPatch;
    std::string _symbol;
    gkg::Patch* _pPatch;

};


#endif
