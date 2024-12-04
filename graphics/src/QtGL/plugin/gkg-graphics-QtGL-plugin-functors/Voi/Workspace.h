#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_Workspace_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_Workspace_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-cppext/StdInt.h>

#include <QRect>


namespace gkg
{


class Workspace : public Singleton< Workspace >
{

  public:

    const QRect getAvailableGeometry();

    const QRect getPanelGeometry();
    const QRect getViewerGeometry();
    const QRect getHalfViewerGeometry();

    const QSize getIconSize();

  protected:

    friend class Singleton< Workspace >;

    Workspace();

  private:

    void initialize();

    QRect _availableGeometry;

    QRect _panelGeometry;
    QRect _viewerGeometry;
    QRect _halfViewerGeometry;

    QSize _iconSize;

};


}


#endif
