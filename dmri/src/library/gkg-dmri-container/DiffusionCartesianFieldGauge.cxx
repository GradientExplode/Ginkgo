#include <gkg-dmri-container/DiffusionCartesianFieldGauge.h>
#include <gkg-core-io/Eraser.h>

#include <iomanip>


gkg::DiffusionCartesianFieldGauge::DiffusionCartesianFieldGauge( 
                                                              int32_t maxCount )
                                 : _count( 0 ),
                                   _maxCount( maxCount ),
                                   _siteIndexAlreadyDisplayed( false )
{
}


void gkg::DiffusionCartesianFieldGauge::add( int32_t value )
{

  _count += value;

  display();  

}


void gkg::DiffusionCartesianFieldGauge::reset()
{

  _count = 0;

  display();  

}


void gkg::DiffusionCartesianFieldGauge::display()
{
  
  if ( _siteIndexAlreadyDisplayed )
  {

    std::cout << gkg::Eraser( 28 );

  }

  std::cout << " [ " << std::setw( 10 ) << _count 
  	    << " / " << std::setw( 10 ) << _maxCount
  	    << " ]" << std::flush;

  _siteIndexAlreadyDisplayed = true;

}
