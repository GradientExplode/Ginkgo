#ifndef _gkg_dmri_io_DefaultBinaryItemWriterOfTwoPoolModel_i_h_
#define _gkg_dmri_io_DefaultBinaryItemWriterOfTwoPoolModel_i_h_


#include <gkg-core-io/DefaultBinaryItemWriter_i.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModel.h>
#include <gkg-processing-io/VectorBinaryItemWriter.h>


//
// class DefaultBinaryItemWriter< TwoPoolModel >
//

namespace gkg
{


template <>
inline
DefaultBinaryItemWriter< TwoPoolModel >::DefaultBinaryItemWriter()
                                        : ItemWriter< TwoPoolModel >()
{
}


template <>
inline
DefaultBinaryItemWriter< TwoPoolModel >::~DefaultBinaryItemWriter()
{
}


template <>
inline
void DefaultBinaryItemWriter< TwoPoolModel >::write( std::ostream & os,
                                                     const TwoPoolModel* pItem,
                                                     size_t count ) const
{

  ItemWriter< Vector >& itemW_TwoPoolModel =
    VectorBinaryItemWriter::getInstance();
  ItemWriter< float >& itemW_float =
    DefaultBinaryItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_TwoPoolModel.write( os, pItem[ i ].getCoefficients() );
    itemW_float.write( os, pItem[ i ].getDiffusionTime() );

  }

}


template <>
inline
void DefaultBinaryItemWriter< TwoPoolModel >::write( largefile_ofstream & os,
                                                     const TwoPoolModel* pItem,
                                                     size_t count ) const
{

  ItemWriter< Vector >& itemW_TwoPoolModel =
    VectorBinaryItemWriter::getInstance();
  ItemWriter< float >& itemW_float =
    DefaultBinaryItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_TwoPoolModel.write( os, pItem[ i ].getCoefficients() );
    itemW_float.write( os, pItem[ i ].getDiffusionTime() );

  }

}


template <>
inline
void DefaultBinaryItemWriter< TwoPoolModel >::write( largefile_fstream & fs,
                                                     const TwoPoolModel* pItem,
                                                     size_t count ) const
{

  ItemWriter< Vector >& itemW_TwoPoolModel =
    VectorBinaryItemWriter::getInstance();
  ItemWriter< float >& itemW_float =
    DefaultBinaryItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_TwoPoolModel.write( fs, pItem[ i ].getCoefficients() );
    itemW_float.write( fs, pItem[ i ].getDiffusionTime() );

  }

}


}


#endif
