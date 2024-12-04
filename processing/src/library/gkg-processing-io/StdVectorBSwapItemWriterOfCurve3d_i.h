#ifndef _gkg_processing_io_StdVectorBSwapItemWriterOfCurve3d_i_h_
#define _gkg_processing_io_StdVectorBSwapItemWriterOfCurve3d_i_h_


#include <gkg-core-io/StdVectorBSwapItemWriter.h>
#include <gkg-core-io/ItemWriter_i.h>
#include <gkg-core-io/DefaultItemIOFactory.h>
#include <gkg-processing-container/Curve3d.h>


//
// class StdVectorBSwapItemWriter< Curve3d< float > >
//

namespace gkg
{


template <>
StdVectorBSwapItemWriter< Curve3d< float > >::StdVectorBSwapItemWriter()
                               : ItemWriter< std::vector< Curve3d< float > > >()
{
}


template <>
StdVectorBSwapItemWriter< Curve3d< float > >::~StdVectorBSwapItemWriter()
{
}


template <>
void StdVectorBSwapItemWriter< Curve3d< float > >::write(
                                   std::ostream& os,
                                   const std::vector< Curve3d< float > >* pItem,
                                   size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, true );
  ItemWriter< Curve3d< float > >& itemW2 =
    DefaultItemIOFactory< Curve3d< float > >::getInstance().getWriter(
                                                                  false, true );

  const std::vector< Curve3d< float > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    itemW2.write( os, &pointer->front(), pointer->size() );

  }

}


template <>
void StdVectorBSwapItemWriter< Curve3d< float > >::write(
                                   largefile_ofstream& os,
                                   const std::vector< Curve3d< float > >* pItem,
                                   size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, true );
  ItemWriter< Curve3d< float > >& itemW2 =
    DefaultItemIOFactory< Curve3d< float > >::getInstance().getWriter(
                                                                  false, true );

  const std::vector< Curve3d< float > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    itemW2.write( os, &pointer->front(), pointer->size() );

  }

}


template <>
void StdVectorBSwapItemWriter< Curve3d< float > >::write(
                                   largefile_fstream& fs,
                                   const std::vector< Curve3d< float > >* pItem,
                                   size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, true );
  ItemWriter< Curve3d< float > >& itemW2 =
    DefaultItemIOFactory< Curve3d< float > >::getInstance().getWriter(
                                                                  false, true );

  const std::vector< Curve3d< float > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( fs, pointer->size() );
    itemW2.write( fs, &pointer->front(), pointer->size() );

  }

}


}


//
// class StdVectorBSwapItemWriter< Curve3d< double > >
//

namespace gkg
{


template <>
StdVectorBSwapItemWriter< Curve3d< double > >::StdVectorBSwapItemWriter()
                              : ItemWriter< std::vector< Curve3d< double > > >()
{
}


template <>
StdVectorBSwapItemWriter< Curve3d< double > >::~StdVectorBSwapItemWriter()
{
}


template <>
void StdVectorBSwapItemWriter< Curve3d< double > >::write(
                                  std::ostream& os,
                                  const std::vector< Curve3d< double > >* pItem,
                                  size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, true );
  ItemWriter< Curve3d< double > >& itemW2 =
    DefaultItemIOFactory< Curve3d< double > >::getInstance().getWriter(
                                                                  false, true );

  const std::vector< Curve3d< double > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    itemW2.write( os, &pointer->front(), pointer->size() );

  }

}


template <>
void StdVectorBSwapItemWriter< Curve3d< double > >::write(
                                  largefile_ofstream& os,
                                  const std::vector< Curve3d< double > >* pItem,
                                  size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, true );
  ItemWriter< Curve3d< double > >& itemW2 =
    DefaultItemIOFactory< Curve3d< double > >::getInstance().getWriter(
                                                                  false, true );

  const std::vector< Curve3d< double > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    itemW2.write( os, &pointer->front(), pointer->size() );

  }

}


template <>
void StdVectorBSwapItemWriter< Curve3d< double > >::write(
                                  largefile_fstream& fs,
                                  const std::vector< Curve3d< double > >* pItem,
                                  size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, true );
  ItemWriter< Curve3d< double > >& itemW2 =
    DefaultItemIOFactory< Curve3d< double > >::getInstance().getWriter(
                                                                  false, true );

  const std::vector< Curve3d< double > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( fs, pointer->size() );
    itemW2.write( fs, &pointer->front(), pointer->size() );

  }

}


}


#endif
