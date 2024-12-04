#ifndef _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_GlobalTractographyEvent_h_
#define _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_GlobalTractographyEvent_h_


#include <QtCore/QEvent>


#define GT_OPENGL_OFFSET  2001


namespace gkg
{


class GlobalTractographyEvent : public QEvent
{

  public:

    GlobalTractographyEvent( int32_t id );

};


}



#endif

