#ifndef _gkg_graphics_X11_core_Browser_h_
#define _gkg_graphics_X11_core_Browser_h_


#include <deque>
#include <gkg-graphics-X11-core/InputHandler.h>
#include <gkg-core-pattern/Observer.h>


namespace gkg
{


class Action;
class TellTaleState;


// select an item
class Browser : public InputHandler, public Observer
{

  public:

    Browser( Glyph* glyph,
             Style* style,
             Action* accept,
             Action* cancel );
    virtual ~Browser();

    virtual void appendSelectable( TellTaleState* state );
    virtual void replaceSelectable( Glyph::Index index, TellTaleState* state );
    virtual void removeSelectable( Glyph::Index index );

    virtual TellTaleState* getState( Glyph::Index index ) const;

    virtual void select( Glyph::Index index );
    virtual Glyph::Index getSelected() const;

    virtual void choose( Glyph::Index index ) const;
    virtual void cancel();

    virtual void press( const Event& event );
    virtual void drag( const Event& event );
    virtual void release( const Event& event );
    virtual void doubleClick( const Event& event );

    virtual void update( const Observable* observable, void* arg = 0 );

  private:

    void active( Glyph::Index i, bool b );

    Action* _accept;
    Action* _cancel;
    std::deque< TellTaleState* > _items;
    Glyph::Index _item;

};


}


#endif
