#include <gkg-graphics-QtGL-plugin-functors/Voi/ProfileStrategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/StrategyFactory.h>
#include <gkg-processing-container/Volume_i.h>

#include <QGridLayout>

#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_marker.h>


gkg::ProfileStrategy::ProfileStrategy()
                    : gkg::VoiStrategy()
{
}


void gkg::ProfileStrategy::paint( QPainter* painter, const QRectF& /* rect */ )
{

  if ( _samples[ 0 ].size() || _samples[ 1 ].size() || 
       _samples[ 2 ].size() || _samples[ 3 ].size() )
  {

    QWidget* hiddenWidget = new QWidget;
    QwtPlot* graphic[ 4 ];
    QwtPlotMarker* marker[ 4 ];
    QwtPlotCurve* profile[ 4 ];

    hiddenWidget->setStyleSheet( "background-color: black; color: lightgray;" );
    hiddenWidget->setFixedSize( da->width(), da->height() );

    QGridLayout* layout = new QGridLayout;
    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->setSpacing( 0 );

    graphic[ 0 ] = new QwtPlot( QwtText( "X Profile" ) );
    graphic[ 1 ] = new QwtPlot( QwtText( "Y Profile" ) );
    graphic[ 2 ] = new QwtPlot( QwtText( "Z Profile" ) );
    graphic[ 3 ] = new QwtPlot( QwtText( "T Profile" ) );

    layout->addWidget( graphic[ 0 ], 0, 0 ); 
    layout->addWidget( graphic[ 1 ], 0, 1 ); 
    layout->addWidget( graphic[ 2 ], 1, 0 ); 
    layout->addWidget( graphic[ 3 ], 1, 1 );
    hiddenWidget->setLayout( layout );

    int32_t i;
    for ( i = 0; i < 4; i++ )
    {

      graphic[ i ]->setAutoReplot( true );
      graphic[ i ]->setAxisFont( QwtPlot::yLeft, QFont( "helvetica", 8 ) );
      graphic[ i ]->setAxisFont( QwtPlot::xBottom, QFont( "helvetica", 8 ) );
      profile[ i ] = new QwtPlotCurve();
      profile[ i ]->setPen( QPen( Qt::white ) );
      profile[ i ]->attach( graphic[ i ] );

      marker[ i ] = new QwtPlotMarker();
      marker[ i ]->setLineStyle( QwtPlotMarker::VLine );
      marker[ i ]->setLinePen( QPen( Qt::red, 0, Qt::SolidLine ) );
      marker[ i ]->setLabelAlignment( Qt::AlignRight | Qt::AlignTop );
      marker[ i ]->attach( graphic[ i ] );

    }

    marker[ 0 ]->setXValue( point.x );
    marker[ 1 ]->setXValue( point.y );
    marker[ 2 ]->setXValue( point.z );
    marker[ 3 ]->setXValue( t );

    for ( i = 0; i < 4; i++ )
    {

      profile[ i ]->setData( new QwtPointSeriesData( _samples[ i ] ) );
      marker[ i ]->setLabel( QwtText( _msg ) );

    }

    painter->beginNativePainting();
    glPushAttrib( GL_ALL_ATTRIB_BITS );
    glClear( GL_COLOR_BUFFER_BIT );
    glPopAttrib();
    painter->endNativePainting();
    hiddenWidget->render( painter );

  }

}


void gkg::ProfileStrategy::updateDataLoaded( gkg::VoiData& data )
{

  if ( !data.empty() )
  {

    gkg::Volume< float >& t1 = data.getBaseFusion().getT1();
    gkg::Vector3d< int32_t > p( t1.getSizeX() / 2,
                                t1.getSizeY() / 2,
                                t1.getSizeZ() / 2 );

    updateSlice( data, p, 0 );

  }

}


void gkg::ProfileStrategy::updateSlice( gkg::VoiData& data, 
                                        gkg::Vector3d< int32_t >& p, 
                                        int32_t tt )
{

  if ( !data.empty() )
  {

    gkg::Volume< float >& t1 = data.getBaseFusion().getT1();
    int32_t i, dX = t1.getSizeX();
    int32_t dY = t1.getSizeY();
    int32_t dZ = t1.getSizeZ();
    int32_t dT = t1.getSizeT();

    _samples[ 0 ].clear();
    _samples[ 1 ].clear();
    _samples[ 2 ].clear();
    _samples[ 3 ].clear();

    point = p;

    if ( tt >= 0 )
    {

      t = tt;

    }

    _msg.setNum( t1( point, t ) );

    for ( i = 0; i < dX; i++ )
    {
  
      _samples[ 0 ].push_back( QPointF( i, t1( i, point.y, point.z, t ) ) );

    }
    for ( i = 0; i < dY; i++ )
    {
  
      _samples[ 1 ].push_back( QPointF( i, t1( point.x, i, point.z, t ) ) );

    }
    for ( i = 0; i < dZ; i++ )
    {
  
      _samples[ 2 ].push_back( QPointF( i, t1( point.x, point.y, i, t ) ) );

    }
    for ( i = 0; i < dT; i++ )
    {
  
      _samples[ 3 ].push_back( QPointF( i,
                                        t1( point.x, point.y, point.z, i ) ) );

    }

  }

}


void gkg::ProfileStrategy::updateFrame( gkg::VoiData& data, int32_t frame )
{

  if ( !data.empty() )
  {

    updateSlice( data, point, frame );

  }

}


gkg::VoiStrategy::StgyType gkg::ProfileStrategy::type()
{

  return gkg::VoiStrategy::StgyProfile;

}


RegisterStrategy( ProfileStrategy );
