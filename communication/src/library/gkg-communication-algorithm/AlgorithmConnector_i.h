#ifndef _gkg_communication_algorithm_AlgorithmConnector_i_h_
#define _gkg_communication_algorithm_AlgorithmConnector_i_h_


#include <gkg-communication-algorithm/AlgorithmConnector.h>
#include <gkg-communication-algorithm/TypedAlgorithmParameter_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/Exception.h>


//
// class AlgorithmConnector
//

inline
gkg::AlgorithmConnector::AlgorithmConnector()
{
}


inline
gkg::AlgorithmConnector::~AlgorithmConnector()
{
}




//
// class TypedAlgorithmConnector
//


template < class T >
inline
gkg::TypedAlgorithmConnector< T >::TypedAlgorithmConnector()
                                  : gkg::AlgorithmConnector()
{
}


template < class T >
inline
gkg::TypedAlgorithmConnector< T >::~TypedAlgorithmConnector()
{
}


template < class T >
inline
std::string gkg::TypedAlgorithmConnector< T >::getType() const
{

  return gkg::TypeOf< T >::getName();

}


template < class T >
inline
void gkg::TypedAlgorithmConnector< T >::connect(
                                           gkg::AlgorithmParameter* parameter1,
                                           gkg::AlgorithmParameter* parameter2 )
{

  try
  {

    gkg::TypedAlgorithmParameter< T >* typedParameter1 = 
      dynamic_cast< gkg::TypedAlgorithmParameter< T >* >( parameter1 );
    gkg::TypedAlgorithmParameter< T >* typeParameter2 = 
      dynamic_cast< gkg::TypedAlgorithmParameter< T >* >( parameter2 );

    if ( !parameter1 )
    {

      throw std::runtime_error( "dynamic cast failed on parameter 1" );

    }
    if ( !parameter2 )
    {

      throw std::runtime_error( "dynamic cast failed on parameter 2" );

    }
    
    typedParameter1->connect( typeParameter2 );


  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void "
             "gkg::TypedAlgorithmConnector< T >::connect( "
             "gkg::AlgorithmParameter* p1, "
             "gkg::AlgorithmParameter* p2 ) const" );

}


#endif
