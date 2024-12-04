#ifndef _gkg_core_io_StdVectorAsciiItemReader_i_h_
#define _gkg_core_io_StdVectorAsciiItemReader_i_h_


#include <gkg-core-io/StdVectorAsciiItemReader.h>
#include <gkg-core-io/ItemReader_i.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>


template < class T >
inline
gkg::StdVectorAsciiItemReader< T >::StdVectorAsciiItemReader()
                                 : gkg::ItemReader< std::vector< T > >()
{
}


template < class T >
inline
gkg::StdVectorAsciiItemReader< T >::~StdVectorAsciiItemReader()
{
}


template < class T >
inline
void gkg::StdVectorAsciiItemReader< T >::read( std::istream& is,
                                            std::vector< T >* pItem,
                                            size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( 
                                                           true, false );
  gkg::ItemReader< T >& itemR2 =
    gkg::DefaultItemIOFactory< T >::getInstance().getReader( true, false );

  std::vector< T >* pointer = pItem;
  uint32_t size = 0U;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( is, size );
    pointer->resize( size );
    itemR2.read( is, &pointer->front(), pointer->size() );

  }

}


template < class T >
inline
void gkg::StdVectorAsciiItemReader< T >::read( gkg::largefile_ifstream& is,
                                            std::vector< T >* pItem,
                                            size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( 
                                                           true, false );
  gkg::ItemReader< T >& itemR2 =
    gkg::DefaultItemIOFactory< T >::getInstance().getReader( true, false );

  std::vector< T >* pointer = pItem;
  uint32_t size = 0U;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( is, size );
    pointer->resize( size );
    itemR2.read( is, &pointer->front(), pointer->size() );

  }

}


template < class T >
inline
void gkg::StdVectorAsciiItemReader< T >::read( gkg::largefile_fstream& fs,
                                            std::vector< T >* pItem,
                                            size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( 
                                                           true, false );
  gkg::ItemReader< T >& itemR2 =
    gkg::DefaultItemIOFactory< T >::getInstance().getReader( true, false );

  std::vector< T >* pointer = pItem;
  uint32_t size = 0U;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( fs, size );
    pointer->resize( size );
    itemR2.read( fs, &pointer->front(), pointer->size() );

  }

}


// specializations


// boolean are managed as single bit, so we must get or put boolean items once 
// a time and we cannot use a pointer to boolean items
// boolean are managed as single bit, so we must write boolean items once a time
// and we cannot use a pointer to boolean items

namespace gkg
{


template <>
inline
void gkg::StdVectorAsciiItemReader< bool >::read( std::istream& is,
                                                  std::vector< bool >* pItem,
                                                  size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( 
                                                           true, false );
  gkg::ItemReader< bool >& itemR2 =
    gkg::DefaultItemIOFactory< bool >::getInstance().getReader( true, false );

  std::vector< bool >* pointer = pItem;
  uint32_t size = 0U;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( is, size );
    pointer->resize( size );
    
    std::vector< bool >::iterator s = pointer->begin();
    
    for ( uint32_t x = 0; x < size; x++ )
    {
    
      bool value;
      itemR2.read( is, value );
      *s = value;
      ++ s;
      
    }

  }

}


template <>
inline
void gkg::StdVectorAsciiItemReader< bool >::read( gkg::largefile_ifstream& is,
                                                  std::vector< bool >* pItem,
                                                  size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( 
                                                           true, false );
  gkg::ItemReader< bool >& itemR2 =
    gkg::DefaultItemIOFactory< bool >::getInstance().getReader( true, false );

  std::vector< bool >* pointer = pItem;
  uint32_t size = 0U;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( is, size );
    pointer->resize( size );
    
    std::vector< bool >::iterator s = pointer->begin();
    
    for ( uint32_t x = 0; x < size; x++ )
    {
    
      bool value = false;
      itemR2.read( is, value );
      *s = value;
      ++ s;
      
    }

  }

}


template <>
inline
void gkg::StdVectorAsciiItemReader< bool >::read( gkg::largefile_fstream& fs,
                                                  std::vector< bool >* pItem,
                                                  size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( 
                                                           true, false );
  gkg::ItemReader< bool >& itemR2 =
    gkg::DefaultItemIOFactory< bool >::getInstance().getReader( true, false );

  std::vector< bool >* pointer = pItem;
  uint32_t size = 0U;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( fs, size );
    pointer->resize( size );
    
    std::vector< bool >::iterator s = pointer->begin();
    
    for ( uint32_t x = 0; x < size; x++ )
    {
    
      bool value = false;
      itemR2.read( fs, value );
      *s = value;
      ++ s;
      
    }

  }

}


}


#endif
