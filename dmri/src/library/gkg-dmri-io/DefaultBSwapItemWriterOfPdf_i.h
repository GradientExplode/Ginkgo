#ifndef _gkg_dmri_io_DefaultBSwapItemWriterOfPdf_i_h_
#define _gkg_dmri_io_DefaultBSwapItemWriterOfPdf_i_h_


#include <gkg-core-io/DefaultBSwapItemWriter_i.h>
#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-core-io/StdVectorBSwapItemWriter_i.h>

#include <gkg-dmri-io/DefaultBSwapItemWriterOfDiffusionFeature_i.h>

//
// class DefaultBSwapItemWriter< ProbabilityDensityFunction >
//

namespace gkg
{

template <>
inline
DefaultBSwapItemWriter< ProbabilityDensityFunction
                      >::DefaultBSwapItemWriter()
                        : ItemWriter< ProbabilityDensityFunction >()
{
}


template <>
inline
DefaultBSwapItemWriter< ProbabilityDensityFunction
                      >::~DefaultBSwapItemWriter()
{
}


template <>
inline
void 
DefaultBSwapItemWriter< ProbabilityDensityFunction >::write(
                                   std::ostream & os,
                                   const ProbabilityDensityFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< DiffusionFeature >& itemW_DiffusionFeature =
    DefaultBSwapItemWriter< DiffusionFeature >::getInstance();

  itemW_DiffusionFeature.write( os, pItem, count );

}


template <>
inline
void 
DefaultBSwapItemWriter< ProbabilityDensityFunction >::write(
                                   largefile_ofstream & os,
                                   const ProbabilityDensityFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< DiffusionFeature >& itemW_DiffusionFeature =
    DefaultBSwapItemWriter< DiffusionFeature >::getInstance();

  itemW_DiffusionFeature.write( os, pItem, count );

}


template <>
inline
void
DefaultBSwapItemWriter< ProbabilityDensityFunction >::write(
                                   largefile_fstream & fs,
                                   const ProbabilityDensityFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< DiffusionFeature >& itemW_DiffusionFeature =
    DefaultBSwapItemWriter< DiffusionFeature >::getInstance();

  itemW_DiffusionFeature.write( fs, pItem, count );

}


}


#endif
