#include <gkg-anatomist-plugin-rendering-options/OrientationCountSpinBox.h>
#include <gkg-core-exception/Exception.h>


gkg::OrientationCountSpinBox::OrientationCountSpinBox( QWidget* parent )
                             : QSpinBox( parent )
{

  try
  {

    int32_t maximumIndex = 0;
    int32_t i = 0;
    for ( i = 6; i < 51; i++ )
    {

      _availableOrientationCounts[ maximumIndex ] = i;
      maximumIndex ++;

    }

    for ( i = 55; i <= 100; i += 5 )
    {

      _availableOrientationCounts[ maximumIndex ] = i;
      maximumIndex ++;

    }

    for ( i = 110; i <= 150; i += 10 )
    {

      _availableOrientationCounts[ maximumIndex ] = i;
      maximumIndex ++;

    }

    for ( i = 175; i <= 250; i += 25 )
    {

      _availableOrientationCounts[ maximumIndex ] = i;
      maximumIndex ++;

    }

    for ( i = 300; i <= 500; i += 50 )
    {

      _availableOrientationCounts[ maximumIndex ] = i;
      maximumIndex ++;

    }

    _availableOrientationCounts[ maximumIndex ] = 512;
    maximumIndex ++;
    _availableOrientationCounts[ maximumIndex ] = 550;
    maximumIndex ++;
    _availableOrientationCounts[ maximumIndex ] = 1000;
    maximumIndex ++;
    _availableOrientationCounts[ maximumIndex ] = 2000;
    maximumIndex ++;
    _availableOrientationCounts[ maximumIndex ] = 4000;
    maximumIndex ++;

    setRange( 6, 4000 );

    connect( this, SIGNAL( valueChanged( int ) ),
             this, SLOT( setNewValue( int ) ) );

  }
  GKG_CATCH( "gkg::OrientationCountSpinBox::OrientationCountSpinBox( "
             "QWidget* parent )" );

}


void gkg::OrientationCountSpinBox::setValueFromIndex( int32_t index )
{

  try
  {

    _index = index;
    setValue( _availableOrientationCounts[ index ] );

  }
  GKG_CATCH( "void OrientationCountSpinBox::setValueIndex( int32_t value )" );

}


void gkg::OrientationCountSpinBox::setNewValue( int value )
{

  try
  {

    blockSignals( true );

    if ( value  < 6 )
    {

      setValue( 6 );

    }
    else if ( value > 4000 )
    {

      setValue( 4000 );

    }
    else if ( value < 51 )
    {

      setValue( _availableOrientationCounts[ value - 6 ] );

    }
    else
    {

      std::map< int32_t, int32_t >::iterator
      i = _availableOrientationCounts.begin(),
      ie = _availableOrientationCounts.end();
      int32_t index = 0;
      while ( ( i != ie ) && ( _availableOrientationCounts[ index ] < value ) )
      {

        ++ i;
        ++ index;

      }

      if ( _availableOrientationCounts[ index ] - 1 == value )
      {

        setValue( _availableOrientationCounts[ index - 1 ] );

      }
      else
      {

        setValue( _availableOrientationCounts[ index ] );

      }

    }
    blockSignals( false );

  }
  GKG_CATCH( "void gkg::OrientationCountSpinBox::setNewValue( "
             "int32_t value )" );

}


