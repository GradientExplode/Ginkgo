#ifndef _gkg_dmri_io_DefaultBinaryItemReaderOfTensor_i_h_
#define _gkg_dmri_io_DefaultBinaryItemReaderOfTensor_i_h_

#include <gkg-core-io/DefaultBinaryItemReader_i.h>
#include <gkg-dmri-pdf-dti/Tensor.h>
#include <gkg-processing-io/VectorBinaryItemReader.h>


//
// class DefaultBinaryItemReader< Tensor >
//

namespace gkg
{


template <>
inline
DefaultBinaryItemReader< Tensor >::DefaultBinaryItemReader()
                                  : ItemReader< Tensor >()
{
}


template <>
inline
DefaultBinaryItemReader< Tensor >::~DefaultBinaryItemReader()
{
}


template <>
inline
void DefaultBinaryItemReader< Tensor >::read( std::istream & is,
                                              Tensor* pItem,
                                              size_t count ) const
{

  ItemReader< Vector >& itemR_Tensor =
    VectorBinaryItemReader::getInstance();
  ItemReader< float >& itemR_float =
    DefaultBinaryItemReader< float >::getInstance();
  
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
void DefaultBinaryItemReader< Tensor >::read( largefile_ifstream & is,
                                              Tensor* pItem,
                                              size_t count ) const
{

  ItemReader< Vector >& itemR_Tensor =
    VectorBinaryItemReader::getInstance();
  ItemReader< float >& itemR_float =
    DefaultBinaryItemReader< float >::getInstance();
  
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
void DefaultBinaryItemReader< Tensor >::read( largefile_fstream & fs,
                                              Tensor* pItem,
                                              size_t count ) const
{

  ItemReader< Vector >& itemR_Tensor =
    VectorBinaryItemReader::getInstance();
  ItemReader< float >& itemR_float =
    DefaultBinaryItemReader< float >::getInstance();
  
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
