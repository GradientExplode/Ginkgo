#include <gkg-graphics-X11-core/TellTale.h>
#include <gkg-core-pattern/Observable.h>
#include <gkg-core-pattern/Observer.h>


//
// class TellTaleState
//

gkg::TellTaleState::TellTaleState( const gkg::TellTaleState::Flag flag )
                   : gkg::RCGUIObject(),
                     gkg::Observable(),
                     _flag( flag ),
                     _group( 0 )
{
}


gkg::TellTaleState::~TellTaleState()
{

  leaveGroup();

}


gkg::TellTaleState::Flag gkg::TellTaleState::getFlag() const
{

  return _flag;

}


gkg::TellTaleGroup* gkg::TellTaleState::getGroup() const
{

  return _group;

}


void 
gkg::TellTaleState::set( const gkg::TellTaleState::Flag flag, bool b )
{

  gkg::TellTaleState::Flag 
    newFlag = ( gkg::TellTaleState::Flag )( b ? ( _flag | flag ) :
                                                ( _flag & ~flag ) );
  if ( newFlag != _flag )
  {

    _flag = newFlag;
    setChanged();
    notifyObservers();
    if ( _group )
    {

      _group->update( this );

    }

  }

}


bool gkg::TellTaleState::test(const gkg::TellTaleState::Flag flag ) const
{

  return ( _flag & flag ) == flag;

}


void gkg::TellTaleState::join( gkg::TellTaleGroup* group )
{

  if ( _group != group )
  {

    if ( group )
    {

      group->addReference();

    }
    leaveGroup();
    _group = group;

  }

}


void gkg::TellTaleState::leaveGroup()
{

  if ( _group )
  {

    _group->remove( this );
    _group->removeReference();

  }

}


// class TellTale

gkg::TellTale::TellTale( gkg::Glyph* glyph, gkg::TellTaleState* state )
              : gkg::MonoGlyph( glyph ),
                gkg::Observer(),
                _state( state )
{

  if ( _state )
  {

    _state->addReference();

  }
  _state->addObserver( this );

}


gkg::TellTale::~TellTale()
{

  _state->removeObserver( this );
  if ( _state )
  {

    _state->removeReference();

  }

}


void gkg::TellTale::setState( gkg::TellTaleState* state )
{

  if ( state )
  {

    state->addReference();

  }
  if ( _state )
  {

    _state->removeReference();

  }
  _state = state;

}


gkg::TellTaleState* gkg::TellTale::getState() const
{

  return _state;

}


void gkg::TellTale::unregisterObservable( gkg::Observable* )
{

  _state = 0;

}


//
// class TellTaleGroup

gkg::TellTaleGroup::TellTaleGroup()
                   : gkg::RCGUIObject(),
                     _current( 0 )
{
}


gkg::TellTaleGroup::~TellTaleGroup()
{
}


void gkg::TellTaleGroup::update( gkg::TellTaleState* state )
{

  if ( ( state != _current ) && ( state->test( gkg::TellTaleState::Chosen ) ) )
  {

    if ( _current )
    {

      _current->set( gkg::TellTaleState::Chosen, false );

    }
    _current = state;

  }

}


void gkg::TellTaleGroup::remove( gkg::TellTaleState* state )
{

  if ( _current == state )
  {

    _current = 0;

  }

}

