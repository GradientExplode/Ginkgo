#ifndef _gkg_graphics_X11_viewer2d_Viewer2d_h_
#define _gkg_graphics_X11_viewer2d_Viewer2d_h_


#include <gkg-graphics-X11-core/InputHandler.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


template < class T > class VolumeModel;
class Font;
class Color;


template < class T >
class Viewer2d : public InputHandler
{

  public:

    enum State
    {


      SliceModification,
      RankModification,
      Windowing,
      Idle

    };

    Viewer2d( VolumeModel< T >* volumeModel );
    Viewer2d( VolumeModel< T >* volumeModel, Style* style );
    virtual ~Viewer2d();

    void draw( Canvas* canvas,
               const GeometryAllocation& geometryAllocation ) const;
    void update( const Volume< T >& volume );

    void press( const Event& event );
    void drag( const Event& event );
    void release( const Event& event );
    
  private:

    double getNewThreshold( double oldValue,
                            double deltaValue ) const;

    VolumeModel< T >* _volumeModel;

    Font* _font;
    Color* _annotationColor;
    FloatCoordinate _x0;
    FloatCoordinate _y0;
    FloatCoordinate _x1;
    FloatCoordinate _y1;
    FloatCoordinate _x2;
    FloatCoordinate _y2;

    State _state;

    int32_t _currentSlice;
    int32_t _deltaSlice;

    int32_t _currentRank;
    int32_t _deltaRank;

    double _deltaLowerThreshold;
    double _deltaUpperThreshold;

};


}


#endif

