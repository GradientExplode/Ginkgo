#include <gkg-processing-transform/Referential.h>
#include <gkg-core-exception/Exception.h>


gkg::Referential::Referential( const std::string& name )
{

  try
  {

    setName( name );

  }
  GKG_CATCH( "gkg::Referential::Referential( const std::string& name )" );

}


gkg::Referential::~Referential()
{
}


void gkg::Referential::setName( const std::string& name )
{

  try
  {

    if ( name.empty() )
    {

      throw std::runtime_error( "empty name" );

    }
    _name = name;

  }
  GKG_CATCH( "void gkg::Referential::setName( const std::string& name )" );

}


const std::string& gkg::Referential::getName() const
{

  return _name;

}
