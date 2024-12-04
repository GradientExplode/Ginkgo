#ifndef _gkg_processing_io_DefaultBSwapItemReaderOfCurve3dMap_i_h_
#define _gkg_processing_io_DefaultBSwapItemReaderOfCurve3dMap_i_h_

#include <gkg-core-io/DefaultBSwapItemReader_i.h>
#include <gkg-processing-container/Curve3dMap.h>
#include <gkg-core-io/StdVectorBSwapItemReader.h>


//
// class DefaultBSwapItemReader< Curve3dMap< float > >
//

namespace gkg
{


template <>
inline
DefaultBSwapItemReader< Curve3dMap< float > >::DefaultBSwapItemReader()
                                           : ItemReader< Curve3dMap< float > >()
{
}


template <>
inline
DefaultBSwapItemReader< Curve3dMap< float > >::~DefaultBSwapItemReader()
{
}


template <>
inline
void DefaultBSwapItemReader< Curve3dMap< float > >::read(
                                                     std::istream & is,
                                                     Curve3dMap< float >* pItem,
                                                     size_t count ) const
{

  ItemReader< std::vector< LightCurve3d< float > > >& itemR_vector_of_Curve3d =
    StdVectorBSwapItemReader< LightCurve3d< float > >::getInstance();
  
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
void DefaultBSwapItemReader< Curve3dMap< float > >::read(
                                                     largefile_ifstream & is,
                                                     Curve3dMap< float >* pItem,
                                                     size_t count ) const
{

  ItemReader< std::vector< LightCurve3d< float > > >& itemR_vector_of_Curve3d =
    StdVectorBSwapItemReader< LightCurve3d< float > >::getInstance();
  
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
void DefaultBSwapItemReader< Curve3dMap< float > >::read(
                                                     largefile_fstream & fs,
                                                     Curve3dMap< float >* pItem,
                                                     size_t count ) const
{

  ItemReader< std::vector< LightCurve3d< float > > >& itemR_vector_of_Curve3d =
    StdVectorBSwapItemReader< LightCurve3d< float > >::getInstance();
  
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
// class DefaultBSwapItemReader< Curve3dMap< double > >
//

namespace gkg
{


template <>
inline
DefaultBSwapItemReader< Curve3dMap< double > >::DefaultBSwapItemReader()
                                          : ItemReader< Curve3dMap< double > >()
{
}


template <>
inline
DefaultBSwapItemReader< Curve3dMap< double > >::~DefaultBSwapItemReader()
{
}


template <>
inline
void DefaultBSwapItemReader< Curve3dMap< double > >::read(
                                                    std::istream & is,
                                                    Curve3dMap< double >* pItem,
                                                    size_t count ) const
{

  ItemReader< std::vector< LightCurve3d< double > > >& itemR_vector_of_Curve3d =
    StdVectorBSwapItemReader< LightCurve3d< double > >::getInstance();
  
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
void DefaultBSwapItemReader< Curve3dMap< double > >::read(
                                                    largefile_ifstream & is,
                                                    Curve3dMap< double >* pItem,
                                                    size_t count ) const
{

  ItemReader< std::vector< LightCurve3d< double > > >& itemR_vector_of_Curve3d =
    StdVectorBSwapItemReader< LightCurve3d< double > >::getInstance();
  
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
void DefaultBSwapItemReader< Curve3dMap< double > >::read(
                                                    largefile_fstream & fs,
                                                    Curve3dMap< double >* pItem,
                                                    size_t count ) const
{

  ItemReader< std::vector< LightCurve3d< double > > >& itemR_vector_of_Curve3d =
    StdVectorBSwapItemReader< LightCurve3d< double > >::getInstance();
  
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
