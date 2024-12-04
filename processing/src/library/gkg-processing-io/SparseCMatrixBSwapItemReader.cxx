#include <gkg-processing-io/SparseCMatrixBSwapItemReader.h>
#include <gkg-core-io/ItemReader_i.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>


gkg::SparseCMatrixBSwapItemReader::SparseCMatrixBSwapItemReader()
                                 : gkg::ItemReader< gkg::SparseCMatrix >()
{
}


gkg::SparseCMatrixBSwapItemReader::~SparseCMatrixBSwapItemReader()
{
}


void gkg::SparseCMatrixBSwapItemReader::read( std::istream& is,
                                              gkg::SparseCMatrix* pItem,
                                              size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( false,
                                                                    true );
  gkg::ItemReader< std::complex< double > >& itemR2 =
    gkg::DefaultItemIOFactory< 
               std::complex< double > >::getInstance().getReader( false,
                                                                  true );

  gkg::SparseCMatrix* pointer = pItem;
  uint32_t size1 = 0U;
  uint32_t size2 = 0U;
  uint32_t nonZeroElementCount = 0U;
  uint32_t k, s1, s2;
  std::complex< double > value;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( is, size1 );
    itemR1.read( is, size2 );
    itemR1.read( is, nonZeroElementCount );
    pointer->reallocate( size1, size2 );

    for ( k = 0; k < nonZeroElementCount; k++ )
    {

      itemR1.read( is, s1 );
      itemR1.read( is, s2 );
      itemR2.read( is, value );
      ( *pointer )( ( int32_t )s1, ( int32_t )s2 ) = value;

    }

  }

}


void gkg::SparseCMatrixBSwapItemReader::read( gkg::largefile_ifstream& is,
                                              gkg::SparseCMatrix* pItem,
                                              size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( false,
                                                                    true );
  gkg::ItemReader< std::complex< double > >& itemR2 =
    gkg::DefaultItemIOFactory<
               std::complex< double > >::getInstance().getReader( false,
                                                                  true );

  gkg::SparseCMatrix* pointer = pItem;
  uint32_t size1 = 0U;
  uint32_t size2 = 0U;
  uint32_t nonZeroElementCount = 0U;
  uint32_t k, s1, s2;
  std::complex< double > value;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( is, size1 );
    itemR1.read( is, size2 );
    itemR1.read( is, nonZeroElementCount );
    pointer->reallocate( size1, size2 );

    for ( k = 0; k < nonZeroElementCount; k++ )
    {

      itemR1.read( is, s1 );
      itemR1.read( is, s2 );
      itemR2.read( is, value );
      ( *pointer )( ( int32_t )s1, ( int32_t )s2 ) = value;

    }

  }

}


void gkg::SparseCMatrixBSwapItemReader::read( gkg::largefile_fstream& fs,
                                              gkg::SparseCMatrix* pItem,
                                              size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( false,
                                                                    true );
  gkg::ItemReader< std::complex< double > >& itemR2 =
    gkg::DefaultItemIOFactory< 
               std::complex< double > >::getInstance().getReader( false,
                                                                  true );

  gkg::SparseCMatrix* pointer = pItem;
  uint32_t size1 = 0U;
  uint32_t size2 = 0U;
  uint32_t nonZeroElementCount = 0U;
  uint32_t k, s1, s2;
  std::complex< double > value;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( fs, size1 );
    itemR1.read( fs, size2 );
    itemR1.read( fs, nonZeroElementCount );
    pointer->reallocate( size1, size2 );

    for ( k = 0; k < nonZeroElementCount; k++ )
    {

      itemR1.read( fs, s1 );
      itemR1.read( fs, s2 );
      itemR2.read( fs, value );
      ( *pointer )( ( int32_t )s1, ( int32_t )s2 ) = value;

    }

  }

}
