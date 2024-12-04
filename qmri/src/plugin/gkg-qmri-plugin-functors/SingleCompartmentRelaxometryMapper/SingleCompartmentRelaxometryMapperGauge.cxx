#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMapperGauge.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip> 


gkg::SingleCompartmentRelaxometryMapperGauge::
                 SingleCompartmentRelaxometryMapperGauge( int32_t maximumCount )
                                             : _count( 0 ), 
                                               _maximumCount( maximumCount ), 
                                               _siteIndexAlreadyDisplayed(
                                                                         false )
{
}


void gkg::SingleCompartmentRelaxometryMapperGauge::add( int32_t value )
{

  try
  {
  
    _count += value; 
    
    display();
    
  }
  GKG_CATCH( "void gkg::SingleCompartmentRelaxometryMapperGauge::" 
             "add( int32_t value)" ); 

}


void gkg::SingleCompartmentRelaxometryMapperGauge::reset()
{

  try
  {
  
    _count = 0; 
    
    display(); 
    
  }
  GKG_CATCH( "void gkg::SingleCompartmentRelaxometryMapperGauge::reset()" ); 
  
}


void gkg::SingleCompartmentRelaxometryMapperGauge::display()
{

  try
  {
    
    if ( _siteIndexAlreadyDisplayed )
    {
    
      std::cout << gkg::Eraser( 28 );
    
    }
    
    std::cout << " [ " << std::setw( 10 ) << _count 
              << " / " << std::setw( 10 ) << _maximumCount
              << " ]" << std::flush; 
    
    _siteIndexAlreadyDisplayed = true; 
  
  }
  GKG_CATCH( "void gkg::SingleCompartmentRelaxometryMapperGauge::display()" ); 

}
