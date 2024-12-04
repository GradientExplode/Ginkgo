#include <gkg-graphics-X11-core/Browser.h>
#include <gkg-graphics-X11-core/TellTale.h>
#include <gkg-graphics-X11-core/Action.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-core/Hit.h>
#include <gkg-graphics-X11-core/Style.h>


gkg::Browser::Browser( gkg::Glyph* glyph,
                       gkg::Style* style,
                       gkg::Action* accept,
                       gkg::Action* cancel )
             : gkg::InputHandler( glyph, style ),
               gkg::Observer(),
               _accept( accept ),
               _cancel( cancel ),
               _item( -1 )
{

  if ( _accept )
  {

     _accept->addReference();

  }
  if ( _cancel )
  {

     _cancel->addReference();

  }

}


gkg::Browser::~Browser()
{

  if ( _accept )
  {

     _accept->removeReference();

  }
  if ( _cancel )
  {

     _cancel->removeReference();

  }

}


void gkg::Browser::appendSelectable( gkg::TellTaleState* state )
{

  _items.push_back( state );

}


void gkg::Browser::replaceSelectable( gkg::Glyph::Index index,
                                      gkg::TellTaleState* state )
{

  _items[ index ] = state;

}


void gkg::Browser::removeSelectable( gkg::Glyph::Index index )
{

  _items.erase( _items.begin() + index );

}


gkg::TellTaleState* gkg::Browser::getState( gkg::Glyph::Index index ) const
{

  return _items[ index ];

}


void gkg::Browser::select( gkg::Glyph::Index index )
{

  if ( _item != index )
  {

    if ( _item != -1 )
    {

      active( _item, false );

    }
    if ( index >= -1 && index < ( gkg::Glyph::Index )_items.size() )
    {

      _item = index;
      if ( index >= 0 )
      {

        active( _item, true );

      }

    }

  }

}


gkg::Glyph::Index gkg::Browser::getSelected() const
{

  return _item;

}


void gkg::Browser::choose( gkg::Glyph::Index index ) const
{

  if ( ( index != -1 ) && _accept )
  {

    _accept->execute();

  }

}


void gkg::Browser::cancel()
{

  if ( _cancel )
  {

    _cancel->execute();

  }

}


void gkg::Browser::press( const gkg::Event& event )
{

  gkg::Hit hit( &event );
  repick( 0, hit );
  if ( hit.hasAny() )
  {

    select( hit.getIndex( 0 ) );

  }

}


void gkg::Browser::drag( const gkg::Event& event )
{

  if ( isInside( event ) )
  {

    gkg::Hit hit( &event );
    repick( 0, hit );
    if ( hit.hasAny() )
    {

      select( hit.getIndex( 0 ) );
      return;

    }

  }
  select( -1 );

}


void gkg::Browser::release( const gkg::Event& )
{

  if ( getStyle()->getValueIsOn( "singleClick" ) )
  {

    choose( _item );

  }

}


void gkg::Browser::doubleClick( const gkg::Event& )
{

  choose( _item );

}


void gkg::Browser::update( const gkg::Observable*, void* )
{

  redraw();

}


void gkg::Browser::active( gkg::Glyph::Index index, bool b )
{

  gkg::TellTaleState* state = _items[ index ];
  state->addObserver( this );
  state->set( gkg::TellTaleState::Active, b );
  state->removeObserver( this );

}

