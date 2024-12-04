#ifndef _gkg_dmri_io_DefaultBSwapItemWriterOfTensor_i_h_
#define _gkg_dmri_io_DefaultBSwapItemWriterOfTensor_i_h_


#include <gkg-core-io/DefaultBSwapItemWriter_i.h>
#include <gkg-dmri-pdf-dti/Tensor.h>
#include <gkg-processing-io/VectorBSwapItemWriter.h>


//
// class DefaultBSwapItemWriter< Tensor >
//

namespace gkg
{


template <>
inline
DefaultBSwapItemWriter< Tensor >::DefaultBSwapItemWriter()
                                 : ItemWriter< Tensor >()
{
}


template <>
inline
DefaultBSwapItemWriter< Tensor >::~DefaultBSwapItemWriter()
{
}


template <>
inline
void DefaultBSwapItemWriter< Tensor >::write( std::ostream & os,
                                              const Tensor* pItem,
                                              size_t count ) const
{

  ItemWriter< Vector >& itemW_Tensor =
    VectorBSwapItemWriter::getInstance();
  ItemWriter< float >& itemW_float =
    DefaultBSwapItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_Tensor.write( os, pItem[ i ].getCoefficients() );
    itemW_float.write( os, pItem[ i ].getDiffusionTime() );

  }

}


template <>
inline
void DefaultBSwapItemWriter< Tensor >::write( largefile_ofstream & os,
                                              const Tensor* pItem,
                                              size_t count ) const
{

  ItemWriter< Vector >& itemW_Tensor =
    VectorBSwapItemWriter::getInstance();
  ItemWriter< float >& itemW_float =
    DefaultBSwapItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_Tensor.write( os, pItem[ i ].getCoefficients() );
    itemW_float.write( os, pItem[ i ].getDiffusionTime() );

  }

}


template <>
inline
void DefaultBSwapItemWriter< Tensor >::write( largefile_fstream & fs,
                                              const Tensor* pItem,
                                              size_t count ) const
{

  ItemWriter< Vector >& itemW_Tensor =
    VectorBSwapItemWriter::getInstance();
  ItemWriter< float >& itemW_float =
    DefaultBSwapItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_Tensor.write( fs, pItem[ i ].getCoefficients() );
    itemW_float.write( fs, pItem[ i ].getDiffusionTime() );

  }

}


}


#endif
