#ifndef _gkg_graphics_X11_core_WidgetFactoryImplementation_h_
#define _gkg_graphics_X11_core_WidgetFactoryImplementation_h_


#include <string>
#include <list>
#include <gkg-graphics-X11-base/Color.h>
#include <gkg-graphics-X11-core/TellTale.h>


namespace gkg
{


class WidgetFactory;
class Style;
class Color;
class Font;
class Glyph;
class Cursor;
class Button;
class MenuItem;
class Action;


class WidgetFactoryImplementation
{

  public:

    WidgetFactoryImplementation( WidgetFactory* widgetFactory );
    ~WidgetFactoryImplementation();

    WidgetFactory* getTarget() const;

    void setColoringMode( ColoringMode coloringMode );
    ColoringMode getColoringMode() const;

    void setStyle( Style* style );
    Style* getStyle() const;
    const Font* getFont();
    const Color* getForeground();
    const Color* getBackground();
    void setAlias( const std::string& alias );
    void pushStyle();
    void popStyle();

    Cursor* getHandCursor();
    Cursor* getLFastCursor();
    Cursor* getLUFastCursor();
    Cursor* getUFastCursor();
    Cursor* getRUFastCursor();
    Cursor* getRFastCursor();
    Cursor* getRDFastCursor();
    Cursor* getDFastCursor();
    Cursor* getLDFastCursor();

    void setSavedText( const std::string& text );
    const std::string& getSavedText() const;

    Glyph* createFancyLabel( const std::string& label );

    Glyph* buildFancyLabel( Glyph* above, Glyph* below, Glyph* glyph ) const;
    FloatColorIntensity getLabelShading() const;

    TellTaleState* beginStyle( const std::string& s1,
                               const std::string& s2,
                               TellTaleState::Flag flag );
    TellTaleState* beginMenubarItemStyle();
    TellTaleState* beginMenuItemStyle();
    TellTaleState* beginCheckMenuItemStyle();
    TellTaleState* beginRadioMenuItemStyle( TellTaleGroup* group );
    TellTaleState* beginMenuItemSeparatorStyle();
    MenuItem* endMenuItemStyle( Glyph* glyph, TellTaleState* tellTaleState );
    TellTaleState* beginPushButtonStyle();
    TellTaleState* beginDefaultButtonStyle();
    TellTaleState* beginCheckBoxStyle();
    TellTaleState* beginPaletteButtonStyle();
    TellTaleState* beginRadioButtonStyle( TellTaleGroup* group );
    TellTaleState* beginNoMarginPushButtonStyle();
    Button* endButtonStyle( Glyph* glyph, TellTaleState* tellTaleState,
                            Action* action);

  private:

    void update();
    void updateStyleInfo();
    void reportError( const std::string& operation,
                      const std::string& name,
                      const std::string& value );

    WidgetFactory* _target;

    ColoringMode _coloringMode;
    Style* _style;
    bool _styleChanged;
    const Font* _font;
    const Color* _foreground;
    const Color* _background;
    std::list< Style* > _styles;

    Cursor* _handCursor;
    Cursor* _lFastCursor;
    Cursor* _luFastCursor;
    Cursor* _uFastCursor;
    Cursor* _ruFastCursor;
    Cursor* _rFastCursor;
    Cursor* _rdFastCursor;
    Cursor* _dFastCursor;
    Cursor* _ldFastCursor;

    bool _initializedLabelStyles;
    std::string _chiseledLabelStyle;
    std::string _raisedLabelStyle;

    std::string _savedText;

};


}


#endif
