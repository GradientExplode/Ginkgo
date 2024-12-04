#ifndef _gkg_communication_algorithm_VectorReader_i_h_
#define _gkg_communication_algorithm_VectorReader_i_h_


#include <gkg-communication-algorithm/VectorReader.h>
#include <gkg-communication-algorithm/TypedAlgorithmParameter_i.h>
#include <fstream>


template < class T >
inline
gkg::VectorReader< T >::VectorReader()
                       : gkg::Algorithm( "vector_reader" )
{

  _fileName = new gkg::TypedAlgorithmParameter< std::string >(
                                                       "filename",
                                                       true,
                                                       false,
                                                       "this is the filename" );
  _vector = new gkg::TypedAlgorithmParameter< std::vector< T > >(
                                                       "vector",
                                                       false,
                                                       true,
                                                       "this is the vector" );

  this->addParameter( _fileName );
  this->addParameter( _vector );

}


template < class T >
inline
gkg::VectorReader< T >::~VectorReader()
{
}


template < class T >
inline
bool gkg::VectorReader< T >::check()
{

  // to be implemented

  // check memory capacity,...

  // check that fileName is readable on disk

  return true;

}


template < class T >
inline
void gkg::VectorReader< T >::run()
{

  size_t count;

  std::ifstream is( _fileName->getPointee()->c_str() );

  is >> count;

  *_vector->getPointee() = std::vector< T >( count );

  for ( size_t i = 0; i < count; i++ )
  {

    is >> ( *_vector->getPointee() )[ i ];

  }

  is.close();

}


#endif
