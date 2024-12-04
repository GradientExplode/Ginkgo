#include <gkg-graphics-X11-core/Button.h>
#include <gkg-graphics-X11-core/TellTale.h>
#include <gkg-core-pattern/Observable.h>
#include <gkg-graphics-X11-core/Action.h>
#include <gkg-graphics-X11-core/Event.h>


gkg::Button::Button( gkg::Glyph* glyph,
                     gkg::Style* style,
                     gkg::TellTaleState* tellTaleState,
                     gkg::Action* action )
            : gkg::ActiveHandler( glyph, style ),
              _state( tellTaleState ),
              _action( action )
{

  if ( _state )
  {

    _state->addReference();

  }
  _state->addObserver( this );
  if ( _action )
  {

    _action->addReference();

  }

}


gkg::Button::~Button()
{

  _state->removeObserver( this );
  if ( _state )
  {

    _state->removeReference();

  }
  if ( _action )
  {

    _action->removeReference();

  }

}


void gkg::Button::setState( gkg::TellTaleState* tellTaleState )
{

  if ( tellTaleState != _state )
  {

    if ( tellTaleState )
    {

      tellTaleState->addReference();

    }
    tellTaleState->addObserver( this );
    _state->removeObserver( this );
    if ( _state )
    {

      _state->removeReference();

    }
    _state = tellTaleState;

  }

}


gkg::TellTaleState* gkg::Button::getState() const
{

  return _state;

}


void gkg::Button::setAction( gkg::Action* action )
{

  if ( action != _action )
  {

    if ( action )
    {

      action->addReference();

    }
    if ( _action )
    {

      _action->removeReference();

    }
    _action = action;

  }

}


gkg::Action* gkg::Button::getAction() const
{

  return _action;

}


void gkg::Button::press( const gkg::Event& )
{


  if ( _state->test( gkg::TellTaleState::Enabled ) )
  {

    _state->set( gkg::TellTaleState::Active, true );

  }

}


void gkg::Button::release( const gkg::Event& event )
{

  if ( _state->test( gkg::TellTaleState::Enabled ) )
  {

    _state->set( gkg::TellTaleState::Active, false );
    if ( isInside( event ) )
    {

      bool chosen = _state->test( gkg::TellTaleState::Chosen );
      bool act = !chosen;
      if ( _state->test( gkg::TellTaleState::Toggle ) )
      {

        _state->set( gkg::TellTaleState::Chosen, act );
        act = true;

      }
      else if ( _state->test( gkg::TellTaleState::Choosable ) )
      {

        _state->set( gkg::TellTaleState::Chosen, true );

      }
      if ( act )
      {

        if ( _action )
        {

          _state->set( gkg::TellTaleState::Running, true );
          _action->execute();
          _state->set( gkg::TellTaleState::Running, false );

        }

      }

    }

  }

}


void gkg::Button::enter()
{

  if ( _state->test( gkg::TellTaleState::Enabled ) )
  {

    _state->set( gkg::TellTaleState::Visible, true );

  }

}


void gkg::Button::leave()
{

  if ( _state->test( gkg::TellTaleState::Enabled ) )
  {

    _state->set( gkg::TellTaleState::Visible, false );

  }

}


void gkg::Button::update( const gkg::Observable*, void* )
{

  redraw();

}
