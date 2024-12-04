#ifndef _gkg_graphics_X11_core_HitImplementation_h_
#define _gkg_graphics_X11_core_HitImplementation_h_


#include <gkg-graphics-X11-base/Coordinates.h>
#include <gkg-graphics-X11-core/Glyph.h>


#define HIT_TARGET_LIST_FIXED        20
#define HIT_LIST_FIXED                5
#define HIT_STACK_FIXED              25
#define HIT_TARGET_AREA_LIST_FIXED   25


namespace gkg
{


class Event;
class EventHandler;
class Transformer;

struct HitTarget
{

  Glyph* glyph;
  Glyph::Index index;
  EventHandler* eventHandler;

};


struct HitTargetList
{

  int32_t available;
  int32_t used;
  HitTarget* targets;
  HitTarget fixedTargets[ HIT_TARGET_LIST_FIXED ];

};


struct HitList
{

  int32_t available;
  int32_t used;
  HitTargetList* lists;
  HitTargetList fixedLists[ HIT_LIST_FIXED ];

};


struct PossibleHitTarget
{

  bool picked;
  Glyph::Index count;
  int32_t depth;
  HitTarget item;

};


struct HitStack
{

  int32_t available;
  int32_t used;
  PossibleHitTarget* possibleTargets;
  PossibleHitTarget fixedPossibleTargets[ HIT_STACK_FIXED ];
};


struct HitTargetArea
{

  FloatCoordinate left;
  FloatCoordinate bottom;
  FloatCoordinate right;
  FloatCoordinate top;

};


struct HitTargetAreaList
{

  int32_t available;
  int32_t used;
  HitTargetArea* areas;
  HitTargetArea fixedAreas[ HIT_TARGET_AREA_LIST_FIXED ];

};


class HitImplementation
{

  public:

    HitImplementation( const Event* event );
    HitImplementation( FloatCoordinate x1, FloatCoordinate y1,
                       FloatCoordinate x2, FloatCoordinate y2 );
    virtual ~HitImplementation();

    virtual const HitTargetArea& getArea() const;
    virtual void setArea( FloatCoordinate left, FloatCoordinate bottom,
                          FloatCoordinate right, FloatCoordinate top );

    virtual const Event* getEvent() const;
    virtual FloatCoordinate getLeft() const;
    virtual FloatCoordinate getBottom() const;
    virtual FloatCoordinate getRight() const;
    virtual FloatCoordinate getTop() const;

    virtual void pushTransform();
    virtual void setTransform( const Transformer& transformer );
    virtual void popTransform();

    virtual void begin( int32_t depth, Glyph* glyph, Glyph::Index index,
                        EventHandler* eventHandler );
    virtual void setTarget( int32_t depth, Glyph* glyph, Glyph::Index index,
                            EventHandler* eventHandler );
    virtual void end();
    virtual void remove( int32_t depth, Glyph::Index target );

    virtual bool hasAny() const;
    virtual int32_t getCount() const;
    virtual int32_t getDepth( Glyph::Index target ) const;
    virtual Glyph* getTarget( int32_t depth, Glyph::Index target );
    virtual Glyph::Index getIndex( int32_t depth, Glyph::Index target );
    virtual EventHandler* getEventHandler() const;

    virtual void addItem( bool override, int32_t depth, Glyph* glyph,
                          Glyph::Index index, EventHandler* eventHandler,
                          Glyph::Index target );
    virtual HitTarget& getItem( int32_t depth, Glyph::Index index );



  private:

    void initialize( FloatCoordinate x1, FloatCoordinate y1,
                     FloatCoordinate x2, FloatCoordinate y2 ); 

    const Event* _event;
    EventHandler* _defaultEventHandler;
    int32_t _defaultEventHandlerDepth;
    HitList _items;
    HitStack _picks;
    HitTargetAreaList _areas;

};


}


#endif
