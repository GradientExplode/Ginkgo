#include <gkg-graphics-QtGL-plugin-functors/Voi/Workspace.h>

#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QCursor>


gkg::Workspace::Workspace()
              : gkg::Singleton< gkg::Workspace >(),
                _iconSize( 32, 32 )
{

  initialize();

}


const QRect gkg::Workspace::getAvailableGeometry()
{

  return _availableGeometry;

}


const QRect gkg::Workspace::getPanelGeometry()
{

  return _panelGeometry;

}


const QRect gkg::Workspace::getViewerGeometry()
{

  return _viewerGeometry;

}


const QRect gkg::Workspace::getHalfViewerGeometry()
{

  return _halfViewerGeometry;

}


const QSize gkg::Workspace::getIconSize()
{

  return _iconSize;

}


void gkg::Workspace::initialize()
{

  QList< QScreen* > screens = QApplication::screens();
  QPoint p = QCursor::pos();

#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
  QScreen* pscreen = QApplication::screenAt( p );
#else
  int32_t screenNum = QApplication::desktop()->screenNumber( p );
  QScreen* pscreen = screens[ screenNum ];
#endif

  _availableGeometry = pscreen->availableGeometry();

  int32_t panelWidth = 512;
  int32_t width = pscreen->geometry().width() - panelWidth - 
                  pscreen->availableVirtualGeometry().left();
  int32_t height = _availableGeometry.height();
  int32_t dw = width % 2;

  width &= ~0x1;
  height &= ~0x1;
  panelWidth += dw;

  _panelGeometry.setRect( 0, 0,  panelWidth, height );
  _viewerGeometry.setRect( 0, 0, width, height );
  _halfViewerGeometry.setRect( 0, 0, width / 2, height / 2 );

}
