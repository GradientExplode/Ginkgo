#ifndef _gkg_simulation_io_DefaultBinaryItemReaderOfAtom_i_h_
#define _gkg_simulation_io_DefaultBinaryItemReaderOfAtom_i_h_


#include <gkg-core-io/DefaultBinaryItemReader_i.h>
#include <gkg-simulation-virtual-tissue/SphereAtom.h>
#include <gkg-simulation-virtual-tissue/EllipsoidAtom.h>
#include <gkg-core-io/StdVectorBinaryItemReader_i.h>


//
// class DefaultBinaryItemReader< PAtom >
//


namespace gkg
{


template <>
inline
DefaultBinaryItemReader< PAtom >::DefaultBinaryItemReader()
                                 : ItemReader< PAtom >()
{
}


template <>
inline
DefaultBinaryItemReader< PAtom >::~DefaultBinaryItemReader()
{
}


template <>
inline
void 
DefaultBinaryItemReader< PAtom >::read(
                                   std::istream & is,
                                   PAtom* pItem,
                                   size_t count ) const
{

  ItemReader< uint8_t >& itemR_UInt8 =
    DefaultBinaryItemReader< uint8_t >::getInstance();
  ItemReader< std::vector< float > >& itemR_StdVectorFloat =
    StdVectorBinaryItemReader< float >::getInstance();

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
DefaultBinaryItemReader< PAtom >::read(
                                   largefile_ifstream & is,
                                   PAtom* pItem,
                                   size_t count ) const
{

  ItemReader< uint8_t >& itemR_UInt8 =
    DefaultBinaryItemReader< uint8_t >::getInstance();
  ItemReader< std::vector< float > >& itemR_StdVectorFloat =
    StdVectorBinaryItemReader< float >::getInstance();

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
DefaultBinaryItemReader< PAtom >::read(
                                   largefile_fstream & fs,
                                   PAtom* pItem,
                                   size_t count ) const
{

  ItemReader< uint8_t >& itemR_UInt8 =
    DefaultBinaryItemReader< uint8_t >::getInstance();
  ItemReader< std::vector< float > >& itemR_StdVectorFloat =
    StdVectorBinaryItemReader< float >::getInstance();

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

