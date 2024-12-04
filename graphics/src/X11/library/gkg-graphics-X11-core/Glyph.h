#ifndef _gkg_graphics_X11_core_Glyph_h_
#define _gkg_graphics_X11_core_Glyph_h_


#include <gkg-graphics-X11-base/Coordinates.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <gkg-graphics-X11-core/GeometryRequisition.h>
#include <gkg-graphics-X11-core/GeometryAllotment.h>
#include <gkg-graphics-X11-core/GeometryAllocation.h>
#include <gkg-graphics-X11-core/GeometryExtension.h>


namespace gkg
{


class Canvas;
class Hit;
class Printer;


class Glyph : public RCGUIObject
{

  public:

    enum BreakType
    {

      NoBreak,
      PreBreak,
      InBreak,
      PostBreak

    };
    typedef int32_t Index;

    virtual ~Glyph();

    virtual void request( GeometryRequisition& geometryRequisition ) const;
    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;
    virtual void print( Printer* printer,
                        const GeometryAllocation& geometryAllocation ) const;
    virtual void pick( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation,
                       int32_t depth,
                       Hit& hit );
    virtual void undraw();

    virtual Glyph* clone() const;
    virtual Glyph* compose( BreakType breakType );

    virtual void append( Glyph* other );
    virtual void prepend( Glyph* other );
    virtual void insert( Index index, Glyph* other );
    virtual void remove( Index index );
    virtual void replace( Index index, Glyph* other );
    virtual void change( Index index );

    virtual Index getCount() const;
    virtual Glyph* getComponent( Index index ) const;
    virtual void getGeometryAllotment(
                               Index index,
                               DimensionName dimensionName,
                               GeometryAllotment& geometryAllotment ) const;

  protected:

    Glyph();

};


}


#endif
