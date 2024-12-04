#ifndef _gkg_dmri_io_DefaultBinaryItemReaderOfOdf_i_h_
#define _gkg_dmri_io_DefaultBinaryItemReaderOfOdf_i_h_


#include <gkg-core-io/DefaultBinaryItemReader_i.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-io/StdVectorBinaryItemReader_i.h>

#include <gkg-dmri-io/DefaultBinaryItemReaderOfDiffusionFeature_i.h>


//
// class DefaultBinaryItemReader< OrientationDistributionFunction >
//

namespace gkg
{

template <>
inline
DefaultBinaryItemReader< OrientationDistributionFunction
                       >::DefaultBinaryItemReader()
                        : ItemReader< OrientationDistributionFunction >()
{
}


template <>
inline
DefaultBinaryItemReader< OrientationDistributionFunction
                       >::~DefaultBinaryItemReader()
{
}


template <>
inline
void DefaultBinaryItemReader< OrientationDistributionFunction >::read(
                                         std::istream & is,
                                         OrientationDistributionFunction* pItem,
                                         size_t count ) const
{

  ItemReader< DiffusionFeature >& itemR_DiffusionFeature =
    DefaultBinaryItemReader< DiffusionFeature >::getInstance();

  itemR_DiffusionFeature.read( is, pItem, count );

}


template <>
inline
void DefaultBinaryItemReader< OrientationDistributionFunction >::read(
                                         largefile_ifstream & is,
                                         OrientationDistributionFunction* pItem,
                                         size_t count ) const
{

  ItemReader< DiffusionFeature >& itemR_DiffusionFeature =
    DefaultBinaryItemReader< DiffusionFeature >::getInstance();

  itemR_DiffusionFeature.read( is, pItem, count );

}


template <>
inline
void DefaultBinaryItemReader< OrientationDistributionFunction >::read(
                                         largefile_fstream & fs,
                                         OrientationDistributionFunction* pItem,
                                         size_t count ) const
{

  ItemReader< DiffusionFeature >& itemR_DiffusionFeature =
    DefaultBinaryItemReader< DiffusionFeature >::getInstance();

  itemR_DiffusionFeature.read( fs, pItem, count );

}


}


#endif
