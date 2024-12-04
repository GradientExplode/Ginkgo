#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_VolumeBaseFusion_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_VolumeBaseFusion_h_

#include <gkg-processing-container/Volume.h>
#include <gkg-processing-transform/HomogeneousTransform3d.h>

#include <gkg-graphics-QtGL-plugin-functors/Voi/LutInfo.h>
#include <gkg-processing-colorimetry/RGBComponent.h>


namespace gkg
{


class VoiVolumeBaseFusion
{

  public:

    VoiVolumeBaseFusion();
    ~VoiVolumeBaseFusion();

    bool load( const std::string& t1Name,
               const std::string& volBName,
               const std::string& transfoName );
    void setFusion( const int8_t& ); // 0:only T1, 1:T1 + ADC, 2:T1 + RGB

    Volume< float >& getT1();
    VoiLutInfo& getLut( bool shift );

    float getMinT1();
    float getMaxT1();
    int8_t getFusion();
    RGBComponent getFusionColor( int32_t fusion, int32_t x, int32_t y = 0,
                                 int32_t z = 0, int32_t t = 0 );

  private:

    void loadT1( const std::string& );
    void loadVolB( const std::string& );

    void initializeT1();
    void initializeAdc();
    void initializeRgb();
    
    Volume< float > _t1;
    Volume< float > _adc;
    Volume< RGBComponent > _rgb;
    Vector3d< double > _res;
    Vector3d< double > _res2;
    VoiLutInfo _lutT1;
    VoiLutInfo _lutAdc;
    VoiLutInfo _lutRgb;
    HomogeneousTransform3d< float > _trmVolBToT1;
    float _minT1;
    float _maxT1;
    int8_t _fusion;
    int32_t _dimXB;
    int32_t _dimYB;
    int32_t _dimZB;

};


}


#endif
