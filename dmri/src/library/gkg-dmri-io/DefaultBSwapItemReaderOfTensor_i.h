#ifndef _gkg_dmri_io_DefaultBSwapItemReaderOfTensor_i_h_
#define _gkg_dmri_io_DefaultBSwapItemReaderOfTensor_i_h_

#include <gkg-core-io/DefaultBSwapItemReader_i.h>
#include <gkg-dmri-pdf-dti/Tensor.h>
#include <gkg-processing-io/VectorBSwapItemReader.h>


//
// class DefaultBSwapItemReader< Tensor >
//

namespace gkg
{


template <>
inline
DefaultBSwapItemReader< Tensor >::DefaultBSwapItemReader()
                                 : ItemReader< Tensor >()
{
}


template <>
inline
DefaultBSwapItemReader< Tensor >::~DefaultBSwapItemReader()
{
}


template <>
inline
void DefaultBSwapItemReader< Tensor >::read( std::istream & is,
                                             Tensor* pItem,
                                             size_t count ) const
{

  ItemReader< Vector >& itemR_Tensor =
    VectorBSwapItemReader::getInstance();
  ItemReader< float >& itemR_float =
    DefaultBSwapItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    Vector coefficients;
    itemR_Tensor.read( is, coefficients );

    float diffusionTime;
    itemR_float.read( is, diffusionTime );

    pItem[ i ] = Tensor( 0, coefficients, diffusionTime );

  }

}


template <>
inline
void DefaultBSwapItemReader< Tensor >::read( largefile_ifstream & is,
                                             Tensor* pItem,
                                             size_t count ) const
{

  ItemReader< Vector >& itemR_Tensor =
    VectorBSwapItemReader::getInstance();
  ItemReader< float >& itemR_float =
    DefaultBSwapItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    Vector coefficients;
    itemR_Tensor.read( is, coefficients );

    float diffusionTime;
    itemR_float.read( is, diffusionTime );

    pItem[ i ] = Tensor( 0, coefficients, diffusionTime );

  }

}


template <>
inline
void DefaultBSwapItemReader< Tensor >::read( largefile_fstream & fs,
                                             Tensor* pItem,
                                             size_t count ) const
{

  ItemReader< Vector >& itemR_Tensor =
    VectorBSwapItemReader::getInstance();
  ItemReader< float >& itemR_float =
    DefaultBSwapItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    Vector coefficients;
    itemR_Tensor.read( fs, coefficients );

    float diffusionTime;
    itemR_float.read( fs, diffusionTime );

    pItem[ i ] = Tensor( 0, coefficients, diffusionTime );

  }

}


}


#endif
