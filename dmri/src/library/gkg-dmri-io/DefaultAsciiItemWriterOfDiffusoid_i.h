#ifndef _gkg_dmri_io_DefaultAsciiItemWriterOfDiffusoid_i_h_
#define _gkg_dmri_io_DefaultAsciiItemWriterOfDiffusoid_i_h_


#include <gkg-core-io/DefaultAsciiItemWriter_i.h>
#include <gkg-dmri-diffusoid/Diffusoid.h>
#include <gkg-core-io/StdVectorAsciiItemWriter_i.h>


//
// class DefaultAsciiItemWriter< Diffusoid >
//

namespace gkg
{

template <>
inline
DefaultAsciiItemWriter< Diffusoid >::DefaultAsciiItemWriter()
                                    : ItemWriter< Diffusoid >()
{
}


template <>
inline
DefaultAsciiItemWriter< Diffusoid >::~DefaultAsciiItemWriter()
{
}


template <>
inline
void 
DefaultAsciiItemWriter< Diffusoid >::write( std::ostream & os,
                                            const Diffusoid* pItem,
                                            size_t count ) const
{

  ItemWriter< std::vector< float > >& itemW_Diffusoid =
    StdVectorAsciiItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_Diffusoid.write( os, pItem[ i ].getMagnitudes() );

  }

}


template <>
inline
void 
DefaultAsciiItemWriter< Diffusoid >::write( largefile_ofstream & os,
                                            const Diffusoid* pItem,
                                            size_t count ) const
{

  ItemWriter< std::vector< float > >& itemW_Diffusoid =
    StdVectorAsciiItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_Diffusoid.write( os, pItem[ i ].getMagnitudes() );

  }

}


template <>
inline
void
DefaultAsciiItemWriter< Diffusoid >::write( largefile_fstream & fs,
                                            const Diffusoid* pItem,
                                            size_t count ) const
{

  ItemWriter< std::vector< float > >& itemW_Diffusoid =
    StdVectorAsciiItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_Diffusoid.write( fs, pItem[ i ].getMagnitudes() );

  }

}


}


#endif
