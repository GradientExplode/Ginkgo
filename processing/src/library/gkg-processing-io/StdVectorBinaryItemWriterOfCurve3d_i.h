#ifndef _gkg_processing_io_StdVectorBinaryItemWriterOfCurve3d_i_h_
#define _gkg_processing_io_StdVectorBinaryItemWriterOfCurve3d_i_h_


#include <gkg-core-io/StdVectorBinaryItemWriter.h>
#include <gkg-core-io/ItemWriter_i.h>
#include <gkg-core-io/DefaultItemIOFactory.h>
#include <gkg-processing-container/Curve3d.h>


//
// class StdVectorBinaryItemWriter< Curve3d< float > >
//

namespace gkg
{


template <>
StdVectorBinaryItemWriter< Curve3d< float > >::StdVectorBinaryItemWriter()
                               : ItemWriter< std::vector< Curve3d< float > > >()
{
}


template <>
StdVectorBinaryItemWriter< Curve3d< float > >::~StdVectorBinaryItemWriter()
{
}


template <>
void StdVectorBinaryItemWriter< Curve3d< float > >::write(
                                   std::ostream& os,
                                   const std::vector< Curve3d< float > >* pItem,
                                   size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, false );
  ItemWriter< Curve3d< float > >& itemW2 =
    DefaultItemIOFactory< Curve3d< float > >::getInstance().getWriter(
                                                                 false, false );

  const std::vector< Curve3d< float > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    itemW2.write( os, &pointer->front(), pointer->size() );

  }

}


template <>
void StdVectorBinaryItemWriter< Curve3d< float > >::write(
                                   largefile_ofstream& os,
                                   const std::vector< Curve3d< float > >* pItem,
                                   size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, false );
  ItemWriter< Curve3d< float > >& itemW2 =
    DefaultItemIOFactory< Curve3d< float > >::getInstance().getWriter(
                                                                 false, false );

  const std::vector< Curve3d< float > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    itemW2.write( os, &pointer->front(), pointer->size() );

  }

}


template <>
void StdVectorBinaryItemWriter< Curve3d< float > >::write(
                                   largefile_fstream& fs,
                                   const std::vector< Curve3d< float > >* pItem,
                                   size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, false );
  ItemWriter< Curve3d< float > >& itemW2 =
    DefaultItemIOFactory< Curve3d< float > >::getInstance().getWriter(
                                                                 false, false );

  const std::vector< Curve3d< float > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( fs, pointer->size() );
    itemW2.write( fs, &pointer->front(), pointer->size() );

  }

}


}


//
// class StdVectorBinaryItemWriter< Curve3d< double > >
//

namespace gkg
{


template <>
StdVectorBinaryItemWriter< Curve3d< double > >::StdVectorBinaryItemWriter()
                              : ItemWriter< std::vector< Curve3d< double > > >()
{
}


template <>
StdVectorBinaryItemWriter< Curve3d< double > >::~StdVectorBinaryItemWriter()
{
}


template <>
void StdVectorBinaryItemWriter< Curve3d< double > >::write(
                                  std::ostream& os,
                                  const std::vector< Curve3d< double > >* pItem,
                                  size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, false );
  ItemWriter< Curve3d< double > >& itemW2 =
    DefaultItemIOFactory< Curve3d< double > >::getInstance().getWriter(
                                                                 false, false );

  const std::vector< Curve3d< double > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    itemW2.write( os, &pointer->front(), pointer->size() );

  }

}


template <>
void StdVectorBinaryItemWriter< Curve3d< double > >::write(
                                  largefile_ofstream& os,
                                  const std::vector< Curve3d< double > >* pItem,
                                  size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, false );
  ItemWriter< Curve3d< double > >& itemW2 =
    DefaultItemIOFactory< Curve3d< double > >::getInstance().getWriter(
                                                                 false, false );

  const std::vector< Curve3d< double > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    itemW2.write( os, &pointer->front(), pointer->size() );

  }

}


template <>
void StdVectorBinaryItemWriter< Curve3d< double > >::write(
                                  largefile_fstream& fs,
                                  const std::vector< Curve3d< double > >* pItem,
                                  size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, false );
  ItemWriter< Curve3d< double > >& itemW2 =
    DefaultItemIOFactory< Curve3d< double > >::getInstance().getWriter(
                                                                 false, false );

  const std::vector< Curve3d< double > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( fs, pointer->size() );
    itemW2.write( fs, &pointer->front(), pointer->size() );

  }

}


}


#endif
