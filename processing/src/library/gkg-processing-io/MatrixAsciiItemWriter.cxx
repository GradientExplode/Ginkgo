#include <gkg-processing-io/MatrixAsciiItemWriter.h>
#include <gkg-core-io/ItemWriter_i.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>


gkg::MatrixAsciiItemWriter::MatrixAsciiItemWriter()
                           : gkg::ItemWriter< gkg::Matrix >()
{
}


gkg::MatrixAsciiItemWriter::~MatrixAsciiItemWriter()
{
}


void gkg::MatrixAsciiItemWriter::write( std::ostream& os,
                                        const gkg::Matrix* pItem,
                                        size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( true,
                                                                    false );
  gkg::ItemWriter< double >& itemW2 =
    gkg::DefaultItemIOFactory< double >::getInstance().getWriter( true,
                                                                  false );

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


void gkg::MatrixAsciiItemWriter::write( gkg::largefile_ofstream& os,
                                        const gkg::Matrix* pItem,
                                        size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( true,
                                                                    false );
  gkg::ItemWriter< double >& itemW2 =
    gkg::DefaultItemIOFactory< double >::getInstance().getWriter( true,
                                                                  false );

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


void gkg::MatrixAsciiItemWriter::write( gkg::largefile_fstream& fs,
                                        const gkg::Matrix* pItem,
                                        size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( true,
                                                                    false );
  gkg::ItemWriter< double >& itemW2 =
    gkg::DefaultItemIOFactory< double >::getInstance().getWriter( true,
                                                                  false );

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
