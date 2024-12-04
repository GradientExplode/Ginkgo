#ifndef _gkg_dmri_io_DefaultAsciiItemWriterOfDiffusionFeature_i_h_
#define _gkg_dmri_io_DefaultAsciiItemWriterOfDiffusionFeature_i_h_


#include <gkg-core-io/DefaultAsciiItemWriter_i.h>
#include <gkg-dmri-diffusion-feature/DiffusionFeature.h>
#include <gkg-core-io/StdVectorAsciiItemWriter_i.h>


//
// class DefaultAsciiItemWriter< DiffusionFeature >
//

namespace gkg
{

template <>
inline
DefaultAsciiItemWriter< DiffusionFeature >::DefaultAsciiItemWriter()
                                           : ItemWriter< DiffusionFeature >()
{
}


template <>
inline
DefaultAsciiItemWriter< DiffusionFeature >::~DefaultAsciiItemWriter()
{
}


template <>
inline
void 
DefaultAsciiItemWriter< DiffusionFeature >::write(
                                   std::ostream & os,
                                   const DiffusionFeature* pItem,
                                   size_t count ) const
{

  ItemWriter< std::string >& itemW_String =
    DefaultAsciiItemWriter< std::string >::getInstance();
  ItemWriter< std::vector< float > >& itemW_DiffusionFeature =
    StdVectorAsciiItemWriter< float >::getInstance();
  
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
DefaultAsciiItemWriter< DiffusionFeature >::write(
                                   largefile_ofstream & os,
                                   const DiffusionFeature* pItem,
                                   size_t count ) const
{

  ItemWriter< std::string >& itemW_String =
    DefaultAsciiItemWriter< std::string >::getInstance();
  ItemWriter< std::vector< float > >& itemW_DiffusionFeature =
    StdVectorAsciiItemWriter< float >::getInstance();
  
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
DefaultAsciiItemWriter< DiffusionFeature >::write(
                                   largefile_fstream & fs,
                                   const DiffusionFeature* pItem,
                                   size_t count ) const
{

  ItemWriter< std::string >& itemW_String =
    DefaultAsciiItemWriter< std::string >::getInstance();
  ItemWriter< std::vector< float > >& itemW_DiffusionFeature =
    StdVectorAsciiItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_String.write( fs, pItem[ i ].getBasisTypeName() );
    itemW_DiffusionFeature.write( fs, pItem[ i ].getValues() );

  }

}


}


#endif
