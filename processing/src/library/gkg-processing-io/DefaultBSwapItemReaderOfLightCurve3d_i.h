#ifndef _gkg_processing_io_DefaultBSwapItemReaderOfLightCurve3d_i_h_
#define _gkg_processing_io_DefaultBSwapItemReaderOfLightCurve3d_i_h_

#include <gkg-core-io/DefaultBSwapItemReader_i.h>
#include <gkg-processing-container/LightCurve3d.h>
#include <gkg-core-io/StdVectorBSwapItemReader.h>


//
// class DefaultBSwapItemReader< LightCurve3d< float > >
//

namespace gkg
{


template <>
inline
DefaultBSwapItemReader< LightCurve3d< float > >::DefaultBSwapItemReader()
                                                : ItemReader<
                                                       LightCurve3d< float > >()
{
}


template <>
inline
DefaultBSwapItemReader< LightCurve3d< float > >::~DefaultBSwapItemReader()
{
}


template <>
inline
void DefaultBSwapItemReader< LightCurve3d< float > >::read(
                                                   std::istream & is,
                                                   LightCurve3d< float >* pItem,
                                                   size_t count ) const
{

  ItemReader< std::vector< Vector3d< float > > >& itemR_vector_of_Vector3d =
    StdVectorBSwapItemReader< Vector3d< float > >::getInstance();
  ItemReader< bool >& itemR_bool =
    DefaultBSwapItemReader< bool >::getInstance();
  
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
void DefaultBSwapItemReader< LightCurve3d< float > >::read(
                                                   largefile_ifstream & is,
                                                   LightCurve3d< float >* pItem,
                                                   size_t count ) const
{

  ItemReader< std::vector< Vector3d< float > > >& itemR_vector_of_Vector3d =
    StdVectorBSwapItemReader< Vector3d< float > >::getInstance();
  ItemReader< bool >& itemR_bool =
    DefaultBSwapItemReader< bool >::getInstance();
  
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
void DefaultBSwapItemReader< LightCurve3d< float > >::read(
                                                   largefile_fstream & fs,
                                                   LightCurve3d< float >* pItem,
                                                   size_t count ) const
{

  ItemReader< std::vector< Vector3d< float > > >& itemR_vector_of_Vector3d =
    StdVectorBSwapItemReader< Vector3d< float > >::getInstance();
  ItemReader< bool >& itemR_bool =
    DefaultBSwapItemReader< bool >::getInstance();
  
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
// class DefaultBSwapItemReader< LightCurve3d< double > >
//

namespace gkg
{


template <>
inline
DefaultBSwapItemReader< LightCurve3d< double > >::DefaultBSwapItemReader()
                                                 : ItemReader<
                                                      LightCurve3d< double > >()
{
}


template <>
inline
DefaultBSwapItemReader< LightCurve3d< double > >::~DefaultBSwapItemReader()
{
}


template <>
inline
void DefaultBSwapItemReader< LightCurve3d< double > >::read(
                                                  std::istream & is,
                                                  LightCurve3d< double >* pItem,
                                                  size_t count ) const
{

  ItemReader< std::vector< Vector3d< double > > >& itemR_vector_of_Vector3d =
    StdVectorBSwapItemReader< Vector3d< double > >::getInstance();
  ItemReader< bool >& itemR_bool =
    DefaultBSwapItemReader< bool >::getInstance();
  
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
void DefaultBSwapItemReader< LightCurve3d< double > >::read(
                                                  largefile_ifstream & is,
                                                  LightCurve3d< double >* pItem,
                                                  size_t count ) const
{

  ItemReader< std::vector< Vector3d< double > > >& itemR_vector_of_Vector3d =
    StdVectorBSwapItemReader< Vector3d< double > >::getInstance();
  ItemReader< bool >& itemR_bool =
    DefaultBSwapItemReader< bool >::getInstance();
  
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
void DefaultBSwapItemReader< LightCurve3d< double > >::read(
                                                  largefile_fstream & fs,
                                                  LightCurve3d< double >* pItem,
                                                  size_t count ) const
{

  ItemReader< std::vector< Vector3d< double > > >& itemR_vector_of_Vector3d =
    StdVectorBSwapItemReader< Vector3d< double > >::getInstance();
  ItemReader< bool >& itemR_bool =
    DefaultBSwapItemReader< bool >::getInstance();
  
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
