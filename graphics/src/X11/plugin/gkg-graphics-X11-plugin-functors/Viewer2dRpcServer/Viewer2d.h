#ifndef _gkg_graphics_X11_plugin_functors_Viewer2dRpcServer_Viewer2d_h_
#define _gkg_graphics_X11_plugin_functors_Viewer2dRpcServer_Viewer2d_h_


#include <gkg-graphics-X11-core/InputHandler.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-graphics-X11-base/Color.h>
#include <gkg-graphics-X11-base/Coordinates.h>
#include <gkg-processing-colorimetry/RGBComponent.h>


namespace gkg
{


class Font;
class Color;
class Raster;
class Patch;


class Viewer2d : public InputHandler
{

  public:

    enum State
    {


      SliceModification,
      RankModification,
      WindowingAverage,
      WindowingActivation,
      FusionModification,
      Idle

    };

    Viewer2d( bool doRgb = false, bool rainbowPalette = false );
    Viewer2d( Style* style );
    virtual ~Viewer2d();

    void draw( Canvas* canvas,
               const GeometryAllocation& geometryAllocation ) const;
    void update();

    void press( const Event& event );
    void drag( const Event& event );
    void release( const Event& event );
				    
    Volume< int16_t >& getAverageVolume();
    Volume< float >& getActivationVolume();
    Volume< gkg::RGBComponent >& getRGBVolume();

    void reset( bool doRedraw = false );
    void fillRaster( int32_t slice, int32_t rank );
    void threshold( const float& inputValue,
                    FloatColorIntensity& r,
                    FloatColorIntensity& g,
                    FloatColorIntensity& b,
                    FloatColorIntensity& a,
		    double lowerThreshold,
		    double upperThreshold,
		    gkg::FloatColorIntensity pal[ 256 ][ 3 ] ) const;
   
  private:

    void init();
    void processMinimum();
    void processMaximum();
    double getNewThreshold( double oldValue,
                            double deltaValue ) const;

    Volume< int16_t > _volumeAverage;
    Volume< float > _volumeActivation;
    Volume< gkg::RGBComponent > _volumeRGB;
    bool _first;
    bool _multiVolume;
    bool _rainbowPalette;

    Font* _font;
    Color* _annotationColor;
    FloatCoordinate _x0;
    FloatCoordinate _y0;
    FloatCoordinate _x1;
    FloatCoordinate _y1;
    FloatCoordinate _x2;
    FloatCoordinate _y2;

    State _state;

    int32_t _repetitionCount;
    int32_t _currentSlice;
    int32_t _deltaSlice;

    int32_t _currentRank;
    int32_t _deltaRank;

    double _lowerThresholdAverage;
    double _upperThresholdAverage;
    double _deltaLowerThresholdAverage;
    double _deltaUpperThresholdAverage;

    double _minimumAvg;
    double _maximumAvg;
    double _minimumCor;
    double _maximumCor;
    
    double _lowerThresholdActivation;
    double _upperThresholdActivation;
    double _deltaLowerThresholdActivation;
    double _deltaUpperThresholdActivation;
    
    int32_t _fusionPercentage;
    int32_t _deltaFusion;

    Raster* _raster;
    Patch* _patch;

    bool _doRgb;
    
};


}


#endif
