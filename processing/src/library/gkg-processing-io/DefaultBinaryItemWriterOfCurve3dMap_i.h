#ifndef _gkg_processing_io_DefaultBinaryItemWriterOfCurve3dMap_i_h_
#define _gkg_processing_io_DefaultBinaryItemWriterOfCurve3dMap_i_h_


#include <gkg-core-io/DefaultBinaryItemWriter_i.h>
#include <gkg-processing-container/Curve3dMap.h>
#include <gkg-core-io/StdVectorBinaryItemWriter.h>


//
// class DefaultBinaryItemWriter< Curve3dMap< float > >
//

namespace gkg
{


template <>
inline
DefaultBinaryItemWriter< Curve3dMap< float > >::DefaultBinaryItemWriter()
                                           : ItemWriter< Curve3dMap< float > >()
{
}


template <>
inline
DefaultBinaryItemWriter< Curve3dMap< float > >::~DefaultBinaryItemWriter()
{
}


template <>
inline
void DefaultBinaryItemWriter< Curve3dMap< float > >::write(
                                               std::ostream & os,
                                               const Curve3dMap< float >* pItem,
                                               size_t count ) const
{

  ItemWriter< std::vector< LightCurve3d< float > > >& itemW_vector_of_Curve3d =
    StdVectorBinaryItemWriter< LightCurve3d< float > >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Curve3d.write( os, pItem[ i ].getCurve3ds() );

  }

}


template <>
inline
void DefaultBinaryItemWriter< Curve3dMap< float > >::write(
                                               largefile_ofstream & os,
                                               const Curve3dMap< float >* pItem,
                                               size_t count ) const
{

  ItemWriter< std::vector< LightCurve3d< float > > >& itemW_vector_of_Curve3d =
    StdVectorBinaryItemWriter< LightCurve3d< float > >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Curve3d.write( os, pItem[ i ].getCurve3ds() );

  }

}


template <>
inline
void DefaultBinaryItemWriter< Curve3dMap< float > >::write(
                                               largefile_fstream & fs,
                                               const Curve3dMap< float >* pItem,
                                               size_t count ) const
{

  ItemWriter< std::vector< LightCurve3d< float > > >& itemW_vector_of_Curve3d =
    StdVectorBinaryItemWriter< LightCurve3d< float > >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Curve3d.write( fs, pItem[ i ].getCurve3ds() );

  }

}


}


//
// class DefaultBinaryItemWriter< Curve3dMap< double > >
//

namespace gkg
{


template <>
inline
DefaultBinaryItemWriter< Curve3dMap< double > >::DefaultBinaryItemWriter()
                                          : ItemWriter< Curve3dMap< double > >()
{
}


template <>
inline
DefaultBinaryItemWriter< Curve3dMap< double > >::~DefaultBinaryItemWriter()
{
}


template <>
inline
void DefaultBinaryItemWriter< Curve3dMap< double > >::write(
                                              std::ostream & os,
                                              const Curve3dMap< double >* pItem,
                                              size_t count ) const
{

  ItemWriter< std::vector< LightCurve3d< double > > >& itemW_vector_of_Curve3d =
    StdVectorBinaryItemWriter< LightCurve3d< double > >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Curve3d.write( os, pItem[ i ].getCurve3ds() );

  }

}


template <>
inline
void DefaultBinaryItemWriter< Curve3dMap< double > >::write(
                                              largefile_ofstream & os,
                                              const Curve3dMap< double >* pItem,
                                              size_t count ) const
{

  ItemWriter< std::vector< LightCurve3d< double > > >& itemW_vector_of_Curve3d =
    StdVectorBinaryItemWriter< LightCurve3d< double > >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Curve3d.write( os, pItem[ i ].getCurve3ds() );

  }

}


template <>
inline
void DefaultBinaryItemWriter< Curve3dMap< double > >::write(
                                              largefile_fstream & fs,
                                              const Curve3dMap< double >* pItem,
                                              size_t count ) const
{

  ItemWriter< std::vector< LightCurve3d< double > > >& itemW_vector_of_Curve3d =
    StdVectorBinaryItemWriter< LightCurve3d< double > >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Curve3d.write( fs, pItem[ i ].getCurve3ds() );

  }

}


}


#endif
