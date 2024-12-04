#ifndef _gkg_core_io_StdVectorAsciiItemWriter_i_h_
#define _gkg_core_io_StdVectorAsciiItemWriter_i_h_


#include <gkg-core-io/StdVectorAsciiItemWriter.h>
#include <gkg-core-io/ItemWriter_i.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>


template < class T >
inline
gkg::StdVectorAsciiItemWriter< T >::StdVectorAsciiItemWriter()
                                : gkg::ItemWriter< std::vector< T > >()
{
}


template < class T >
inline
gkg::StdVectorAsciiItemWriter< T >::~StdVectorAsciiItemWriter()
{
}


template < class T >
inline
void gkg::StdVectorAsciiItemWriter< T >::write( std::ostream& os,
                                              const std::vector< T >* pItem,
                                              size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( 
                                                           true, false );
  gkg::ItemWriter< T >& itemW2 =
    gkg::DefaultItemIOFactory< T >::getInstance().getWriter( true, false );

  const std::vector< T >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    itemW2.write( os, &pointer->front(), pointer->size() );

  }

}


template < class T >
inline
void gkg::StdVectorAsciiItemWriter< T >::write( gkg::largefile_ofstream& os,
                                             const std::vector< T >* pItem,
                                             size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( 
                                                           true, false );
  gkg::ItemWriter< T >& itemW2 =
    gkg::DefaultItemIOFactory< T >::getInstance().getWriter( true, false );

  const std::vector< T >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    itemW2.write( os, &pointer->front(), pointer->size() );

  }

}


template < class T >
inline
void gkg::StdVectorAsciiItemWriter< T >::write( gkg::largefile_fstream& fs,
                                             const std::vector< T >* pItem,
                                             size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( 
                                                           true, false );
  gkg::ItemWriter< T >& itemW2 =
    gkg::DefaultItemIOFactory< T >::getInstance().getWriter( true, false );

  const std::vector< T >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( fs, pointer->size() );
    itemW2.write( fs, &pointer->front(), pointer->size() );

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
void gkg::StdVectorAsciiItemWriter< bool >::write( 
                                               std::ostream& os,
                                               const std::vector< bool >* pItem,
                                               size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( 
                                                           true, false );
  gkg::ItemWriter< bool >& itemW2 =
    gkg::DefaultItemIOFactory< bool >::getInstance().getWriter( true, false );

  const std::vector< bool >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    
    std::vector< bool >::const_iterator s = pointer->begin();
    
    for ( uint32_t x = 0; x < pointer->size(); x++ )
    {
    
      bool value = *s;
      itemW2.write( os, value );
      ++ s;
      
    }

  }

}


template <>
inline
void gkg::StdVectorAsciiItemWriter< bool >::write( 
                                               gkg::largefile_ofstream& os,
                                               const std::vector< bool >* pItem,
                                               size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( 
                                                           true, false );
  gkg::ItemWriter< bool >& itemW2 =
    gkg::DefaultItemIOFactory< bool >::getInstance().getWriter( true, false );

  const std::vector< bool >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    
    std::vector< bool >::const_iterator s = pointer->begin();
    
    for ( uint32_t x = 0; x < pointer->size(); x++ )
    {
    
      bool value = *s;
      itemW2.write( os, value );
      ++ s;
      
    }

  }

}


template <>
inline
void gkg::StdVectorAsciiItemWriter< bool >::write( 
                                               gkg::largefile_fstream& fs,
                                               const std::vector< bool >* pItem,
                                               size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( 
                                                           true, false );
  gkg::ItemWriter< bool >& itemW2 =
    gkg::DefaultItemIOFactory< bool >::getInstance().getWriter( true, false );

  const std::vector< bool >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( fs, pointer->size() );
    
    std::vector< bool >::const_iterator s = pointer->begin();
    
    for ( uint32_t x = 0; x < pointer->size(); x++ )
    {
    
      bool value = *s;
      itemW2.write( fs, value );
      ++ s;
      
    }

  }

}


}


#endif
