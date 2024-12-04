#ifndef _gkg_graphics_X11_plugin_functors_Viewer2d_Viewer2dProcess_h_
#define _gkg_graphics_X11_plugin_functors_Viewer2d_Viewer2dProcess_h_


#include <gkg-processing-process/Process.h>
#include <string>


namespace gkg
{

class Viewer2dProcess : public Process
{

  public:

    Viewer2dProcess( const float& scale,
                     const bool& verbose,
                     int& result );

    const float& getScale() const;
    const bool& getVerbose() const;
    int& getResult() const;

  private:

    template < class T >
    static void view( Process& process,
                      const std::string& fileNameIn,
                      const AnalyzedObject&,
                      const std::string& );

    const float& _scale;
    const bool& _verbose;
    int& _result;

};


}


#endif
