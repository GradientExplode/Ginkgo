#ifndef _gkg_core_io_DefaultAsciiItemWriter_i_h_
#define _gkg_core_io_DefaultAsciiItemWriter_i_h_


#include <gkg-core-io/DefaultAsciiItemWriter.h>
#include <gkg-core-io/ItemWriter_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <string>


//
// default methods
//

template < class T >
inline
gkg::DefaultAsciiItemWriter< T >::DefaultAsciiItemWriter()
                                 : gkg::ItemWriter< T >()
{
}


template < class T >
inline
gkg::DefaultAsciiItemWriter< T >::~DefaultAsciiItemWriter()
{
}


template < class T >
inline
void gkg::DefaultAsciiItemWriter< T >::write( std::ostream & os,
                                              const T* pItem,
                                              size_t count ) const
{

  const T* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    os << *pointer << " ";

  }

}


template < class T >
inline
void gkg::DefaultAsciiItemWriter< T >::write( gkg::largefile_ofstream & os,
                                              const T* pItem,
                                              size_t count ) const
{

  const T* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    os << *pointer << " ";

  }

}


template < class T >
inline
void gkg::DefaultAsciiItemWriter< T >::write( gkg::largefile_fstream & fs,
                                              const T* pItem,
                                              size_t count ) const
{

  const T* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    fs << *pointer << " ";

  }

}



// specialization(s)

namespace gkg
{


template <>
inline
void DefaultAsciiItemWriter< std::string >::write( std::ostream & os,
                                                   const std::string* pItem,
                                                   size_t count ) const
{

  const std::string* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    os << "'" << *pointer << "' ";

  }

}


template <>
inline
void DefaultAsciiItemWriter< std::string >::write( largefile_ofstream & os,
                                                   const std::string* pItem,
                                                   size_t count ) const
{

  const std::string* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    os << "'" << *pointer << "' ";

  }

}


template <>
inline
void DefaultAsciiItemWriter< std::string >::write( largefile_fstream & fs,
                                                   const std::string* pItem,
                                                   size_t count ) const
{

  const std::string* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    fs << "'" << *pointer << "' ";

  }

}


}


#endif
