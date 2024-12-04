#ifndef _gkg_simulation_io_DefaultBSwapItemReaderOfAtom_i_h_
#define _gkg_simulation_io_DefaultBSwapItemReaderOfAtom_i_h_


#include <gkg-core-io/DefaultBSwapItemReader_i.h>
#include <gkg-simulation-virtual-tissue/SphereAtom.h>
#include <gkg-simulation-virtual-tissue/EllipsoidAtom.h>
#include <gkg-core-io/StdVectorBSwapItemReader_i.h>


//
// class DefaultBSwapItemReader< PAtom >
//


namespace gkg
{


template <>
inline
DefaultBSwapItemReader< PAtom >::DefaultBSwapItemReader()
                                : ItemReader< PAtom >()
{
}


template <>
inline
DefaultBSwapItemReader< PAtom >::~DefaultBSwapItemReader()
{
}


template <>
inline
void 
DefaultBSwapItemReader< PAtom >::read(
                                   std::istream & is,
                                   PAtom* pItem,
                                   size_t count ) const
{

  ItemReader< uint8_t >& itemR_UInt8 =
    DefaultBSwapItemReader< uint8_t >::getInstance();
  ItemReader< std::vector< float > >& itemR_StdVectorFloat =
    StdVectorBSwapItemReader< float >::getInstance();

  size_t i;
  uint8_t type = 0U;
  for ( i = 0; i < count; i++ )
  {

    
    itemR_UInt8.read( is, type );
    if ( type == 1U )
    {

      pItem[ i ] = new gkg::SphereAtom();

    }
    else if ( type == 2U )
    {

      pItem[ i ] = new gkg::EllipsoidAtom();

    }

    itemR_StdVectorFloat.read( is, pItem[ i ]->parameters );

  }

}


template <>
inline
void 
DefaultBSwapItemReader< PAtom >::read(
                                   largefile_ifstream & is,
                                   PAtom* pItem,
                                   size_t count ) const
{

  ItemReader< uint8_t >& itemR_UInt8 =
    DefaultBSwapItemReader< uint8_t >::getInstance();
  ItemReader< std::vector< float > >& itemR_StdVectorFloat =
    StdVectorBSwapItemReader< float >::getInstance();

  size_t i;
  uint8_t type = 0U;
  for ( i = 0; i < count; i++ )
  {

    
    itemR_UInt8.read( is, type );
    if ( type == 1U )
    {

      pItem[ i ] = new gkg::SphereAtom();

    }
    else if ( type == 2U )
    {

      pItem[ i ] = new gkg::EllipsoidAtom();

    }

    itemR_StdVectorFloat.read( is, pItem[ i ]->parameters );

  }

}


template <>
inline
void 
DefaultBSwapItemReader< PAtom >::read(
                                   largefile_fstream & fs,
                                   PAtom* pItem,
                                   size_t count ) const
{

  ItemReader< uint8_t >& itemR_UInt8 =
    DefaultBSwapItemReader< uint8_t >::getInstance();
  ItemReader< std::vector< float > >& itemR_StdVectorFloat =
    StdVectorBSwapItemReader< float >::getInstance();

  size_t i;
  uint8_t type = 0U;
  for ( i = 0; i < count; i++ )
  {

    
    itemR_UInt8.read( fs, type );
    if ( type == 1U )
    {

      pItem[ i ] = new gkg::SphereAtom();

    }
    else if ( type == 2U )
    {

      pItem[ i ] = new gkg::EllipsoidAtom();

    }

    itemR_StdVectorFloat.read( fs, pItem[ i ]->parameters );

  }

}


}


#endif

