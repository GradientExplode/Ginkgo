#include <gkg-core-io/ByteOrder.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-exception/Exception.h>


gkg::ByteOrder::ByteOrder()
{
}


gkg::ByteOrder::~ByteOrder()
{
}


gkg::ByteOrder::Type gkg::ByteOrder::getCurrentType() const
{

  int32_t magicNumber = 0x41424344;
  return ( *( ( char* )&magicNumber ) == 'A' ) ? gkg::ByteOrder::BigEndian :
                                                 gkg::ByteOrder::LittleEndian;

}


std::string gkg::ByteOrder::getCurrentName() const
{

  return getNameFromType( getCurrentType() );

}


std::string gkg::ByteOrder::getCurrentABCDCode() const
{

  return getABCDCodeFromType( getCurrentType() );

}


gkg::ByteOrder::Type 
gkg::ByteOrder::getTypeFromName( const std::string& name ) const
{

  try
  {

    gkg::ByteOrder::Type type;
    if ( name == "LittleEndian" )
    {

      type = gkg::ByteOrder::LittleEndian;

    }
    else if ( name == "BigEndian" )
    {

      type = gkg::ByteOrder::BigEndian;

    }
    else
    {

      throw std::runtime_error( name + "' is not a valid byte order name" );

    }
    return type;

  }
  GKG_CATCH( "gkg::ByteOrder::Type gkg::ByteOrder::getTypeFromName( "
             "const std::string& name ) const" );

}


std::string gkg::ByteOrder::getNameFromType( gkg::ByteOrder::Type type ) const
{

  std::string name = "";
  switch ( type )
  {

    case gkg::ByteOrder::LittleEndian:

      name = "LittleEndian";
      break;

    case gkg::ByteOrder::BigEndian:

      name = "BigEndian";
      break;

  }
  return name;

}


gkg::ByteOrder::Type 
gkg::ByteOrder::getTypeFromABCDCode( const std::string& code ) const
{

  try
  {

    gkg::ByteOrder::Type type;
    if ( code == "DCBA" )
    {

      type = gkg::ByteOrder::LittleEndian;

    }
    else if ( code == "ABCD" )
    {

      type = gkg::ByteOrder::BigEndian;

    }
    else
    {

      throw std::runtime_error( code + "' is not a valid byte order code" );

    }
    return type;

  }
  GKG_CATCH( "gkg::ByteOrder::Type gkg::ByteOrder::getTypeFromABCDCode( "
             "const std::string& code ) const" );

}


std::string 
gkg::ByteOrder::getABCDCodeFromType( gkg::ByteOrder::Type type ) const
{

  std::string code = "";
  switch ( type )
  {

    case gkg::ByteOrder::LittleEndian:

      code = "DCBA";
      break;

    case gkg::ByteOrder::BigEndian:

      code = "ABCD";
      break;

  }
  return code;

}
