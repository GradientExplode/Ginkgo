#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixGauge.h>
#include <gkg-core-io/Eraser.h>

#include <iomanip>


gkg::ConnectivityMatrixGauge::ConnectivityMatrixGauge( int32_t fiberCount )
                            : _count( 0 ),
                              _fiberCount( fiberCount ),
                              _fiberIndexAlreadyDisplayed( false )
{
}


void gkg::ConnectivityMatrixGauge::add( int32_t value )
{

  _count += value;

  display();  

}


void gkg::ConnectivityMatrixGauge::reset()
{

  _count = 0;

  display();  

}


void gkg::ConnectivityMatrixGauge::display()
{
  
  if ( _fiberIndexAlreadyDisplayed )
  {

    std::cout << gkg::Eraser( 28 );

  }

  std::cout << " fiber [ " << std::setw( 7 ) << _count 
  	    << " / " << std::setw( 7 ) << _fiberCount
  	    << " ]" << std::flush;

  _fiberIndexAlreadyDisplayed = true;

}
