#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_ReadProcess_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_ReadProcess_h_


#include <gkg-processing-process/Process.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-colorimetry/RGBComponent.h>

#include <string>


namespace gkg
{


class ReadProcess : public Process
{

  public:
  
    ReadProcess( const std::string& outputType,
                 Volume< int16_t >* volInt16 = 0,
                 Volume< float >* volFloat = 0,
                 Volume< RGBComponent >* volRgb = 0 );
    
    const std::string& getOutputType() const;

    Volume< int16_t >* getOutputVolumeInt16();
    Volume< float >* getOutputVolumeFloat();
    Volume< RGBComponent >* getOutputVolumeRGB();

    bool isRgb();
    void setRgb( bool status );
    
  private:
  
    template < class T >
    static void read( Process& process, 
                      const std::string& fileName,
                      const gkg::AnalyzedObject&,
                      const std::string& );
    static void readRGB( Process& process, 
                         const std::string& fileName,
                         const gkg::AnalyzedObject&,
                         const std::string& );

    std::string _outputType;
    Volume< float >* _volumeFloat;
    Volume< RGBComponent >* _volumeRGB;
    Volume< int16_t >* _volumeInt16;
    
    bool _isRGB;

};


}


#endif
