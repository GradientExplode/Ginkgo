#include <gkg-qmri-plugin-functors/MyelinWaterFractionMapper/MyelinWaterFractionMapperGauge.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


gkg::MyelinWaterFractionMapperGauge::MyelinWaterFractionMapperGauge( 
                                                              int32_t maxCount )
                                    : _count( 0 ),
                                      _maxCount( maxCount ),
                                      _siteIndexAlreadyDisplayed( false )
{
}


void gkg::MyelinWaterFractionMapperGauge::add( int32_t value )
{

  try
  {

    _count += value;

    display();  

  }
  GKG_CATCH( "void gkg::MyelinWaterFractionMapperGauge::add( int32_t value )" );

}


void gkg::MyelinWaterFractionMapperGauge::reset()
{

  try
  {

    _count = 0;

    display();  

  }
  GKG_CATCH( "void gkg::MyelinWaterFractionMapperGauge::reset()" );

}


void gkg::MyelinWaterFractionMapperGauge::display()
{

  try
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
  GKG_CATCH( "void gkg::MyelinWaterFractionMapperGauge::display()" );

}
