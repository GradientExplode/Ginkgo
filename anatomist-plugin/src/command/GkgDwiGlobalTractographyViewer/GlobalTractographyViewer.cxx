#include "GlobalTractographyViewer.h"
#include "GlobalTractographyEvent.h"
#include "GlobalTractographyService.h"
#include "AnatomistViewer.h"
#include "AGlobalTractographyObject.h"

#include <gkg-dmri-global-tractography/SpinGlassConnection.h>
#include <gkg-dmri-container/SpinGlassAndConnectionMap.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-anatomist-plugin-server/AnatomistServer.h>
#include <gkg-core-exception/Exception.h>

#include "GlobalTractographyCallbackIds.h"

#include <QFile>
#include <QUiLoader>
#include <QLayout>
#include <QDoubleSpinBox>
#include <QComboBox>


#define DEFAULT_SINGLE_VIEW_CLIPPING_DISTANCE 200.0
#define DEFAULT_MULTIPLE_VIEW_CLIPPING_DISTANCE 5.0


gkg::GlobalTractographyViewer::GlobalTractographyViewer(
                                                    QWidget* parent,
                                                    int32_t openGLRefreshPeriod,
                                                    bool verbose )
                              : QWidget( parent ),
                                _openGLRefreshPeriod( openGLRefreshPeriod ),
                                _verbose( verbose )
{

  try
  {

    std::string uiPath = gkg::ConfigurationInfo::getInstance().getUiPath(
                                            "GkgDwiGlobalTractographyViewer",
                                            "DwiGlobalTractographyViewer.ui"  );
    QFile file( uiPath.c_str() );
    file.open( QFile::ReadOnly );
    QUiLoader qUiLoader;
    QWidget* qRootWidget = qUiLoader.load( &file );
    file.close();

    _doubleSpinBoxClippingDistance = qRootWidget->findChild< QDoubleSpinBox* >( 
                                             "doubleSpinBox_ClippingDistance" );
    _doubleSpinBoxClippingDistance->setRange( 0.1, 300.0 );
    _doubleSpinBoxClippingDistance->setSingleStep( 0.5 );
    _doubleSpinBoxClippingDistance->setValue(
                                        DEFAULT_SINGLE_VIEW_CLIPPING_DISTANCE );

    _comboBoxViewType = qRootWidget->findChild< QComboBox* >( 
                                                          "comboBox_ViewType" );
    _comboBoxViewType->setEnabled( true );
    _comboBoxViewType->addItem( "Axial" );
    _comboBoxViewType->addItem( "Sagittal" );
    _comboBoxViewType->addItem( "Coronal" );
    _comboBoxViewType->addItem( "3D" );
    _viewer = qRootWidget->findChild< QWidget* >( "widget_Viewer" );

    // creating the anatomist viewer
    _anatomistViewer = new gkg::AnatomistViewer( 800, 
                                                 gkg::AnatomistViewer::Axial,
                                                 "Spin glass and connections",
                                                 _viewer );

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget( qRootWidget );

    this->setLayout( layout );

    //this->setMinimumWidth( 1280 );
    //this->setMinimumHeight( 1024 );

    _globalTractographyService.reset(
                                   new gkg::GlobalTractographyService( this ) );

    _aGlobalTractographyObject = new gkg::AGlobalTractographyObject( 
                   _globalTractographyService->getSpinGlassAndConnectionMap() );

    _anatomistViewer->addGlobalTractographyObject( _aGlobalTractographyObject );

    gkg::AnatomistServer::getInstance().activateDoubleClippingMode(
                                                 "Spin glass and connections" );
    gkg::AnatomistServer::getInstance().setClippingDistance(
                                        "Spin glass and connections",
                                        DEFAULT_SINGLE_VIEW_CLIPPING_DISTANCE );

    QObject::connect( _doubleSpinBoxClippingDistance,
                      SIGNAL( valueChanged( double ) ),
                      this,
                      SLOT( setClippingDistance( double ) ) );

    QObject::connect( _comboBoxViewType,
                      SIGNAL( currentIndexChanged( int ) ),
                      this,
                      SLOT( setViewType( int ) ) );

  }
  GKG_CATCH( "gkg::GlobalTractographyViewer::GlobalTractographyViewer( "
             "QWidget* parent )" );

}


gkg::GlobalTractographyViewer::~GlobalTractographyViewer()
{
}


int32_t gkg::GlobalTractographyViewer::getOpenGLRefreshPeriod() const
{

  try
  {

    return _openGLRefreshPeriod;

  }
  GKG_CATCH( "int32_t gkg::GlobalTractographyViewer::"
             "getOpenGLRefreshPeriod() const" );


}


bool gkg::GlobalTractographyViewer::isVerbose() const
{

  try
  {

    return _verbose;

  }
  GKG_CATCH( "bool gkg::GlobalTractographyViewer::isVerbose() const" );

}


void gkg::GlobalTractographyViewer::customEvent( QEvent* event )
{

  try
  {

    int32_t eventType = ( int32_t )event->type() - ( int32_t )QEvent::User;

    if ( eventType == GT_OPENGL_OFFSET )
    {

//      static bool alreadyPassed = false;

//      if ( !alreadyPassed )
//      {

        gkg::AnatomistServer::getInstance().focusView(
                                                 "Spin glass and connections" );
//        alreadyPassed = true;

//      }
      gkg::AnatomistServer::getInstance().updateViews();

    }
    else
    {

      throw std::runtime_error( "no event available except the OpenGL" );

    }

  }
  GKG_CATCH( "void gkg::GlobalTractographyViewer::customEvent( "
             "QEvent* event )" );

}


void gkg::GlobalTractographyViewer::setClippingDistance( double value )
{

  try
  {


//    static bool alreadyPassed = false;

    gkg::AnatomistServer::getInstance().setClippingDistance(
                                                   "Spin glass and connections",
                                                   value );

//    if ( !alreadyPassed )
//    {

      gkg::AnatomistServer::getInstance().focusView(
                                                 "Spin glass and connections" );
//      alreadyPassed = true;

//    }

    // updating the views
    gkg::AnatomistServer::getInstance().updateViews();

  }
  GKG_CATCH( "void gkg::GlobalTractographyViewer::setClippingDistance( "
             "double value )" );

}


void gkg::GlobalTractographyViewer::setViewType( int viewType )
{

  try
  {

    // mute to axial
    if ( viewType == 0 )
    {

      gkg::AnatomistServer::getInstance().muteAxial(
                                                 "Spin glass and connections" );

    }
    // mute to sagittal
    else if ( viewType == 1 )
    {

      gkg::AnatomistServer::getInstance().muteSagittal(
                                                 "Spin glass and connections" );

    }
    // mute to coronal
    else if ( viewType == 2 )
    {

      gkg::AnatomistServer::getInstance().muteCoronal(
                                                 "Spin glass and connections" );

    }
    // mute to 3D
    else if ( viewType == 3 )
    {

      gkg::AnatomistServer::getInstance().mute3D(
                                                 "Spin glass and connections" );

    }
    gkg::AnatomistServer::getInstance().updateViews();

  }
  GKG_CATCH( "void gkg::GlobalTractographyViewer::setViewType( "
             "int viewType )" );

}


#undef DEFAULT_MULTIPLE_VIEW_CLIPPING_DISTANCE
#undef DEFAULT_SINGLE_VIEW_CLIPPING_DISTANCE
