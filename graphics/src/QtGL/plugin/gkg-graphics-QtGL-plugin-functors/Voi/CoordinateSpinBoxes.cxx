#include <gkg-graphics-QtGL-plugin-functors/Voi/CoordinateSpinBoxes.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>
#include <gkg-processing-container/Volume_i.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>


gkg::CoordinateSpinBoxes::CoordinateSpinBoxes( VoiObservable* d,
                                               QWidget* parent )
                        : QWidget( parent ),
                          gkg::VoiObserver(),
                          _t( 0 )
{

  _data = (gkg::VoiData *)d;

  registerData( d );
  d->registerView( this );

  QLabel* xLab = new QLabel( "X:");
  _spinX = new QSpinBox();
  connect( _spinX, SIGNAL( valueChanged( int ) ), this, SLOT( setX( int ) ) );

  QLabel* yLab = new QLabel( "Y:");
  _spinY = new QSpinBox();
  connect( _spinY, SIGNAL( valueChanged( int ) ), this, SLOT( setY( int ) ) );

  QLabel* zLab = new QLabel( "Z:");
  _spinZ = new QSpinBox();
  connect( _spinZ, SIGNAL( valueChanged( int ) ), this, SLOT( setZ( int ) ) );

  QHBoxLayout* layout = new QHBoxLayout();
  layout->addWidget( xLab );
  layout->addWidget( _spinX, Qt::AlignLeft );
  layout->addWidget( yLab );
  layout->addWidget( _spinY, Qt::AlignLeft );
  layout->addWidget( zLab );
  layout->addWidget( _spinZ, Qt::AlignLeft );

  setLayout( layout );

}


void gkg::CoordinateSpinBoxes::updateDataLoaded()
{

  if ( _data )
  {

    gkg::Volume< float >& t1 = _data->getBaseFusion().getT1();
    int32_t dX = t1.getSizeX();
    int32_t dY = t1.getSizeY();
    int32_t dZ = t1.getSizeZ();
    _spinX->setRange( 0, dX - 1 );
    _spinX->setValue( dX / 2 );
    _spinY->setRange( 0, dY - 1 );
    _spinY->setValue( dY / 2 );
    _spinZ->setRange( 0, dZ - 1 );
    _spinZ->setValue( dZ / 2 );
    _coordinate.x = dX / 2;
    _coordinate.y = dY / 2;
    _coordinate.z = dZ / 2;

  }

}


void gkg::CoordinateSpinBoxes::updateLabelLoaded()
{

  if ( _data && !_data->dataLoaded() )
  {

    int32_t dX = _data->label().getSizeX();
    int32_t dY = _data->label().getSizeY();
    int32_t dZ = _data->label().getSizeZ();
    _spinX->setRange( 0, dX - 1 );
    _spinX->setValue( dX / 2 );
    _spinY->setRange( 0, dY - 1 );
    _spinY->setValue( dY / 2 );
    _spinZ->setRange( 0, dZ - 1 );
    _spinZ->setValue( dZ / 2 );
    _coordinate.x = dX / 2;
    _coordinate.y = dY / 2;
    _coordinate.z = dZ / 2;

  }

}


void gkg::CoordinateSpinBoxes::updateSlice( gkg::Vector3d< int32_t >& p, 
                                            int32_t t )
{

  if ( _data )
  {

    _spinX->setValue( p.x );
    _spinY->setValue( p.y );
    _spinZ->setValue( p.z );
    _coordinate = p;
    _t = t;

  }

}


void gkg::CoordinateSpinBoxes::setX( int value )
{

  if ( _data && !_data->empty() )
  {

    _coordinate.x = value;
    _data->notifySlice( _coordinate, _t );

  }

}


void gkg::CoordinateSpinBoxes::setY( int value )
{

  if ( _data && !_data->empty() )
  {

    _coordinate.y = value;
    _data->notifySlice( _coordinate, _t );

  }

}


void gkg::CoordinateSpinBoxes::setZ( int value )
{

  if ( _data && !_data->empty() )
  {

    _coordinate.z = value;
    _data->notifySlice( _coordinate, _t );

  }

}
