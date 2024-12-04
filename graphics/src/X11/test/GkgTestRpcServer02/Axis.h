#ifndef _gkg_graphics_X11_GkgTestRpcServer02_Axis_h
#define _gkg_graphics_X11_GkgTestRpcServer02_Axis_h


// Axis - position tick marks as on axis


#include <gkg-graphics-X11-core/MonoGlyph.h>
#include <gkg-graphics-X11-core/Page.h>


class Axis : public gkg::MonoGlyph
{

  public:

    virtual void setRange( float first, float last ) = 0;
    virtual void allocate( gkg::Canvas* canvas,
                           const gkg::GeometryAllocation& geometryAllocation,
                           gkg::GeometryExtension& geometryExtension );

  protected:

    Axis( float first, float last );
    virtual ~Axis();

    gkg::GeometryAllocation _geometryAllocation;
    gkg::Page* _page;
    float _first;
    float _last;

};


class YAxis : public Axis
{

  public:

    YAxis( float first, float last );
    virtual 
      void request( gkg::GeometryRequisition& geometryRequisition ) const;
    virtual void setRange( float first, float last );

  protected:

    virtual ~YAxis();

};


class XAxis : public Axis
{

  public:

    XAxis( float first, float last );
    virtual void request( gkg::GeometryRequisition& geometryRequisition ) const;
    virtual void setRange( float first, float last );

  protected:

    virtual ~XAxis();

};


#endif
