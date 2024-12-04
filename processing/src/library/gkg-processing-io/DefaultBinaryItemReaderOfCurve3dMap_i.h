#ifndef _gkg_processing_io_DefaultBinaryItemReaderOfCurve3dMap_i_h_
#define _gkg_processing_io_DefaultBinaryItemReaderOfCurve3dMap_i_h_

#include <gkg-core-io/DefaultBinaryItemReader_i.h>
#include <gkg-processing-container/Curve3dMap.h>
#include <gkg-core-io/StdVectorBinaryItemReader.h>


//
// class DefaultBinaryItemReader< Curve3dMap< float > >
//

namespace gkg
{


template <>
inline
DefaultBinaryItemReader< Curve3dMap< float > >::DefaultBinaryItemReader()
                                           : ItemReader< Curve3dMap< float > >()
{
}


template <>
inline
DefaultBinaryItemReader< Curve3dMap< float > >::~DefaultBinaryItemReader()
{
}


template <>
inline
void DefaultBinaryItemReader< Curve3dMap< float > >::read(
                                                     std::istream & is,
                                                     Curve3dMap< float >* pItem,
                                                     size_t count ) const
{

  ItemReader< std::vector< LightCurve3d< float > > >& itemR_vector_of_Curve3d =
    StdVectorBinaryItemReader< LightCurve3d< float > >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::vector< LightCurve3d< float > > curve3ds;
    itemR_vector_of_Curve3d.read( is, curve3ds );

    pItem[ i ] = Curve3dMap< float >( curve3ds );

  }

}


template <>
inline
void DefaultBinaryItemReader< Curve3dMap< float > >::read(
                                                     largefile_ifstream & is,
                                                     Curve3dMap< float >* pItem,
                                                     size_t count ) const
{

  ItemReader< std::vector< LightCurve3d< float > > >& itemR_vector_of_Curve3d =
    StdVectorBinaryItemReader< LightCurve3d< float > >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::vector< LightCurve3d< float > > curve3ds;
    itemR_vector_of_Curve3d.read( is, curve3ds );

    pItem[ i ] = Curve3dMap< float >( curve3ds );

  }

}


template <>
inline
void DefaultBinaryItemReader< Curve3dMap< float > >::read(
                                                     largefile_fstream & fs,
                                                     Curve3dMap< float >* pItem,
                                                     size_t count ) const
{

  ItemReader< std::vector< LightCurve3d< float > > >& itemR_vector_of_Curve3d =
    StdVectorBinaryItemReader< LightCurve3d< float > >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::vector< LightCurve3d< float > > curve3ds;
    itemR_vector_of_Curve3d.read( fs, curve3ds );

    pItem[ i ] = Curve3dMap< float >( curve3ds );

  }

}


}


//
// class DefaultBinaryItemReader< Curve3dMap< double > >
//

namespace gkg
{


template <>
inline
DefaultBinaryItemReader< Curve3dMap< double > >::DefaultBinaryItemReader()
                                          : ItemReader< Curve3dMap< double > >()
{
}


template <>
inline
DefaultBinaryItemReader< Curve3dMap< double > >::~DefaultBinaryItemReader()
{
}


template <>
inline
void DefaultBinaryItemReader< Curve3dMap< double > >::read(
                                                    std::istream & is,
                                                    Curve3dMap< double >* pItem,
                                                    size_t count ) const
{

  ItemReader< std::vector< LightCurve3d< double > > >& itemR_vector_of_Curve3d =
    StdVectorBinaryItemReader< LightCurve3d< double > >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::vector< LightCurve3d< double > > curve3ds;
    itemR_vector_of_Curve3d.read( is, curve3ds );

    pItem[ i ] = Curve3dMap< double >( curve3ds );

  }

}


template <>
inline
void DefaultBinaryItemReader< Curve3dMap< double > >::read(
                                                    largefile_ifstream & is,
                                                    Curve3dMap< double >* pItem,
                                                    size_t count ) const
{

  ItemReader< std::vector< LightCurve3d< double > > >& itemR_vector_of_Curve3d =
    StdVectorBinaryItemReader< LightCurve3d< double > >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::vector< LightCurve3d< double > > curve3ds;
    itemR_vector_of_Curve3d.read( is, curve3ds );

    pItem[ i ] = Curve3dMap< double >( curve3ds );

  }

}


template <>
inline
void DefaultBinaryItemReader< Curve3dMap< double > >::read(
                                                    largefile_fstream & fs,
                                                    Curve3dMap< double >* pItem,
                                                    size_t count ) const
{

  ItemReader< std::vector< LightCurve3d< double > > >& itemR_vector_of_Curve3d =
    StdVectorBinaryItemReader< LightCurve3d< double > >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::vector< LightCurve3d< double > > curve3ds;
    itemR_vector_of_Curve3d.read( fs, curve3ds );

    pItem[ i ] = Curve3dMap< double >( curve3ds );

  }

}


}


#endif
