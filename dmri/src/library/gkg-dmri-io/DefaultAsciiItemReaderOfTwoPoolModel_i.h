#ifndef _gkg_dmri_io_DefaultAsciiItemReaderOfTwoPoolModel_i_h_
#define _gkg_dmri_io_DefaultAsciiItemReaderOfTwoPoolModel_i_h_

#include <gkg-core-io/DefaultAsciiItemReader_i.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModel.h>
#include <gkg-processing-io/VectorAsciiItemReader.h>


//
// class DefaultAsciiItemReader< TwoPoolModel >
//

namespace gkg
{


template <>
inline
DefaultAsciiItemReader< TwoPoolModel >::DefaultAsciiItemReader()
                                       : ItemReader< TwoPoolModel >()
{
}


template <>
inline
DefaultAsciiItemReader< TwoPoolModel >::~DefaultAsciiItemReader()
{
}


template <>
inline
void DefaultAsciiItemReader< TwoPoolModel >::read( std::istream & is,
                                                   TwoPoolModel* pItem,
                                                   size_t count ) const
{

  ItemReader< Vector >& itemR_TwoPoolModel =
    VectorAsciiItemReader::getInstance();
  ItemReader< float >& itemR_float =
    DefaultAsciiItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    Vector coefficients;
    itemR_TwoPoolModel.read( is, coefficients );

    float diffusionTime = 1.0f;
    itemR_float.read( is, diffusionTime );

    pItem[ i ].setCoefficients( coefficients, diffusionTime );

  }

}


template <>
inline
void DefaultAsciiItemReader< TwoPoolModel >::read( largefile_ifstream & is,
                                                   TwoPoolModel* pItem,
                                                   size_t count ) const
{

  ItemReader< Vector >& itemR_TwoPoolModel =
    VectorAsciiItemReader::getInstance();
  ItemReader< float >& itemR_float =
    DefaultAsciiItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    Vector coefficients;
    itemR_TwoPoolModel.read( is, coefficients );

    float diffusionTime= 1.0f;
    itemR_float.read( is, diffusionTime );

    pItem[ i ].setCoefficients( coefficients, diffusionTime );

  }

}


template <>
inline
void DefaultAsciiItemReader< TwoPoolModel >::read( largefile_fstream & fs,
                                                   TwoPoolModel* pItem,
                                                   size_t count ) const
{

  ItemReader< Vector >& itemR_TwoPoolModel =
    VectorAsciiItemReader::getInstance();
  ItemReader< float >& itemR_float =
    DefaultAsciiItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    Vector coefficients;
    itemR_TwoPoolModel.read( fs, coefficients );

    float diffusionTime = 1.0f;
    itemR_float.read( fs, diffusionTime );

    pItem[ i ].setCoefficients( coefficients, diffusionTime );

  }

}


}


#endif
