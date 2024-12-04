#ifndef _gkg_processing_io_StdVectorBSwapItemReaderOfCurve3d_i_h_
#define _gkg_processing_io_StdVectorBSwapItemReaderOfCurve3d_i_h_


#include <gkg-core-io/StdVectorBSwapItemReader.h>
#include <gkg-core-io/ItemReader_i.h>
#include <gkg-core-io/DefaultItemIOFactory.h>
#include <gkg-processing-container/Curve3d.h>


//
// class StdVectorBSwapItemReader< Curve3d< float > >
//

namespace gkg
{


template <>
StdVectorBSwapItemReader< Curve3d< float > >::StdVectorBSwapItemReader()
                               : ItemReader< std::vector< Curve3d< float > > >()
{
}


template <>
StdVectorBSwapItemReader< Curve3d< float > >::~StdVectorBSwapItemReader()
{
}


template <>
void StdVectorBSwapItemReader< Curve3d< float > >::read(
                                         std::istream& is,
                                         std::vector< Curve3d< float > >* pItem,
                                         size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( 
                                                           false, true );
  gkg::ItemReader< Curve3d< float > >& itemR2 =
    gkg::DefaultItemIOFactory< Curve3d< float > >::getInstance().getReader(
                                                                  false, true );

  std::vector< Curve3d< float > >* pointer = pItem;
  uint32_t size = 0U;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( is, size );
    pointer->resize( size );
    itemR2.read( is, &pointer->front(), pointer->size() );

  }


}


template <>
void StdVectorBSwapItemReader< Curve3d< float > >::read(
                                         largefile_ifstream& is,
                                         std::vector< Curve3d< float > >* pItem,
                                         size_t count ) const
{

  ItemReader< uint32_t >& itemR1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getReader( false, true );
  ItemReader< Curve3d< float > >& itemR2 =
    DefaultItemIOFactory< Curve3d< float > >::getInstance().getReader(
                                                                  false, true );

  std::vector< Curve3d< float > >* pointer = pItem;
  uint32_t size = 0U;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( is, size );
    pointer->resize( size );
    itemR2.read( is, &pointer->front(), pointer->size() );

  }

}


template <>
void StdVectorBSwapItemReader< Curve3d< float > >::read( 
                                         largefile_fstream& fs,
                                         std::vector< Curve3d< float > >* pItem,
                                         size_t count ) const
{

  ItemReader< uint32_t >& itemR1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getReader( false, true );
  ItemReader< Curve3d< float > >& itemR2 =
    DefaultItemIOFactory< Curve3d< float > >::getInstance().getReader(
                                                                  false, true );

  std::vector< Curve3d< float > >* pointer = pItem;
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
// class StdVectorBSwapItemReader< Curve3d< double > >
//

namespace gkg
{


template <>
StdVectorBSwapItemReader< Curve3d< double > >::StdVectorBSwapItemReader()
                              : ItemReader< std::vector< Curve3d< double > > >()
{
}


template <>
StdVectorBSwapItemReader< Curve3d< double > >::~StdVectorBSwapItemReader()
{
}


template <>
void StdVectorBSwapItemReader< Curve3d< double > >::read(
                                        std::istream& is,
                                        std::vector< Curve3d< double > >* pItem,
                                        size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( 
                                                           false, true );
  gkg::ItemReader< Curve3d< double > >& itemR2 =
    gkg::DefaultItemIOFactory< Curve3d< double > >::getInstance().getReader(
                                                                  false, true );

  std::vector< Curve3d< double > >* pointer = pItem;
  uint32_t size = 0U;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( is, size );
    pointer->resize( size );
    itemR2.read( is, &pointer->front(), pointer->size() );

  }


}


template <>
void StdVectorBSwapItemReader< Curve3d< double > >::read(
                                        largefile_ifstream& is,
                                        std::vector< Curve3d< double > >* pItem,
                                        size_t count ) const
{

  ItemReader< uint32_t >& itemR1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getReader( false, true );
  ItemReader< Curve3d< double > >& itemR2 =
    DefaultItemIOFactory< Curve3d< double > >::getInstance().getReader(
                                                                  false, true );

  std::vector< Curve3d< double > >* pointer = pItem;
  uint32_t size = 0U;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemR1.read( is, size );
    pointer->resize( size );
    itemR2.read( is, &pointer->front(), pointer->size() );

  }

}


template <>
void StdVectorBSwapItemReader< Curve3d< double > >::read( 
                                        largefile_fstream& fs,
                                        std::vector< Curve3d< double > >* pItem,
                                        size_t count ) const
{

  ItemReader< uint32_t >& itemR1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getReader( false, true );
  ItemReader< Curve3d< double > >& itemR2 =
    DefaultItemIOFactory< Curve3d< double > >::getInstance().getReader(
                                                                  false, true );

  std::vector< Curve3d< double > >* pointer = pItem;
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
