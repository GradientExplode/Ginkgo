#ifndef _gkg_processing_io_StdVectorBinaryItemWriterOfLightCurve3d_i_h_
#define _gkg_processing_io_StdVectorBinaryItemWriterOfLightCurve3d_i_h_


#include <gkg-core-io/StdVectorBinaryItemWriter.h>
#include <gkg-core-io/ItemWriter_i.h>
#include <gkg-core-io/DefaultItemIOFactory.h>
#include <gkg-processing-container/LightCurve3d.h>


//
// class StdVectorBinaryItemWriter< LightCurve3d< float > >
//

namespace gkg
{


template <>
StdVectorBinaryItemWriter< LightCurve3d< float > >::StdVectorBinaryItemWriter()
                          : ItemWriter< std::vector< LightCurve3d< float > > >()
{
}


template <>
StdVectorBinaryItemWriter< LightCurve3d< float > >::~StdVectorBinaryItemWriter()
{
}


template <>
void StdVectorBinaryItemWriter< LightCurve3d< float > >::write(
                              std::ostream& os,
                              const std::vector< LightCurve3d< float > >* pItem,
                              size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, false );
  ItemWriter< LightCurve3d< float > >& itemW2 =
    DefaultItemIOFactory< LightCurve3d< float > >::getInstance().getWriter(
                                                                 false, false );

  const std::vector< LightCurve3d< float > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    itemW2.write( os, &pointer->front(), pointer->size() );

  }

}


template <>
void StdVectorBinaryItemWriter< LightCurve3d< float > >::write(
                              largefile_ofstream& os,
                              const std::vector< LightCurve3d< float > >* pItem,
                              size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, false );
  ItemWriter< LightCurve3d< float > >& itemW2 =
    DefaultItemIOFactory< LightCurve3d< float > >::getInstance().getWriter(
                                                                 false, false );

  const std::vector< LightCurve3d< float > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    itemW2.write( os, &pointer->front(), pointer->size() );

  }

}


template <>
void StdVectorBinaryItemWriter< LightCurve3d< float > >::write(
                              largefile_fstream& fs,
                              const std::vector< LightCurve3d< float > >* pItem,
                              size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, false );
  ItemWriter< LightCurve3d< float > >& itemW2 =
    DefaultItemIOFactory< LightCurve3d< float > >::getInstance().getWriter(
                                                                 false, false );

  const std::vector< LightCurve3d< float > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( fs, pointer->size() );
    itemW2.write( fs, &pointer->front(), pointer->size() );

  }

}


}


//
// class StdVectorBinaryItemWriter< LightCurve3d< double > >
//

namespace gkg
{


template <>
StdVectorBinaryItemWriter< LightCurve3d< double > >::StdVectorBinaryItemWriter()
                         : ItemWriter< std::vector< LightCurve3d< double > > >()
{
}


template <>
StdVectorBinaryItemWriter< LightCurve3d< double > >::
                                                    ~StdVectorBinaryItemWriter()
{
}


template <>
void StdVectorBinaryItemWriter< LightCurve3d< double > >::write(
                             std::ostream& os,
                             const std::vector< LightCurve3d< double > >* pItem,
                             size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, false );
  ItemWriter< LightCurve3d< double > >& itemW2 =
    DefaultItemIOFactory< LightCurve3d< double > >::getInstance().getWriter(
                                                                 false, false );

  const std::vector< LightCurve3d< double > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    itemW2.write( os, &pointer->front(), pointer->size() );

  }

}


template <>
void StdVectorBinaryItemWriter< LightCurve3d< double > >::write(
                             largefile_ofstream& os,
                             const std::vector< LightCurve3d< double > >* pItem,
                             size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, false );
  ItemWriter< LightCurve3d< double > >& itemW2 =
    DefaultItemIOFactory< LightCurve3d< double > >::getInstance().getWriter(
                                                                 false, false );

  const std::vector< LightCurve3d< double > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    itemW2.write( os, &pointer->front(), pointer->size() );

  }

}


template <>
void StdVectorBinaryItemWriter< LightCurve3d< double > >::write(
                             largefile_fstream& fs,
                             const std::vector< LightCurve3d< double > >* pItem,
                             size_t count ) const
{

  ItemWriter< uint32_t >& itemW1 =
    DefaultItemIOFactory< uint32_t >::getInstance().getWriter( false, false );
  ItemWriter< LightCurve3d< double > >& itemW2 =
    DefaultItemIOFactory< LightCurve3d< double > >::getInstance().getWriter(
                                                                 false, false );

  const std::vector< LightCurve3d< double > >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( fs, pointer->size() );
    itemW2.write( fs, &pointer->front(), pointer->size() );

  }

}


}


#endif
