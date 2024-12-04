#ifndef _gkg_processing_io_DefaultBinaryItemReaderOfCurve3d_i_h_
#define _gkg_processing_io_DefaultBinaryItemReaderOfCurve3d_i_h_

#include <gkg-core-io/DefaultBinaryItemReader_i.h>
#include <gkg-processing-container/Curve3d.h>
#include <gkg-core-io/StdVectorBinaryItemReader.h>


//
// class DefaultBinaryItemReader< Curve3d< float > >
//

namespace gkg
{


template <>
inline
DefaultBinaryItemReader< Curve3d< float > >::DefaultBinaryItemReader()
                                            : ItemReader< Curve3d< float > >()
{
}


template <>
inline
DefaultBinaryItemReader< Curve3d< float > >::~DefaultBinaryItemReader()
{
}


template <>
inline
void DefaultBinaryItemReader< Curve3d< float > >::read( std::istream & is,
                                                        Curve3d< float >* pItem,
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

    pItem[ i ] = Curve3d< float >( points, isClosed );

  }

}


template <>
inline
void DefaultBinaryItemReader< Curve3d< float > >::read( largefile_ifstream & is,
                                                        Curve3d< float >* pItem,
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

    pItem[ i ] = Curve3d< float >( points, isClosed );

  }

}


template <>
inline
void DefaultBinaryItemReader< Curve3d< float > >::read( largefile_fstream & fs,
                                                        Curve3d< float >* pItem,
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

    pItem[ i ] = Curve3d< float >( points, isClosed );

  }

}


}


//
// class DefaultBinaryItemReader< Curve3d< double > >
//

namespace gkg
{


template <>
inline
DefaultBinaryItemReader< Curve3d< double > >::DefaultBinaryItemReader()
                                             : ItemReader< Curve3d< double > >()
{
}


template <>
inline
DefaultBinaryItemReader< Curve3d< double > >::~DefaultBinaryItemReader()
{
}


template <>
inline
void DefaultBinaryItemReader< Curve3d< double > >::read(
                                                       std::istream & is,
                                                       Curve3d< double >* pItem,
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

    pItem[ i ] = Curve3d< double >( points, isClosed );

  }

}


template <>
inline
void DefaultBinaryItemReader< Curve3d< double > >::read(
                                                       largefile_ifstream & is,
                                                       Curve3d< double >* pItem,
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

    pItem[ i ] = Curve3d< double >( points, isClosed );

  }

}


template <>
inline
void DefaultBinaryItemReader< Curve3d< double > >::read(
                                                       largefile_fstream & fs,
                                                       Curve3d< double >* pItem,
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

    pItem[ i ] = Curve3d< double >( points, isClosed );

  }

}


}


#endif
