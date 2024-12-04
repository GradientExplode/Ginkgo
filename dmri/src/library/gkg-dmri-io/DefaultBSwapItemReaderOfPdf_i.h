#ifndef _gkg_dmri_io_DefaultBSwapItemReaderOfPdf_i_h_
#define _gkg_dmri_io_DefaultBSwapItemReaderOfPdf_i_h_


#include <gkg-core-io/DefaultBSwapItemReader_i.h>
#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-core-io/StdVectorBSwapItemReader_i.h>


#include <gkg-dmri-io/DefaultAsciiItemReaderOfDiffusionFeature_i.h>


//
// class DefaultBSwapItemReader< ProbabilityDensityFunction >
//

namespace gkg
{

template <>
inline
DefaultBSwapItemReader< ProbabilityDensityFunction
                      >::DefaultBSwapItemReader()
                        : ItemReader< ProbabilityDensityFunction >()
{
}


template <>
inline
DefaultBSwapItemReader< ProbabilityDensityFunction
                      >::~DefaultBSwapItemReader()
{
}


template <>
inline
void DefaultBSwapItemReader< ProbabilityDensityFunction >::read(
                                         std::istream & is,
                                         ProbabilityDensityFunction* pItem,
                                         size_t count ) const
{

  ItemReader< DiffusionFeature >& itemR_DiffusionFeature =
    DefaultBSwapItemReader< DiffusionFeature >::getInstance();

  itemR_DiffusionFeature.read( is, pItem, count );

}


template <>
inline
void DefaultBSwapItemReader< ProbabilityDensityFunction >::read(
                                         largefile_ifstream & is,
                                         ProbabilityDensityFunction* pItem,
                                         size_t count ) const
{

  ItemReader< DiffusionFeature >& itemR_DiffusionFeature =
    DefaultBSwapItemReader< DiffusionFeature >::getInstance();

  itemR_DiffusionFeature.read( is, pItem, count );

}


template <>
inline
void DefaultBSwapItemReader< ProbabilityDensityFunction >::read(
                                         largefile_fstream & fs,
                                         ProbabilityDensityFunction* pItem,
                                         size_t count ) const
{

  ItemReader< DiffusionFeature >& itemR_DiffusionFeature =
    DefaultBSwapItemReader< DiffusionFeature >::getInstance();

  itemR_DiffusionFeature.read( fs, pItem, count );

}


}


#endif
