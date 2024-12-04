#ifndef _gkg_dmri_io_DefaultBinaryItemWriterOfPdf_i_h_
#define _gkg_dmri_io_DefaultBinaryItemWriterOfPdf_i_h_


#include <gkg-core-io/DefaultBinaryItemWriter_i.h>
#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-core-io/StdVectorBinaryItemWriter_i.h>

#include <gkg-dmri-io/DefaultBinaryItemWriterOfDiffusionFeature_i.h>


//
// class DefaultBinaryItemWriter< ProbabilityDensityFunction >
//

namespace gkg
{

template <>
inline
DefaultBinaryItemWriter< ProbabilityDensityFunction
                       >::DefaultBinaryItemWriter()
                         : ItemWriter< ProbabilityDensityFunction >()
{
}


template <>
inline
DefaultBinaryItemWriter< ProbabilityDensityFunction
                       >::~DefaultBinaryItemWriter()
{
}


template <>
inline
void 
DefaultBinaryItemWriter< ProbabilityDensityFunction >::write(
                                   std::ostream & os,
                                   const ProbabilityDensityFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< DiffusionFeature >& itemW_DiffusionFeature =
    DefaultBinaryItemWriter< DiffusionFeature >::getInstance();

  itemW_DiffusionFeature.write( os, pItem, count );

}


template <>
inline
void 
DefaultBinaryItemWriter< ProbabilityDensityFunction >::write(
                                   largefile_ofstream & os,
                                   const ProbabilityDensityFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< DiffusionFeature >& itemW_DiffusionFeature =
    DefaultBinaryItemWriter< DiffusionFeature >::getInstance();

  itemW_DiffusionFeature.write( os, pItem, count );

}


template <>
inline
void
DefaultBinaryItemWriter< ProbabilityDensityFunction >::write(
                                   largefile_fstream & fs,
                                   const ProbabilityDensityFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< DiffusionFeature >& itemW_DiffusionFeature =
    DefaultBinaryItemWriter< DiffusionFeature >::getInstance();

  itemW_DiffusionFeature.write( fs, pItem, count );

}


}


#endif
