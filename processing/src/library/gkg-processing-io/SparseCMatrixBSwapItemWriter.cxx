#include <gkg-processing-io/SparseCMatrixBSwapItemWriter.h>
#include <gkg-core-io/ItemWriter_i.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>


gkg::SparseCMatrixBSwapItemWriter::SparseCMatrixBSwapItemWriter()
                                  : gkg::ItemWriter< gkg::SparseCMatrix >()
{
}


gkg::SparseCMatrixBSwapItemWriter::~SparseCMatrixBSwapItemWriter()
{
}


void gkg::SparseCMatrixBSwapItemWriter::write( std::ostream& os,
                                               const gkg::SparseCMatrix* pItem,
                                               size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false,
                                                                    true );
  gkg::ItemWriter< std::complex< double > >& itemW2 =
    gkg::DefaultItemIOFactory<
               std::complex< double > >::getInstance().getWriter( false,
                                                                  true );

  const gkg::SparseCMatrix* pointer = pItem;  
  int32_t size1 = 0;
  int32_t size2 = 0;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    const std::map< size_t, std::complex< double > >& data = pointer->getData();
    size1 = pointer->getSize1();
    size2 = pointer->getSize2();

    itemW1.write( os, size1 );
    itemW1.write( os, size2 );
    itemW1.write( os, pointer->getNonZeroElementCount() );

    std::map< size_t, std::complex< double > >::const_iterator
      s = data.begin(),
      se = data.end();
    while ( s != se )
    {

      itemW1.write( os, ( uint32_t )( s->first / ( uint64_t )size2 ) );
      itemW1.write( os, ( uint32_t )( s->first % ( uint64_t )size2 ) );
      itemW2.write( os, s->second );
      ++ s;

    }

  }

}


void gkg::SparseCMatrixBSwapItemWriter::write( gkg::largefile_ofstream& os,
                                               const gkg::SparseCMatrix* pItem,
                                               size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false,
                                                                    true );
  gkg::ItemWriter< std::complex< double > >& itemW2 =
    gkg::DefaultItemIOFactory<
               std::complex< double > >::getInstance().getWriter( false,
                                                                  true );

  const gkg::SparseCMatrix* pointer = pItem;  
  int32_t size1 = 0;
  int32_t size2 = 0;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    const std::map< size_t, std::complex< double > >& data = pointer->getData();
    size1 = pointer->getSize1();
    size2 = pointer->getSize2();

    itemW1.write( os, size1 );
    itemW1.write( os, size2 );
    itemW1.write( os, pointer->getNonZeroElementCount() );

    std::map< size_t, std::complex< double > >::const_iterator
      s = data.begin(),
      se = data.end();
    while ( s != se )
    {

      itemW1.write( os, ( uint32_t )( s->first / ( uint64_t )size2 ) );
      itemW1.write( os, ( uint32_t )( s->first % ( uint64_t )size2 ) );
      itemW2.write( os, s->second );
      ++ s;

    }

  }

}


void gkg::SparseCMatrixBSwapItemWriter::write( gkg::largefile_fstream& fs,
                                               const gkg::SparseCMatrix* pItem,
                                               size_t count ) const
{


  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false,
                                                                    true );
  gkg::ItemWriter< std::complex< double > >& itemW2 =
    gkg::DefaultItemIOFactory< 
               std::complex< double > >::getInstance().getWriter( false,
                                                                  true );

  const gkg::SparseCMatrix* pointer = pItem;  
  int32_t size1 = 0;
  int32_t size2 = 0;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    const std::map< size_t, std::complex< double > >& data = pointer->getData();
    size1 = pointer->getSize1();
    size2 = pointer->getSize2();

    itemW1.write( fs, size1 );
    itemW1.write( fs, size2 );
    itemW1.write( fs, pointer->getNonZeroElementCount() );

    std::map< size_t, std::complex< double > >::const_iterator
      s = data.begin(),
      se = data.end();
    while ( s != se )
    {

      itemW1.write( fs, ( uint32_t )( s->first / ( uint64_t )size2 ) );
      itemW1.write( fs, ( uint32_t )( s->first % ( uint64_t )size2 ) );
      itemW2.write( fs, s->second );
      ++ s;

    }

  }

}
