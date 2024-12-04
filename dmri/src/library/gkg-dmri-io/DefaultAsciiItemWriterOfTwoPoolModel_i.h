#ifndef _gkg_dmri_io_DefaultAsciiItemWriterOfTwoPoolModel_i_h_
#define _gkg_dmri_io_DefaultAsciiItemWriterOfTwoPoolModel_i_h_


#include <gkg-core-io/DefaultAsciiItemWriter_i.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModel.h>
#include <gkg-processing-io/VectorAsciiItemWriter.h>


//
// class DefaultAsciiItemWriter< TwoPoolModel >
//

namespace gkg
{


template <>
inline
DefaultAsciiItemWriter< TwoPoolModel >::DefaultAsciiItemWriter()
                                       : ItemWriter< TwoPoolModel >()
{
}


template <>
inline
DefaultAsciiItemWriter< TwoPoolModel >::~DefaultAsciiItemWriter()
{
}


template <>
inline
void DefaultAsciiItemWriter< TwoPoolModel >::write( std::ostream & os,
                                                    const TwoPoolModel* pItem,
                                                    size_t count ) const
{

  ItemWriter< Vector >& itemW_TwoPoolModel =
    VectorAsciiItemWriter::getInstance();
  ItemWriter< float >& itemW_float =
    DefaultAsciiItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_TwoPoolModel.write( os, pItem[ i ].getCoefficients() );
    itemW_float.write( os, pItem[ i ].getDiffusionTime() );

  }

}


template <>
inline
void DefaultAsciiItemWriter< TwoPoolModel >::write( largefile_ofstream & os,
                                                    const TwoPoolModel* pItem,
                                                    size_t count ) const
{

  ItemWriter< Vector >& itemW_TwoPoolModel =
    VectorAsciiItemWriter::getInstance();
  ItemWriter< float >& itemW_float =
    DefaultAsciiItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_TwoPoolModel.write( os, pItem[ i ].getCoefficients() );
    itemW_float.write( os, pItem[ i ].getDiffusionTime() );

  }

}


template <>
inline
void DefaultAsciiItemWriter< TwoPoolModel >::write( largefile_fstream & fs,
                                                    const TwoPoolModel* pItem,
                                                    size_t count ) const
{

  ItemWriter< Vector >& itemW_TwoPoolModel =
    VectorAsciiItemWriter::getInstance();
  ItemWriter< float >& itemW_float =
    DefaultAsciiItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_TwoPoolModel.write( fs, pItem[ i ].getCoefficients() );
    itemW_float.write( fs, pItem[ i ].getDiffusionTime() );

  }

}


}


#endif
