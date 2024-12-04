#include <gkg-graphics-QtGL-plugin-functors/Voi/ToolBar.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Workspace.h>

#include <QHBoxLayout>
#include <QToolButton>
#include <QIcon>
#include <QButtonGroup>


gkg::ToolBar::ToolBar( QWidget* parent )
            : QWidget( parent )
{

  QSize iconSize = gkg::Workspace::getInstance().getIconSize();
  QToolButton* view3D = new QToolButton;
  view3D->setIcon( QIcon( QPixmap( ":voi/icon/view3d.png" ) ) );
  view3D->setIconSize( iconSize );
  view3D->setToolTip( "3D rendering" );

  QToolButton* histogramB = new QToolButton;
  histogramB->setIcon( QIcon( QPixmap( ":voi/icon/histogram.png" ) ) );
  histogramB->setIconSize( iconSize );
  histogramB->setToolTip( "Histogram" );

  QToolButton* profileB = new QToolButton;
  profileB->setIcon( QIcon( QPixmap( ":voi/icon/profile.png" ) ) );
  profileB->setIconSize( iconSize );
  profileB->setToolTip( "Profiles" );

  QToolButton* focusB = new QToolButton;
  focusB->setIcon( QIcon( QPixmap( ":voi/icon/focus.png" ) ) );
  focusB->setIconSize( iconSize );
  focusB->setToolTip( "Centering the 3D scene" );

  QHBoxLayout* layout = new QHBoxLayout;
  layout->setSpacing( 0 );
  layout->setContentsMargins( 0, 0, 0, 0 );
  layout->addWidget( view3D );
  layout->addWidget( histogramB );
  layout->addWidget( profileB );
  layout->addWidget( focusB );
  layout->addStretch();

  QButtonGroup* buttonGroup = new QButtonGroup;
  buttonGroup->addButton( view3D, 0 );
  buttonGroup->addButton( histogramB, 1 );
  buttonGroup->addButton( profileB, 2 );
  buttonGroup->addButton( focusB, 3 );

  connect( buttonGroup, SIGNAL( buttonClicked( int ) ),
           this, SLOT( buttonClick( int ) ) );

  setLayout( layout );

}


void gkg::ToolBar::buttonClick( int id )
{

  emit changeView( id );

}
