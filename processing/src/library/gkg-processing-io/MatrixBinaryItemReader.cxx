#include <gkg-processing-io/MatrixBinaryItemReader.h>
#include <gkg-core-io/ItemReader_i.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>


gkg::MatrixBinaryItemReader::MatrixBinaryItemReader()
                           : gkg::ItemReader< gkg::Matrix >()
{
}


gkg::MatrixBinaryItemReader::~MatrixBinaryItemReader()
{
}


void gkg::MatrixBinaryItemReader::read( std::istream& is,
                                        gkg::Matrix* pItem,
                                        size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( false,
                                                                    false );
  gkg::ItemReader< double >& itemR2 =
    gkg::DefaultItemIOFactory< double >::getInstance().getReader( false,
                                                                  false );

  gkg::Matrix* pointer = pItem;
  uint32_t size1 = 0U;
  uint32_t size2 = 0U;
  int32_t k, l;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( is, size1 );
    itemR1.read( is, size2 );
    pointer->reallocate( size1, size2 );

    for ( k = 0; k < pointer->getSize1(); k++ )
    {

      for ( l = 0; l < pointer->getSize2(); l++ )
      {

        itemR2.read( is, ( *pointer )( k, l ) );

      }

    }

  }

}


void gkg::MatrixBinaryItemReader::read( gkg::largefile_ifstream& is,
                                        gkg::Matrix* pItem,
                                        size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( false,
                                                                    false );
  gkg::ItemReader< double >& itemR2 =
    gkg::DefaultItemIOFactory< double >::getInstance().getReader( false,
                                                                  false );

  gkg::Matrix* pointer = pItem;
  uint32_t size1 = 0U;
  uint32_t size2 = 0U;
  int32_t k, l;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( is, size1 );
    itemR1.read( is, size2 );
    pointer->reallocate( size1, size2 );

    for ( k = 0; k < pointer->getSize1(); k++ )
    {

      for ( l = 0; l < pointer->getSize2(); l++ )
      {

        itemR2.read( is, ( *pointer )( k, l ) );

      }

    }

  }

}


void gkg::MatrixBinaryItemReader::read( gkg::largefile_fstream& fs,
                                        gkg::Matrix* pItem,
                                        size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( false,
                                                                    false );
  gkg::ItemReader< double >& itemR2 =
    gkg::DefaultItemIOFactory< double >::getInstance().getReader( false,
                                                                  false );

  gkg::Matrix* pointer = pItem;
  uint32_t size1 = 0U;
  uint32_t size2 = 0U;
  int32_t k, l;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( fs, size1 );
    itemR1.read( fs, size2 );
    pointer->reallocate( size1, size2 );

    for ( k = 0; k < pointer->getSize1(); k++ )
    {

      for ( l = 0; l < pointer->getSize2(); l++ )
      {

        itemR2.read( fs, ( *pointer )( k, l ) );

      }

    }

  }

}
