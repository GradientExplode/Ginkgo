#ifndef _gkg_dmri_io_DefaultAsciiItemReaderOfPdf_i_h_
#define _gkg_dmri_io_DefaultAsciiItemReaderOfPdf_i_h_


#include <gkg-core-io/DefaultAsciiItemReader_i.h>
#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-core-io/StdVectorAsciiItemReader_i.h>

#include <gkg-dmri-io/DefaultAsciiItemReaderOfDiffusionFeature_i.h>

//
// class DefaultAsciiItemReader< ProbabilityDensityFunction >
//

namespace gkg
{

template <>
inline
DefaultAsciiItemReader< ProbabilityDensityFunction
                      >::DefaultAsciiItemReader()
                        : ItemReader< ProbabilityDensityFunction >()
{
}


template <>
inline
DefaultAsciiItemReader< ProbabilityDensityFunction
                      >::~DefaultAsciiItemReader()
{
}


template <>
inline
void DefaultAsciiItemReader< ProbabilityDensityFunction >::read(
                                         std::istream & is,
                                         ProbabilityDensityFunction* pItem,
                                         size_t count ) const
{

  ItemReader< DiffusionFeature >& itemR_DiffusionFeature =
    DefaultAsciiItemReader< DiffusionFeature >::getInstance();

  itemR_DiffusionFeature.read( is, pItem, count );

}


template <>
inline
void DefaultAsciiItemReader< ProbabilityDensityFunction >::read(
                                         largefile_ifstream & is,
                                         ProbabilityDensityFunction* pItem,
                                         size_t count ) const
{

  ItemReader< DiffusionFeature >& itemR_DiffusionFeature =
    DefaultAsciiItemReader< DiffusionFeature >::getInstance();

  itemR_DiffusionFeature.read( is, pItem, count );

}


template <>
inline
void DefaultAsciiItemReader< ProbabilityDensityFunction >::read(
                                         largefile_fstream & fs,
                                         ProbabilityDensityFunction* pItem,
                                         size_t count ) const
{

  ItemReader< DiffusionFeature >& itemR_DiffusionFeature =
    DefaultAsciiItemReader< DiffusionFeature >::getInstance();

  itemR_DiffusionFeature.read( fs, pItem, count );

}


}


#endif
