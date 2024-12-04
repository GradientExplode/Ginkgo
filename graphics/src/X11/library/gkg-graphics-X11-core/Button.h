#ifndef _gkg_graphics_X11_core_Button_h_
#define _gkg_graphics_X11_core_Button_h_


#include <gkg-graphics-X11-core/ActiveHandler.h>
#include <gkg-core-pattern/Observer.h>


namespace gkg
{


class Action;
class TellTaleState;
class Observable;


// clickable Action
class Button : public ActiveHandler, public Observer
{

  public:

    Button( Glyph* glyph,
            Style* style,
            TellTaleState* tellTaleState,
            Action* action );
    virtual ~Button();

    virtual void setState( TellTaleState* tellTaleState );
    virtual TellTaleState* getState() const;

    virtual void setAction( Action* action );
    virtual Action* getAction() const;

    virtual void press( const Event& );
    virtual void release( const Event& event );
    virtual void enter();
    virtual void leave();

    virtual void update( const Observable* observable, void* arg = 0 );

  private:

    TellTaleState* _state;
    Action* _action;

};


}


#endif
