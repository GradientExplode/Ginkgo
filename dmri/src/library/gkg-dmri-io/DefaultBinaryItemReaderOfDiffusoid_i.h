#ifndef _gkg_dmri_io_DefaultBinaryItemReaderOfDiffusoid_i_h_
#define _gkg_dmri_io_DefaultBinaryItemReaderOfDiffusoid_i_h_


#include <gkg-core-io/DefaultBinaryItemReader_i.h>
#include <gkg-dmri-diffusoid/Diffusoid.h>
#include <gkg-core-io/StdVectorBinaryItemReader_i.h>


//
// class DefaultBinaryItemReader< Diffusoid >
//

namespace gkg
{

template <>
inline
DefaultBinaryItemReader< Diffusoid >::DefaultBinaryItemReader()
                                     : ItemReader< Diffusoid >()
{
}


template <>
inline
DefaultBinaryItemReader< Diffusoid >::~DefaultBinaryItemReader()
{
}


template <>
inline
void DefaultBinaryItemReader< Diffusoid >::read( std::istream & is,
                                                 Diffusoid* pItem,
                                                 size_t count ) const
{

  ItemReader< std::vector< float > >& itemR_Diffusoid =
    StdVectorBinaryItemReader< float >::getInstance();
  
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
void DefaultBinaryItemReader< Diffusoid >::read( largefile_ifstream & is,
                                                 Diffusoid* pItem,
                                                 size_t count ) const
{

  ItemReader< std::vector< float > >& itemR_Diffusoid =
    StdVectorBinaryItemReader< float >::getInstance();
  
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
void DefaultBinaryItemReader< Diffusoid >::read( largefile_fstream & fs,
                                                 Diffusoid* pItem,
                                                 size_t count ) const
{

  ItemReader< std::vector< float > >& itemR_Diffusoid =
    StdVectorBinaryItemReader< float >::getInstance();
  
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
