#include <gkg-qmri-plugin-functors/MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryMapperGauge.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip> 


gkg::MultiCompartmentRelaxometryMapperGauge::
                    MultiCompartmentRelaxometryMapperGauge( int32_t maxCount )
                                           : _count( 0 ), 
                                             _maxCount( maxCount ), 
                                             _siteIndexAlreadyDisplayed( false )
{
}


void gkg::MultiCompartmentRelaxometryMapperGauge::add( int32_t value )
{

  try
  {
  
    _count += value; 
    
    display();
    
  }
  GKG_CATCH( "void gkg::MultiCompartmentRelaxometryMapperGauge::" 
             "add( int32_t value)" ); 

}


void gkg::MultiCompartmentRelaxometryMapperGauge::reset()
{

  try
  {
  
    _count = 0; 
    
    display(); 
    
  }
  GKG_CATCH( "void gkg::MultiCompartmentRelaxometryMapperGauge::reset()" ); 
  
}


void gkg::MultiCompartmentRelaxometryMapperGauge::display()
{

  try
  {
    
    if ( _siteIndexAlreadyDisplayed )
    {
    
      std::cout << gkg::Eraser( 28 );
    
    }
    
    std::cout << " [ " << std::setw( 10 ) << _count 
              << " / " << std::setw( 10 ) << _maxCount
              << " ] " << std::flush; 
    
    _siteIndexAlreadyDisplayed = true; 
  
  }
  GKG_CATCH( "void gkg::MultiCompartmentRelaxometryMapperGauge::display()" ); 

}
