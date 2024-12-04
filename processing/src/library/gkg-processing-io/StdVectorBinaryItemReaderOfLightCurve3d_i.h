#ifndef _gkg_processing_io_StdVectorBinaryItemReaderOfLightCurve3d_i_h_
#define _gkg_processing_io_StdVectorBinaryItemReaderOfLightCurve3d_i_h_


#include <gkg-core-io/StdVectorBinaryItemReader.h>
#include <gkg-core-io/ItemReader_i.h>
#include <gkg-core-io/DefaultItemIOFactory.h>
#include <gkg-processing-container/LightCurve3d.h>


//
// class StdVectorBinaryItemReader< LightCurve3d< float > >
//

namespace gkg
{


template <>
StdVectorBinaryItemReader< LightCurve3d< float > >::StdVectorBinaryItemReader()
                          : ItemReader< std::vector< LightCurve3d< float > > >()
{
}


template <>
StdVectorBinaryItemReader< LightCurve3d< float > >::~StdVectorBinaryItemReader()
{
}


template <>
void StdVectorBinaryItemReader< LightCurve3d< float > >::read(
                                    std::istream& is,
                                    std::vector< LightCurve3d< float > >* pItem,
                                    size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( 
                                                           false, false );
  gkg::ItemReader< LightCurve3d< float > >& itemR2 =
    gkg::DefaultItemIOFactory< LightCurve3d< float > >::getInstance().getReader(
                                                                 false, false );

  std::vector< LightCurve3d< float > >* pointer = pItem;
  uint32_t size = 0U;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( is, size );
    pointer->resize( size );
    itemR2.read( is, &pointer->front(), pointer->size() );

  }


}


template <>
void StdVectorBinaryItemReader< LightCurve3d< float > >::read(
                                    largefile_ifstream& is,
                                    std::vector< LightCurve3d< float > >* pItem,
                                    size_t count ) const
{

  ItemReader< uint32_t >& itemR1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getReader( false, false );
  ItemReader< LightCurve3d< float > >& itemR2 =
    DefaultItemIOFactory< LightCurve3d< float > >::getInstance().getReader(
                                                                 false, false );

  std::vector< LightCurve3d< float > >* pointer = pItem;
  uint32_t size = 0U;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( is, size );
    pointer->resize( size );
    itemR2.read( is, &pointer->front(), pointer->size() );

  }

}


template <>
void StdVectorBinaryItemReader< LightCurve3d< float > >::read( 
                                    largefile_fstream& fs,
                                    std::vector< LightCurve3d< float > >* pItem,
                                    size_t count ) const
{

  ItemReader< uint32_t >& itemR1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getReader( false, false );
  ItemReader< LightCurve3d< float > >& itemR2 =
    DefaultItemIOFactory< LightCurve3d< float > >::getInstance().getReader(
                                                                 false, false );

  std::vector< LightCurve3d< float > >* pointer = pItem;
  uint32_t size = 0U;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( fs, size );
    pointer->resize( size );
    itemR2.read( fs, &pointer->front(), pointer->size() );

  }

}


}


//
// class StdVectorBinaryItemReader< LightCurve3d< double > >
//

namespace gkg
{


template <>
StdVectorBinaryItemReader< LightCurve3d< double > >::StdVectorBinaryItemReader()
                         : ItemReader< std::vector< LightCurve3d< double > > >()
{
}


template <>
StdVectorBinaryItemReader< LightCurve3d< double > >::
                                                    ~StdVectorBinaryItemReader()
{
}


template <>
void StdVectorBinaryItemReader< LightCurve3d< double > >::read(
                                   std::istream& is,
                                   std::vector< LightCurve3d< double > >* pItem,
                                   size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( 
                                                           false, false );
  gkg::ItemReader< LightCurve3d< double > >& itemR2 =
    gkg::DefaultItemIOFactory< LightCurve3d< double > >::getInstance().
                                                      getReader( false, false );

  std::vector< LightCurve3d< double > >* pointer = pItem;
  uint32_t size = 0U;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( is, size );
    pointer->resize( size );
    itemR2.read( is, &pointer->front(), pointer->size() );

  }


}


template <>
void StdVectorBinaryItemReader< LightCurve3d< double > >::read(
                                   largefile_ifstream& is,
                                   std::vector< LightCurve3d< double > >* pItem,
                                   size_t count ) const
{

  ItemReader< uint32_t >& itemR1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getReader( false, false );
  ItemReader< LightCurve3d< double > >& itemR2 =
    DefaultItemIOFactory< LightCurve3d< double > >::getInstance().getReader(
                                                                 false, false );

  std::vector< LightCurve3d< double > >* pointer = pItem;
  uint32_t size = 0U;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( is, size );
    pointer->resize( size );
    itemR2.read( is, &pointer->front(), pointer->size() );

  }

}


template <>
void StdVectorBinaryItemReader< LightCurve3d< double > >::read( 
                                   largefile_fstream& fs,
                                   std::vector< LightCurve3d< double > >* pItem,
                                   size_t count ) const
{

  ItemReader< uint32_t >& itemR1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getReader( false, false );
  ItemReader< LightCurve3d< double > >& itemR2 =
    DefaultItemIOFactory< LightCurve3d< double > >::getInstance().getReader(
                                                                 false, false );

  std::vector< LightCurve3d< double > >* pointer = pItem;
  uint32_t size = 0U;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( fs, size );
    pointer->resize( size );
    itemR2.read( fs, &pointer->front(), pointer->size() );

  }

}


}


#endif
