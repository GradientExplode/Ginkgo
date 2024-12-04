#ifndef _gkg_dmri_io_DefaultBSwapItemWriterOfOdf_i_h_
#define _gkg_dmri_io_DefaultBSwapItemWriterOfOdf_i_h_


#include <gkg-core-io/DefaultBSwapItemWriter_i.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-io/StdVectorBSwapItemWriter_i.h>

#include <gkg-dmri-io/DefaultBSwapItemWriterOfDiffusionFeature_i.h>

//
// class DefaultBSwapItemWriter< OrientationDistributionFunction >
//

namespace gkg
{

template <>
inline
DefaultBSwapItemWriter< OrientationDistributionFunction
                      >::DefaultBSwapItemWriter()
                        : ItemWriter< OrientationDistributionFunction >()
{
}


template <>
inline
DefaultBSwapItemWriter< OrientationDistributionFunction
                      >::~DefaultBSwapItemWriter()
{
}


template <>
inline
void 
DefaultBSwapItemWriter< OrientationDistributionFunction >::write(
                                   std::ostream & os,
                                   const OrientationDistributionFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< DiffusionFeature >& itemW_DiffusionFeature =
    DefaultBSwapItemWriter< DiffusionFeature >::getInstance();

  itemW_DiffusionFeature.write( os, pItem, count );

}


template <>
inline
void 
DefaultBSwapItemWriter< OrientationDistributionFunction >::write(
                                   largefile_ofstream & os,
                                   const OrientationDistributionFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< DiffusionFeature >& itemW_DiffusionFeature =
    DefaultBSwapItemWriter< DiffusionFeature >::getInstance();

  itemW_DiffusionFeature.write( os, pItem, count );

}


template <>
inline
void
DefaultBSwapItemWriter< OrientationDistributionFunction >::write(
                                   largefile_fstream & fs,
                                   const OrientationDistributionFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< DiffusionFeature >& itemW_DiffusionFeature =
    DefaultBSwapItemWriter< DiffusionFeature >::getInstance();

  itemW_DiffusionFeature.write( fs, pItem, count );

}


}


#endif
