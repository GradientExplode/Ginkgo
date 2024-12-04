#include <gkg-processing-io/CMatrixBSwapItemWriter.h>
#include <gkg-core-io/ItemWriter_i.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>


gkg::CMatrixBSwapItemWriter::CMatrixBSwapItemWriter()
                            : gkg::ItemWriter< gkg::CMatrix >()
{
}


gkg::CMatrixBSwapItemWriter::~CMatrixBSwapItemWriter()
{
}


void gkg::CMatrixBSwapItemWriter::write( std::ostream& os,
                                         const gkg::CMatrix* pItem,
                                         size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false,
                                                                    true );
  gkg::ItemWriter< std::complex< double > >& itemW2 =
    gkg::DefaultItemIOFactory< std::complex< double > >::getInstance().
                                                        getWriter( false,
                                                                   true );

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


void gkg::CMatrixBSwapItemWriter::write( gkg::largefile_ofstream& os,
                                         const gkg::CMatrix* pItem,
                                         size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, 
                                                                    true );
  gkg::ItemWriter< std::complex< double > >& itemW2 =
    gkg::DefaultItemIOFactory< std::complex< double > >::getInstance().
                                                        getWriter( false,
                                                                   true );

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


void gkg::CMatrixBSwapItemWriter::write( gkg::largefile_fstream& fs,
                                         const gkg::CMatrix* pItem,
                                         size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false,
                                                                    true );
  gkg::ItemWriter< std::complex< double > >& itemW2 =
    gkg::DefaultItemIOFactory< std::complex< double > >::getInstance().
                                                        getWriter( false,
                                                                   true );

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
