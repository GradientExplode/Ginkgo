#ifndef _gkg_graphics_X11_core_ChoiceItem_h_
#define _gkg_graphics_X11_core_ChoiceItem_h_


#include <gkg-graphics-X11-core/TellTale.h>


namespace gkg
{


class Deck;


// telltale with different look for each state
class ChoiceItem : public TellTale
{

  public:

    ChoiceItem( TellTaleState* tellTaleState );
    ChoiceItem( TellTaleState* tellTaleState,
                Glyph* normal,
                Glyph* pressed );
    ChoiceItem(	TellTaleState* tellTaleState,
                Glyph* disabled,
                Glyph* enabled,
                Glyph* visible,
                Glyph* visibleActive,
                Glyph* active,
                Glyph* chosen,
                Glyph* visibleChosen,
                Glyph* activeChosen,
                Glyph* visibleActiveChosen,
                Glyph* disabledChosen );
    virtual ~ChoiceItem();

    virtual void setLook( const TellTaleState::Flag include,
                          const TellTaleState::Flag exclude,
                          Glyph* glyph );
    virtual Glyph* getLook( const TellTaleState::Flag flag ) const;

    virtual void update( const Observable* observable, void* arg = 0 );

  private:

    void init();

    Deck* _deck;
    Glyph::Index _indices[ TellTaleState::MaxFlag ];

};


}


#endif
