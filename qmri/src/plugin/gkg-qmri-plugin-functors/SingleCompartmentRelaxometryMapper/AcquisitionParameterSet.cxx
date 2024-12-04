#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/AcquisitionParameterSet.h>
#include <gkg-core-exception/Exception.h> 
#include <fstream>


gkg::AcquisitionParameterSet::AcquisitionParameterSet()
{
}


gkg::AcquisitionParameterSet::~AcquisitionParameterSet()
{
}


void gkg::AcquisitionParameterSet::readTextFile( const std::string& fileName, 
                                               int32_t count, 
                                               gkg::Vector& values )
{

  try
  {
  
    if ( values.getSize() != count )
    {
    
      values.reallocate( count ); 
      
    }
    
    std::ifstream is ( fileName.c_str() );
    
    if ( !is )
    {
    
      throw std::runtime_error( std::string("unable to open '" ) + 
                                fileName + "'" );

    }
    
    int32_t t; 
    for ( t = 0; t < count; t++ ) 
    {
    
      if ( is.eof() )
      {
      
        throw std::runtime_error(
                                "not enough values provided in the text file" );
      
      }
      is >> values( t );
      
    }
    
    is.close();
    
  }
  GKG_CATCH( "void gkg::SingleCompartmentRelaxometryMCMCFunction::"
             "readTextFile( "
             "const std::string& fileName, "
             "int32_t count, "
             "gkg::Vector& values ) const" );
             
}




