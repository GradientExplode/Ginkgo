#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_Observable_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_Observable_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/Observer.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-cppext/StdInt.h>

#include <set>
#include <list>


namespace gkg
{


class VoiObservable
{

  public:

    VoiObservable() {}
    virtual ~VoiObservable();

    void registerView( VoiObserver * );
    void unregisterView( VoiObserver * );

    void notifyDataLoaded();
    void notifyLabelLoaded();

    void notifyData();
    void notifyPalette();
    void notifyLabel( std::list< Vector3d< int32_t > >& );

    void notifyScrollBar();

    void notifyZoom();
  
    void notifySlice( Vector3d< int32_t >&, int32_t t = -1 );
    void notifyFrame( int32_t );

    void notifyRedraw();

    void notifyMode( VoiModeStrategy * );

  protected:

    std::set< VoiObserver * > views;

};


}


#endif
