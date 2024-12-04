#ifndef _gkg_dmri_io_DefaultBinaryItemWriterOfDiffusionFeature_i_h_
#define _gkg_dmri_io_DefaultBinaryItemWriterOfDiffusionFeature_i_h_


#include <gkg-core-io/DefaultBinaryItemWriter_i.h>
#include <gkg-dmri-diffusion-feature/DiffusionFeature.h>
#include <gkg-core-io/StdVectorBinaryItemWriter_i.h>


//
// class DefaultBinaryItemWriter< DiffusionFeature >
//

namespace gkg
{

template <>
inline
DefaultBinaryItemWriter< DiffusionFeature >::DefaultBinaryItemWriter()
                                            : ItemWriter< DiffusionFeature >()
{
}


template <>
inline
DefaultBinaryItemWriter< DiffusionFeature >::~DefaultBinaryItemWriter()
{
}


template <>
inline
void 
DefaultBinaryItemWriter< DiffusionFeature >::write(
                                   std::ostream & os,
                                   const DiffusionFeature* pItem,
                                   size_t count ) const
{

  ItemWriter< std::string >& itemW_String =
    DefaultBinaryItemWriter< std::string >::getInstance();
  ItemWriter< std::vector< float > >& itemW_DiffusionFeature =
    StdVectorBinaryItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_String.write( os, pItem[ i ].getBasisTypeName() );
    itemW_DiffusionFeature.write( os, pItem[ i ].getValues() );

  }

}


template <>
inline
void 
DefaultBinaryItemWriter< DiffusionFeature >::write(
                                   largefile_ofstream & os,
                                   const DiffusionFeature* pItem,
                                   size_t count ) const
{

  ItemWriter< std::string >& itemW_String =
    DefaultBinaryItemWriter< std::string >::getInstance();

  ItemWriter< std::vector< float > >& itemW_DiffusionFeature =
    StdVectorBinaryItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_String.write( os, pItem[ i ].getBasisTypeName() );
    itemW_DiffusionFeature.write( os, pItem[ i ].getValues() );

  }

}


template <>
inline
void
DefaultBinaryItemWriter< DiffusionFeature >::write(
                                   largefile_fstream & fs,
                                   const DiffusionFeature* pItem,
                                   size_t count ) const
{

  ItemWriter< std::string >& itemW_String =
    DefaultBinaryItemWriter< std::string >::getInstance();
  ItemWriter< std::vector< float > >& itemW_DiffusionFeature =
    StdVectorBinaryItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_String.write( fs, pItem[ i ].getBasisTypeName() );
    itemW_DiffusionFeature.write( fs, pItem[ i ].getValues() );

  }

}


}


#endif
