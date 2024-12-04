#ifndef _gkg_dmri_io_DefaultAsciiItemWriterOfOdf_i_h_
#define _gkg_dmri_io_DefaultAsciiItemWriterOfOdf_i_h_


#include <gkg-core-io/DefaultAsciiItemWriter_i.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-io/StdVectorAsciiItemWriter_i.h>

#include <gkg-dmri-io/DefaultAsciiItemWriterOfDiffusionFeature_i.h>


//
// class DefaultAsciiItemWriter< OrientationDistributionFunction >
//

namespace gkg
{

template <>
inline
DefaultAsciiItemWriter< OrientationDistributionFunction
                      >::DefaultAsciiItemWriter()
                        : ItemWriter< OrientationDistributionFunction >()
{
}


template <>
inline
DefaultAsciiItemWriter< OrientationDistributionFunction
                      >::~DefaultAsciiItemWriter()
{
}


template <>
inline
void 
DefaultAsciiItemWriter< OrientationDistributionFunction >::write(
                                   std::ostream & os,
                                   const OrientationDistributionFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< DiffusionFeature >& itemW_DiffusionFeature =
    DefaultAsciiItemWriter< DiffusionFeature >::getInstance();

  itemW_DiffusionFeature.write( os, pItem, count );

}


template <>
inline
void 
DefaultAsciiItemWriter< OrientationDistributionFunction >::write(
                                   largefile_ofstream & os,
                                   const OrientationDistributionFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< DiffusionFeature >& itemW_DiffusionFeature =
    DefaultAsciiItemWriter< DiffusionFeature >::getInstance();

  itemW_DiffusionFeature.write( os, pItem, count );

}


template <>
inline
void
DefaultAsciiItemWriter< OrientationDistributionFunction >::write(
                                   largefile_fstream & fs,
                                   const OrientationDistributionFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< DiffusionFeature >& itemW_DiffusionFeature =
    DefaultAsciiItemWriter< DiffusionFeature >::getInstance();

  itemW_DiffusionFeature.write( fs, pItem, count );

}


}


#endif
