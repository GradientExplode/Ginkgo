#ifndef _gkg_processing_io_DefaultBinaryItemReaderOfLightCurve3d_i_h_
#define _gkg_processing_io_DefaultBinaryItemReaderOfLightCurve3d_i_h_

#include <gkg-core-io/DefaultBinaryItemReader_i.h>
#include <gkg-processing-container/LightCurve3d.h>
#include <gkg-core-io/StdVectorBinaryItemReader.h>


//
// class DefaultBinaryItemReader< LightCurve3d< float > >
//

namespace gkg
{


template <>
inline
DefaultBinaryItemReader< LightCurve3d< float > >::DefaultBinaryItemReader()
                                                 : ItemReader<
                                                       LightCurve3d< float > >()
{
}


template <>
inline
DefaultBinaryItemReader< LightCurve3d< float > >::~DefaultBinaryItemReader()
{
}


template <>
inline
void DefaultBinaryItemReader< LightCurve3d< float > >::read(
                                                   std::istream & is,
                                                   LightCurve3d< float >* pItem,
                                                   size_t count ) const
{

  ItemReader< std::vector< Vector3d< float > > >& itemR_vector_of_Vector3d =
    StdVectorBinaryItemReader< Vector3d< float > >::getInstance();
  ItemReader< bool >& itemR_bool =
    DefaultBinaryItemReader< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::vector< Vector3d< float > > points;
    itemR_vector_of_Vector3d.read( is, points );

    bool isClosed;
    itemR_bool.read( is, isClosed );

    pItem[ i ] = LightCurve3d< float >( points, isClosed );

  }

}


template <>
inline
void DefaultBinaryItemReader< LightCurve3d< float > >::read(
                                                   largefile_ifstream & is,
                                                   LightCurve3d< float >* pItem,
                                                   size_t count ) const
{

  ItemReader< std::vector< Vector3d< float > > >& itemR_vector_of_Vector3d =
    StdVectorBinaryItemReader< Vector3d< float > >::getInstance();
  ItemReader< bool >& itemR_bool =
    DefaultBinaryItemReader< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::vector< Vector3d< float > > points;
    itemR_vector_of_Vector3d.read( is, points );

    bool isClosed;
    itemR_bool.read( is, isClosed );

    pItem[ i ] = LightCurve3d< float >( points, isClosed );

  }

}


template <>
inline
void DefaultBinaryItemReader< LightCurve3d< float > >::read(
                                                   largefile_fstream & fs,
                                                   LightCurve3d< float >* pItem,
                                                   size_t count ) const
{

  ItemReader< std::vector< Vector3d< float > > >& itemR_vector_of_Vector3d =
    StdVectorBinaryItemReader< Vector3d< float > >::getInstance();
  ItemReader< bool >& itemR_bool =
    DefaultBinaryItemReader< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::vector< Vector3d< float > > points;
    itemR_vector_of_Vector3d.read( fs, points );

    bool isClosed;
    itemR_bool.read( fs, isClosed );

    pItem[ i ] = LightCurve3d< float >( points, isClosed );

  }

}


}


//
// class DefaultBinaryItemReader< LightCurve3d< double > >
//

namespace gkg
{


template <>
inline
DefaultBinaryItemReader< LightCurve3d< double > >::DefaultBinaryItemReader()
                                                  : ItemReader<
                                                      LightCurve3d< double > >()
{
}


template <>
inline
DefaultBinaryItemReader< LightCurve3d< double > >::~DefaultBinaryItemReader()
{
}


template <>
inline
void DefaultBinaryItemReader< LightCurve3d< double > >::read(
                                                  std::istream & is,
                                                  LightCurve3d< double >* pItem,
                                                  size_t count ) const
{

  ItemReader< std::vector< Vector3d< double > > >& itemR_vector_of_Vector3d =
    StdVectorBinaryItemReader< Vector3d< double > >::getInstance();
  ItemReader< bool >& itemR_bool =
    DefaultBinaryItemReader< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::vector< Vector3d< double > > points;
    itemR_vector_of_Vector3d.read( is, points );

    bool isClosed;
    itemR_bool.read( is, isClosed );

    pItem[ i ] = LightCurve3d< double >( points, isClosed );

  }

}


template <>
inline
void DefaultBinaryItemReader< LightCurve3d< double > >::read(
                                                  largefile_ifstream & is,
                                                  LightCurve3d< double >* pItem,
                                                  size_t count ) const
{

  ItemReader< std::vector< Vector3d< double > > >& itemR_vector_of_Vector3d =
    StdVectorBinaryItemReader< Vector3d< double > >::getInstance();
  ItemReader< bool >& itemR_bool =
    DefaultBinaryItemReader< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::vector< Vector3d< double > > points;
    itemR_vector_of_Vector3d.read( is, points );

    bool isClosed;
    itemR_bool.read( is, isClosed );

    pItem[ i ] = LightCurve3d< double >( points, isClosed );

  }

}


template <>
inline
void DefaultBinaryItemReader< LightCurve3d< double > >::read(
                                                  largefile_fstream & fs,
                                                  LightCurve3d< double >* pItem,
                                                  size_t count ) const
{

  ItemReader< std::vector< Vector3d< double > > >& itemR_vector_of_Vector3d =
    StdVectorBinaryItemReader< Vector3d< double > >::getInstance();
  ItemReader< bool >& itemR_bool =
    DefaultBinaryItemReader< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::vector< Vector3d< double > > points;
    itemR_vector_of_Vector3d.read( fs, points );

    bool isClosed;
    itemR_bool.read( fs, isClosed );

    pItem[ i ] = LightCurve3d< double >( points, isClosed );

  }

}


}


#endif
