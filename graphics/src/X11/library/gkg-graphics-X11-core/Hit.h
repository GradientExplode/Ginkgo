#ifndef _gkg_graphics_X11_core_Hit_h_
#define _gkg_graphics_X11_core_Hit_h_


#include <gkg-graphics-X11-base/Coordinates.h>
#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class Event;
class EventHandler;
class HitImplementation;
class Transformer;


class Hit
{

  public:

    Hit( const Event* event );
    Hit( FloatCoordinate x, FloatCoordinate y );
    Hit( FloatCoordinate left,
         FloatCoordinate bottom,
         FloatCoordinate right,
         FloatCoordinate top );
    virtual ~Hit();

    virtual const Event* getEvent() const;
    virtual FloatCoordinate getLeft() const;
    virtual FloatCoordinate getBottom() const;
    virtual FloatCoordinate getRight() const;
    virtual FloatCoordinate getTop() const;

    virtual void pushTransform();
    virtual void setTransform( const Transformer& transformer );
    virtual void popTransform();

    virtual void begin( int32_t depth, Glyph* glyph, Glyph::Index index,
                        EventHandler* handler = 0 );
    virtual void setTarget( int32_t depth, Glyph* glyph, Glyph::Index index,
                            EventHandler* eventHandler = 0 );
    virtual void end();
    virtual void remove( int32_t depth, Glyph::Index target = 0 );
    virtual void retarget( int32_t depth, Glyph* glyph, Glyph::Index index,
                           EventHandler* eventHandler = 0,
                           Glyph::Index target = 0 );

    virtual bool hasAny() const;
    virtual int32_t getCount() const;
    virtual int32_t getDepth( Glyph::Index target = 0 ) const;
    virtual Glyph* getTarget( int32_t depth, Glyph::Index target = 0 ) const;
    virtual Glyph::Index getIndex( int32_t depth, 
                                   Glyph::Index target = 0 ) const;
    virtual EventHandler* getEventHandler() const;

    HitImplementation* getImplementation() const;

  private:

    HitImplementation* _hitImplementation;

};


}


#endif
