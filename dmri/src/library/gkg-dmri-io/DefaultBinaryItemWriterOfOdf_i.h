#ifndef _gkg_dmri_io_DefaultBinaryItemWriterOfOdf_i_h_
#define _gkg_dmri_io_DefaultBinaryItemWriterOfOdf_i_h_


#include <gkg-core-io/DefaultBinaryItemWriter_i.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-io/StdVectorBinaryItemWriter_i.h>

#include <gkg-dmri-io/DefaultBinaryItemWriterOfDiffusionFeature_i.h>


//
// class DefaultBinaryItemWriter< OrientationDistributionFunction >
//

namespace gkg
{

template <>
inline
DefaultBinaryItemWriter< OrientationDistributionFunction
                       >::DefaultBinaryItemWriter()
                         : ItemWriter< OrientationDistributionFunction >()
{
}


template <>
inline
DefaultBinaryItemWriter< OrientationDistributionFunction
                       >::~DefaultBinaryItemWriter()
{
}


template <>
inline
void 
DefaultBinaryItemWriter< OrientationDistributionFunction >::write(
                                   std::ostream & os,
                                   const OrientationDistributionFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< DiffusionFeature >& itemW_DiffusionFeature =
    DefaultBinaryItemWriter< DiffusionFeature >::getInstance();

  itemW_DiffusionFeature.write( os, pItem, count );

}


template <>
inline
void 
DefaultBinaryItemWriter< OrientationDistributionFunction >::write(
                                   largefile_ofstream & os,
                                   const OrientationDistributionFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< DiffusionFeature >& itemW_DiffusionFeature =
    DefaultBinaryItemWriter< DiffusionFeature >::getInstance();

  itemW_DiffusionFeature.write( os, pItem, count );

}


template <>
inline
void
DefaultBinaryItemWriter< OrientationDistributionFunction >::write(
                                   largefile_fstream & fs,
                                   const OrientationDistributionFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< DiffusionFeature >& itemW_DiffusionFeature =
    DefaultBinaryItemWriter< DiffusionFeature >::getInstance();

  itemW_DiffusionFeature.write( fs, pItem, count );

}


}


#endif
