#ifndef _gkg_core_io_ItemWriter_i_h_
#define _gkg_core_io_ItemWriter_i_h_


#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-cppext/LargeFileStream_i.h>


template < class T >
inline
gkg::ItemWriter< T >::ItemWriter()
{
}


template < class T >
inline
gkg::ItemWriter< T >::~ItemWriter()
{
}


template < class T >
inline
void gkg::ItemWriter< T >::write( std::ostream& os, const T& item ) const
{

  write( os, &item, 1U );

}


template < class T >
inline
void gkg::ItemWriter< T >::write( gkg::largefile_ofstream& os,
                                  const T& item ) const
{

  write( os, &item, 1U );

}


template < class T >
inline
void gkg::ItemWriter< T >::write( gkg::largefile_fstream& fs,
                                  const T& item ) const
{

  write( fs, &item, 1U );

}


#endif
