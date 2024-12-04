#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_Data_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_Data_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/Observable.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/LutInfo.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/LabelInfo.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/EvolInfo.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/LabelStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/VolumeBaseFusion.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Slicer.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-coordinates/Vector2d.h>
#include <gkg-processing-container/Volume.h>

#include <list>


namespace gkg
{


class VoiData : public VoiObservable
{

  public:

    VoiData();
    virtual ~VoiData();

    VoiVolumeBaseFusion& getBaseFusion();
    const VoiVolumeBaseFusion& getBaseFusion() const;
    Volume< int16_t >& label();
    VoiLutInfo& lutInfo( bool shiftPressed = false );
    VoiLabelInfo& labelInfo();
    const VoiLabelInfo& labelInfo() const;
    VoiEvolutionInfo& evolInfo();
    const VoiEvolutionInfo& evolInfo() const;
    bool dataLoaded() const;
    bool labelLoaded() const;
    float min();
    float max();
    float plage();
    int32_t fusionLabel();
    void setFusionLabel( int32_t );
    int32_t fusionBaseVolume();
    void setFusionBaseVolume( int32_t );
    bool empty() const;
    void setBrushSize( int32_t bs );
    void setCircleBrushSize( int32_t bs );
    void modifyLabelInfo( int32_t, const std::string&, 
                          int32_t, int32_t, int32_t );
    void setCurrentLabel( int32_t i );
    void setLabelPoint( Vector3d< int32_t >&, int32_t, int32_t );
    void fillLabel( Vector3d< int32_t >&, int32_t, int32_t );
    void evolutionLabel( Vector3d< int32_t >&, int32_t, int32_t );
    void splineToLabel( std::list< QPoint >&, int32_t slice, int32_t label );
    void morphologyLabel( Volume< int16_t >& roi, int32_t label );
    void undoEvolution();
    void clearSelectedLabel();
    void clearAllLabels();
    float grayLevel( Vector3d< int32_t >&, int32_t );
    void setLabelStrategy( VoiLabelStrategy * );
    void saveLabel( const std::string&, const QString& );
    void setMorphologyRadius( double radius );
    double getMorphologyRadius();
    void setMorphologyOperation( int32_t operation );
    int32_t getMorphologyOperation();
    void setCylinderDiameter( int32_t diameter );
    int32_t getCylinderDiameter();
    void setCylinderOrientation( int32_t dir );
    int32_t getCylinderOrientation();
    void toggleContour();
    bool isContour();

    void initialize( const std::string& t1Name,
                     const std::string& volBName,
                     const std::string& transfoName,
                     const std::string& labelName );

    Slicer& getSlicer();

  private:
  
    bool dataOk;
    bool labelOk;
    bool _isContour;
    Volume< int16_t > _labels;
    VoiVolumeBaseFusion _baseFusion;
    float minimum;
    float maximum;
    VoiLabelInfo labInfo;
    VoiEvolutionInfo evInfo;
    float pctPlage;
    int32_t fusFactLabel;
    int32_t fusFactBaseVolume;
    std::list< Vector2d< int32_t > > _brushOffsets;
    double _morphoRadius;
    int32_t _morphoOperation;
    int32_t _cylinderDiameter;
    int32_t _cylinderOrientation;
    VoiLabelStrategy* strgy;
    Slicer _slicer;

};


}


#endif
