#include <gkg-processing-io/CMatrixAsciiItemWriter.h>
#include <gkg-core-io/ItemWriter_i.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>


gkg::CMatrixAsciiItemWriter::CMatrixAsciiItemWriter()
                            : gkg::ItemWriter< gkg::CMatrix >()
{
}


gkg::CMatrixAsciiItemWriter::~CMatrixAsciiItemWriter()
{
}


void gkg::CMatrixAsciiItemWriter::write( std::ostream& os,
                                         const gkg::CMatrix* pItem,
                                         size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( true,
                                                                    false );
  gkg::ItemWriter< std::complex< double > >& itemW2 =
    gkg::DefaultItemIOFactory< std::complex< double > >::getInstance().
                                                        getWriter( true,
                                                                   false );

  const gkg::CMatrix* pointer = pItem;
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


void gkg::CMatrixAsciiItemWriter::write( gkg::largefile_ofstream& os,
                                         const gkg::CMatrix* pItem,
                                         size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( true,
                                                                    false );
  gkg::ItemWriter< std::complex< double > >& itemW2 =
    gkg::DefaultItemIOFactory< std::complex< double > >::getInstance().
                                                        getWriter( true,
                                                                   false );

  const gkg::CMatrix* pointer = pItem;
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


void gkg::CMatrixAsciiItemWriter::write( gkg::largefile_fstream& fs,
                                         const gkg::CMatrix* pItem,
                                         size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( true,
                                                                    false );
  gkg::ItemWriter< std::complex< double > >& itemW2 =
    gkg::DefaultItemIOFactory< std::complex< double > >::getInstance().
                                                        getWriter( true,
                                                                   false );

  const gkg::CMatrix* pointer = pItem;
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
