#include <gkg-processing-io/VectorBinaryItemWriter.h>
#include <gkg-core-io/ItemWriter_i.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>


gkg::VectorBinaryItemWriter::VectorBinaryItemWriter()
                            : gkg::ItemWriter< gkg::Vector >()
{
}


gkg::VectorBinaryItemWriter::~VectorBinaryItemWriter()
{
}


void gkg::VectorBinaryItemWriter::write( std::ostream& os,
                                         const gkg::Vector* pItem,
                                         size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false,
                                                                    false );
  gkg::ItemWriter< double >& itemW2 =
    gkg::DefaultItemIOFactory< double >::getInstance().getWriter( false,
                                                                  false );

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


void gkg::VectorBinaryItemWriter::write( gkg::largefile_ofstream& os,
                                         const gkg::Vector* pItem,
                                         size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false,
                                                                    false );
  gkg::ItemWriter< double >& itemW2 =
    gkg::DefaultItemIOFactory< double >::getInstance().getWriter( false,
                                                                  false );

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


void gkg::VectorBinaryItemWriter::write( gkg::largefile_fstream& fs,
                                         const gkg::Vector* pItem,
                                         size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false,
                                                                    false );
  gkg::ItemWriter< double >& itemW2 =
    gkg::DefaultItemIOFactory< double >::getInstance().getWriter( false,
                                                                  false );

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
