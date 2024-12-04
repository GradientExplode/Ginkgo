#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_VoiStatistic_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_VoiStatistic_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>

#include <map>


namespace gkg
{


class VoiStatistic
{

  public:

    struct Stat
    {

      Stat();

      double mean;
      double sigma;
      double volume;
      double surface;

    };

    VoiStatistic();
    virtual ~VoiStatistic();

    void compute( VoiData& );
    void save( const std::string& );

    bool empty();

    Stat statistic( int32_t, int32_t );

  private:

    std::map< int32_t, std::map< int32_t, std::pair< std::string, Stat > > > 
                                                                    _statistics;

};


}


#endif
