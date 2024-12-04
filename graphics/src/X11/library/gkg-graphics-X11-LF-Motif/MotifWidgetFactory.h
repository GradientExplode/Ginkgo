#ifndef _gkg_graphics_X11_LF_Motif_MotifWidgetFactory_h_
#define _gkg_graphics_X11_LF_Motif_MotifWidgetFactory_h_


#include <gkg-graphics-X11-core/WidgetFactory.h>
#include <gkg-core-pattern/SingletonAccessor.h>


namespace gkg
{


class MotifWidgetFactoryImplementation;


// object for creating common UI Motif-ish components
class MotifWidgetFactory : public WidgetFactory,
                           public SingletonAccessor< MotifWidgetFactory,
                                                     WidgetFactory >
{


  public:

    ~MotifWidgetFactory();

    //
    // L&F name
    //
    std::string getName() const;

    //
    // style management
    //
    void styleChanged( Style* style );

    //
    // widget creators
    //

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
    Glyph* createMenubarItemLook( Glyph* glyph,
                                  TellTaleState* state ) const;
    Glyph* createMenuItemLook( Glyph* glyph,
                               TellTaleState* state ) const;
    Glyph* createCheckMenuItemLook( Glyph* glyph,
                                    TellTaleState* state ) const;
    Glyph* createRadioMenuItemLook( Glyph* glyph,
                                    TellTaleState* state ) const;
    Glyph* createMenuItemSeparatorLook() const;

    // buttons
    Glyph* createPushButtonLook( Glyph* glyph,
                                 TellTaleState* state ) const;
    Glyph* createDefaultButtonLook( Glyph* glyph,
                                    TellTaleState* state ) const;
    Glyph* createCheckBoxLook( Glyph* glyph,
                               TellTaleState* state ) const;
    Glyph* createPaletteButtonLook( Glyph* glyph,
                                    TellTaleState* state ) const;
    Glyph* createRadioButtonLook( Glyph* glyph,
                                  TellTaleState* state ) const;
    Glyph* createNoMarginPushButtonLook( Glyph* glyph,
                                         TellTaleState* state ) const;

    // adjusters
    Glyph* createSliderLook( DimensionName dimensionName,
                             Adjustable* adjustable ) const;
    Glyph* createScrollBarLook( DimensionName dimensionName,
                                Adjustable* adjustable ) const;
    Glyph* createPannerLook( Adjustable* adjustableX,
                             Adjustable* adjustableY ) const;
    Glyph* createEnlargerLook( TellTaleState* tellTaleState ) const;
    Glyph* createReducerLook( TellTaleState* tellTaleState ) const;
    Glyph* createUpMoverLook( TellTaleState* tellTaleState ) const;
    Glyph* createDownMoverLook( TellTaleState* tellTaleState ) const;
    Glyph* createLeftMoverLook( TellTaleState* tellTaleState ) const;
    Glyph* createRightMoverLook( TellTaleState* tellTaleState ) const;

    static std::string getStaticName();

  protected:

    friend class Singleton< MotifWidgetFactory >;

    MotifWidgetFactory();

    MotifWidgetFactoryImplementation* _motifWidgetFactoryImplementation;

};


}


#endif
