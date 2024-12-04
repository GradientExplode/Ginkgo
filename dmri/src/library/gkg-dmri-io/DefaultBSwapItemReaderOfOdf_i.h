#ifndef _gkg_dmri_io_DefaultBSwapItemReaderOfOdf_i_h_
#define _gkg_dmri_io_DefaultBSwapItemReaderOfOdf_i_h_


#include <gkg-core-io/DefaultBSwapItemReader_i.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-io/StdVectorBSwapItemReader_i.h>


#include <gkg-dmri-io/DefaultAsciiItemReaderOfDiffusionFeature_i.h>


//
// class DefaultBSwapItemReader< OrientationDistributionFunction >
//

namespace gkg
{

template <>
inline
DefaultBSwapItemReader< OrientationDistributionFunction
                      >::DefaultBSwapItemReader()
                        : ItemReader< OrientationDistributionFunction >()
{
}


template <>
inline
DefaultBSwapItemReader< OrientationDistributionFunction
                      >::~DefaultBSwapItemReader()
{
}


template <>
inline
void DefaultBSwapItemReader< OrientationDistributionFunction >::read(
                                         std::istream & is,
                                         OrientationDistributionFunction* pItem,
                                         size_t count ) const
{

  ItemReader< DiffusionFeature >& itemR_DiffusionFeature =
    DefaultBSwapItemReader< DiffusionFeature >::getInstance();

  itemR_DiffusionFeature.read( is, pItem, count );

}


template <>
inline
void DefaultBSwapItemReader< OrientationDistributionFunction >::read(
                                         largefile_ifstream & is,
                                         OrientationDistributionFunction* pItem,
                                         size_t count ) const
{

  ItemReader< DiffusionFeature >& itemR_DiffusionFeature =
    DefaultBSwapItemReader< DiffusionFeature >::getInstance();

  itemR_DiffusionFeature.read( is, pItem, count );

}


template <>
inline
void DefaultBSwapItemReader< OrientationDistributionFunction >::read(
                                         largefile_fstream & fs,
                                         OrientationDistributionFunction* pItem,
                                         size_t count ) const
{

  ItemReader< DiffusionFeature >& itemR_DiffusionFeature =
    DefaultBSwapItemReader< DiffusionFeature >::getInstance();

  itemR_DiffusionFeature.read( fs, pItem, count );

}


}


#endif
