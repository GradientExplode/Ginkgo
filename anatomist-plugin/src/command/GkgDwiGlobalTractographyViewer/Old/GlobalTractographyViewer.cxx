#include "GlobalTractographyViewer.h"
#include "GlobalTractographyEvent.h"
#include "GlobalTractographyParameterIds.h"
#include "GlobalTractographyService.h"
#include "AnatomistViewer.h"
#include "GlobalTractographyOptimizationInformation.h"
#include "AGlobalTractographyObject.h"

#include <gkg-dmri-global-tractography/SpinGlassConnection.h>
#include <gkg-dmri-container/SpinGlassAndConnectionMap.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-anatomist-plugin-server/AnatomistServer.h>
#include <gkg-core-exception/Exception.h>

#include "GlobalTractographyCallbackIds.h"

#include <QtCore/QFile>
#include <QtUiTools/QUiLoader>
#include <QtGui/QLayout>
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QComboBox>
#include <Qt/qvector.h>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_series_data.h>


#define DEFAULT_SINGLE_VIEW_CLIPPING_DISTANCE 200.0
#define DEFAULT_MULTIPLE_VIEW_CLIPPING_DISTANCE 5.0


static char
parameterInformation[ GLOBAL_TRACTOGRAPHY_PARAMETER_COUNT ][ 2 ][ 100 ] =
{

  { "iteration", "int32_t" },
  { "temperature", "float" },
  { "operationType", "int32_t" },
  { "spinGlassCount", "int32_t" },
  { "fiberPortionCount", "int32_t" },
  { "creationCount", "int32_t" },
  { "deletionCount", "int32_t" },
  { "randomMoveCount", "int32_t" },
  { "optimalMoveCount", "int32_t" },
  { "connectionCount", "int32_t" },
  { "nothingToDoCount", "int32_t" },
  { "newExternalEnergy", "float" },
  { "newInternalEnergy", "float" },
  { "newGlobalEnergy", "float" },
  { "deltaGlobalEnergy", "float" },
  { "greenRatio", "float" },
  { "acceptModification", "bool" },
  { "chosenVoxel", "vector_of_int32_t" },
  { "chosenSpinGlassPosition", "vector_of_float" },
  { "chosenSpinGlassOrientation", "vector_of_float" },
  { "newVoxel", "vector_of_int32_t" },
  { "newSpinGlassPosition", "vector_of_float" },
  { "newSpinGlassOrientation", "vector_of_float" }

};

gkg::GlobalTractographyViewer::GlobalTractographyViewer( QWidget* parent,
                                                         bool singleView,
                                                         bool verbose )
                              : QWidget( parent ),
                                _singleView( singleView ),
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

    _parameters = qRootWidget->findChild< QTableWidget* >(
                                                     "tableWidget_Parameters" );
    _qwtPlot = qRootWidget->findChild< QwtPlot* >( "qwtPlot_Plotter" );
    _doubleSpinBoxClippingDistance = qRootWidget->findChild< QDoubleSpinBox* >( 
                                             "doubleSpinBox_ClippingDistance" );
    _doubleSpinBoxClippingDistance->setRange( 0.1, 300.0 );
    _doubleSpinBoxClippingDistance->setSingleStep( 0.5 );
    if ( _singleView )
    {

      _doubleSpinBoxClippingDistance->setValue(
                                        DEFAULT_SINGLE_VIEW_CLIPPING_DISTANCE );

    }
    else
    {

      _doubleSpinBoxClippingDistance->setValue(
                                      DEFAULT_MULTIPLE_VIEW_CLIPPING_DISTANCE );

    }
    _comboBoxViewType = qRootWidget->findChild< QComboBox* >( 
                                                          "comboBox_ViewType" );
    _comboBoxViewType->setEnabled( _singleView );
    _comboBoxViewType->addItem( "Axial" );
    _comboBoxViewType->addItem( "Sagittal" );
    _comboBoxViewType->addItem( "Coronal" );
    _comboBoxViewType->addItem( "3D" );
    _viewer = qRootWidget->findChild< QWidget* >( "widget_Viewer" );

    // adding parameters to the table
    int32_t p = 0;
    for ( p = 0; p < GLOBAL_TRACTOGRAPHY_PARAMETER_COUNT; p++ )
    {

      _parameters->setItem( p,
                            0,
                            new QTableWidgetItem(
                                  QString( parameterInformation[ p ][ 0 ] ) ) );

      if ( std::string( parameterInformation[ p ][ 1 ] ) == "int32_t" )
      {

        _parameterValueWidgets[ p ] = new QTableWidgetItem( QString( "0" ) );

      }
      else if ( std::string( parameterInformation[ p ][ 1 ] ) == "float" )
      {

        _parameterValueWidgets[ p ] = new QTableWidgetItem( QString( "0.0" ) );

      }
      else if ( std::string( parameterInformation[ p ][ 1 ] ) == "bool" )
      {

        _parameterValueWidgets[ p ] =
                                     new QTableWidgetItem( QString( "false" ) );

      }
      else if ( std::string( parameterInformation[ p ][ 1 ] ) ==
                "vector_of_int32_t" )
      {

        _parameterValueWidgets[ p ] =
                               new QTableWidgetItem( QString( "( 0, 0, 0 )" ) );

      }
      else if ( std::string( parameterInformation[ p ][ 1 ] ) ==
                "vector_of_float" )
      {

        _parameterValueWidgets[ p ] =
                         new QTableWidgetItem( QString( "( 0.0, 0.0, 0.0 )" ) );

      }

      _parameters->setItem( p, 1, _parameterValueWidgets[ p ] );

    }

    // adding curves to the plot viewer
    int32_t c = 0;
    for ( c = 0; c < GLOBAL_TRACTOGRAPHY_PARAMETER_COUNT; c++ )
    {

      if ( c == GT_PARAMETER_ITERATION )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve( "Iteration" );

      }
      else if ( c == GT_PARAMETER_TEMPERATURE )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve( "Temperature" );

      }
      else if ( c == GT_PARAMETER_OPERATION_TYPE )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve( "Operation type" );

      }
      else if ( c == GT_PARAMETER_SPIN_GLASS_COUNT )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve(
                                                           "Spin glass count" );

      }
      else if ( c == GT_PARAMETER_FIBER_PORTION_COUNT )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve(
                                                        "Fiber portion count" );

      }
      else if ( c == GT_PARAMETER_CREATION_COUNT )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve( "Creation count" );

      }
      else if ( c == GT_PARAMETER_DELETION_COUNT )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve( "Deletion count" );

      }
      else if ( c == GT_PARAMETER_RANDOM_MOVE_COUNT )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve(
                                                          "Random move count" );

      }
      else if ( c == GT_PARAMETER_OPTIMAL_MOVE_COUNT )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve(
                                                         "Optimal move count" );

      }
      else if ( c == GT_PARAMETER_CONNECTION_COUNT )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve(
                                                           "Connection count" );

      }
      else if ( c == GT_PARAMETER_NOTHING_TO_DO_COUNT )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve(
                                                        "Nothing to do count" );

      }
      else if ( c == GT_PARAMETER_NEW_EXTERNAL_ENERGY )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve(
                                                        "New external energy" );

      }
      else if ( c == GT_PARAMETER_NEW_INTERNAL_ENERGY )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve(
                                                        "New internal energy" );

      }
      else if ( c == GT_PARAMETER_NEW_GLOBAL_ENERGY )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve(
                                                          "New global energy" );

      }
      else if ( c == GT_PARAMETER_DELTA_GLOBAL_ENERGY )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve(
                                                        "Delta global energy" );

      }
      else if ( c == GT_PARAMETER_GREEN_RATIO )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve( "Green ration" );

      }
      else if ( c == GT_PARAMETER_ACCEPT_MODIFICATION )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve(
                                                        "Accept modification" );

      }
      else if ( c == GT_PARAMETER_CHOSEN_VOXEL )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve( "Chosen voxel" );

      }
      else if ( c == GT_PARAMETER_CHOSEN_SPIN_GLASS_POSITION )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve(
                                                 "Chosen spin glass position" );

      }
      else if ( c == GT_PARAMETER_CHOSEN_SPIN_GLASS_ORIENTATION )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve(
                                              "Chosen spin glass orientation" );

      }
      else if ( c == GT_PARAMETER_NEW_VOXEL )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve( "New voxel" );

      }
      else if ( c == GT_PARAMETER_NEW_SPIN_GLASS_POSITION )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve(
                                                    "New spin glass position" );

      }
      else if ( c == GT_PARAMETER_NEW_SPIN_GLASS_ORIENTATION )
      {

        _parameterPlotCurveWidgets[ c ] = new QwtPlotCurve(
                                                 "New spin glass orientation" );

      }

      _parameterPlotCurveWidgets[ c ]->attach( _qwtPlot );

    }

    // creating the anatomist viewer
    if ( _singleView )
    {

      _anatomistViewer = new gkg::AnatomistViewer( 800, _viewer );
      _anatomistViewer->createWindow( gkg::AnatomistViewer::Axial, 1, 0, 0,
                                      "Spin glass and connections" );

    }
    else
    {

      _anatomistViewer = new gkg::AnatomistViewer( 400, _viewer );
      _anatomistViewer->createWindow( gkg::AnatomistViewer::Coronal, 2, 0, 0,
                                      "Spin glass and connections - Coronal" );
      _anatomistViewer->createWindow( gkg::AnatomistViewer::Sagittal, 2, 0, 1,
                                      "Spin glass and connections - Sagittal" );
      _anatomistViewer->createWindow( gkg::AnatomistViewer::Axial, 2, 1, 0,
                                      "Spin glass and connections - Axial" );
      _anatomistViewer->createWindow( gkg::AnatomistViewer::ThreeD, 2, 1, 1,
                                      "Spin glass and connections - 3D" );

    }
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget( qRootWidget );

    this->setLayout( layout );

    //this->setMinimumWidth( 1280 );
    //this->setMinimumHeight( 1024 );

    _globalTractographyService.reset(
                                   new gkg::GlobalTractographyService( this ) );

    _aGlobalTractographyObject = new gkg::AGlobalTractographyObject( 
                     _globalTractographyService->getSpinGlassAndConnectionMap(),
                     _globalTractographyService->
                               getGlobalTractographyOptimizationInformation() );

    _anatomistViewer->addGlobalTractographyObject( _aGlobalTractographyObject );


    if ( _singleView )
    {

      gkg::AnatomistServer::getInstance().activateDoubleClippingMode(
                                                 "Spin glass and connections" );
      gkg::AnatomistServer::getInstance().setClippingDistance(
                                        "Spin glass and connections",
                                        DEFAULT_SINGLE_VIEW_CLIPPING_DISTANCE );

    }
    else
    {

      gkg::AnatomistServer::getInstance().activateDoubleClippingMode(
                                      "Spin glass and connections - Coronal" );
      gkg::AnatomistServer::getInstance().setClippingDistance(
                                      "Spin glass and connections - Coronal",
                                      DEFAULT_MULTIPLE_VIEW_CLIPPING_DISTANCE );

      gkg::AnatomistServer::getInstance().activateDoubleClippingMode(
                                      "Spin glass and connections - Sagittal" );
      gkg::AnatomistServer::getInstance().setClippingDistance(
                                      "Spin glass and connections - Sagittal",
                                      DEFAULT_MULTIPLE_VIEW_CLIPPING_DISTANCE );

      gkg::AnatomistServer::getInstance().activateDoubleClippingMode(
                                      "Spin glass and connections - Axial" );
      gkg::AnatomistServer::getInstance().setClippingDistance(
                                      "Spin glass and connections - Axial",
                                      DEFAULT_MULTIPLE_VIEW_CLIPPING_DISTANCE );

    }

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


bool gkg::GlobalTractographyViewer::isVerbose() const
{

  try
  {

    return _verbose;

  }
  GKG_CATCH( "bool gkg::GlobalTractographyViewer::isVerbose() const" );

}


QString gkg::GlobalTractographyViewer::getParameterValueAsString(
                                                     int32_t parameterId ) const
{

  try
  {

    const gkg::RCPointer< gkg::GlobalTractographyOptimizationInformation >&
      globalTractographyOptimizationInformation =
        _globalTractographyService->
                                 getGlobalTractographyOptimizationInformation();

    std::ostringstream os;
    if ( std::string( parameterInformation[ parameterId ][ 1 ] ) == "int32_t" )
    {

      int32_t value = 0;
      if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
           "iteration" )
      {

        value = globalTractographyOptimizationInformation->getLastIteration();

      }
      else if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
                "operationType" )
      {

        int32_t operationTypeInteger =
              globalTractographyOptimizationInformation->getLastOperationType();

        // nothing to do
        if ( operationTypeInteger == NOTHING_TO_DO_LABEL )
        {

          value = 0;

        }
        // creation
        else if ( operationTypeInteger == SPIN_GLASS_CREATION_LABEL )
        {

          value = 1;

        }
        // deletion
        else if ( operationTypeInteger == SPIN_GLASS_DELETION_LABEL )
        {

          value = 2;

        }
        // random move
        else if ( operationTypeInteger == SPIN_GLASS_RANDOM_MOVE_LABEL )
        {

          value = 3;

        }
        // optimal move
        else if ( operationTypeInteger == SPIN_GLASS_OPTIMAL_MOVE_LABEL )
        {

          value = 4;

        }
        // connection
        else if ( operationTypeInteger == SPIN_GLASS_CONNECTION_LABEL )
        {

          value = 5;

        }

      }
      else if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
                "spinGlassCount" )
      {

        value =
             globalTractographyOptimizationInformation->getLastSpinGlassCount();

      }
      else if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
                "fiberPortionCount" )
      {

        value =
          globalTractographyOptimizationInformation->getLastFiberPortionCount();

      }
      else if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
                "creationCount" )
      {

        value =
              globalTractographyOptimizationInformation->getLastCreationCount();

      }
      else if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
                "deletionCount" )
      {

        value =
              globalTractographyOptimizationInformation->getLastDeletionCount();

      }
      else if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
                "randomMoveCount" )
      {

        value =
            globalTractographyOptimizationInformation->getLastRandomMoveCount();

      }
      else if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
                "optimalMoveCount" )
      {

        value =
           globalTractographyOptimizationInformation->getLastOptimalMoveCount();

      }
      else if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
                "connectionCount" )
      {

        value =
            globalTractographyOptimizationInformation->getLastConnectionCount();

      }
      else if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
                "nothingToDoCount" )
      {

        value =
           globalTractographyOptimizationInformation->getLastNothingToDoCount();

      }
      os << value;

    }
    else if ( std::string( parameterInformation[ parameterId ][ 1 ] ) ==
              "float" )
    {

      float value = 0.0f;
      if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
           "temperature" )
      {

        value = globalTractographyOptimizationInformation->getLastTemperature();

      }
      else if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
                "newExternalEnergy" )
      {

        value =
          globalTractographyOptimizationInformation->getLastNewExternalEnergy();

      }
      else if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
                "newInternalEnergy" )
      {

        value =
          globalTractographyOptimizationInformation->getLastNewInternalEnergy();

      }
      else if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
                "newGlobalEnergy" )
      {

        value =
            globalTractographyOptimizationInformation->getLastNewGlobalEnergy();

      }
      else if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
                "deltaGlobalEnergy" )
      {

        value =
          globalTractographyOptimizationInformation->getLastDeltaGlobalEnergy();

      }
      else if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
                "greenRatio" )
      {

        value = globalTractographyOptimizationInformation->getLastGreenRatio();

      }
      os << value;

    }
/*    else if ( std::string( parameterInformation[ parameterId ][ 1 ] ) ==
              "operation_t" )
    {

      gkg::GlobalTractographyOperationType
        operationType =
              globalTractographyOptimizationInformation->getLastOperationType();

      std::string operationTypeName = "";
      if ( operationType == gkg::Creation )
      {

        operationTypeName = "Creation";

      }
      else if ( operationType == gkg::Deletion )
      {

        operationTypeName = "Deletion";

      }
      else if ( operationType == gkg::RandomMove )
      {

        operationTypeName = "RandomMove";

      }
      else if ( operationType == gkg::OptimalMove )
      {

        operationTypeName = "OptimalMove";

      }
      else if ( operationType == gkg::Connection )
      {

        operationTypeName = "Connection";

      }
      else if ( operationType == gkg::NothingToDo )
      {

        operationTypeName = "NothingToDo";

      }
      os << operationTypeName;

    }*/
    else if ( std::string( parameterInformation[ parameterId ][ 1 ] ) ==
              "bool" )
    {

      bool value = false;
      if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
           "acceptModification" )
      {

        value =
         globalTractographyOptimizationInformation->getLastAcceptModification();

      }
      os << ( value ? "true" : "false" );

    }
    else if ( std::string( parameterInformation[ parameterId ][ 1 ] ) ==
              "vector_of_int32_t" )
    {

      gkg::Vector3d< int32_t > value( 0, 0, 0 );
      if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
           "chosenVoxel" )
      {

        value = globalTractographyOptimizationInformation->getLastChosenVoxel();

      }
      else if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
                "newVoxel" )
      {

        value = globalTractographyOptimizationInformation->getLastNewVoxel();

      }
      os << value;

    }
    else if ( std::string( parameterInformation[ parameterId ][ 1 ] ) ==
              "vector_of_float" )
    {

      gkg::Vector3d< float > value( 0.0, 0.0, 0.0 );
      if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
           "chosenSpinGlassPosition" )
      {

        value = globalTractographyOptimizationInformation->
                                               getLastChosenSpinGlassPosition();

      }
      else if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
                "chosenSpinGlassOrientation" )
      {

        value = globalTractographyOptimizationInformation->
                                            getLastChosenSpinGlassOrientation();

      }
      else if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
                "newSpinGlassPosition" )
      {

        value = globalTractographyOptimizationInformation->
                                                  getLastNewSpinGlassPosition();

      }
      else if ( std::string( parameterInformation[ parameterId ][ 0 ] ) ==
                "newSpinGlassOrientation" )
      {

        value = globalTractographyOptimizationInformation->
                                               getLastNewSpinGlassOrientation();

      }
      os << value;

    }
    return QString( os.str().c_str() );

  }
  GKG_CATCH( "QString gkg::GlobalTractographyViewer::"
             "getParameterValueAsString( int32_t parameterId ) const" );

}


QVector< double > gkg::GlobalTractographyViewer::getQVectorInformation(
                                                          int32_t plotId ) const
{

  try
  {

    const gkg::RCPointer< gkg::GlobalTractographyOptimizationInformation >&
      globalTractographyOptimizationInformation =
        _globalTractographyService->
                                 getGlobalTractographyOptimizationInformation();

    QVector< double > parameterInformation;

    if ( plotId == GT_PARAMETER_TEMPERATURE )
    {

      const std::list< float >& temperatures =
                   globalTractographyOptimizationInformation->getTemperatures();

      std::list< float >::const_iterator
        t = temperatures.begin(),
        te = temperatures.end();
        
      while ( t != te )
      {

        parameterInformation.push_back( ( double )*t );
        ++t;

      }

    }
/*    else if ( plotId == GT_PARAMETER_OPERATION_TYPE )
    {

      const std::list< gkg::GlobalTractographyOperationType >& operationTypes =
                 globalTractographyOptimizationInformation->getOperationTypes();

      std::list< gkg::GlobalTractographyOperationType >::const_iterator
        o = operationTypes.begin(),
        oe = operationTypes.end();
        
      while ( o != oe )
      {

        if ( *o == gkg::Creation )
        {

          parameterInformation.push_back( 1.0 );

        }
        else if ( *o == gkg::Deletion )
        {

          parameterInformation.push_back( 2.0 );

        }
        else if ( *o == gkg::RandomMove )
        {

          parameterInformation.push_back( 3.0 );

        }
        else if ( *o == gkg::OptimalMove )
        {

          parameterInformation.push_back( 4.0 );

        }
        else if ( *o == gkg::Connection )
        {

          parameterInformation.push_back( 5.0 );

        }
        else
        {

          throw std::runtime_error( "invalid connection type" );

        }

        ++o;

      }

    }*/
    else if ( plotId == GT_PARAMETER_OPERATION_TYPE )
    {

      const std::list< int32_t >& operationTypes =
                 globalTractographyOptimizationInformation->getOperationTypes();

      std::list< int32_t >::const_iterator
        o = operationTypes.begin(),
        oe = operationTypes.end();
        
      while ( o != oe )
      {

        if ( *o == NOTHING_TO_DO_LABEL )
        {

          parameterInformation.push_back( 1.0 );

        }
        else if ( *o == SPIN_GLASS_CREATION_LABEL )
        {

          parameterInformation.push_back( 2.0 );

        }
        else if ( *o == SPIN_GLASS_DELETION_LABEL )
        {

          parameterInformation.push_back( 3.0 );

        }
        else if ( *o == SPIN_GLASS_RANDOM_MOVE_LABEL )
        {

          parameterInformation.push_back( 4.0 );

        }
        else if ( *o == SPIN_GLASS_OPTIMAL_MOVE_LABEL )
        {

          parameterInformation.push_back( 5.0 );

        }
        else if ( *o == SPIN_GLASS_CONNECTION_LABEL )
        {

          parameterInformation.push_back( 6.0 );

        }
        else
        {

          throw std::runtime_error( "invalid connection type" );

        }

        ++o;

      }

    }
    else if ( plotId == GT_PARAMETER_SPIN_GLASS_COUNT )
    {

      const std::list< int32_t >& spinGlassCounts =
                globalTractographyOptimizationInformation->getSpinGlassCounts();

      std::list< int32_t >::const_iterator
        s = spinGlassCounts.begin(),
        se = spinGlassCounts.end();

      while ( s != se )
      {

        parameterInformation.push_back( ( double )*s );
        ++s;

      }

    }
    else if ( plotId == GT_PARAMETER_FIBER_PORTION_COUNT )
    {

      const std::list< int32_t >& fiberPortionCounts =
             globalTractographyOptimizationInformation->getFiberPortionCounts();

      std::list< int32_t >::const_iterator
        f = fiberPortionCounts.begin(),
        fe = fiberPortionCounts.end();

      while ( f != fe )
      {

        parameterInformation.push_back( ( double )*f );
        ++f;

      }

    }
    else if ( plotId == GT_PARAMETER_CREATION_COUNT )
    {

      const std::list< int32_t >& creationCounts =
                 globalTractographyOptimizationInformation->getCreationCounts();

      std::list< int32_t >::const_iterator
        c = creationCounts.begin(),
        ce = creationCounts.end();

      while ( c != ce )
      {

        parameterInformation.push_back( ( double )*c );
        ++c;

      }

    }
    else if ( plotId == GT_PARAMETER_DELETION_COUNT )
    {

      const std::list< int32_t >& deletionCounts =
                 globalTractographyOptimizationInformation->getDeletionCounts();

      std::list< int32_t >::const_iterator
        d = deletionCounts.begin(),
        de = deletionCounts.end();

      while ( d != de )
      {

        parameterInformation.push_back( ( double )*d );
        ++d;

      }

    }
    else if ( plotId == GT_PARAMETER_RANDOM_MOVE_COUNT )
    {

      const std::list< int32_t >& randomMoveCounts =
               globalTractographyOptimizationInformation->getRandomMoveCounts();

      std::list< int32_t >::const_iterator
        r = randomMoveCounts.begin(),
        re = randomMoveCounts.end();

      while ( r != re )
      {

        parameterInformation.push_back( ( double )*r );
        ++r;

      }

    }
    else if ( plotId == GT_PARAMETER_OPTIMAL_MOVE_COUNT )
    {

      const std::list< int32_t >& optimalMoveCounts =
              globalTractographyOptimizationInformation->getOptimalMoveCounts();

      std::list< int32_t >::const_iterator
        o = optimalMoveCounts.begin(),
        oe = optimalMoveCounts.end();

      while ( o != oe )
      {

        parameterInformation.push_back( ( double )*o );
        ++o;

      }

    }
    else if ( plotId == GT_PARAMETER_CONNECTION_COUNT )
    {

      const std::list< int32_t >& connectionCounts =
               globalTractographyOptimizationInformation->getConnectionCounts();

      std::list< int32_t >::const_iterator
        c = connectionCounts.begin(),
        ce = connectionCounts.end();

      while ( c != ce )
      {

        parameterInformation.push_back( ( double )*c );
        ++c;

      }

    }
    else if ( plotId == GT_PARAMETER_NOTHING_TO_DO_COUNT )
    {

      const std::list< int32_t >& nothingToDoCounts =
              globalTractographyOptimizationInformation->getNothingToDoCounts();

      std::list< int32_t >::const_iterator
        n = nothingToDoCounts.begin(),
        ne = nothingToDoCounts.end();

      while ( n != ne )
      {

        parameterInformation.push_back( ( double )*n );
        ++n;

      }

    }
    else if ( plotId == GT_PARAMETER_NEW_EXTERNAL_ENERGY )
    {

      const std::list< float >& newExternalEnergies =
            globalTractographyOptimizationInformation->getNewExternalEnergies();

      std::list< float >::const_iterator
        n = newExternalEnergies.begin(),
        ne = newExternalEnergies.end();

      while ( n != ne )
      {

        parameterInformation.push_back( ( double )*n );
        ++n;

      }

    }
    else if ( plotId == GT_PARAMETER_NEW_INTERNAL_ENERGY )
    {

      const std::list< float >& newInternalEnergies =
            globalTractographyOptimizationInformation->getNewInternalEnergies();

      std::list< float >::const_iterator
        n = newInternalEnergies.begin(),
        ne = newInternalEnergies.end();

      while ( n != ne )
      {

        parameterInformation.push_back( ( double )*n );
        ++n;

      }

    }
    else if ( plotId == GT_PARAMETER_NEW_GLOBAL_ENERGY )
    {

      const std::list< float >& newGlobalEnergies =
              globalTractographyOptimizationInformation->getNewGlobalEnergies();

      std::list< float >::const_iterator
        n = newGlobalEnergies.begin(),
        ne = newGlobalEnergies.end();

      while ( n != ne )
      {

        parameterInformation.push_back( ( double )*n );
        ++n;

      }

    }
    else if ( plotId == GT_PARAMETER_DELTA_GLOBAL_ENERGY )
    {

      const std::list< float >& deltaGlobalEnergies =
            globalTractographyOptimizationInformation->getDeltaGlobalEnergies();

      std::list< float >::const_iterator
        d = deltaGlobalEnergies.begin(),
        de = deltaGlobalEnergies.end();

      while ( d != de )
      {

        parameterInformation.push_back( ( double )*d );
        ++d;

      }

    }
    else if ( plotId == GT_PARAMETER_GREEN_RATIO )
    {

      const std::list< float >& greenRatios =
                    globalTractographyOptimizationInformation->getGreenRatios();

      std::list< float >::const_iterator
        g = greenRatios.begin(),
        ge = greenRatios.end();

      while ( g != ge )
      {

        parameterInformation.push_back( ( double )*g );
        ++g;

      }

    }
    else if ( plotId == GT_PARAMETER_ACCEPT_MODIFICATION )
    {

      const std::list< bool >& acceptModifications =
            globalTractographyOptimizationInformation->getAcceptModifications();

      std::list< bool >::const_iterator
        a = acceptModifications.begin(),
        ae = acceptModifications.end();

      while ( a != ae )
      {

        if ( *a == true )
        {

          parameterInformation.push_back( 1.0 );

        }
        else if ( *a == false )
        {

          parameterInformation.push_back( 0.0 );

        }
        else
        {

          throw std::runtime_error( "invalid value of acceptModification" );

        }
        ++a;

      }

    }

    return parameterInformation;

  }
  GKG_CATCH( "QVector< double > "
             "gkg::GlobalTractographyViewer::getQVectorInformation( "
             "int32_t plotId ) const" )

}


void gkg::GlobalTractographyViewer::customEvent( QEvent* event )
{

  try
  {

    int32_t eventType = ( int32_t )event->type() - ( int32_t )QEvent::User;

    // we are in the case of a parameter update event
    if ( ( eventType >= GT_PARAMETER_OFFSET ) &&
         ( eventType < GT_PLOT_OFFSET ) )
    {

      int32_t parameterId = eventType - GT_PARAMETER_OFFSET;
      _parameterValueWidgets[ parameterId ]->setText( 
                                     getParameterValueAsString( parameterId ) );

    }
    // we are in the case of a plot update event
    else if ( ( eventType >= GT_PLOT_OFFSET ) &&
              ( eventType < GT_OPENGL_OFFSET ) )
    {

      int32_t plotId = eventType - GT_PLOT_OFFSET;

      const gkg::RCPointer< gkg::GlobalTractographyOptimizationInformation >&
        globalTractographyOptimizationInformation =
          _globalTractographyService->
                                 getGlobalTractographyOptimizationInformation();

      QVector< double > QVectorOfIterations;
      const std::list< int32_t >& iterations =
                     globalTractographyOptimizationInformation->getIterations();

      std::list< int32_t >::const_iterator
        i = iterations.begin(),
        ie = iterations.end();
        
      while ( i != ie )
      {

        QVectorOfIterations.push_back( ( double )*i );
        ++i;

      }

      if ( plotId == GT_PARAMETER_ITERATION )
      {

        _parameterPlotCurveWidgets[ plotId ]->setSamples( QVectorOfIterations,
                                                          QVectorOfIterations );

      }
      else
      {

        QVector< double > parameterInformation;
        parameterInformation = this->getQVectorInformation( plotId );

        if ( plotId == GT_PARAMETER_OPERATION_TYPE ||
             plotId == GT_PARAMETER_ACCEPT_MODIFICATION )
        {

          _parameterPlotCurveWidgets[ plotId ]->setSamples(
                                                         QVectorOfIterations,
                                                         parameterInformation );

          _parameterPlotCurveWidgets[ plotId ]->setStyle(
                                                         QwtPlotCurve::Sticks );

        }
        else if ( plotId == GT_PARAMETER_CREATION_COUNT ||
                  plotId == GT_PARAMETER_DELETION_COUNT ||
                  plotId == GT_PARAMETER_RANDOM_MOVE_COUNT ||
                  plotId == GT_PARAMETER_OPTIMAL_MOVE_COUNT ||
                  plotId == GT_PARAMETER_CONNECTION_COUNT ||
                  plotId == GT_PARAMETER_NOTHING_TO_DO_COUNT )
        {

          _parameterPlotCurveWidgets[ plotId ]->setSamples(
                                                         QVectorOfIterations,
                                                         parameterInformation );

          _parameterPlotCurveWidgets[ plotId ]->setStyle( QwtPlotCurve::Steps );

        }
        else
        {

          _parameterPlotCurveWidgets[ plotId ]->setSamples(
                                                         QVectorOfIterations,
                                                         parameterInformation );

        }

      }

      QObject::connect( _parameters, SIGNAL( cellDoubleClicked( int, int ) ),
                        this, SLOT( drawCurve( int, int ) ) );

    }
    // we are in the case of an OpenGL update event
    else if ( eventType == GT_OPENGL_OFFSET )
    {

      if ( _singleView )
      {

        gkg::AnatomistServer::getInstance().focusView(
                                                 "Spin glass and connections" );
        gkg::AnatomistServer::getInstance().updateViews();

      }
      else
      {

        gkg::AnatomistServer::getInstance().focusView(
                                      "Spin glass and connections - Coronal" );
        gkg::AnatomistServer::getInstance().focusView(
                                      "Spin glass and connections - Sagittal" );
        gkg::AnatomistServer::getInstance().focusView(
                                      "Spin glass and connections - Axial" );
        gkg::AnatomistServer::getInstance().focusView(
                                      "Spin glass and connections - 3D" );
        gkg::AnatomistServer::getInstance().updateViews();

      }

    }

  }
  GKG_CATCH( "void gkg::GlobalTractographyViewer::customEvent( "
             "QEvent* event )" );

}


void gkg::GlobalTractographyViewer::drawCurve( int row, int column )
{

  try
  {

    int32_t i = 0;
    for ( i = 0; i < GLOBAL_TRACTOGRAPHY_PARAMETER_COUNT; i++ )
    {

      if ( i == row )
      {

        _parameterPlotCurveWidgets[ i ]->show();

      }
      else
      {

        _parameterPlotCurveWidgets[ i ]->hide();

      }

    }

    _qwtPlot->replot();

  }
  GKG_CATCH( "void gkg::GlobalTractographyViewer::drawCurve( "
             "int row, int column )" );

}


void gkg::GlobalTractographyViewer::setClippingDistance( double value )
{

  try
  {

    if ( _singleView )
    {

      gkg::AnatomistServer::getInstance().setClippingDistance(
                                        "Spin glass and connections",
                                        value );
      gkg::AnatomistServer::getInstance().focusView(
                                        "Spin glass and connections" );

    }
    else
    {

      gkg::AnatomistServer::getInstance().setClippingDistance(
                                        "Spin glass and connections - Axial",
                                        value );
      gkg::AnatomistServer::getInstance().setClippingDistance(
                                        "Spin glass and connections - Sagittal",
                                        value );
      gkg::AnatomistServer::getInstance().setClippingDistance(
                                        "Spin glass and connections - Coronal",
                                        value );

      gkg::AnatomistServer::getInstance().focusView(
                                      "Spin glass and connections - Axial" );
      gkg::AnatomistServer::getInstance().focusView(
                                      "Spin glass and connections - Sagittal" );
      gkg::AnatomistServer::getInstance().focusView(
                                      "Spin glass and connections - Coronal" );

    }

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

    if ( _singleView )
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

  }
  GKG_CATCH( "void gkg::GlobalTractographyViewer::setViewType( "
             "int32_t viewType )" );

}


#undef DEFAULT_MULTIPLE_VIEW_CLIPPING_DISTANCE
#undef DEFAULT_SINGLE_VIEW_CLIPPING_DISTANCE
