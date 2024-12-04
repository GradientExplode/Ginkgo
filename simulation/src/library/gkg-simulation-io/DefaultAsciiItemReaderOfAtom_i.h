#ifndef _gkg_simulation_io_DefaultAsciiItemReaderOfAtom_i_h_
#define _gkg_simulation_io_DefaultAsciiItemReaderOfAtom_i_h_


#include <gkg-core-io/DefaultAsciiItemReader_i.h>
#include <gkg-simulation-virtual-tissue/SphereAtom.h>
#include <gkg-simulation-virtual-tissue/EllipsoidAtom.h>
#include <gkg-core-io/StdVectorAsciiItemReader_i.h>


//
// class DefaultAsciiItemReader< PAtom >
//


namespace gkg
{


template <>
inline
DefaultAsciiItemReader< PAtom >::DefaultAsciiItemReader()
                                : ItemReader< PAtom >()
{
}


template <>
inline
DefaultAsciiItemReader< PAtom >::~DefaultAsciiItemReader()
{
}


template <>
inline
void 
DefaultAsciiItemReader< PAtom >::read(
                                   std::istream & is,
                                   PAtom* pItem,
                                   size_t count ) const
{

  ItemReader< uint8_t >& itemR_UInt8 =
    DefaultAsciiItemReader< uint8_t >::getInstance();
  ItemReader< std::vector< float > >& itemR_StdVectorFloat =
    StdVectorAsciiItemReader< float >::getInstance();

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
DefaultAsciiItemReader< PAtom >::read(
                                   largefile_ifstream& is,
                                   PAtom* pItem,
                                   size_t count ) const
{


  ItemReader< uint8_t >& itemR_UInt8 =
    DefaultAsciiItemReader< uint8_t >::getInstance();
  ItemReader< std::vector< float > >& itemR_StdVectorFloat =
    StdVectorAsciiItemReader< float >::getInstance();

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
DefaultAsciiItemReader< PAtom >::read(
                                   largefile_fstream & fs,
                                   PAtom* pItem,
                                   size_t count ) const
{


  ItemReader< uint8_t >& itemR_UInt8 =
    DefaultAsciiItemReader< uint8_t >::getInstance();
  ItemReader< std::vector< float > >& itemR_StdVectorFloat =
    StdVectorAsciiItemReader< float >::getInstance();

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

