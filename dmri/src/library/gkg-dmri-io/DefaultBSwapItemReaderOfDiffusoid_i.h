#ifndef _gkg_dmri_io_DefaultBSwapItemReaderOfDiffusoid_i_h_
#define _gkg_dmri_io_DefaultBSwapItemReaderOfDiffusoid_i_h_


#include <gkg-core-io/DefaultBSwapItemReader_i.h>
#include <gkg-dmri-diffusoid/Diffusoid.h>
#include <gkg-core-io/StdVectorBSwapItemReader_i.h>


//
// class DefaultBSwapItemReader< Diffusoid >
//

namespace gkg
{

template <>
inline
DefaultBSwapItemReader< Diffusoid >::DefaultBSwapItemReader()
                                    : ItemReader< Diffusoid >()
{
}


template <>
inline
DefaultBSwapItemReader< Diffusoid >::~DefaultBSwapItemReader()
{
}


template <>
inline
void DefaultBSwapItemReader< Diffusoid >::read( std::istream & is,
                                                Diffusoid* pItem,
                                                size_t count ) const
{

  ItemReader< std::vector< float > >& itemR_Diffusoid =
    StdVectorBSwapItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::vector< float > magnitudes;
    itemR_Diffusoid.read( is, magnitudes );
    pItem[ i ].setMagnitudes( magnitudes );

  }

}


template <>
inline
void DefaultBSwapItemReader< Diffusoid >::read( largefile_ifstream & is,
                                                Diffusoid* pItem,
                                                size_t count ) const
{

  ItemReader< std::vector< float > >& itemR_Diffusoid =
    StdVectorBSwapItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::vector< float > magnitudes;
    itemR_Diffusoid.read( is, magnitudes );
    pItem[ i ].setMagnitudes( magnitudes );

  }

}


template <>
inline
void DefaultBSwapItemReader< Diffusoid >::read( largefile_fstream & fs,
                                                Diffusoid* pItem,
                                                size_t count ) const
{

  ItemReader< std::vector< float > >& itemR_Diffusoid =
    StdVectorBSwapItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::vector< float > magnitudes;
    itemR_Diffusoid.read( fs, magnitudes );
    pItem[ i ].setMagnitudes( magnitudes );

  }

}


}


#endif
