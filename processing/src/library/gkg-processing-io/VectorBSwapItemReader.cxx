#include <gkg-processing-io/VectorBSwapItemReader.h>
#include <gkg-core-io/ItemReader_i.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>


gkg::VectorBSwapItemReader::VectorBSwapItemReader()
                           : gkg::ItemReader< gkg::Vector >()
{
}


gkg::VectorBSwapItemReader::~VectorBSwapItemReader()
{
}


void gkg::VectorBSwapItemReader::read( std::istream& is,
                                       gkg::Vector* pItem,
                                       size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( false, 
                                                                    true );
  gkg::ItemReader< double >& itemR2 =
    gkg::DefaultItemIOFactory< double >::getInstance().getReader( false,
                                                                  true );

  gkg::Vector* pointer = pItem;
  uint32_t size = 0U;
  int32_t k;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( is, size );
    pointer->reallocate( size );

    for ( k = 0; k < pointer->getSize(); k++ )
    {

      itemR2.read( is, ( *pointer )( k ) );

    }

  }

}


void gkg::VectorBSwapItemReader::read( gkg::largefile_ifstream& is,
                                       gkg::Vector* pItem,
                                       size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( false, 
                                                                    true );
  gkg::ItemReader< double >& itemR2 =
    gkg::DefaultItemIOFactory< double >::getInstance().getReader( false,
                                                                  true );

  gkg::Vector* pointer = pItem;
  uint32_t size = 0U;
  int32_t k;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( is, size );
    pointer->reallocate( size );

    for ( k = 0; k < pointer->getSize(); k++ )
    {

      itemR2.read( is, ( *pointer )( k ) );

    }

  }

}


void gkg::VectorBSwapItemReader::read( gkg::largefile_fstream& fs,
                                       gkg::Vector* pItem,
                                       size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( false, 
                                                                    true );
  gkg::ItemReader< double >& itemR2 =
    gkg::DefaultItemIOFactory< double >::getInstance().getReader( false,
                                                                  true );

  gkg::Vector* pointer = pItem;
  uint32_t size = 0U;
  int32_t k;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( fs, size );
    pointer->reallocate( size );

    for ( k = 0; k < pointer->getSize(); k++ )
    {

      itemR2.read( fs, ( *pointer )( k ) );

    }

  }

}
