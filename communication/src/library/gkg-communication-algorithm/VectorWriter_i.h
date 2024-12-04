#ifndef _gkg_communication_algorithm_VectorWriter_i_h_
#define _gkg_communication_algorithm_VectorWriter_i_h_


#include <gkg-communication-algorithm/VectorWriter.h>
#include <gkg-communication-algorithm/TypedAlgorithmParameter_i.h>
#include <fstream>


template < class T >
inline
gkg::VectorWriter< T >::VectorWriter()
                       : gkg::Algorithm( "vector_writer" )
{

  _fileName = new gkg::TypedAlgorithmParameter< std::string >(
                                                       "filename",
                                                       true,
                                                       false,
                                                       "this is the filename" );
  _vector = new gkg::TypedAlgorithmParameter< std::vector< T > >(
                                                       "vector",
                                                       true,
                                                       false,
                                                       "this is the vector" );

  this->addParameter( _fileName );
  this->addParameter( _vector );

}


template < class T >
inline
gkg::VectorWriter< T >::~VectorWriter()
{
}


template < class T >
inline
bool gkg::VectorWriter< T >::check()
{

  // to be implemented

  // check memory capacity,...

  // check that fileName is writable on disk

  return true;

}


template < class T >
inline
void gkg::VectorWriter< T >::run()
{

  std::ofstream os( _fileName->getPointee()->c_str() );
  for ( size_t i = 0; i < _vector->getPointee()->size(); i++ )
  {

    os << ( *_vector->getPointee() )[ i ];

  }

  os.close();

}


#endif
