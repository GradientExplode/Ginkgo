#ifndef _gkg_dmri_io_DefaultAsciiItemReaderOfDiffusoid_i_h_
#define _gkg_dmri_io_DefaultAsciiItemReaderOfDiffusoid_i_h_


#include <gkg-core-io/DefaultAsciiItemReader_i.h>
#include <gkg-dmri-diffusoid/Diffusoid.h>
#include <gkg-core-io/StdVectorAsciiItemReader_i.h>


//
// class DefaultAsciiItemReader< Diffusoid >
//

namespace gkg
{

template <>
inline
DefaultAsciiItemReader< Diffusoid >::DefaultAsciiItemReader()
                                    : ItemReader< Diffusoid >()
{
}


template <>
inline
DefaultAsciiItemReader< Diffusoid >::~DefaultAsciiItemReader()
{
}


template <>
inline
void DefaultAsciiItemReader< Diffusoid >::read( std::istream & is,
                                                Diffusoid* pItem,
                                                size_t count ) const
{

  ItemReader< std::vector< float > >& itemR_Diffusoid =
    StdVectorAsciiItemReader< float >::getInstance();
  
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
void DefaultAsciiItemReader< Diffusoid >::read( largefile_ifstream & is,
                                                Diffusoid* pItem,
                                                size_t count ) const
{

  ItemReader< std::vector< float > >& itemR_Diffusoid =
    StdVectorAsciiItemReader< float >::getInstance();
  
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
void DefaultAsciiItemReader< Diffusoid >::read( largefile_fstream & fs,
                                                Diffusoid* pItem,
                                                size_t count ) const
{

  ItemReader< std::vector< float > >& itemR_Diffusoid =
    StdVectorAsciiItemReader< float >::getInstance();
  
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
