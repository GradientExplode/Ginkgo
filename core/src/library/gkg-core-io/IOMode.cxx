#include <gkg-core-io/IOMode.h>
#include <gkg-core-exception/Exception.h>


gkg::IOMode::IOMode()
{
}


gkg::IOMode::~IOMode()
{
}


gkg::IOMode::Type 
gkg::IOMode::getTypeFromName( const std::string& name ) const
{

  try
  {

    gkg::IOMode::Type type;
    if ( name == "ascii" )
    {

      type = gkg::IOMode::Ascii;

    }
    else if ( ( name == "binary" ) ||
              ( name == "binar" ) )
    {

      type = gkg::IOMode::Binary;

    }
    else
    {

      throw std::runtime_error(  name + "' is not a valid open mode name" );

    }
    return type;

  }
  GKG_CATCH( "gkg::IOMode::Type gkg::IOMode::getTypeFromName( const "
             "std::string& name ) const" );

}


std::string gkg::IOMode::getNameFromType( gkg::IOMode::Type type ) const
{

  std::string name = "";
  switch ( type )
  {

    case gkg::IOMode::Ascii:

      name = "ascii";
      break;

    case gkg::IOMode::Binary:

      name = "binary";
      break;

  }
  return name;

}
