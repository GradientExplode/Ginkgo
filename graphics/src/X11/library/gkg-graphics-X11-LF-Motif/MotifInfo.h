#ifndef _gkg_graphics_X11_LF_Motif_MotifInfo_h_
#define _gkg_graphics_X11_LF_Motif_MotifInfo_h_


#include <gkg-graphics-X11-base/Coordinates.h>
#include <gkg-graphics-X11-base/Color.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>


namespace gkg
{


class Style;
class Color;


class MotifInfo : public RCGUIObject
{

  public:

    MotifInfo( Style* style, ColoringMode coloringMode );
    ~MotifInfo();

    Style* getStyle() const;
    ColoringMode getColoringMode() const;

    FloatCoordinate getThickness() const;
    float getToggleScale() const;
    float getRadioScale() const;
    FloatCoordinate getMoverSize() const;
    FloatCoordinate getSliderSize() const;

    const Color* getFlat() const;
    const Color* getLight() const;
    const Color* getDull() const;
    const Color* getDark() const;
    const Color* getGrayOut() const;

  private:

    void load();
    void unload();

    Style* _style;
    ColoringMode _coloringMode;

    FloatCoordinate _thickness;
    float _toggleScale;
    float _radioScale;
    FloatCoordinate _moverSize;
    FloatCoordinate _sliderSize;

    const Color* _flat;
    const Color* _light;
    const Color* _dull;
    const Color* _dark;
    const Color* _grayOut;

};


}


#endif
