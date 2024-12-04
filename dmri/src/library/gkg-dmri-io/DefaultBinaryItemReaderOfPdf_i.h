#ifndef _gkg_dmri_io_DefaultBinaryItemReaderOfPdf_i_h_
#define _gkg_dmri_io_DefaultBinaryItemReaderOfPdf_i_h_


#include <gkg-core-io/DefaultBinaryItemReader_i.h>
#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-core-io/StdVectorBinaryItemReader_i.h>

#include <gkg-dmri-io/DefaultBinaryItemReaderOfDiffusionFeature_i.h>


//
// class DefaultBinaryItemReader< ProbabilityDensityFunction >
//

namespace gkg
{

template <>
inline
DefaultBinaryItemReader< ProbabilityDensityFunction
                       >::DefaultBinaryItemReader()
                        : ItemReader< ProbabilityDensityFunction >()
{
}


template <>
inline
DefaultBinaryItemReader< ProbabilityDensityFunction
                       >::~DefaultBinaryItemReader()
{
}


template <>
inline
void DefaultBinaryItemReader< ProbabilityDensityFunction >::read(
                                         std::istream & is,
                                         ProbabilityDensityFunction* pItem,
                                         size_t count ) const
{

  ItemReader< DiffusionFeature >& itemR_DiffusionFeature =
    DefaultBinaryItemReader< DiffusionFeature >::getInstance();

  itemR_DiffusionFeature.read( is, pItem, count );

}


template <>
inline
void DefaultBinaryItemReader< ProbabilityDensityFunction >::read(
                                         largefile_ifstream & is,
                                         ProbabilityDensityFunction* pItem,
                                         size_t count ) const
{

  ItemReader< DiffusionFeature >& itemR_DiffusionFeature =
    DefaultBinaryItemReader< DiffusionFeature >::getInstance();

  itemR_DiffusionFeature.read( is, pItem, count );

}


template <>
inline
void DefaultBinaryItemReader< ProbabilityDensityFunction >::read(
                                         largefile_fstream & fs,
                                         ProbabilityDensityFunction* pItem,
                                         size_t count ) const
{

  ItemReader< DiffusionFeature >& itemR_DiffusionFeature =
    DefaultBinaryItemReader< DiffusionFeature >::getInstance();

  itemR_DiffusionFeature.read( fs, pItem, count );

}


}


#endif
