#include <gkg-processing-io/CVectorBinaryItemWriter.h>
#include <gkg-core-io/ItemWriter_i.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>


gkg::CVectorBinaryItemWriter::CVectorBinaryItemWriter()
                            : gkg::ItemWriter< gkg::CVector >()
{
}


gkg::CVectorBinaryItemWriter::~CVectorBinaryItemWriter()
{
}


void gkg::CVectorBinaryItemWriter::write( std::ostream& os,
                                         const gkg::CVector* pItem,
                                         size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false,
                                                                    false );
  gkg::ItemWriter< std::complex< double > >& itemW2 =
    gkg::DefaultItemIOFactory< std::complex< double > >::getInstance().
                                                        getWriter( false,
                                                                   false );

  const gkg::CVector* pointer = pItem;
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


void gkg::CVectorBinaryItemWriter::write( gkg::largefile_ofstream& os,
                                         const gkg::CVector* pItem,
                                         size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false,
                                                                    false );
  gkg::ItemWriter< std::complex< double > >& itemW2 =
    gkg::DefaultItemIOFactory< std::complex< double > >::getInstance().
                                                        getWriter( false,
                                                                   false );

  const gkg::CVector* pointer = pItem;
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


void gkg::CVectorBinaryItemWriter::write( gkg::largefile_fstream& fs,
                                         const gkg::CVector* pItem,
                                         size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false,
                                                                    false );
  gkg::ItemWriter< std::complex< double > >& itemW2 =
    gkg::DefaultItemIOFactory< std::complex< double > >::getInstance().
                                                        getWriter( false,
                                                                   false );

  const gkg::CVector* pointer = pItem;
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
