#ifndef _gkg_core_io_StdVectorBinaryItemReader_i_h_
#define _gkg_core_io_StdVectorBinaryItemReader_i_h_


#include <gkg-core-io/StdVectorBinaryItemReader.h>
#include <gkg-core-io/ItemReader_i.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>


template < class T >
inline
gkg::StdVectorBinaryItemReader< T >::StdVectorBinaryItemReader()
                                 : gkg::ItemReader< std::vector< T > >()
{
}


template < class T >
inline
gkg::StdVectorBinaryItemReader< T >::~StdVectorBinaryItemReader()
{
}


template < class T >
inline
void gkg::StdVectorBinaryItemReader< T >::read( std::istream& is,
                                             std::vector< T >* pItem,
                                             size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( 
                                                           false, false );
  gkg::ItemReader< T >& itemR2 =
    gkg::DefaultItemIOFactory< T >::getInstance().getReader( false, false );

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
void gkg::StdVectorBinaryItemReader< T >::read( gkg::largefile_ifstream& is,
                                             std::vector< T >* pItem,
                                             size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( 
                                                           false, false );
  gkg::ItemReader< T >& itemR2 =
    gkg::DefaultItemIOFactory< T >::getInstance().getReader( false, false );

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
void gkg::StdVectorBinaryItemReader< T >::read( gkg::largefile_fstream& fs,
                                             std::vector< T >* pItem,
                                             size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( 
                                                           false, false );
  gkg::ItemReader< T >& itemR2 =
    gkg::DefaultItemIOFactory< T >::getInstance().getReader( false, false );

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
void gkg::StdVectorBinaryItemReader< bool >::read( std::istream& is,
                                                   std::vector< bool >* pItem,
                                                   size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( 
                                                           false, false );
  gkg::ItemReader< bool >& itemR2 =
    gkg::DefaultItemIOFactory< bool >::getInstance().getReader( false, false );

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
void gkg::StdVectorBinaryItemReader< bool >::read( gkg::largefile_ifstream& is,
                                                   std::vector< bool >* pItem,
                                                   size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( 
                                                           false, false );
  gkg::ItemReader< bool >& itemR2 =
    gkg::DefaultItemIOFactory< bool >::getInstance().getReader( false, false );

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
void gkg::StdVectorBinaryItemReader< bool >::read( gkg::largefile_fstream& fs,
                                                   std::vector< bool >* pItem,
                                                   size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( 
                                                           false, false );
  gkg::ItemReader< bool >& itemR2 =
    gkg::DefaultItemIOFactory< bool >::getInstance().getReader( false, false );

  std::vector< bool >* pointer = pItem;
  uint32_t size = 0U;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( fs, size );
    pointer->resize( size );
    
    std::vector< bool >::iterator s = pointer->begin();
    
    for ( uint32_t x = 0; x < size; x++ )
    {
    
      bool value;
      itemR2.read( fs, value );
      *s = value;
      ++ s;
      
    }

  }

}


}


#endif
