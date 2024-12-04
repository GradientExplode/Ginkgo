#ifndef _gkg_processing_io_DefaultBinaryItemWriterOfLightCurve3d_i_h_
#define _gkg_processing_io_DefaultBinaryItemWriterOfLightCurve3d_i_h_


#include <gkg-core-io/DefaultBinaryItemWriter_i.h>
#include <gkg-processing-container/LightCurve3d.h>
#include <gkg-core-io/StdVectorBinaryItemWriter.h>


//
// class DefaultBinaryItemWriter< LightCurve3d< float > >
//

namespace gkg
{


template <>
inline
DefaultBinaryItemWriter< LightCurve3d< float > >::DefaultBinaryItemWriter()
                                                 : ItemWriter<
                                                       LightCurve3d< float > >()
{
}


template <>
inline
DefaultBinaryItemWriter< LightCurve3d< float > >::~DefaultBinaryItemWriter()
{
}


template <>
inline
void DefaultBinaryItemWriter< LightCurve3d< float > >::write(
                                             std::ostream & os,
                                             const LightCurve3d< float >* pItem,
                                             size_t count ) const
{

  ItemWriter< std::vector< Vector3d< float > > >& itemW_vector_of_Vector3d =
    StdVectorBinaryItemWriter< Vector3d< float > >::getInstance();
  ItemWriter< bool >& itemW_bool =
    DefaultBinaryItemWriter< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Vector3d.write( os, pItem[ i ].getPoints() );
    itemW_bool.write( os, pItem[ i ].isClosed() );

  }

}


template <>
inline
void DefaultBinaryItemWriter< LightCurve3d< float > >::write(
                                             largefile_ofstream & os,
                                             const LightCurve3d< float >* pItem,
                                             size_t count ) const
{

  ItemWriter< std::vector< Vector3d< float > > >& itemW_vector_of_Vector3d =
    StdVectorBinaryItemWriter< Vector3d< float > >::getInstance();
  ItemWriter< bool >& itemW_bool =
    DefaultBinaryItemWriter< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Vector3d.write( os, pItem[ i ].getPoints() );
    itemW_bool.write( os, pItem[ i ].isClosed() );

  }

}


template <>
inline
void DefaultBinaryItemWriter< LightCurve3d< float > >::write(
                                             largefile_fstream & fs,
                                             const LightCurve3d< float >* pItem,
                                             size_t count ) const
{

  ItemWriter< std::vector< Vector3d< float > > >& itemW_vector_of_Vector3d =
    StdVectorBinaryItemWriter< Vector3d< float > >::getInstance();
  ItemWriter< bool >& itemW_bool =
    DefaultBinaryItemWriter< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Vector3d.write( fs, pItem[ i ].getPoints() );
    itemW_bool.write( fs, pItem[ i ].isClosed() );

  }

}


}


//
// class DefaultBinaryItemWriter< LightCurve3d< double > >
//

namespace gkg
{


template <>
inline
DefaultBinaryItemWriter< LightCurve3d< double > >::DefaultBinaryItemWriter()
                                                  : ItemWriter<
                                                      LightCurve3d< double > >()
{
}


template <>
inline
DefaultBinaryItemWriter< LightCurve3d< double > >::~DefaultBinaryItemWriter()
{
}


template <>
inline
void DefaultBinaryItemWriter< LightCurve3d< double > >::write(
                                            std::ostream & os,
                                            const LightCurve3d< double >* pItem,
                                            size_t count ) const
{

  ItemWriter< std::vector< Vector3d< double > > >& itemW_vector_of_Vector3d =
    StdVectorBinaryItemWriter< Vector3d< double > >::getInstance();
  ItemWriter< bool >& itemW_bool =
    DefaultBinaryItemWriter< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Vector3d.write( os, pItem[ i ].getPoints() );
    itemW_bool.write( os, pItem[ i ].isClosed() );

  }

}


template <>
inline
void DefaultBinaryItemWriter< LightCurve3d< double > >::write(
                                            largefile_ofstream & os,
                                            const LightCurve3d< double >* pItem,
                                            size_t count ) const
{

  ItemWriter< std::vector< Vector3d< double > > >& itemW_vector_of_Vector3d =
    StdVectorBinaryItemWriter< Vector3d< double > >::getInstance();
  ItemWriter< bool >& itemW_bool =
    DefaultBinaryItemWriter< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Vector3d.write( os, pItem[ i ].getPoints() );
    itemW_bool.write( os, pItem[ i ].isClosed() );

  }

}


template <>
inline
void DefaultBinaryItemWriter< LightCurve3d< double > >::write(
                                            largefile_fstream & fs,
                                            const LightCurve3d< double >* pItem,
                                            size_t count ) const
{

  ItemWriter< std::vector< Vector3d< double > > >& itemW_vector_of_Vector3d =
    StdVectorBinaryItemWriter< Vector3d< double > >::getInstance();
  ItemWriter< bool >& itemW_bool =
    DefaultBinaryItemWriter< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_vector_of_Vector3d.write( fs, pItem[ i ].getPoints() );
    itemW_bool.write( fs, pItem[ i ].isClosed() );

  }

}


}


#endif
