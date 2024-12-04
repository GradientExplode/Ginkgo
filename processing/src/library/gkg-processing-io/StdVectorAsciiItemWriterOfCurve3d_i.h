#ifndef _gkg_processing_io_StdVectorAsciiItemWriterOfCurve3d_i_h_
#define _gkg_processing_io_StdVectorAsciiItemWriterOfCurve3d_i_h_


#include <gkg-core-io/StdVectorAsciiItemWriter.h>
#include <gkg-core-io/ItemWriter_i.h>
#include <gkg-core-io/DefaultItemIOFactory.h>
#include <gkg-processing-container/Curve3d.h>


//
// class StdVectorAsciiItemWriter< Curve3d< float > >
//

namespace gkg
{


template <>
StdVectorAsciiItemWriter< Curve3d< float > >::StdVectorAsciiItemWriter()
                               : ItemWriter< std::vector< Curve3d< float > > >()
{
}


template <>
StdVectorAsciiItemWriter< Curve3d< float > >::~StdVectorAsciiItemWriter()
{
}


template <>
void StdVectorAsciiItemWriter< Curve3d< float > >::write(
                                   std::ostream& os,
                                   const std::vector< Curve3d< float > >* pItem,
                                   size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( true, false );
  ItemWriter< Curve3d< float > >& itemW2 =
    DefaultItemIOFactory< Curve3d< float > >::getInstance().getWriter(
                                                                  true, false );

  const std::vector< Curve3d< float > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    itemW2.write( os, &pointer->front(), pointer->size() );

  }

}


template <>
void StdVectorAsciiItemWriter< Curve3d< float > >::write(
                                   largefile_ofstream& os,
                                   const std::vector< Curve3d< float > >* pItem,
                                   size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( true, false );
  ItemWriter< Curve3d< float > >& itemW2 =
    DefaultItemIOFactory< Curve3d< float > >::getInstance().getWriter(
                                                                  true, false );

  const std::vector< Curve3d< float > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    itemW2.write( os, &pointer->front(), pointer->size() );

  }

}


template <>
void StdVectorAsciiItemWriter< Curve3d< float > >::write(
                                   largefile_fstream& fs,
                                   const std::vector< Curve3d< float > >* pItem,
                                   size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( true, false );
  ItemWriter< Curve3d< float > >& itemW2 =
    DefaultItemIOFactory< Curve3d< float > >::getInstance().getWriter(
                                                                  true, false );

  const std::vector< Curve3d< float > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( fs, pointer->size() );
    itemW2.write( fs, &pointer->front(), pointer->size() );

  }

}


}


//
// class StdVectorAsciiItemWriter< Curve3d< double > >
//

namespace gkg
{


template <>
StdVectorAsciiItemWriter< Curve3d< double > >::StdVectorAsciiItemWriter()
                              : ItemWriter< std::vector< Curve3d< double > > >()
{
}


template <>
StdVectorAsciiItemWriter< Curve3d< double > >::~StdVectorAsciiItemWriter()
{
}


template <>
void StdVectorAsciiItemWriter< Curve3d< double > >::write(
                                  std::ostream& os,
                                  const std::vector< Curve3d< double > >* pItem,
                                  size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( true, false );
  ItemWriter< Curve3d< double > >& itemW2 =
    DefaultItemIOFactory< Curve3d< double > >::getInstance().getWriter(
                                                                  true, false );

  const std::vector< Curve3d< double > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    itemW2.write( os, &pointer->front(), pointer->size() );

  }

}


template <>
void StdVectorAsciiItemWriter< Curve3d< double > >::write(
                                  largefile_ofstream& os,
                                  const std::vector< Curve3d< double > >* pItem,
                                  size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( true, false );
  ItemWriter< Curve3d< double > >& itemW2 =
    DefaultItemIOFactory< Curve3d< double > >::getInstance().getWriter(
                                                                  true, false );

  const std::vector< Curve3d< double > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    itemW2.write( os, &pointer->front(), pointer->size() );

  }

}


template <>
void StdVectorAsciiItemWriter< Curve3d< double > >::write(
                                  largefile_fstream& fs,
                                  const std::vector< Curve3d< double > >* pItem,
                                  size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( true, false );
  ItemWriter< Curve3d< double > >& itemW2 =
    DefaultItemIOFactory< Curve3d< double > >::getInstance().getWriter(
                                                                  true, false );

  const std::vector< Curve3d< double > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( fs, pointer->size() );
    itemW2.write( fs, &pointer->front(), pointer->size() );

  }

}


}


#endif
