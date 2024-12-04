#ifndef _gkg_graphics_X11_core_TellTale_h_
#define _gkg_graphics_X11_core_TellTale_h_


#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <gkg-graphics-X11-core/MonoGlyph.h>
#include <gkg-core-pattern/Observer.h>
#include <gkg-core-pattern/Observable.h>


namespace gkg
{


class TellTaleGroup;


class TellTaleState : public RCGUIObject, public Observable
{

  public:

    enum Flag
    {

      FlagNone = 0x0,
      Enabled = 0x1,
      Visible = 0x2,
      EnabledVisible = 0x3,
      Active = 0x4,
      EnabledActive = 0x5,
      VisibleActive = 0x6,
      EnabledVisibleActive = 0x7,
      Chosen = 0x8,
      EnabledChosen = 0x9,
      VisibleChosen = 0xa,
      EnabledVisibleChosen = 0xb,
      ActiveChosen = 0xc,
      EnabledActiveChosen = 0xd,
      VisibleActiveChosen = 0xe,
      EnabledVisibleActiveChosen = 0xf,
      Running = 0x10,
      Choosable = 0x20,
      Toggle = 0x40,
      MaxFlag = 0x80

    };

    TellTaleState( const Flag flag = FlagNone );
    virtual ~TellTaleState();

    Flag getFlag() const;
    TellTaleGroup* getGroup() const;

    virtual void set( const Flag flag, bool b );
    virtual bool test( const Flag flag ) const;
    virtual void join( TellTaleGroup* group );
    virtual void leaveGroup();

  private:

    Flag _flag;
    TellTaleGroup* _group;

};


// glyph with a state that may affect appearance
class TellTale : public MonoGlyph, public Observer
{

  public:

    virtual ~TellTale();

    virtual void setState( TellTaleState* state );
    virtual TellTaleState* getState() const;

    virtual void unregisterObservable( Observable* observable );

  protected:

    TellTale( Glyph* glyph, TellTaleState* state = 0 );

  private:

    TellTaleState* _state;

};


class TellTaleGroup : public RCGUIObject
{

  public:

    TellTaleGroup();
    virtual ~TellTaleGroup();

    virtual void update( TellTaleState* state );
    virtual void remove( TellTaleState* state );

  private:

    TellTaleState* _current;

};


}


#endif
