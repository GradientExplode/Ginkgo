#ifndef _gkg_core_io_DefaultAsciiItemReader_i_h_
#define _gkg_core_io_DefaultAsciiItemReader_i_h_


#include <gkg-core-io/DefaultAsciiItemReader.h>
#include <gkg-core-io/ItemReader_i.h>
#include <string>


//
// default methods
//

template < class T >
inline
gkg::DefaultAsciiItemReader< T >::DefaultAsciiItemReader()
                                 : gkg::ItemReader< T >()
{
}


template < class T >
inline
gkg::DefaultAsciiItemReader< T >::~DefaultAsciiItemReader()
{
}


template < class T >
inline
void gkg::DefaultAsciiItemReader< T >::read( std::istream & is,
                                             T* pItem,
                                             size_t count ) const
{

  T* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    is >> *pointer;

  }

}


template < class T >
inline
void gkg::DefaultAsciiItemReader< T >::read( gkg::largefile_ifstream & is,
                                             T* pItem,
                                             size_t count ) const
{

  T* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    is >> *pointer;

  }

}


template < class T >
inline
void gkg::DefaultAsciiItemReader< T >::read( gkg::largefile_fstream & fs,
                                             T* pItem,
                                             size_t count ) const
{

  T* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    fs >> *pointer;

  }

}


// specialization(s)

namespace gkg
{


template <>
inline
void DefaultAsciiItemReader< std::string >::read( std::istream & is,
                                                  std::string* pItem,
                                                  size_t count ) const
{

  std::string* pointer = pItem;
  char character = '\0';
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    while ( character != '\'' )
    {

      is >> character;

    }
    character = '\0';
    do
    {

      is >> character;
      if ( character != '\'' )
      {

        *pointer += character;

      }

    }
    while ( character != '\'' );

    character = '\0';

  }

}


template <>
inline
void DefaultAsciiItemReader< std::string >::read( largefile_ifstream & is,
                                                  std::string* pItem,
                                                  size_t count ) const
{

  std::string* pointer = pItem;
  char character = '\0';
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    while ( character != '\'' )
    {

      is >> character;

    }
    character = '\0';
    do
    {

      is >> character;
      if ( character != '\'' )
      {

        *pointer += character;

      }

    }
    while ( character != '\'' );

    character = '\0';

  }

}


template <>
inline
void DefaultAsciiItemReader< std::string >::read( largefile_fstream & fs,
                                                  std::string* pItem,
                                                  size_t count ) const
{

  std::string* pointer = pItem;
  char character = '\0';
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    while ( character != '\'' )
    {

      fs >> character;

    }
    character = '\0';
    do
    {

      fs >> character;
      if ( character != '\'' )
      {

        *pointer += character;

      }

    }
    while ( character != '\'' );

    character = '\0';

  }

}


}


#endif
