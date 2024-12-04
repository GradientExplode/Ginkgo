#ifndef _gkg_dmri_io_DefaultAsciiItemWriterOfPdf_i_h_
#define _gkg_dmri_io_DefaultAsciiItemWriterOfPdf_i_h_


#include <gkg-core-io/DefaultAsciiItemWriter_i.h>
#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-core-io/StdVectorAsciiItemWriter_i.h>

#include <gkg-dmri-io/DefaultAsciiItemWriterOfDiffusionFeature_i.h>


//
// class DefaultAsciiItemWriter< ProbabilityDensityFunction >
//

namespace gkg
{

template <>
inline
DefaultAsciiItemWriter< ProbabilityDensityFunction
                      >::DefaultAsciiItemWriter()
                        : ItemWriter< ProbabilityDensityFunction >()
{
}


template <>
inline
DefaultAsciiItemWriter< ProbabilityDensityFunction
                      >::~DefaultAsciiItemWriter()
{
}


template <>
inline
void 
DefaultAsciiItemWriter< ProbabilityDensityFunction >::write(
                                   std::ostream & os,
                                   const ProbabilityDensityFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< DiffusionFeature >& itemW_DiffusionFeature =
    DefaultAsciiItemWriter< DiffusionFeature >::getInstance();

  itemW_DiffusionFeature.write( os, pItem, count );

}


template <>
inline
void 
DefaultAsciiItemWriter< ProbabilityDensityFunction >::write(
                                   largefile_ofstream & os,
                                   const ProbabilityDensityFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< DiffusionFeature >& itemW_DiffusionFeature =
    DefaultAsciiItemWriter< DiffusionFeature >::getInstance();

  itemW_DiffusionFeature.write( os, pItem, count );

}


template <>
inline
void
DefaultAsciiItemWriter< ProbabilityDensityFunction >::write(
                                   largefile_fstream & fs,
                                   const ProbabilityDensityFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< DiffusionFeature >& itemW_DiffusionFeature =
    DefaultAsciiItemWriter< DiffusionFeature >::getInstance();

  itemW_DiffusionFeature.write( fs, pItem, count );

}


}


#endif
