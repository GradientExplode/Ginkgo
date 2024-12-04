#include <gkg-graphics-X11-core/ChoiceItem.h>
#include <gkg-graphics-X11-core/Deck.h>
#include <gkg-graphics-X11-core/Target.h>


gkg::ChoiceItem::ChoiceItem( gkg::TellTaleState* tellTaleState )
                : gkg::TellTale( 0, tellTaleState )
{

  init();

}


gkg::ChoiceItem::ChoiceItem( gkg::TellTaleState* tellTaleState,
                             gkg::Glyph* normal,
                             gkg::Glyph* pressed )
                : gkg::TellTale( 0, tellTaleState )
{

  init();
  setLook( gkg::TellTaleState::FlagNone,
           gkg::TellTaleState::Active, normal );
  setLook( gkg::TellTaleState::EnabledActive,
           gkg::TellTaleState::FlagNone,
           pressed );

}


gkg::ChoiceItem::ChoiceItem( gkg::TellTaleState* tellTaleState,
                             gkg::Glyph* disabled,
                             gkg::Glyph* enabled,
                             gkg::Glyph* visible,
                             gkg::Glyph* visibleActive,
                             gkg::Glyph* active,
                             gkg::Glyph* chosen,
                             gkg::Glyph* visibleChosen,
                             gkg::Glyph* activeChosen,
                             gkg::Glyph* visibleActiveChosen,
                             gkg::Glyph* disabledChosen )
                : gkg::TellTale( 0, tellTaleState )
{

  init();
  setLook( gkg::TellTaleState::FlagNone,
           gkg::TellTaleState::EnabledChosen,
           disabled );
  setLook( gkg::TellTaleState::Enabled,
           gkg::TellTaleState::VisibleActiveChosen,
           enabled );
  setLook( gkg::TellTaleState::EnabledVisible,
           gkg::TellTaleState::ActiveChosen,
           visible );
  setLook( gkg::TellTaleState::EnabledVisibleActive,
           gkg::TellTaleState::Chosen,
           visibleActive );
  setLook( gkg::TellTaleState::EnabledActive,
           gkg::TellTaleState::VisibleChosen,
           active );
  setLook( gkg::TellTaleState::EnabledChosen,
           gkg::TellTaleState::VisibleActive,
           chosen );
  setLook( gkg::TellTaleState::EnabledVisibleChosen,
           gkg::TellTaleState::Active,
           visibleChosen );
  setLook( gkg::TellTaleState::EnabledActiveChosen,
           gkg::TellTaleState::Visible,
           activeChosen );
  setLook( gkg::TellTaleState::EnabledVisibleActiveChosen,
           gkg::TellTaleState::FlagNone,
           visibleActiveChosen );
  setLook( gkg::TellTaleState::Chosen,
           gkg::TellTaleState::Enabled,
           disabledChosen );

}


gkg::ChoiceItem::~ChoiceItem()
{ 
}


void gkg::ChoiceItem::setLook( const gkg::TellTaleState::Flag include,
                               const gkg::TellTaleState::Flag exclude,
                               gkg::Glyph* glyph )
{

  gkg::Glyph::Index index = -1;
  gkg::TellTaleState::Flag flag = getState()->getFlag();
  for ( int32_t s = 0; s < ( int32_t )gkg::TellTaleState::MaxFlag; s++ )
  {

    if ( ( s & include) == include && ( s & exclude ) == 0 )
    {

      gkg::Glyph::Index& i = _indices[ s ];
      if ( i == -1 )
      {

        if ( index == -1 )
        {

          index = _deck->getCount();
          _deck->append( glyph );

        }
        i = index;
        if ( s == flag)
        {

          _deck->flipTo( index );

        }

      }
      else
      {

        _deck->replace( i, glyph );

      }

    }

  }

}


gkg::Glyph* 
gkg::ChoiceItem::getLook( const gkg::TellTaleState::Flag flag ) const
{

  if ( ( flag < gkg::TellTaleState::MaxFlag ) && ( _indices[ flag ] != -1 ) )
  {

    return _deck->getComponent( _indices[ flag ] );

  }
  return 0;

}


void gkg::ChoiceItem::update( const gkg::Observable*, void* )
{

  gkg::TellTaleState::Flag flag = getState()->getFlag();
  if ( ( flag < gkg::TellTaleState::MaxFlag ) && ( _indices[ flag ] != -1 ) )
  {

    _deck->flipTo( _indices[ flag ] );

  }

}


void gkg::ChoiceItem::init()
{

  _deck = new gkg::Deck;
  for ( int32_t s = 0; s < ( int32_t )gkg::TellTaleState::MaxFlag; s++ )
  {

    _indices[ s ] = -1;

  }
  getState()->set( gkg::TellTaleState::Enabled, true );
  setBody( new gkg::Target( _deck, gkg::Target::PrimitiveHit ) );

}

