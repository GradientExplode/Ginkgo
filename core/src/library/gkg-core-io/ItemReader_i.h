#ifndef _gkg_core_io_ItemReader_i_h_
#define _gkg_core_io_ItemReader_i_h_


#include <gkg-core-io/ItemReader.h>
#include <gkg-core-cppext/LargeFileStream_i.h>
#include <gkg-core-cppext/StringStream.h>


template < class T >
inline
gkg::ItemReader< T >::ItemReader()
{
}


template < class T >
inline
gkg::ItemReader< T >::~ItemReader()
{
}


template < class T >
inline
void gkg::ItemReader< T >::read( std::istream& is, T& item ) const
{

  read( is, &item, 1U );

}


template < class T >
inline
void gkg::ItemReader< T >::read( gkg::largefile_ifstream& is, T& item ) const
{

  read( is, &item, 1U );

}


template < class T >
inline
void gkg::ItemReader< T >::read( gkg::largefile_fstream& fs, T& item ) const
{

  read( fs, &item, 1U );

}


template < class T >
inline
void gkg::ItemReader< T >::read( const std::string& s, T& item ) const
{

  read( s, &item, 1U );

}


template < class T >
inline
void 
gkg::ItemReader< T >::read( const std::string& s, T* pItem, size_t count ) const
{

  std::istringstream is( s.c_str() );
  read( is, pItem, count );

}


#endif
