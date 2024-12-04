#ifndef _gkg_graphics_X11_core_Composition_h_
#define _gkg_graphics_X11_core_Composition_h_


#include <deque>
#include <gkg-graphics-X11-core/MonoGlyph.h>


namespace gkg
{


class Compositor;
class Patch;


class Break
{

  public:

    enum Status
    {

      BreakViewed = 0x01,
      BreakValid = 0x02

    };

    Break();

    void setPatch( Patch* patch );
    Patch* getPatch() const;

    bool isViewed() const;
    void setViewed( bool value );

    bool isValid() const;
    void setValid( bool value );

    void setBegin( FloatCoordinate begin );
    FloatCoordinate getBegin() const;

    void setEnd( FloatCoordinate end );
    FloatCoordinate getEnd() const;

    void setFirst( Glyph::Index first );
    void incFirst();
    void decFirst();
    Glyph::Index getFirst() const;

    void setLast( Glyph::Index last );
    void incLast();
    void decLast();
    Glyph::Index getLast() const;

  private:

    Patch* _patch;
    int32_t _status;
    FloatCoordinate _begin;
    FloatCoordinate _end;
    Glyph::Index _first;
    Glyph::Index _last;

};


class Composition : public MonoGlyph
{

  public:

    struct Component
    {

      Glyph* glyph;

    };

    virtual ~Composition();

    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void pick( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation,
                       int32_t depth,
                       Hit& hit );
    virtual bool repair();

    virtual Glyph::Index getItem( Glyph::Index index );
    virtual Glyph::Index getBeginningOf( Glyph::Index item ) const;
    virtual Glyph::Index getEndOf( Glyph::Index item ) const;
    virtual void setMargin( Glyph::Index item,
                            FloatCoordinate begin, FloatCoordinate end );
    virtual void view( Glyph::Index first, Glyph::Index last );

    virtual void append( Glyph* other );
    virtual void prepend( Glyph* other );
    virtual void insert( Glyph::Index index, Glyph* other );
    virtual void remove( Glyph::Index index );
    virtual void replace( Glyph::Index index, Glyph* other );
    virtual void change( Glyph::Index index );

    virtual Glyph::Index getCount() const;
    virtual Glyph* getComponent( Glyph::Index index ) const;
    virtual void getGeometryAllotment(
                                   Glyph::Index index,
                                   DimensionName dimensionName,
                                   GeometryAllotment& geometryAllotment ) const;

  protected:

    Composition( Glyph* context,
                 Compositor* compositor,
                 Glyph* separator,
                 DimensionName dimensionName,
                 FloatCoordinate span,
                 FloatCoordinate stretch,
                 FloatCoordinate shrink,
                 Glyph::Index size );
    virtual void doRepair( Glyph::Index firstComponent,
                           Glyph::Index firstBreak,
                           Glyph::Index* breaks,
                           Glyph::Index breakCount );
    virtual void setDamage( Glyph::Index first, Glyph::Index last );
    virtual Glyph* getSeparator( Break& theBreak );
    virtual Glyph* makeItem( Break& theBreak, bool created );
    virtual Glyph* make( Break& theBreak );

  private:

    Compositor* _compositor;
    std::deque< Component > _components;
    std::deque< Break > _breaks;
    Glyph* _separator;
    bool _viewAll;
    bool _damaged;
    Glyph::Index _firstDamage;
    Glyph::Index _lastDamage;
    Glyph::Index _item;
    DimensionName _dimensionName;
    FloatCoordinate _span;
    bool _resizable;

};


}


#endif
