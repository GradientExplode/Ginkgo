#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_ModeStrategy_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_ModeStrategy_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/Cursor.h>
#include <gkg-processing-coordinates/Vector3d.h>

#include <QPoint>


class QPainter;


namespace gkg
{


class VoiDrawA;
class VoiData;


class VoiModeStrategy
{

  public:

    enum ModeId
    {
    
      SelectionMode = 1,
      PaintMode,
      EvolutionMode,
      FillingMode,
      PickingColorMode,
      StatisticMode,
      ContrastMode,
      SplineMode,
      MorphologyMode,
      ACSelectionMode,
      PCSelectionMode,
      HemiSelectionMode,
      COMMode,
      CylinderMode,
      DistanceMode
      
    };

    VoiModeStrategy();
    virtual ~VoiModeStrategy();

    virtual ModeId id();

    virtual void pressed( VoiDrawA&, VoiData&, 
                          Vector3d< int32_t >&, int32_t,
                          QPoint&,
                          Qt::KeyboardModifiers modifiers = Qt::NoModifier );
    virtual void erase( VoiDrawA&, VoiData&, 
                        Vector3d< int32_t >&, int32_t,
                        QPoint&,
                        Qt::KeyboardModifiers modifiers = Qt::NoModifier );
    virtual void moved( VoiDrawA&, VoiData&, 
                        Vector3d< int32_t >&, int32_t,
                        QPoint&,
                        Qt::KeyboardModifiers modifiers = Qt::NoModifier );
    virtual void released( VoiData& );

    virtual QCursor cursor();
    virtual void setIndicator( QPainter*, QPoint& );
    
};


}


#endif
