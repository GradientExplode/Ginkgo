#ifndef _gkg_graphics_X11_core_WidgetFactory_h_
#define _gkg_graphics_X11_core_WidgetFactory_h_


#include <string>
#include <gkg-graphics-X11-base/Color.h>
#include <gkg-graphics-X11-core/Label.h>
#include <gkg-graphics-X11-core/Button.h>
#include <gkg-graphics-X11-core/Menu.h>
#include <gkg-graphics-X11-core/Stepper.h>
#include <gkg-graphics-X11-core/ForwardScroller.h>
#include <gkg-graphics-X11-core/BackwardScroller.h>
#include <gkg-graphics-X11-core/ForwardPager.h>
#include <gkg-graphics-X11-core/BackwardPager.h>
#include <gkg-graphics-X11-core/ArrowDown.h>
#include <gkg-graphics-X11-core/ArrowLeft.h>
#include <gkg-graphics-X11-core/ArrowRight.h>
#include <gkg-graphics-X11-core/ArrowUp.h>
#include <gkg-graphics-X11-core/Action.h>
#include <gkg-graphics-X11-core/ActionCallback.h>
#include <gkg-graphics-X11-core/MacroAction.h>
#include <gkg-graphics-X11-core/TellTale.h>
#include <gkg-graphics-X11-core/ComboBox.h>
#include <gkg-graphics-X11-core/SpinBox.h>


namespace gkg
{


class Adjustable;
class Color;
class Cursor;
class Font;
class String;
class Style;

class WidgetFactoryImplementation;


// object for creating common UI components
class WidgetFactory
{


  public:

    virtual ~WidgetFactory();

    //
    // L&F name
    //
    virtual std::string getName() const;

    //
    //
    //
    virtual void setColoringMode( ColoringMode coloringMode );
    virtual ColoringMode getColoringMode() const;

    virtual void setSavedText( const std::string& text );
    virtual const std::string& getSavedText() const;

    //
    // style management
    //
    virtual void setStyle( Style* style );
    virtual Style* getStyle() const;
    virtual const Font* getFont() const;
    virtual const Color* getForeground() const;
    virtual const Color* getBackground() const;
    virtual void beginStyle( const std::string& name );
    virtual void beginStyle( const std::string& name,
                             const std::string& alias );
    virtual void setAlias( const std::string& alias );
    virtual void endStyle();
    virtual void pushStyle();
    virtual void pushStyle( Style* style );
    virtual void popStyle();
    virtual void styleChanged( Style* style );

    //
    // shared objects
    //
    virtual Cursor* getHandCursor() const;
    virtual Cursor* getLFastCursor() const;
    virtual Cursor* getLUFastCursor() const;
    virtual Cursor* getUFastCursor() const;
    virtual Cursor* getRUFastCursor() const;
    virtual Cursor* getRFastCursor() const;
    virtual Cursor* getRDFastCursor() const;
    virtual Cursor* getDFastCursor() const;
    virtual Cursor* getLDFastCursor() const;

    //
    // widget creators
    //

    // shading
    virtual MonoGlyph* createOutsetFrame( Glyph* glyph ) const = 0;
    virtual MonoGlyph* createInsetFrame( Glyph* glyph ) const = 0;
    virtual MonoGlyph* createBrightInsetFrame( Glyph* glyph ) const = 0;

    // styled low-level glyphs
    virtual Glyph* createLabel( const std::string& label ) const;
    virtual Glyph* createChiseledLabel( const std::string& label ) const;
    virtual Glyph* createRaisedLabel( const std::string& label ) const;
    virtual Glyph* createFancyLabel( const std::string& label ) const;

    // menus
    virtual Menu* createMenubar();
    virtual Menu* createPulldown();
    virtual Menu* createPullright();

    virtual MenuItem* createMenubarItem( const std::string& label  ) const;
    virtual MenuItem* createMenubarItem( Glyph* glyph ) const;

    virtual MenuItem* createMenuItem( const std::string& label ) const;
    virtual MenuItem* createMenuItem( Glyph* glyph ) const;

    virtual MenuItem* createCheckMenuItem( const std::string& label ) const;
    virtual MenuItem* createCheckMenuItem( Glyph* glyph ) const;

    virtual MenuItem* createRadioMenuItem( TellTaleGroup* group,
                                           const std::string& label ) const;
    virtual MenuItem* createRadioMenuItem( TellTaleGroup* group,
                                           Glyph* glyph ) const;

    virtual MenuItem* createMenuItemSeparator() const;

    // buttons
    virtual Button* createPushButton( const std::string& label,
                                      Action* action ) const;
    virtual Button* createPushButton( Glyph* glyph,
                                      Action* action ) const;

    virtual Button* createDefaultButton( const std::string& label,
                                         Action* action ) const;
    virtual Button* createDefaultButton( Glyph* glyph,
                                         Action* action ) const;

    virtual Button* createCheckBox( const std::string& label,
                                    Action* action ) const;
    virtual Button* createCheckBox( Glyph* glyph,
                                    Action* action ) const;

    virtual Button* createPaletteButton( const std::string& label,
                                         Action* action ) const;
    virtual Button* createPaletteButton( Glyph* glyph,
                                         Action* action ) const;

    virtual Button* createRadioButton( TellTaleGroup* group,
                                       const std::string& label,
                                       Action* action ) const;
    virtual Button* createRadioButton( TellTaleGroup* group,
                                       Glyph* glyph,
                                       Action* action ) const;

    virtual Button* createNoMarginPushButton( Glyph* glyph,
                                              Action* action ) const;

    // combo, spin boxes
    virtual ComboBox* createComboBox() const;
    virtual SpinBox* createSpinBox() const;

    // actions
    virtual Action* createQuitAction() const;

    // adjusters
    virtual Glyph* createHSlider( Adjustable* adjustable );
    virtual Glyph* createHScrollBar( Adjustable* adjustable );
    virtual Glyph* createVSlider( Adjustable* adjustable );
    virtual Glyph* createVScrollBar( Adjustable* adjustable );
    virtual Glyph* createPanner( Adjustable* adjustableX,
                                 Adjustable* adjustableY );
    virtual Stepper* createEnlarger( Adjustable* adjustable );
    virtual Stepper* createReducer( Adjustable* adjustable );
    virtual Stepper* createUpMover( Adjustable* adjustable );
    virtual Stepper* createDownMover( Adjustable* adjustable );
    virtual Stepper* createLeftMover( Adjustable* adjustable );
    virtual Stepper* createRightMover( Adjustable* adjustable );

    //
    // looks implemented by subclass, usually not called directly
    //

    // menus
    virtual Glyph* createMenubarLook() const = 0;
    virtual Glyph* createPulldownLook() const = 0;
    virtual Glyph* createPullrightLook() const;
    virtual Glyph* createMenubarItemLook(
                                       Glyph* glyph,
                                       TellTaleState* tellTaleState ) const = 0;
    virtual Glyph* createMenuItemLook( Glyph* glyph,
                                       TellTaleState* tellTaleState ) const = 0;
    virtual Glyph* createCheckMenuItemLook(
                                       Glyph* glyph,
                                       TellTaleState* tellTaleState ) const = 0;
    virtual Glyph* createRadioMenuItemLook(
                                       Glyph* glyph,
                                       TellTaleState* tellTaleState ) const = 0;
    virtual Glyph* createMenuItemSeparatorLook() const = 0;

    // buttons
    virtual Glyph* createPushButtonLook(
                                       Glyph* glyph,
                                       TellTaleState* tellTaleState ) const = 0;
    virtual Glyph* createDefaultButtonLook(
                                       Glyph* glyph,
                                       TellTaleState* tellTaleState ) const = 0;
    virtual Glyph* createCheckBoxLook( Glyph* glyph,
                                       TellTaleState* tellTaleState ) const = 0;
    virtual Glyph* createPaletteButtonLook(
                                       Glyph* glyph,
                                       TellTaleState* tellTaleState ) const = 0;
    virtual Glyph* createRadioButtonLook(
                                       Glyph* glyph,
                                       TellTaleState* tellTaleState ) const = 0;
    virtual Glyph* createNoMarginPushButtonLook(
                                       Glyph* glyph,
                                       TellTaleState* tellTaleState ) const = 0;

    // adjusters
    virtual Glyph* createSliderLook( DimensionName dimensionName,
                                     Adjustable* adjustable ) const = 0;
    virtual Glyph* createScrollBarLook( DimensionName dimensionName,
                                        Adjustable* adjustable ) const = 0;
    virtual Glyph* createPannerLook( Adjustable* adjustableX,
                                     Adjustable* adjustableY ) const = 0;
    virtual Glyph* createEnlargerLook( TellTaleState* tellTaleState ) const = 0;
    virtual Glyph* createReducerLook( TellTaleState* tellTaleState ) const = 0;
    virtual Glyph* createUpMoverLook( TellTaleState* tellTaleState ) const = 0;
    virtual Glyph* createDownMoverLook( 
                                       TellTaleState* tellTaleState ) const = 0;
    virtual Glyph* createLeftMoverLook(
                                       TellTaleState* tellTaleState ) const = 0;
    virtual Glyph* createRightMoverLook(
                                       TellTaleState* tellTaleState ) const = 0;

    WidgetFactoryImplementation* getImplementation() const;

  protected:

    WidgetFactory();

    WidgetFactoryImplementation* _widgetFactoryImplementation;

};


}


#endif
