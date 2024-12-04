#ifndef _gkg_dmri_io_DefaultBSwapItemWriterOfDiffusoid_i_h_
#define _gkg_dmri_io_DefaultBSwapItemWriterOfDiffusoid_i_h_


#include <gkg-core-io/DefaultBSwapItemWriter_i.h>
#include <gkg-dmri-diffusoid/Diffusoid.h>
#include <gkg-core-io/StdVectorBSwapItemWriter_i.h>


//
// class DefaultBSwapItemWriter< Diffusoid >
//

namespace gkg
{

template <>
inline
DefaultBSwapItemWriter< Diffusoid >::DefaultBSwapItemWriter()
                                    : ItemWriter< Diffusoid >()
{
}


template <>
inline
DefaultBSwapItemWriter< Diffusoid >::~DefaultBSwapItemWriter()
{
}


template <>
inline
void 
DefaultBSwapItemWriter< Diffusoid >::write( std::ostream & os,
                                            const Diffusoid* pItem,
                                            size_t count ) const
{

  ItemWriter< std::vector< float > >& itemW_Diffusoid =
    StdVectorBSwapItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_Diffusoid.write( os, pItem[ i ].getMagnitudes() );

  }

}


template <>
inline
void 
DefaultBSwapItemWriter< Diffusoid >::write( largefile_ofstream & os,
                                            const Diffusoid* pItem,
                                            size_t count ) const
{

  ItemWriter< std::vector< float > >& itemW_Diffusoid =
    StdVectorBSwapItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_Diffusoid.write( os, pItem[ i ].getMagnitudes() );

  }

}


template <>
inline
void
DefaultBSwapItemWriter< Diffusoid >::write( largefile_fstream & fs,
                                            const Diffusoid* pItem,
                                            size_t count ) const
{

  ItemWriter< std::vector< float > >& itemW_Diffusoid =
    StdVectorBSwapItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_Diffusoid.write( fs, pItem[ i ].getMagnitudes() );

  }

}


}


#endif
