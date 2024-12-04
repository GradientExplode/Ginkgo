#include <gkg-communication-algorithm/Algorithm.h>
#include <gkg-communication-algorithm/AlgorithmParameter_i.h>
#include <gkg-core-exception/Exception.h>


gkg::Algorithm::Algorithm( const std::string& name )
               : _name( name )
{
}


gkg::Algorithm::~Algorithm()
{

  std::vector< gkg::AlgorithmParameter* >::iterator p = _parameters.begin(),
                                                    pe = _parameters.end();
  while ( p != pe )
  {

    delete *p;
    ++ p;

  }

}


const std::string& gkg::Algorithm::getName() const
{

  return _name;

}


gkg::AlgorithmParameter* 
gkg::Algorithm::getParameter( const std::string& name ) const
{

  std::vector< gkg::AlgorithmParameter* >::const_iterator
    p = _parameters.begin(),
    pe = _parameters.end();

  while ( p != pe )
  {

    if ( ( *p )->getSignature().getName() == name )
    {

      return *p;

    }
    ++ p;

  }
  return 0;

}


void gkg::Algorithm::addParameter( gkg::AlgorithmParameter* algorithmParameter )
{

  try
  {

    if ( !algorithmParameter )
    {

      throw std::runtime_error( "null parameter" );

    }

    if ( this->getParameter( algorithmParameter->getSignature().getName() )
           == 0 )
    {

      _parameters.push_back( algorithmParameter );

    }
    else
    {

      throw std::runtime_error( "parameter name already exists" );

    }

  }
  GKG_CATCH( "void gkg::Algorithm::addParameter( "
             "gkg::AlgorithmParameter* algorithmParameter )" );

}
