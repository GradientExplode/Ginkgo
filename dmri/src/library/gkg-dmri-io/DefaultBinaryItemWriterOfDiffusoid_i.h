#ifndef _gkg_dmri_io_DefaultBinaryItemWriterOfDiffusoid_i_h_
#define _gkg_dmri_io_DefaultBinaryItemWriterOfDiffusoid_i_h_


#include <gkg-core-io/DefaultBinaryItemWriter_i.h>
#include <gkg-dmri-diffusoid/Diffusoid.h>
#include <gkg-core-io/StdVectorBinaryItemWriter_i.h>


//
// class DefaultBinaryItemWriter< Diffusoid >
//

namespace gkg
{

template <>
inline
DefaultBinaryItemWriter< Diffusoid >::DefaultBinaryItemWriter()
                                     : ItemWriter< Diffusoid >()
{
}


template <>
inline
DefaultBinaryItemWriter< Diffusoid >::~DefaultBinaryItemWriter()
{
}


template <>
inline
void 
DefaultBinaryItemWriter< Diffusoid >::write( std::ostream & os,
                                             const Diffusoid* pItem,
                                             size_t count ) const
{

  ItemWriter< std::vector< float > >& itemW_Diffusoid =
    StdVectorBinaryItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_Diffusoid.write( os, pItem[ i ].getMagnitudes() );

  }

}


template <>
inline
void 
DefaultBinaryItemWriter< Diffusoid >::write( largefile_ofstream & os,
                                             const Diffusoid* pItem,
                                             size_t count ) const
{

  ItemWriter< std::vector< float > >& itemW_Diffusoid =
    StdVectorBinaryItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_Diffusoid.write( os, pItem[ i ].getMagnitudes() );

  }

}


template <>
inline
void
DefaultBinaryItemWriter< Diffusoid >::write( largefile_fstream & fs,
                                             const Diffusoid* pItem,
                                             size_t count ) const
{

  ItemWriter< std::vector< float > >& itemW_Diffusoid =
    StdVectorBinaryItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_Diffusoid.write( fs, pItem[ i ].getMagnitudes() );

  }

}


}


#endif
