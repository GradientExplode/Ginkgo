#ifndef _gkg_graphics_QtGL_plugin_functors_Abacus_AbacusMainWindow_h_
#define _gkg_graphics_QtGL_plugin_functors_Abacus_AbacusMainWindow_h_


#include <gkg-graphics-QtGL-qtext/Qt.h>
#include <gkg-graphics-QtGL-qtext/Qwt.h>
#include <gkg-processing-container/Volume.h>

#include <QMainWindow>
#include <QScrollBar>
#include <QTableWidget>

#include <string>


namespace gkg
{


class AbacusMainWindow : public QMainWindow
{

  Q_OBJECT

  public:

    enum DistributionType
    {

      Unknown = 0,
      Rice,
      NonCentralChi

    };

    AbacusMainWindow();

    void initializeArg( const std::string& );

  private slots:

    void loadVolume();
    void changeV( int value );
    void changeS( int value );

  private:
  
    void draw();

    QwtPlot* _plot;
    QwtPlotCurve* _curve;
    QwtPlotCurve* _fit;
    
    int32_t _n;
    QVector< QPointF > _samples;

    QLabel* _v;
    QLabel* _s;
    QScrollBar* _sliderV;
    QScrollBar* _sliderS;
    QTableWidget* _table;
    QLabel* _epsilon;
    
    int32_t _valueSlider;
    int32_t _sigmaSlider;
    double _value;
    double _sigma;
    double _vStep;
    double _sStep;
    double _vStart;
    double _sStart;
    double _channelCount;
    DistributionType _distribution;

    Volume< float > _data;
  
};


}


#endif
