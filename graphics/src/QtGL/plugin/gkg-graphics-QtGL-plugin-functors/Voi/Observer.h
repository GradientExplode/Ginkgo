#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_Observer_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_Observer_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/ModeStrategy.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-cppext/StdInt.h>

#include <set>
#include <list>


namespace gkg
{


class VoiObservable;


class VoiObserver
{

  public:

    VoiObserver() { }
    virtual ~VoiObserver();

    void registerData( VoiObservable * );
    void unregisterData( VoiObservable * );

    virtual void updateDataLoaded(); 
    virtual void updateLabelLoaded(); 

    virtual void updateData(); 
    virtual void updatePalette(); 
    virtual void updateLabel( std::list< Vector3d< int32_t > >& );

    virtual void updateScrollBar();

    virtual void updateZoom();

    virtual void updateSlice( Vector3d< int32_t >&, int32_t );
    virtual void updateFrame( int32_t );

    virtual void redrawView();

    virtual void updateMode( VoiModeStrategy * );

  protected:

    std::set< VoiObservable* > datas;

};


}


#endif
