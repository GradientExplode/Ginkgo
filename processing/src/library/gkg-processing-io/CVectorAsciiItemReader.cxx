#include <gkg-processing-io/CVectorAsciiItemReader.h>
#include <gkg-core-io/ItemReader_i.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>


gkg::CVectorAsciiItemReader::CVectorAsciiItemReader()
                            : gkg::ItemReader< gkg::CVector >()
{
}


gkg::CVectorAsciiItemReader::~CVectorAsciiItemReader()
{
}


void gkg::CVectorAsciiItemReader::read( std::istream& is,
                                        gkg::CVector* pItem,
                                        size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( true,
                                                                    false );
  gkg::ItemReader< std::complex< double > >& itemR2 =
    gkg::DefaultItemIOFactory< std::complex< double > >::getInstance().
                                                        getReader( true,
                                                                   false );

  gkg::CVector* pointer = pItem;
  uint32_t size = 0U;
  int32_t k;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( is, size );
    pointer->reallocate( size );

    for ( k = 0; k < pointer->getSize(); k++ )
    {

      itemR2.read( is, ( *pointer )( k ) );

    }

  }

}


void gkg::CVectorAsciiItemReader::read( gkg::largefile_ifstream& is,
                                        gkg::CVector* pItem,
                                        size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( true,
                                                                    false );
  gkg::ItemReader< std::complex< double > >& itemR2 =
    gkg::DefaultItemIOFactory< std::complex< double > >::getInstance().
                                                        getReader( true,
                                                                   false );

  gkg::CVector* pointer = pItem;
  uint32_t size = 0U;
  int32_t k;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( is, size );
    pointer->reallocate( size );

    for ( k = 0; k < pointer->getSize(); k++ )
    {

      itemR2.read( is, ( *pointer )( k ) );

    }

  }

}


void gkg::CVectorAsciiItemReader::read( gkg::largefile_fstream& fs,
                                        gkg::CVector* pItem,
                                        size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( true,
                                                                    false );
  gkg::ItemReader< std::complex< double > >& itemR2 =
    gkg::DefaultItemIOFactory< std::complex< double > >::getInstance().
                                                        getReader( true,
                                                                   false );

  gkg::CVector* pointer = pItem;
  uint32_t size = 0U;
  int32_t k;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( fs, size );
    pointer->reallocate( size );

    for ( k = 0; k < pointer->getSize(); k++ )
    {

      itemR2.read( fs, ( *pointer )( k ) );

    }

  }

}
