#ifndef _gkg_dmri_io_DefaultAsciiItemReaderOfOdf_i_h_
#define _gkg_dmri_io_DefaultAsciiItemReaderOfOdf_i_h_


#include <gkg-core-io/DefaultAsciiItemReader_i.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-io/StdVectorAsciiItemReader_i.h>

#include <gkg-dmri-io/DefaultAsciiItemReaderOfDiffusionFeature_i.h>

//
// class DefaultAsciiItemReader< OrientationDistributionFunction >
//

namespace gkg
{

template <>
inline
DefaultAsciiItemReader< OrientationDistributionFunction
                      >::DefaultAsciiItemReader()
                        : ItemReader< OrientationDistributionFunction >()
{
}


template <>
inline
DefaultAsciiItemReader< OrientationDistributionFunction
                      >::~DefaultAsciiItemReader()
{
}


template <>
inline
void DefaultAsciiItemReader< OrientationDistributionFunction >::read(
                                         std::istream & is,
                                         OrientationDistributionFunction* pItem,
                                         size_t count ) const
{

  ItemReader< DiffusionFeature >& itemR_DiffusionFeature =
    DefaultAsciiItemReader< DiffusionFeature >::getInstance();

  itemR_DiffusionFeature.read( is, pItem, count );

}


template <>
inline
void DefaultAsciiItemReader< OrientationDistributionFunction >::read(
                                         largefile_ifstream & is,
                                         OrientationDistributionFunction* pItem,
                                         size_t count ) const
{

  ItemReader< DiffusionFeature >& itemR_DiffusionFeature =
    DefaultAsciiItemReader< DiffusionFeature >::getInstance();

  itemR_DiffusionFeature.read( is, pItem, count );

}


template <>
inline
void DefaultAsciiItemReader< OrientationDistributionFunction >::read(
                                         largefile_fstream & fs,
                                         OrientationDistributionFunction* pItem,
                                         size_t count ) const
{

  ItemReader< DiffusionFeature >& itemR_DiffusionFeature =
    DefaultAsciiItemReader< DiffusionFeature >::getInstance();

  itemR_DiffusionFeature.read( fs, pItem, count );

}


}


#endif
