#include <gkg-processing-io/AimsAcPcIhDiskFormat.h>
#include <gkg-processing-transform/AcPcIhCoordinates.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-exception/ObjectTypeException.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>


gkg::AimsAcPcIhDiskFormat::AimsAcPcIhDiskFormat()
                          : gkg::TypedDiskFormat< gkg::AcPcIhCoordinates >()
{

  this->_extensions.insert( ".APC" );

}


gkg::AimsAcPcIhDiskFormat::~AimsAcPcIhDiskFormat()
{
}


std::string gkg::AimsAcPcIhDiskFormat::getName() const
{

  return "aimsapc";

}


void gkg::AimsAcPcIhDiskFormat::readHeader( const std::string& name,
                                            gkg::HeaderedObject& object ) const


{

  try
  {

    // building APC file name
    std::string apcName = this->getNameWithoutExtension( name ) + ".APC";

    // checking that APC file can be read
    gkg::AcPcIhCoordinates acPcIhCoordinates;
    readAPCFile( apcName, acPcIhCoordinates );
      
    // no exception was launched at this point, so file is OK and we know it is
    // a AcPcIhCoordinates
    object.getHeader().addAttribute( "object_type",
                                       std::string( "AcPcIhCoordinates" ) );

  }
  GKG_CATCH( "void gkg::AimsAcPcIhDiskFormat::readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}




void gkg::AimsAcPcIhDiskFormat::writeHeader(
                                   const std::string& /*name*/,
                                   const gkg::HeaderedObject& /*object*/ ) const


{

  // does nothing special because APC file does't have a meta header info file

}
    
 
    
    
void gkg::AimsAcPcIhDiskFormat::read( const std::string& name,
                                      gkg::AcPcIhCoordinates& object ) const
{

  try
  {


    // reading APC header
    readHeader( name, object );

    // building APC file name
    std::string apcName = this->getNameWithoutExtension( name ) + ".APC";

    // reading APC file
    readAPCFile( apcName, object );

  }
  GKG_CATCH( "void gkg::AimsAcPcIhDiskFormat::read( "
             "const std::string& name, "
             "gkg::AcPcIhCoordinates& object ) const" );

}
    
    
void gkg::AimsAcPcIhDiskFormat::write( const std::string& name,
                                       gkg::AcPcIhCoordinates& object,
                                       bool ascii ) const
{

  try
  {
    
    // sanity check
    if ( !ascii )
    {
    
      throw std::runtime_error( "only ASCII mode is supported" );
    
    }
    
    // building APC file name
    std::string apcName = this->getNameWithoutExtension( name ) + ".APC";

    // writing file
    std::ofstream os( apcName.c_str() );
    os << "AC: " << object.integerAnteriorCommissure.x << " "
                 << object.integerAnteriorCommissure.y << " "
                 << object.integerAnteriorCommissure.z << std::endl;
    os << "PC: " << object.integerPosteriorCommissure.x << " "
                 << object.integerPosteriorCommissure.y << " "
                 << object.integerPosteriorCommissure.z << std::endl;
    os << "IH: " << object.integerInterHemispheric.x << " "
                 << object.integerInterHemispheric.y << " "
                 << object.integerInterHemispheric.z << std::endl;
    os << "The previous coordinates, used by the system, are defined in voxels"
       << std::endl;
    os << "They stem from the following coordinates in millimeters:"
       << std::endl;
    os << "ACmm: " << object.anteriorCommissure.x << " "
                   << object.anteriorCommissure.y << " "
                   << object.anteriorCommissure.z << std::endl;
    os << "PCmm: " << object.posteriorCommissure.x << " "
                   << object.posteriorCommissure.y << " "
                   << object.posteriorCommissure.z << std::endl;
    os << "IHmm: " << object.interHemispheric.x << " "
                   << object.interHemispheric.y << " "
                   << object.interHemispheric.z << std::endl;
    os.close();

    
  }
  GKG_CATCH( "void gkg::AimsAcPcIhDiskFormat::write( "
             "const std::string& name, "
             "gkg::AcPcIhCoordinates& object, "
             "bool ascii ) const" );

}


bool gkg::AimsAcPcIhDiskFormat::hasReader() const
{

  return true;

}


bool gkg::AimsAcPcIhDiskFormat::hasWriter() const
{

  return true;

}


void gkg::AimsAcPcIhDiskFormat::readAPCFile(
                                         const std::string& apcName,
                                         gkg::AcPcIhCoordinates& object  ) const
{

  try
  {
  
  
    std::ifstream is( apcName.c_str() );
    
    
    if ( !is )
    {
    
      throw gkg::FileNotFoundException( apcName );

    }

    std::string tmp;

    is >> tmp;
    if ( tmp != "AC:" )
    {
    
      throw std::runtime_error( "corrupted ACP file" );
    
    }
    is >> object.integerAnteriorCommissure.x
       >> object.integerAnteriorCommissure.y
       >> object.integerAnteriorCommissure.z;
    if ( !is )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );

    }

    is >> tmp;
    if ( tmp != "PC:" )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }
    is >> object.integerPosteriorCommissure.x
       >> object.integerPosteriorCommissure.y
       >> object.integerPosteriorCommissure.z;
    if ( !is )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );

    }

    is >> tmp;
    if ( tmp != "IH:" )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }
    is >> object.integerInterHemispheric.x
       >> object.integerInterHemispheric.y
       >> object.integerInterHemispheric.z;
    if ( !is )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );

    }

    is >> tmp;
    if ( tmp != "The" )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }   
    
    
    is >> tmp;
    if ( tmp != "previous" )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }   
     
    
    is >> tmp;
    if ( tmp != "coordinates," )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }   
     
        
    is >> tmp;
    if ( tmp != "used" )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }   
     
    is >> tmp;
    if ( tmp != "by" )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }   
     
    is >> tmp;
    if ( tmp != "the" )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }   

    is >> tmp;
    if ( tmp != "system," )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }   
    
    is >> tmp;
    if ( tmp != "are" )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }   

    is >> tmp;
    if ( tmp != "defined" )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }   

    is >> tmp;
    if ( tmp != "in" )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }   
  
    
    is >> tmp;
    if ( tmp != "voxels" )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }   
    
    is >> tmp;
    if ( tmp != "They" )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }   

    is >> tmp;
    if ( tmp != "stem" )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }   

    is >> tmp;
    if ( tmp != "from" )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }   

    is >> tmp;
    if ( tmp != "the" )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }   

    is >> tmp;
    if ( tmp != "following" )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }   

    is >> tmp;
    if ( tmp != "coordinates" )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }   

    is >> tmp;
    if ( tmp != "in" )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }   

    is >> tmp;
    if ( tmp != "millimeters:" )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }   


    is >> tmp;
    if ( tmp != "ACmm:" )
    {
    
      throw std::runtime_error( "corrupted ACP file" );
    
    }
    is >> object.anteriorCommissure.x
       >> object.anteriorCommissure.y
       >> object.anteriorCommissure.z;
    if ( !is )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );

    }

    is >> tmp;
    if ( tmp != "PCmm:" )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }
    is >> object.posteriorCommissure.x
       >> object.posteriorCommissure.y
       >> object.posteriorCommissure.z;
    if ( !is )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );

    }

    is >> tmp;
    if ( tmp != "IHmm:" )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );
    
    }
    is >> object.interHemispheric.x
       >> object.interHemispheric.y
       >> object.interHemispheric.z;
    if ( !is )
    {
    
      throw std::runtime_error( "corrupted AIMS APC file" );

    }

    is.close();
  
  }
  GKG_CATCH( "void gkg::AimsAcPcIhDiskFormat::readAPCFile( "
             "const std::string& apcName, "
             "gkg::AcPcIhCoordinates& object  ) const" );

}


//
// registrating AIMS APC disk format for "AcPcIhCoordinates" object factory
//

static bool initialize()
{


  try
  {

    gkg::DiskFormatFactory< gkg::AcPcIhCoordinates 
                          >::getInstance().registerDiskFormat(
         gkg::AimsAcPcIhDiskFormat::getInstance().getName(),
         &gkg::AimsAcPcIhDiskFormat::getInstance() );


    return true;

  }
  GKG_CATCH( "AIMS APC disk format registration" );

}


static bool initialized __attribute((unused)) = initialize();
