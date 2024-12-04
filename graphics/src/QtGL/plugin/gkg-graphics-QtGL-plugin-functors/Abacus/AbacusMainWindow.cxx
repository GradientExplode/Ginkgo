#include <gkg-graphics-QtGL-plugin-functors/Abacus/AbacusMainWindow.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-processing-numericalanalysis/RiceFunction.h>
#include <gkg-processing-numericalanalysis/NonCentralChiFunction.h>
#include <gkg-processing-numericalanalysis/GaussianMixtureFunction.h>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFileDialog>

#include <cassert>


gkg::AbacusMainWindow::AbacusMainWindow()
                     : QMainWindow(),
		       _n( 80 ),
                       _valueSlider( 0 ),
                       _sigmaSlider( 0 ),
		       _value( 0.0 ),
		       _sigma( 0.0 ),
                       _vStep( 1.0 ),
                       _sStep( 1.0 ),
                       _vStart( 0.0 ),
                       _sStart( 0.0 ),
                       _channelCount( 1.0 ),
                       _distribution( gkg::AbacusMainWindow::Unknown )
{

  setWindowTitle( "GkgAbacus" );
  setFont( QFont( "helvetica", 10 ) );
  
  // File menu
  QMenu* menuFile = menuBar()->addMenu( "&File" );
  menuFile->addAction( "&Open...", this,  SLOT( loadVolume() ),
  		       Qt::CTRL + Qt::Key_O );
  menuFile->addSeparator();
  menuFile->addAction( "E&xit", qApp,  SLOT( quit() ), Qt::CTRL + Qt::Key_Q );
  
  // panel
  QWidget* widget = new QWidget( this );
  
  QHBoxLayout* mainLayout = new QHBoxLayout( widget );
  
  _plot = new QwtPlot;
  _plot->setAutoReplot( true );
  _plot->setFixedSize( 640, 480 );

  QwtSymbol* symbol = new QwtSymbol( QwtSymbol::Cross,
                                     QBrush( QColor( 0, 0, 255 ) ),
				     QPen( QColor( 0, 0, 255 ) ),
				     QSize( 10, 10 ) );
  _curve = new QwtPlotCurve( "Distribution" );
  _curve->setStyle( QwtPlotCurve::NoCurve );
  _curve->setSymbol( symbol );
  _curve->attach( _plot );
  _fit = new QwtPlotCurve( "Gaussian mixture" );
  _fit->setPen( QPen( QColor( 255, 0, 0 ) ) );
  _fit->attach( _plot );
  QwtPlotGrid* grid = new QwtPlotGrid();
  grid->enableXMin( true );
  grid->enableYMin( true );

#if QWT_VERSION < 0x060100
  grid->setMinPen( QPen( Qt::DashLine ) );
#else
  grid->setMinorPen( QPen( Qt::DashLine ) );
#endif
  grid->attach( _plot );
  
  QVBoxLayout* vLayout = new QVBoxLayout;
  
  QHBoxLayout* h1Layout = new QHBoxLayout;
  
  QGridLayout* gLayout = new QGridLayout;
  gLayout->setHorizontalSpacing( 5 );
  
  QLabel* vLabel = new QLabel( QString( "v" ) );
  vLabel->setFixedHeight( 16 );
  vLabel->setAlignment( Qt::AlignHCenter );
  QLabel* sLabel = new QLabel( QString( "s" ) );
  sLabel->setFixedHeight( 16 );
  sLabel->setAlignment( Qt::AlignHCenter );
  _v = new QLabel;
  _v->setNum( _value );
  _v->setAlignment( Qt::AlignHCenter );
  _v->setFixedSize( 30, 16 );
  _s = new QLabel;
  _s->setNum( _sigma );
  _s->setAlignment( Qt::AlignHCenter );
  _s->setFixedSize( 30, 16 );
  _sliderV = new QScrollBar( Qt::Vertical );
  _sliderV->setRange( 0, 0 );  
  _sliderS = new QScrollBar( Qt::Vertical );
  _sliderS->setRange( 0, 0 );  
  gLayout->addWidget( vLabel, 0, 0, Qt::AlignHCenter );
  gLayout->addWidget( sLabel, 0, 1, Qt::AlignHCenter );
  gLayout->addWidget( _v, 1, 0, Qt::AlignHCenter );
  gLayout->addWidget( _s, 1, 1, Qt::AlignHCenter );
  gLayout->addWidget( _sliderV, 2, 0, Qt::AlignHCenter );
  gLayout->addWidget( _sliderS, 2, 1, Qt::AlignHCenter );
  
  QStringList labels( "Value" );
  _table = new QTableWidget( 0, 1 );
  _table->setHorizontalHeaderLabels( labels );
  _table->setColumnWidth( 0, 80 );
  
  h1Layout->addLayout( gLayout );
  h1Layout->addWidget( _table );
  
  QHBoxLayout* hLayout = new QHBoxLayout;
  QLabel* eLabel = new QLabel( QString( "e=" ) );
  eLabel->setAlignment( Qt::AlignRight );
  eLabel->setFixedSize( 32, 16 );
  _epsilon = new QLabel( QString( "0" ) );
  _epsilon->setAlignment( Qt::AlignLeft );
  _epsilon->setFixedHeight( 16 );
  hLayout->addWidget( eLabel );
  hLayout->addWidget( _epsilon );
  
  vLayout->addLayout( h1Layout );
  vLayout->addLayout( hLayout );
  
  mainLayout->addWidget( _plot );
  mainLayout->addLayout( vLayout );
  
  setCentralWidget( widget );
  
  connect( _sliderV, SIGNAL( valueChanged( int ) ), 
           this, SLOT( changeV( int ) ) );
  connect( _sliderS, SIGNAL( valueChanged( int ) ), 
           this, SLOT( changeS( int ) ) );
  
}


void gkg::AbacusMainWindow::initializeArg( const std::string& volName )
{  

  if ( !volName.empty() )
  {
  
    gkg::Reader::getInstance().read( volName, _data );

    if ( _data.getHeader().hasAttribute( "abacus" ) )
    {

      std::string abacusStr;
      _data.getHeader().getAttribute( "abacus", abacusStr );

      if ( abacusStr == "rice" )
      {

        _distribution = gkg::AbacusMainWindow::Rice;
        setWindowTitle( "GkgAbacus - Rice" );

      }
      else if ( abacusStr == "non central chi" )
      {

        _distribution = gkg::AbacusMainWindow::NonCentralChi;

        if ( _data.getHeader().hasAttribute( "channelCount" ) )
        {

          _data.getHeader().getAttribute( "channelCount", _channelCount );

        }

        QString title = 
              tr( "GkgAbacus - Non Central Chi (N = %1)" ).arg( _channelCount );
        setWindowTitle( title );

      }

      _data.getHeader().getAttribute( "resolutionX", _vStep );
      _data.getHeader().getAttribute( "resolutionY", _sStep );
      _data.getHeader().getAttribute( "signalStart", _vStart );
      _data.getHeader().getAttribute( "sigmaStart", _sStart );

    }

    if ( _distribution != gkg::AbacusMainWindow::Unknown )
    {

      _value = _vStart;
      _sigma = _sStart;

      _sliderV->setRange( 0, _data.getSizeX() - 1 );
      _sliderS->setRange( ( _sStart > 0.0 ) ? 0 : 1, _data.getSizeY() - 1 );

      _v->setNum( _vStart );
      _s->setNum( _sStart );
    
      int32_t i, n = _data.getSizeZ() / 3;
      _table->setRowCount( _data.getSizeZ() );
    
      QStringList labels;
      for ( i = 0; i < n; i++ )
      {
    
        labels << tr( "a%1" ).arg( i );
        labels << tr( "m%1" ).arg( i );
        labels << tr( "s%1" ).arg( i );
    
      }
    
      _table->setVerticalHeaderLabels( labels );
    
      draw();

    }

  }

}


void gkg::AbacusMainWindow::loadVolume()
{

  QString filt = "Volume files (*.vimg *.img *.ima);;Vida files (*.vimg);;";
  filt += "Gis files (*.ima);;Analyze files (*.img);;DICOM (*);;All files (*)";

  QString fname = QFileDialog::getOpenFileName( this, "Open abacus", 
                                                QString(), filt );
						
  initializeArg( fname.toStdString() );

}


void gkg::AbacusMainWindow::changeV( int value )
{

  _valueSlider = value;
  _value = _vStart + _vStep * value;
  _v->setNum( _value );
  draw();

}


void gkg::AbacusMainWindow::changeS( int value )
{

  _sigmaSlider = value;
  _sigma = _sStart + _sStep * value;
  _s->setNum( _sigma );
  draw();

}


void gkg::AbacusMainWindow::draw()
{

  // Rician distribution
  gkg::Matrix curveX( _n, 1 );
  gkg::Matrix curveY( _n, 1 );

  switch ( _distribution )
  {

    case gkg::AbacusMainWindow::Rice:
    {

      gkg::Vector parameters( 2 );
      parameters( 0 ) = _value;
      parameters( 1 ) = _sigma;
      gkg::RiceFunction rice( parameters );
      rice.getValuesAt( curveX, curveY );
      break;

    }

    case gkg::AbacusMainWindow::NonCentralChi:
    {

      gkg::Vector parameters( 3 );
      parameters( 0 ) = _value;
      parameters( 1 ) = _sigma;
      parameters( 2 ) = _channelCount;
      gkg::NonCentralChiFunction nonCentralChi( parameters );
      nonCentralChi.getValuesAt( curveX, curveY );
      break;

    }

    default:
      return;
      break;

  }
  
  _samples.clear();

  int32_t i;
  for ( i = 0; i < _n; i++ )
  {

    QPointF sample( curveX( i, 0 ), curveY( i, 0 ) );
    _samples.push_back( sample );
  
  }
  
  QwtPointSeriesData* curveSamples = new QwtPointSeriesData( _samples );
  _curve->setData( curveSamples );
  
  // Rician fitted by a Gaussian mixture
  int32_t dz = _data.getSizeZ();
  double value = 0.0;
  QString itemText;
  gkg::Vector gaussianParameters( dz );
  for ( i = 0; i < dz; i++ )
  {
  
    value = _data( _valueSlider, _sigmaSlider, i );
    gaussianParameters( i ) = value;
    itemText.setNum( value );
    QTableWidgetItem* item = new QTableWidgetItem( itemText );
    _table->setItem( i, 0, item );
  
  }
  
  gkg::GaussianMixtureFunction mixture( gaussianParameters );
  gkg::Vector vectX( 1 );
  QVector< QPointF > fitSamples;
  double error = 0.0, y1 = 0.0, delta = 0.0;
  for ( i = 0; i < _n; i++ )
  {
  
    QPointF sample = _samples[ i ];
    vectX( 0 ) = sample.x();
    y1 = sample.y();
    double y = mixture.getValueWithParametersAt( vectX, gaussianParameters );
    delta = y - y1;
    error += delta * delta;
    sample.setY( y );
    fitSamples.push_back( sample );
  
  }
  
  QwtPointSeriesData* fitCurve = new QwtPointSeriesData( fitSamples );
  _fit->setData( fitCurve );
  _epsilon->setNum( error );

}
