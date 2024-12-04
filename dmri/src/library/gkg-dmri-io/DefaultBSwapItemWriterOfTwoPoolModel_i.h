#ifndef _gkg_dmri_io_DefaultBSwapItemWriterOfTwoPoolModel_i_h_
#define _gkg_dmri_io_DefaultBSwapItemWriterOfTwoPoolModel_i_h_


#include <gkg-core-io/DefaultBSwapItemWriter_i.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModel.h>
#include <gkg-processing-io/VectorBSwapItemWriter.h>


//
// class DefaultBSwapItemWriter< TwoPoolModel >
//

namespace gkg
{


template <>
inline
DefaultBSwapItemWriter< TwoPoolModel >::DefaultBSwapItemWriter()
                                       : ItemWriter< TwoPoolModel >()
{
}


template <>
inline
DefaultBSwapItemWriter< TwoPoolModel >::~DefaultBSwapItemWriter()
{
}


template <>
inline
void DefaultBSwapItemWriter< TwoPoolModel >::write( std::ostream & os,
                                                    const TwoPoolModel* pItem,
                                                    size_t count ) const
{

  ItemWriter< Vector >& itemW_TwoPoolModel =
    VectorBSwapItemWriter::getInstance();
  ItemWriter< float >& itemW_float =
    DefaultBSwapItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_TwoPoolModel.write( os, pItem[ i ].getCoefficients() );
    itemW_float.write( os, pItem[ i ].getDiffusionTime() );

  }

}


template <>
inline
void DefaultBSwapItemWriter< TwoPoolModel >::write( largefile_ofstream & os,
                                                    const TwoPoolModel* pItem,
                                                    size_t count ) const
{

  ItemWriter< Vector >& itemW_TwoPoolModel =
    VectorBSwapItemWriter::getInstance();
  ItemWriter< float >& itemW_float =
    DefaultBSwapItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_TwoPoolModel.write( os, pItem[ i ].getCoefficients() );
    itemW_float.write( os, pItem[ i ].getDiffusionTime() );

  }

}


template <>
inline
void DefaultBSwapItemWriter< TwoPoolModel >::write( largefile_fstream & fs,
                                                    const TwoPoolModel* pItem,
                                                    size_t count ) const
{ 

  ItemWriter< Vector >& itemW_TwoPoolModel =
    VectorBSwapItemWriter::getInstance();
  ItemWriter< float >& itemW_float =
    DefaultBSwapItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_TwoPoolModel.write( fs, pItem[ i ].getCoefficients() );
    itemW_float.write( fs, pItem[ i ].getDiffusionTime() );

  }

}


}


#endif
