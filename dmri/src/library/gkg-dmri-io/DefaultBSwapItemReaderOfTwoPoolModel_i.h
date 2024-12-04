#ifndef _gkg_dmri_io_DefaultBSwapItemReaderOfTwoPoolModel_i_h_
#define _gkg_dmri_io_DefaultBSwapItemReaderOfTwoPoolModel_i_h_

#include <gkg-core-io/DefaultBSwapItemReader_i.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModel.h>
#include <gkg-processing-io/VectorBSwapItemReader.h>


//
// class DefaultBSwapItemReader< TwoPoolModel >
//

namespace gkg
{


template <>
inline
DefaultBSwapItemReader< TwoPoolModel >::DefaultBSwapItemReader()
                                       : ItemReader< TwoPoolModel >()
{
}


template <>
inline
DefaultBSwapItemReader< TwoPoolModel >::~DefaultBSwapItemReader()
{
}


template <>
inline
void DefaultBSwapItemReader< TwoPoolModel >::read( std::istream & is,
                                                   TwoPoolModel* pItem,
                                                   size_t count ) const
{

  ItemReader< Vector >& itemR_TwoPoolModel =
    VectorBSwapItemReader::getInstance();
  ItemReader< float >& itemR_float =
    DefaultBSwapItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    Vector coefficients;
    itemR_TwoPoolModel.read( is, coefficients );

    float diffusionTime;
    itemR_float.read( is, diffusionTime );

    pItem[ i ].setCoefficients( coefficients, diffusionTime );

  }

}


template <>
inline
void DefaultBSwapItemReader< TwoPoolModel >::read( largefile_ifstream & is,
                                                   TwoPoolModel* pItem,
                                                   size_t count ) const
{

  ItemReader< Vector >& itemR_TwoPoolModel =
    VectorBSwapItemReader::getInstance();
  ItemReader< float >& itemR_float =
    DefaultBSwapItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    Vector coefficients;
    itemR_TwoPoolModel.read( is, coefficients );

    float diffusionTime;
    itemR_float.read( is, diffusionTime );

    pItem[ i ].setCoefficients( coefficients, diffusionTime );

  }

}


template <>
inline
void DefaultBSwapItemReader< TwoPoolModel >::read( largefile_fstream & fs,
                                                   TwoPoolModel* pItem,
                                                   size_t count ) const
{

  ItemReader< Vector >& itemR_TwoPoolModel =
    VectorBSwapItemReader::getInstance();
  ItemReader< float >& itemR_float =
    DefaultBSwapItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    Vector coefficients;
    itemR_TwoPoolModel.read( fs, coefficients );

    float diffusionTime;
    itemR_float.read( fs, diffusionTime );

    pItem[ i ].setCoefficients( coefficients, diffusionTime );

  }

}


}


#endif
