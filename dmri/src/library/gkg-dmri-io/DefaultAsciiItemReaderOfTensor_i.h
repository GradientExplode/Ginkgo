#ifndef _gkg_dmri_io_DefaultAsciiItemReaderOfTensor_i_h_
#define _gkg_dmri_io_DefaultAsciiItemReaderOfTensor_i_h_

#include <gkg-core-io/DefaultAsciiItemReader_i.h>
#include <gkg-dmri-pdf-dti/Tensor.h>
#include <gkg-processing-io/VectorAsciiItemReader.h>


//
// class DefaultAsciiItemReader< Tensor >
//

namespace gkg
{


template <>
inline
DefaultAsciiItemReader< Tensor >::DefaultAsciiItemReader()
                                 : ItemReader< Tensor >()
{
}


template <>
inline
DefaultAsciiItemReader< Tensor >::~DefaultAsciiItemReader()
{
}


template <>
inline
void DefaultAsciiItemReader< Tensor >::read( std::istream & is,
                                             Tensor* pItem,
                                             size_t count ) const
{

  ItemReader< Vector >& itemR_Tensor =
    VectorAsciiItemReader::getInstance();
  ItemReader< float >& itemR_float =
    DefaultAsciiItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    Vector coefficients;
    itemR_Tensor.read( is, coefficients );

    float diffusionTime = 1.0f;
    itemR_float.read( is, diffusionTime );

    pItem[ i ] = Tensor( 0, coefficients, diffusionTime );

  }

}


template <>
inline
void DefaultAsciiItemReader< Tensor >::read( largefile_ifstream & is,
                                             Tensor* pItem,
                                             size_t count ) const
{

  ItemReader< Vector >& itemR_Tensor =
    VectorAsciiItemReader::getInstance();
  ItemReader< float >& itemR_float =
    DefaultAsciiItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    Vector coefficients;
    itemR_Tensor.read( is, coefficients );

    float diffusionTime = 1.0f;
    itemR_float.read( is, diffusionTime );

    pItem[ i ] = Tensor( 0, coefficients, diffusionTime );

  }

}


template <>
inline
void DefaultAsciiItemReader< Tensor >::read( largefile_fstream & fs,
                                             Tensor* pItem,
                                             size_t count ) const
{

  ItemReader< Vector >& itemR_Tensor =
    VectorAsciiItemReader::getInstance();
  ItemReader< float >& itemR_float =
    DefaultAsciiItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    Vector coefficients;
    itemR_Tensor.read( fs, coefficients );

    float diffusionTime = 1.0f;
    itemR_float.read( fs, diffusionTime );

    pItem[ i ] = Tensor( 0, coefficients, diffusionTime );

  }

}


}


#endif
