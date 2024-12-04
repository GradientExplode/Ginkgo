#ifndef _gkg_dmri_io_DefaultBinaryItemWriterOfTensor_i_h_
#define _gkg_dmri_io_DefaultBinaryItemWriterOfTensor_i_h_


#include <gkg-core-io/DefaultBinaryItemWriter_i.h>
#include <gkg-dmri-pdf-dti/Tensor.h>
#include <gkg-processing-io/VectorBinaryItemWriter.h>


//
// class DefaultBinaryItemWriter< Tensor >
//

namespace gkg
{


template <>
inline
DefaultBinaryItemWriter< Tensor >::DefaultBinaryItemWriter()
                                  : ItemWriter< Tensor >()
{
}


template <>
inline
DefaultBinaryItemWriter< Tensor >::~DefaultBinaryItemWriter()
{
}


template <>
inline
void DefaultBinaryItemWriter< Tensor >::write( std::ostream & os,
                                               const Tensor* pItem,
                                               size_t count ) const
{

  ItemWriter< Vector >& itemW_Tensor =
    VectorBinaryItemWriter::getInstance();
  ItemWriter< float >& itemW_float =
    DefaultBinaryItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_Tensor.write( os, pItem[ i ].getCoefficients() );
    itemW_float.write( os, pItem[ i ].getDiffusionTime() );

  }

}


template <>
inline
void DefaultBinaryItemWriter< Tensor >::write( largefile_ofstream & os,
                                               const Tensor* pItem,
                                               size_t count ) const
{

  ItemWriter< Vector >& itemW_Tensor =
    VectorBinaryItemWriter::getInstance();
  ItemWriter< float >& itemW_float =
    DefaultBinaryItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_Tensor.write( os, pItem[ i ].getCoefficients() );
    itemW_float.write( os, pItem[ i ].getDiffusionTime() );

  }

}


template <>
inline
void DefaultBinaryItemWriter< Tensor >::write( largefile_fstream & fs,
                                               const Tensor* pItem,
                                               size_t count ) const
{

  ItemWriter< Vector >& itemW_Tensor =
    VectorBinaryItemWriter::getInstance();
  ItemWriter< float >& itemW_float =
    DefaultBinaryItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_Tensor.write( fs, pItem[ i ].getCoefficients() );
    itemW_float.write( fs, pItem[ i ].getDiffusionTime() );

  }

}


}


#endif
