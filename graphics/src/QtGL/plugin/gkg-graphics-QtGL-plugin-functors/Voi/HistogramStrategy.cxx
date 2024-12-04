#include <gkg-graphics-QtGL-plugin-functors/Voi/HistogramStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/StrategyFactory.h>
#include <gkg-processing-histogram/HistogramAnalyzer.h>

#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_histogram.h>
#include <qwt/qwt_scale_widget.h>

#include <QPalette>

#include <cmath>


gkg::HistogramStrategy::HistogramStrategy()
                      : gkg::VoiStrategy()
{
}


void gkg::HistogramStrategy::paint( QPainter* painter, 
                                    const QRectF& /* rect */ )
{

  if ( _histogram.size() )
  {

    QwtPlot* qwtPlot = new QwtPlot( QwtText( "Histogram" ) );
    QwtPlotHistogram* qwtHisto = new QwtPlotHistogram();
    QwtPlotHistogram* qwtHistoLog = new QwtPlotHistogram();
    QVector< QwtIntervalSample > samples;
    QVector< QwtIntervalSample > samplesLog;

    qwtPlot->setStyleSheet( "background-color: black;" );
    qwtPlot->setFixedSize( da->width(), da->height() );
    qwtPlot->enableAxis( QwtPlot::yRight );
    qwtPlot->axisWidget( QwtPlot::yRight )->setStyleSheet( 
                                                        "color: darkMagenta;" );
    qwtPlot->setAutoReplot( true );
    qwtHisto->setZ( 0 );
    qwtHisto->setStyle( QwtPlotHistogram::Columns );
    qwtHisto->setPen( QPen( Qt::gray ) );
    qwtHisto->setBrush( QBrush( Qt::gray ) );
    qwtHisto->setYAxis( QwtPlot::yLeft );
    qwtHistoLog->setZ( 0 );
    qwtHistoLog->setStyle( QwtPlotHistogram::Columns );
    qwtHistoLog->setPen( QPen( Qt::darkMagenta ) );
    qwtHistoLog->setBrush( QBrush( Qt::darkMagenta ) );
    qwtHistoLog->setYAxis( QwtPlot::yRight );

    int32_t i = 0;
    std::vector< double >::const_iterator
      h = _histogram.begin(),
      he = _histogram.end();

    while ( h != he )
    {

      samples.push_back( QwtIntervalSample( double( *h ), i, i + 1 ) );
      samplesLog.push_back( QwtIntervalSample( *h ? std::log( *h ) : 0.0, 
                                               i, i + 1 ) );
      i++;
      ++h;

    }


    qwtHistoLog->setData( new QwtIntervalSeriesData( samplesLog ) );
    qwtHisto->setData( new QwtIntervalSeriesData( samples ) );
    qwtHistoLog->attach( qwtPlot );
    qwtHisto->attach( qwtPlot );

    int32_t posX = da->width() * 4 / 5;

    painter->beginNativePainting();
    glPushAttrib( GL_ALL_ATTRIB_BITS );
    glClear( GL_COLOR_BUFFER_BIT );
    glPopAttrib();
    painter->endNativePainting();
    qwtPlot->render( painter );
    painter->setPen( Qt::gray );
    painter->setFont( QFont( "helvetica", 12 ) );
    painter->drawText( posX, 70, "histogram" );
    painter->setPen( Qt::darkMagenta );
    painter->drawText( posX, 100, "log(histogram)" );

  }

}


void gkg::HistogramStrategy::updateDataLoaded( gkg::VoiData& data )
{
  
  if ( !data.empty() )
  {

    float volMin = data.min();
    float volMax = data.max();
    int32_t range = int32_t( volMax - volMin );

    if ( ( range > 512 ) || ( range < 16 ) )
    {

      range = 512;

    }

    _histogram.clear();
    _histogram.resize( range, 0.0 );

    gkg::HistogramAnalyzer::getInstance().getHistogram( 
                                                   data.getBaseFusion().getT1(),
                                                   _histogram,
                                                   volMin,
                                                   volMax,
                                                   -1, false );
 
  }

}


gkg::VoiStrategy::StgyType gkg::HistogramStrategy::type()
{

  return gkg::VoiStrategy::StgyHistogram;

}


RegisterStrategy( HistogramStrategy );
