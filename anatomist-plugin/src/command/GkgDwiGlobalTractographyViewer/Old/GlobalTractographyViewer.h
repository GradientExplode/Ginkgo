#ifndef _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_GlobalTractographyViewer_h_
#define _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_GlobalTractographyViewer_h_


#include <QtGui/QWidget>
#include <gkg-core-pattern/RCPointer.h>


#define GLOBAL_TRACTOGRAPHY_PARAMETER_COUNT 23


class QTableWidget;
class QTableWidgetItem;
class QwtPlot;
class QwtPlotCurve;
class QDoubleSpinBox;
class QComboBox;


namespace gkg
{


class AnatomistViewer;
class GlobalTractographyService;
class AGlobalTractographyObject;


class GlobalTractographyViewer : public QWidget
{

  Q_OBJECT
  
  public:
  
    typedef QTableWidgetItem* 
                   ParameterValueWidgets[ GLOBAL_TRACTOGRAPHY_PARAMETER_COUNT ];

    typedef QwtPlotCurve*
               ParameterPlotCurveWidgets[ GLOBAL_TRACTOGRAPHY_PARAMETER_COUNT ];

    GlobalTractographyViewer( QWidget* parent = 0,
                              bool singleView = false,
                              bool verbose = false );
    virtual ~GlobalTractographyViewer();

    bool isVerbose() const;

    QString getParameterValueAsString( int32_t parameterId ) const;
    QVector< double > getQVectorInformation( int32_t plotId ) const;

    void customEvent( QEvent* event );    

  public slots:

    void drawCurve( int row, int column );
    void setClippingDistance( double value );
    void setViewType( int viewType );

  protected:

    bool _singleView;
    bool _verbose;

    QTableWidget* _parameters;
    QwtPlot* _qwtPlot;
    QDoubleSpinBox* _doubleSpinBoxClippingDistance;
    QComboBox* _comboBoxViewType;
    QWidget* _viewer;
    AnatomistViewer* _anatomistViewer;

    ParameterValueWidgets _parameterValueWidgets;
    ParameterPlotCurveWidgets _parameterPlotCurveWidgets;

    RCPointer< GlobalTractographyService > _globalTractographyService;

    // this object cannot be encapsulated within an RCPointer because it
    // inheritates a anatomist::SliceableObject that is already inheriting from
    // a soma::rc_pointer causing an ambiguity
    AGlobalTractographyObject* _aGlobalTractographyObject;

};



}


#endif
