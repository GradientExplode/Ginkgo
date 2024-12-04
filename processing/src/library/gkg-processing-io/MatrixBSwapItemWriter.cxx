#include <gkg-processing-io/MatrixBSwapItemWriter.h>
#include <gkg-core-io/ItemWriter_i.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>


gkg::MatrixBSwapItemWriter::MatrixBSwapItemWriter()
                           : gkg::ItemWriter< gkg::Matrix >()
{
}


gkg::MatrixBSwapItemWriter::~MatrixBSwapItemWriter()
{
}


void gkg::MatrixBSwapItemWriter::write( std::ostream& os,
                                        const gkg::Matrix* pItem,
                                        size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false,
                                                                    true );
  gkg::ItemWriter< double >& itemW2 =
    gkg::DefaultItemIOFactory< double >::getInstance().getWriter( false,
                                                                  true );

  const gkg::Matrix* pointer = pItem;
  int32_t k, l;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->getSize1() );
    itemW1.write( os, pointer->getSize2() );
    for ( k = 0; k < pointer->getSize1(); k++ )
    {

      for ( l = 0; l < pointer->getSize2(); l++ )
      {

        itemW2.write( os, ( *pointer )( k, l ) );

      }

    }

  }

}


void gkg::MatrixBSwapItemWriter::write( gkg::largefile_ofstream& os,
                                        const gkg::Matrix* pItem,
                                        size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, 
                                                                    true );
  gkg::ItemWriter< double >& itemW2 =
    gkg::DefaultItemIOFactory< double >::getInstance().getWriter( false,
                                                                  true );

  const gkg::Matrix* pointer = pItem;
  int32_t k, l;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->getSize1() );
    itemW1.write( os, pointer->getSize2() );
    for ( k = 0; k < pointer->getSize1(); k++ )
    {

      for ( l = 0; l < pointer->getSize2(); l++ )
      {

        itemW2.write( os, ( *pointer )( k, l ) );

      }

    }

  }

}


void gkg::MatrixBSwapItemWriter::write( gkg::largefile_fstream& fs,
                                        const gkg::Matrix* pItem,
                                        size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false,
                                                                    true );
  gkg::ItemWriter< double >& itemW2 =
    gkg::DefaultItemIOFactory< double >::getInstance().getWriter( false,
                                                                  true );

  const gkg::Matrix* pointer = pItem;
  int32_t k, l;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( fs, pointer->getSize1() );
    itemW1.write( fs, pointer->getSize2() );
    for ( k = 0; k < pointer->getSize1(); k++ )
    {

      for ( l = 0; l < pointer->getSize2(); l++ )
      {

        itemW2.write( fs, ( *pointer )( k, l ) );

      }

    }

  }

}
