#ifndef _gkg_graphics_X11_core_MacroAction_h_
#define _gkg_graphics_X11_core_MacroAction_h_


#include <deque>
#include <gkg-graphics-X11-core/Action.h>


namespace gkg
{


class MacroAction : public Action
{

  public:

    MacroAction( Action* a0 = 0, Action* a1 = 0,
                 Action* a2 = 0, Action* a3 = 0 );
    virtual ~MacroAction();

    virtual void prepend( Action* action );
    virtual void append( Action* action );
    virtual void insert( int32_t index, Action* action );
    virtual void remove( int32_t index );

    virtual int32_t getCount() const;
    virtual Action* getAction( int32_t index ) const;

    virtual void execute();

  private:

    std::deque< Action* > _actions;

};


}


#endif
