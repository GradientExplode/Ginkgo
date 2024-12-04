#ifndef _gkg_dmri_io_DefaultBinaryItemReaderOfTwoPoolModel_i_h_
#define _gkg_dmri_io_DefaultBinaryItemReaderOfTwoPoolModel_i_h_

#include <gkg-core-io/DefaultBinaryItemReader_i.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModel.h>
#include <gkg-processing-io/VectorBinaryItemReader.h>


//
// class DefaultBinaryItemReader< TwoPoolModel >
//

namespace gkg
{


template <>
inline
DefaultBinaryItemReader< TwoPoolModel >::DefaultBinaryItemReader()
                                        : ItemReader< TwoPoolModel >()
{
}


template <>
inline
DefaultBinaryItemReader< TwoPoolModel >::~DefaultBinaryItemReader()
{
}


template <>
inline
void DefaultBinaryItemReader< TwoPoolModel >::read( std::istream & is,
                                                    TwoPoolModel* pItem,
                                                    size_t count ) const
{

  ItemReader< Vector >& itemR_TwoPoolModel =
    VectorBinaryItemReader::getInstance();
  ItemReader< float >& itemR_float =
    DefaultBinaryItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    Vector coefficients;
    itemR_TwoPoolModel.read( is, coefficients );

    float diffusionTime;
    itemR_float.read( is, diffusionTime );

    pItem[ i ] .setCoefficients( coefficients, diffusionTime );

  }

}


template <>
inline
void DefaultBinaryItemReader< TwoPoolModel >::read( largefile_ifstream & is,
                                                    TwoPoolModel* pItem,
                                                    size_t count ) const
{

  ItemReader< Vector >& itemR_TwoPoolModel =
    VectorBinaryItemReader::getInstance();
  ItemReader< float >& itemR_float =
    DefaultBinaryItemReader< float >::getInstance();
  
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
void DefaultBinaryItemReader< TwoPoolModel >::read( largefile_fstream & fs,
                                                    TwoPoolModel* pItem,
                                                    size_t count ) const
{

  ItemReader< Vector >& itemR_TwoPoolModel =
    VectorBinaryItemReader::getInstance();
  ItemReader< float >& itemR_float =
    DefaultBinaryItemReader< float >::getInstance();
  
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
