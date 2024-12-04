#ifndef _gkg_dmri_io_DefaultAsciiItemWriterOfTensor_i_h_
#define _gkg_dmri_io_DefaultAsciiItemWriterOfTensor_i_h_


#include <gkg-core-io/DefaultAsciiItemWriter_i.h>
#include <gkg-dmri-pdf-dti/Tensor.h>
#include <gkg-processing-io/VectorAsciiItemWriter.h>


//
// class DefaultAsciiItemWriter< Tensor >
//

namespace gkg
{


template <>
inline
DefaultAsciiItemWriter< Tensor >::DefaultAsciiItemWriter()
                                 : ItemWriter< Tensor >()
{
}


template <>
inline
DefaultAsciiItemWriter< Tensor >::~DefaultAsciiItemWriter()
{
}


template <>
inline
void DefaultAsciiItemWriter< Tensor >::write( std::ostream & os,
                                              const Tensor* pItem,
                                              size_t count ) const
{

  ItemWriter< Vector >& itemW_Tensor =
    VectorAsciiItemWriter::getInstance();
  ItemWriter< float >& itemW_float =
    DefaultAsciiItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_Tensor.write( os, pItem[ i ].getCoefficients() );
    itemW_float.write( os, pItem[ i ].getDiffusionTime() );

  }

}


template <>
inline
void DefaultAsciiItemWriter< Tensor >::write( largefile_ofstream & os,
                                              const Tensor* pItem,
                                              size_t count ) const
{

  ItemWriter< Vector >& itemW_Tensor =
    VectorAsciiItemWriter::getInstance();
  ItemWriter< float >& itemW_float =
    DefaultAsciiItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_Tensor.write( os, pItem[ i ].getCoefficients() );
    itemW_float.write( os, pItem[ i ].getDiffusionTime() );

  }

}


template <>
inline
void DefaultAsciiItemWriter< Tensor >::write( largefile_fstream & fs,
                                              const Tensor* pItem,
                                              size_t count ) const
{

  ItemWriter< Vector >& itemW_Tensor =
    VectorAsciiItemWriter::getInstance();
  ItemWriter< float >& itemW_float =
    DefaultAsciiItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_Tensor.write( fs, pItem[ i ].getCoefficients() );
    itemW_float.write( fs, pItem[ i ].getDiffusionTime() );

  }

}


}


#endif
