#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_LabelStrategy_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_LabelStrategy_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/EvolInfo.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-coordinates/Vector2d.h>
#include <gkg-processing-container/Volume.h>

#include <QPoint>
#include <QRect>

#include <list>


namespace gkg
{


class VoiData;


class VoiLabelStrategy
{

  public:

    enum FillSize
    {
    
      maxFill = 100000,

    };

    enum TypeId
    {
    
      ModeAxial = 1,
      ModeCoronal,
      ModeSagittal,
      Mode3D,
      
    };

    virtual ~VoiLabelStrategy();

    virtual TypeId id();

    virtual float grayLevel( Volume< float >&, 
                             Vector3d< int32_t >&, int32_t );

    virtual void doit( VoiData*, Volume< int16_t >&, Vector3d< int32_t >&, 
                       int32_t, int32_t, std::list< Vector2d< int32_t > >& );
    virtual void fill( VoiData*, Volume< int16_t >&, int32_t, int32_t, int32_t, 
                       int32_t, int32_t, int32_t* );
    virtual void evolution( VoiData*, Vector3d< int32_t >&, 
                            int32_t, int32_t, VoiEvolutionInfo& );
    virtual void spline( VoiData*, Volume< int16_t >&,
                         std::list< QPoint >&, int32_t, int32_t );
    virtual void morphology( VoiData*, Volume< int16_t >&, int32_t );
    virtual void undo( VoiData* );
    virtual void clearSelection( VoiData*, int32_t );
    virtual void clearAll( VoiData* );

  protected:

    std::list< Vector3d< int32_t > > bck;

};


}


#endif
