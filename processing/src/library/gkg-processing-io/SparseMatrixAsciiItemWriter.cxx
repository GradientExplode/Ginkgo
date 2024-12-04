#include <gkg-processing-io/SparseMatrixAsciiItemWriter.h>
#include <gkg-core-io/ItemWriter_i.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>


gkg::SparseMatrixAsciiItemWriter::SparseMatrixAsciiItemWriter()
                                 : gkg::ItemWriter< gkg::SparseMatrix >()
{
}


gkg::SparseMatrixAsciiItemWriter::~SparseMatrixAsciiItemWriter()
{
}


void gkg::SparseMatrixAsciiItemWriter::write( std::ostream& os,
                                        const gkg::SparseMatrix* pItem,
                                        size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( true,
                                                                    false );
  gkg::ItemWriter< double >& itemW2 =
    gkg::DefaultItemIOFactory< double >::getInstance().getWriter( true,
                                                                  false );

  const gkg::SparseMatrix* pointer = pItem;  
  int32_t size1 = 0;
  int32_t size2 = 0;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    const std::map< size_t, double >& data = pointer->getData();
    size1 = pointer->getSize1();
    size2 = pointer->getSize2();

    itemW1.write( os, size1 );
    itemW1.write( os, size2 );
    itemW1.write( os, pointer->getNonZeroElementCount() );

    std::map< size_t, double >::const_iterator
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


void gkg::SparseMatrixAsciiItemWriter::write( gkg::largefile_ofstream& os,
                                              const gkg::SparseMatrix* pItem,
                                              size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( true,
                                                                    false );
  gkg::ItemWriter< double >& itemW2 =
    gkg::DefaultItemIOFactory< double >::getInstance().getWriter( true,
                                                                  false );

  const gkg::SparseMatrix* pointer = pItem;  
  int32_t size1 = 0;
  int32_t size2 = 0;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    const std::map< size_t, double >& data = pointer->getData();
    size1 = pointer->getSize1();
    size2 = pointer->getSize2();

    itemW1.write( os, size1 );
    itemW1.write( os, size2 );
    itemW1.write( os, pointer->getNonZeroElementCount() );

    std::map< size_t, double >::const_iterator
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


void gkg::SparseMatrixAsciiItemWriter::write( gkg::largefile_fstream& fs,
                                              const gkg::SparseMatrix* pItem,
                                              size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( true,
                                                                    false );
  gkg::ItemWriter< double >& itemW2 =
    gkg::DefaultItemIOFactory< double >::getInstance().getWriter( true,
                                                                  false );

  const gkg::SparseMatrix* pointer = pItem;  
  int32_t size1 = 0;
  int32_t size2 = 0;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    const std::map< size_t, double >& data = pointer->getData();
    size1 = pointer->getSize1();
    size2 = pointer->getSize2();

    itemW1.write( fs, size1 );
    itemW1.write( fs, size2 );
    itemW1.write( fs, pointer->getNonZeroElementCount() );

    std::map< size_t, double >::const_iterator
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
