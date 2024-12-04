#include <gkg-processing-io/VectorBSwapItemWriter.h>
#include <gkg-core-io/ItemWriter_i.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>


gkg::VectorBSwapItemWriter::VectorBSwapItemWriter()
                           : gkg::ItemWriter< gkg::Vector >()
{
}


gkg::VectorBSwapItemWriter::~VectorBSwapItemWriter()
{
}


void gkg::VectorBSwapItemWriter::write( std::ostream& os,
                                        const gkg::Vector* pItem,
                                        size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, 
                                                                    true );
  gkg::ItemWriter< double >& itemW2 =
    gkg::DefaultItemIOFactory< double >::getInstance().getWriter( false,
                                                                  true );

  const gkg::Vector* pointer = pItem;
  int32_t k;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->getSize() );
    for ( k = 0; k < pointer->getSize(); k++ )
    {

      itemW2.write( os, ( *pointer )( k ) );

    }

  }

}


void gkg::VectorBSwapItemWriter::write( gkg::largefile_ofstream& os,
                                        const gkg::Vector* pItem,
                                        size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, 
                                                                    true );
  gkg::ItemWriter< double >& itemW2 =
    gkg::DefaultItemIOFactory< double >::getInstance().getWriter( false,
                                                                  true );

  const gkg::Vector* pointer = pItem;
  int32_t k;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->getSize() );
    for ( k = 0; k < pointer->getSize(); k++ )
    {

      itemW2.write( os, ( *pointer )( k ) );

    }

  }

}


void gkg::VectorBSwapItemWriter::write( gkg::largefile_fstream& fs,
                                        const gkg::Vector* pItem,
                                        size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, 
                                                                    true );
  gkg::ItemWriter< double >& itemW2 =
    gkg::DefaultItemIOFactory< double >::getInstance().getWriter( false,
                                                                  true );

  const gkg::Vector* pointer = pItem;
  int32_t k;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( fs, pointer->getSize() );
    for ( k = 0; k < pointer->getSize(); k++ )
    {

      itemW2.write( fs, ( *pointer )( k ) );

    }

  }

}
