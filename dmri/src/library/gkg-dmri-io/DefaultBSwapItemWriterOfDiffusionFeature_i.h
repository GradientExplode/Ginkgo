#ifndef _gkg_dmri_io_DefaultBSwapItemWriterOfDiffusionFeature_i_h_
#define _gkg_dmri_io_DefaultBSwapItemWriterOfDiffusionFeature_i_h_


#include <gkg-core-io/DefaultBSwapItemWriter_i.h>
#include <gkg-dmri-diffusion-feature/DiffusionFeature.h>
#include <gkg-core-io/StdVectorBSwapItemWriter_i.h>


//
// class DefaultBSwapItemWriter< DiffusionFeature >
//

namespace gkg
{

template <>
inline
DefaultBSwapItemWriter< DiffusionFeature >::DefaultBSwapItemWriter()
                                           : ItemWriter< DiffusionFeature >()
{
}


template <>
inline
DefaultBSwapItemWriter< DiffusionFeature >::~DefaultBSwapItemWriter()
{
}


template <>
inline
void 
DefaultBSwapItemWriter< DiffusionFeature >::write(
                                   std::ostream & os,
                                   const DiffusionFeature* pItem,
                                   size_t count ) const
{

  ItemWriter< std::string >& itemW_String =
    DefaultBSwapItemWriter< std::string >::getInstance();
  ItemWriter< std::vector< float > >& itemW_DiffusionFeature =
    StdVectorBSwapItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_String.write( os, pItem[i].getBasisTypeName() );
    itemW_DiffusionFeature.write( os, pItem[ i ].getValues() );

  }

}


template <>
inline
void 
DefaultBSwapItemWriter< DiffusionFeature >::write(
                                   largefile_ofstream & os,
                                   const DiffusionFeature* pItem,
                                   size_t count ) const
{

  ItemWriter< std::string >& itemW_String =
    DefaultBSwapItemWriter< std::string >::getInstance();
  ItemWriter< std::vector< float > >& itemW_DiffusionFeature =
    StdVectorBSwapItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_String.write( os, pItem[i].getBasisTypeName() );
    itemW_DiffusionFeature.write( os, pItem[ i ].getValues() );

  }

}


template <>
inline
void
DefaultBSwapItemWriter< DiffusionFeature >::write(
                                   largefile_fstream & fs,
                                   const DiffusionFeature* pItem,
                                   size_t count ) const
{

  ItemWriter< std::string >& itemW_String =
    DefaultBSwapItemWriter< std::string >::getInstance();
  ItemWriter< std::vector< float > >& itemW_DiffusionFeature =
    StdVectorBSwapItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_String.write( fs, pItem[i].getBasisTypeName() );
    itemW_DiffusionFeature.write( fs, pItem[ i ].getValues() );

  }

}


}


#endif
