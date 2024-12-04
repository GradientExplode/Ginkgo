#ifndef _gkg_processing_io_DefaultAsciiItemReaderOfLightCurve3d_i_h_
#define _gkg_processing_io_DefaultAsciiItemReaderOfLightCurve3d_i_h_

#include <gkg-core-io/DefaultAsciiItemReader_i.h>
#include <gkg-processing-container/LightCurve3d.h>
#include <gkg-core-io/StdVectorAsciiItemReader.h>


//
// class DefaultAsciiItemReader< LightCurve3d< float > >
//

namespace gkg
{


template <>
inline
DefaultAsciiItemReader< LightCurve3d< float > >::DefaultAsciiItemReader()
                                                : ItemReader<
                                                       LightCurve3d< float > >()
{
}


template <>
inline
DefaultAsciiItemReader< LightCurve3d< float > >::~DefaultAsciiItemReader()
{
}


template <>
inline
void DefaultAsciiItemReader< LightCurve3d< float > >::read(
                                                   std::istream & is,
                                                   LightCurve3d< float >* pItem,
                                                   size_t count ) const
{

  ItemReader< std::vector< Vector3d< float > > >& itemR_vector_of_Vector3d =
    StdVectorAsciiItemReader< Vector3d< float > >::getInstance();
  ItemReader< bool >& itemR_bool =
    DefaultAsciiItemReader< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::vector< Vector3d< float > > points;
    itemR_vector_of_Vector3d.read( is, points );

    bool isClosed = false;
    itemR_bool.read( is, isClosed );

    pItem[ i ] = LightCurve3d< float >( points, isClosed );

  }

}


template <>
inline
void DefaultAsciiItemReader< LightCurve3d< float > >::read(
                                                   largefile_ifstream & is,
                                                   LightCurve3d< float >* pItem,
                                                   size_t count ) const
{

  ItemReader< std::vector< Vector3d< float > > >& itemR_vector_of_Vector3d =
    StdVectorAsciiItemReader< Vector3d< float > >::getInstance();
  ItemReader< bool >& itemR_bool =
    DefaultAsciiItemReader< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::vector< Vector3d< float > > points;
    itemR_vector_of_Vector3d.read( is, points );

    bool isClosed = false;
    itemR_bool.read( is, isClosed );

    pItem[ i ] = LightCurve3d< float >( points, isClosed );

  }

}


template <>
inline
void DefaultAsciiItemReader< LightCurve3d< float > >::read(
                                                   largefile_fstream & fs,
                                                   LightCurve3d< float >* pItem,
                                                   size_t count ) const
{

  ItemReader< std::vector< Vector3d< float > > >& itemR_vector_of_Vector3d =
    StdVectorAsciiItemReader< Vector3d< float > >::getInstance();
  ItemReader< bool >& itemR_bool =
    DefaultAsciiItemReader< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::vector< Vector3d< float > > points;
    itemR_vector_of_Vector3d.read( fs, points );

    bool isClosed = false;
    itemR_bool.read( fs, isClosed );

    pItem[ i ] = LightCurve3d< float >( points, isClosed );

  }

}


}


//
// class DefaultAsciiItemReader< LightCurve3d< double > >
//

namespace gkg
{


template <>
inline
DefaultAsciiItemReader< LightCurve3d< double > >::DefaultAsciiItemReader()
                                                 : ItemReader<
                                                      LightCurve3d< double > >()
{
}


template <>
inline
DefaultAsciiItemReader< LightCurve3d< double > >::~DefaultAsciiItemReader()
{
}


template <>
inline
void DefaultAsciiItemReader< LightCurve3d< double > >::read(
                                                  std::istream & is,
                                                  LightCurve3d< double >* pItem,
                                                  size_t count ) const
{

  ItemReader< std::vector< Vector3d< double > > >& itemR_vector_of_Vector3d =
    StdVectorAsciiItemReader< Vector3d< double > >::getInstance();
  ItemReader< bool >& itemR_bool =
    DefaultAsciiItemReader< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::vector< Vector3d< double > > points;
    itemR_vector_of_Vector3d.read( is, points );

    bool isClosed = false;
    itemR_bool.read( is, isClosed );

    pItem[ i ] = LightCurve3d< double >( points, isClosed );

  }

}


template <>
inline
void DefaultAsciiItemReader< LightCurve3d< double > >::read(
                                                  largefile_ifstream & is,
                                                  LightCurve3d< double >* pItem,
                                                  size_t count ) const
{

  ItemReader< std::vector< Vector3d< double > > >& itemR_vector_of_Vector3d =
    StdVectorAsciiItemReader< Vector3d< double > >::getInstance();
  ItemReader< bool >& itemR_bool =
    DefaultAsciiItemReader< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::vector< Vector3d< double > > points;
    itemR_vector_of_Vector3d.read( is, points );

    bool isClosed = false;
    itemR_bool.read( is, isClosed );

    pItem[ i ] = LightCurve3d< double >( points, isClosed );

  }

}


template <>
inline
void DefaultAsciiItemReader< LightCurve3d< double > >::read(
                                                  largefile_fstream & fs,
                                                  LightCurve3d< double >* pItem,
                                                  size_t count ) const
{

  ItemReader< std::vector< Vector3d< double > > >& itemR_vector_of_Vector3d =
    StdVectorAsciiItemReader< Vector3d< double > >::getInstance();
  ItemReader< bool >& itemR_bool =
    DefaultAsciiItemReader< bool >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::vector< Vector3d< double > > points;
    itemR_vector_of_Vector3d.read( fs, points );

    bool isClosed = false;
    itemR_bool.read( fs, isClosed );

    pItem[ i ] = LightCurve3d< double >( points, isClosed );

  }

}


}


#endif
