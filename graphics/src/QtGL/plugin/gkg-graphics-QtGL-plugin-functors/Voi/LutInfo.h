#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_LutInfo_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_LutInfo_h_


#include <gkg-core-cppext/StdInt.h>
#include <vector>


namespace gkg
{


class VoiLutInfo
{

  public:

    enum LutSize
    {
    
      nLut = 65536,

    };

    VoiLutInfo();
    virtual ~VoiLutInfo();

    std::vector< uint8_t >& lut();
    const std::vector< uint8_t >& lut() const;

    float minLut();
    float maxLut();
    float getScaling();

    void setOrig( float omin, float omax );
    float getOrig();

    void computeLut( float, float );

    uint8_t operator [] ( float n );

  private:

    std::vector< uint8_t > lut16;

    float lutMin;
    float lutMax;

    float origMin;
    float origMax;
    float scaling;

};


}


#endif
