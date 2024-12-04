#ifndef _gkg_processing_io_DefaultBSwapItemWriterOfCurve3dMap_i_h_
#define _gkg_processing_io_DefaultBSwapItemWriterOfCurve3dMap_i_h_


#include <gkg-core-io/DefaultBSwapItemWriter_i.h>
#include <gkg-processing-container/Curve3dMap.h>
#include <gkg-core-io/StdVectorBSwapItemWriter.h>


//
// class DefaultBSwapItemWriter< Curve3dMap< float > >
//

namespace gkg
{


template <>
inline
DefaultBSwapItemWriter< Curve3dMap< float > >::DefaultBSwapItemWriter()
                                           : ItemWriter< Curve3dMap< float > >()
{
}


template <>
inline
DefaultBSwapItemWriter< Curve3dMap< float > >::~DefaultBSwapItemWriter()
{
}


template <>
inline
void DefaultBSwapItemWriter< Curve3dMap< float > >::write(
                                               std::ostream & os,
                                               const Curve3dMap< float >* pItem,
                                               size_t count ) const
{

  ItemWriter< std::vector< LightCurve3d< float > > >& itemW_vector_of_Curve3d =
    StdVectorBSwapItemWriter< LightCurve3d< float > >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Curve3d.write( os, pItem[ i ].getCurve3ds() );

  }

}


template <>
inline
void DefaultBSwapItemWriter< Curve3dMap< float > >::write(
                                               largefile_ofstream & os,
                                               const Curve3dMap< float >* pItem,
                                               size_t count ) const
{

  ItemWriter< std::vector< LightCurve3d< float > > >& itemW_vector_of_Curve3d =
    StdVectorBSwapItemWriter< LightCurve3d< float > >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Curve3d.write( os, pItem[ i ].getCurve3ds() );

  }

}


template <>
inline
void DefaultBSwapItemWriter< Curve3dMap< float > >::write(
                                               largefile_fstream & fs,
                                               const Curve3dMap< float >* pItem,
                                               size_t count ) const
{

  ItemWriter< std::vector< LightCurve3d< float > > >& itemW_vector_of_Curve3d =
    StdVectorBSwapItemWriter< LightCurve3d< float > >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Curve3d.write( fs, pItem[ i ].getCurve3ds() );

  }

}


}


//
// class DefaultBSwapItemWriter< Curve3dMap< double > >
//

namespace gkg
{


template <>
inline
DefaultBSwapItemWriter< Curve3dMap< double > >::DefaultBSwapItemWriter()
                                          : ItemWriter< Curve3dMap< double > >()
{
}


template <>
inline
DefaultBSwapItemWriter< Curve3dMap< double > >::~DefaultBSwapItemWriter()
{
}


template <>
inline
void DefaultBSwapItemWriter< Curve3dMap< double > >::write(
                                              std::ostream & os,
                                              const Curve3dMap< double >* pItem,
                                              size_t count ) const
{

  ItemWriter< std::vector< LightCurve3d< double > > >& itemW_vector_of_Curve3d =
    StdVectorBSwapItemWriter< LightCurve3d< double > >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Curve3d.write( os, pItem[ i ].getCurve3ds() );

  }

}


template <>
inline
void DefaultBSwapItemWriter< Curve3dMap< double > >::write(
                                              largefile_ofstream & os,
                                              const Curve3dMap< double >* pItem,
                                              size_t count ) const
{

  ItemWriter< std::vector< LightCurve3d< double > > >& itemW_vector_of_Curve3d =
    StdVectorBSwapItemWriter< LightCurve3d< double > >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Curve3d.write( os, pItem[ i ].getCurve3ds() );

  }

}


template <>
inline
void DefaultBSwapItemWriter< Curve3dMap< double > >::write(
                                              largefile_fstream & fs,
                                              const Curve3dMap< double >* pItem,
                                              size_t count ) const
{

  ItemWriter< std::vector< LightCurve3d< double > > >& itemW_vector_of_Curve3d =
    StdVectorBSwapItemWriter< LightCurve3d< double > >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Curve3d.write( fs, pItem[ i ].getCurve3ds() );

  }

}


}


#endif
