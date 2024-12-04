#ifndef _gkg_graphics_X11_LF_Motif_MotifWidgetFactoryImplementation_h_
#define _gkg_graphics_X11_LF_Motif_MotifWidgetFactoryImplementation_h_


#include <string>
#include <list>
#include <gkg-graphics-X11-core/Bevel.h>


namespace gkg
{


class MotifWidgetFactory;
class Style;
class TellTaleState;
class Glyph;
class Slider;
class Adjustable;
class ChoiceItem;
class MotifInfo;


class MotifWidgetFactoryImplementation 
{

  public:

    MotifWidgetFactoryImplementation( MotifWidgetFactory* motifWidgetFactory );
    ~MotifWidgetFactoryImplementation();

    MotifWidgetFactory* getTarget() const;

    void styleChanged( Style* style );

    // shading
    MonoGlyph* createOutsetFrame( Glyph* glyph ) const;
    MonoGlyph* createInsetFrame( Glyph* glyph ) const;
    MonoGlyph* createBrightInsetFrame( Glyph* glyph ) const;

    //
    // looks implemented by subclass, usually not called directly
    //

    // menus
    Glyph* createMenubarLook() const;
    Glyph* createPulldownLook() const;
    Glyph* createMenubarItemLook(
             Glyph* glyph,
             TellTaleState* tellTaleState ) const;
    Glyph* createMenuItemLook(
             Glyph* glyph,
             TellTaleState* tellTaleState ) const;
    Glyph* createCheckMenuItemLook(
             Glyph* glyph,
             TellTaleState* tellTaleState ) const;
    Glyph* createRadioMenuItemLook(
             Glyph* glyph,
             TellTaleState* tellTaleState ) const;
    Glyph* createMenuItemSeparatorLook() const;

    // buttons
    Glyph* createPushButtonLook(
             Glyph* glyph,
             TellTaleState* tellTaleState ) const;
    Glyph* createDefaultButtonLook(
             Glyph* glyph,
             TellTaleState* tellTaleState ) const;
    Glyph* createCheckBoxLook(
             Glyph* glyph,
             TellTaleState* tellTaleState ) const;
    Glyph* createPaletteButtonLook(
             Glyph* glyph,
             TellTaleState* tellTaleState ) const;
    Glyph* createRadioButtonLook(
             Glyph* glyph,
             TellTaleState* tellTaleState ) const;
    Glyph* createNoMarginPushButtonLook(
             Glyph* glyph,
             TellTaleState* tellTaleState ) const;

    // adjusters
    Glyph* createSliderLook(
             DimensionName dimensionName,
             Adjustable* adjustable ) const;
    Glyph* createScrollBarLook(
             DimensionName dimensionName,
             Adjustable* adjustable ) const;
    Glyph* createPannerLook(
             Adjustable* adjustableX,
             Adjustable* adjustableY ) const;
    Glyph* createEnlargerLook(
             TellTaleState* tellTaleState ) const;
    Glyph* createReducerLook(
             TellTaleState* tellTaleState ) const;
    Glyph* createUpMoverLook(
             TellTaleState* tellTaleState ) const;
    Glyph* createDownMoverLook(
             TellTaleState* tellTaleState ) const;
    Glyph* createLeftMoverLook(
             TellTaleState* tellTaleState ) const;
    Glyph* createRightMoverLook(
             TellTaleState* tellTaleState ) const;

  private:

    Glyph* makeMenuItem( TellTaleState* tellTaleState,
                         Glyph* check,
                         Glyph* ) const;
    ChoiceItem* makeButton( TellTaleState* tellTaleState,
                            Glyph* normal,
                            Glyph* pressed,
                            Glyph* disabled ) const;
    ChoiceItem* makeToggle( TellTaleState* tellTaleState,
                            Bevel::Beveler beveler,
                            float scale ) const;
    ChoiceItem* makeMover( TellTaleState* tellTaleState,
                           Bevel::Beveler beveler ) const;
    Slider* makeSlider( Slider* slider ) const;

    bool match( const Style& s1, const Style& s2, const std::string& name );

    MotifWidgetFactory* _target;
    MotifInfo* _motifInfo;
    std::list< MotifInfo* > _motifInfos;

};


}


#endif
