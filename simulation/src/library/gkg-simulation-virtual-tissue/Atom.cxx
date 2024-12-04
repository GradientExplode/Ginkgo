#include <gkg-simulation-virtual-tissue/Atom.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::Atom::Atom()
{
}


gkg::Atom::Atom( const gkg::Atom& other )
          : parameters( other.parameters )
{
}


gkg::Atom::~Atom()
{
}


gkg::Atom& 
gkg::Atom::operator=( const gkg::Atom& other )
{

  try
  {

    if ( other.getType() != this->getType() )
    {

      throw std::runtime_error( "not the same atom type" );

    }
    parameters = other.parameters;

    return *this;

  }
  GKG_CATCH( "gkg::Atom& "
             "gkg::Atom::operator=( const gkg::Atom& other )" );

}


void gkg::Atom::setParameters( const float* inputParameters )
{

  try
  {

    const float* ip = inputParameters;
    std::vector< float >::iterator
      p = parameters.begin(),
      pe = parameters.end();
    while ( p != pe )
    {

      *p = *ip;
      ++ ip;
      ++ p;

    }

  }
  GKG_CATCH( "void gkg::Atom::setParameters( const float* inputParameters )" );

}


void gkg::Atom::copyParameters( float* outputParameters )
{

  try
  {
  
    float* o = outputParameters;
    std::vector< float >::const_iterator i = parameters.begin(),
                                         ie = parameters.end();
    while ( i != ie )
    {
    
      *o = *i;
      ++ i;
      ++ o;
    
    }
  
  }
  GKG_CATCH( "void gkg::Atom::copyParameters( float* outputParameters )" );

}

bool gkg::Atom::hasCompressedRepresentation() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "bool gkg::Atom::hasCompressedRepresentation() const" );

}


void gkg::Atom::switchRepresentation()
{

  // by default, nothing to do

}


uint8_t gkg::Atom::getType() const
{

  try
  {

    // should not enter this method, so return a nul type id
    return 0U;

  }
  GKG_CATCH( "uint8_t gkg::Atom::getType() const" );

}



int32_t gkg::Atom::getParameterCount() const
{

  try
  {

    return ( int32_t )parameters.size();

  }
  GKG_CATCH( "int32_t gkg::Atom::getParameterCount() const" );

}

