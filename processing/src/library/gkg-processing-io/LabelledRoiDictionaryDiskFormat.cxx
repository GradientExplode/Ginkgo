#include <gkg-processing-io/LabelledRoiDictionaryDiskFormat.h>
#include <gkg-processing-roi/LabelledRoiDictionary.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-exception/ObjectTypeException.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>


gkg::LabelledRoiDictionaryDiskFormat::LabelledRoiDictionaryDiskFormat()
                                     : gkg::TypedDiskFormat< 
				                  gkg::LabelledRoiDictionary >()
{

  this->_extensions.insert( getHeaderExtension() );

}


gkg::LabelledRoiDictionaryDiskFormat::~LabelledRoiDictionaryDiskFormat()
{
}


std::string gkg::LabelledRoiDictionaryDiskFormat::getName() const
{

  return "labelled-roi-dictionary";

}


void gkg::LabelledRoiDictionaryDiskFormat::readHeader( 
                                             const std::string& name,
                                             gkg::HeaderedObject& object ) const
{
  try
  {

    // reading the meta-information header
    try
    {

      this->gkg::DiskFormat::readHeader( name, object );
      
    }
    catch ( gkg::FileNotFoundException& e )
    {

      // std::cerr << e.what() << ", "
      //           << "meta-information not present"
      //           << std::endl;

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( e.what() );

    }
    
    // checking that it is a LabelledRoiDictionary
    std::string tmpObjectType;
    object.getHeader().getAttribute( "object_type", tmpObjectType );
    if ( tmpObjectType != "LabelledRoiDictionary" )
    {

      throw gkg::ObjectTypeException( this->getNameWithoutExtension( name ) +
                                      getHeaderExtension() );

    }

  }
  GKG_CATCH( "void gkg::LabelledRoiDictionaryDiskFormat::readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}

void gkg::LabelledRoiDictionaryDiskFormat::read( 
                                           const std::string& name,
	                                   LabelledRoiDictionary& object ) const
{

  try
  {
  
    readHeader( name, object );
  
  }
  GKG_CATCH( "void gkg::LabelledRoiDictionaryDiskFormat::read( "
             "const std::string& name, LabelledRoiDictionary& object ) const" );

}
	   
void gkg::LabelledRoiDictionaryDiskFormat::write( const std::string& name,
	                                          LabelledRoiDictionary& object,
	                                          bool ) const
{

  try
  {
  
    writeHeader( name, object );
  
  }
  GKG_CATCH( "void gkg::LabelledRoiDictionaryDiskFormat::write( "
             "const std::string& name, LabelledRoiDictionary& object, "
	     "bool ) const" );

}

bool gkg::LabelledRoiDictionaryDiskFormat::hasReader() const
{

  return true;

}

bool gkg::LabelledRoiDictionaryDiskFormat::hasWriter() const
{

  return true;

}



//
// registrating disk format for "LabelledRoiDictionary" object factory
//

static bool initialize()
{


  try
  {

    gkg::DiskFormatFactory< gkg::LabelledRoiDictionary
                          >::getInstance().registerDiskFormat(
      gkg::LabelledRoiDictionaryDiskFormat::getInstance().getName(),
      &gkg::LabelledRoiDictionaryDiskFormat::getInstance() );


    return true;

  }
  GKG_CATCH( "LabelledRoiDictionary disk format registration" );

}


static bool initialized __attribute__((unused)) = initialize();
