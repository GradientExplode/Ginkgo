#ifndef _gkg_communication_algorithm_AlgorithmParameter_i_h_
#define _gkg_communication_algorithm_AlgorithmParameter_i_h_

#include <gkg-communication-algorithm/AlgorithmParameter.h>
#include <gkg-communication-algorithm/AlgorithmConnector.h>
#include <gkg-communication-algorithm/TypedAlgorithmParameter_i.h>
#include <gkg-core-exception/Exception.h>


template < class T >
inline
void gkg::AlgorithmParameter::set( const T& value )
{

  try
  {

    gkg::TypedAlgorithmParameter< T >*
    typedParameter = dynamic_cast< gkg::TypedAlgorithmParameter< T >* >( this );

    if ( typedParameter == 0 )
    {

      throw std::runtime_error( "unvalid algorithm parameter dynamic cast" );

    }
    else
    {

      typedParameter->set( value );

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::AlgorithmParameter::set< T >( const T& value )" );

}





inline
void gkg::AlgorithmParameter::connect( AlgorithmParameter* other )
{

  try
  {

    std::string type = this->getSignature().getType();

    if (  other->getSignature().getType() == type )
    {

      gkg::AlgorithmConnectorFactory::getInstance().getAlgorithmConnector(
            type )->connect( this, other );

    }
    else
    {

      throw std::runtime_error( "parameter types do not match" );

    }

  }
  GKG_CATCH( "inline "
             "void gkg::AlgorithmParameter::connect( "
             "AlgorithmParameter* other )" );

}


#endif
