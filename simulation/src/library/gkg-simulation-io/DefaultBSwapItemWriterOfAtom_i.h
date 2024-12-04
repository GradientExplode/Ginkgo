#ifndef _gkg_simulation_io_DefaultBSwapItemWriterOfAtom_i_h_
#define _gkg_simulation_io_DefaultBSwapItemWriterOfAtom_i_h_


#include <gkg-core-io/DefaultBSwapItemWriter_i.h>
#include <gkg-simulation-virtual-tissue/Atom.h>
#include <gkg-core-io/StdVectorBSwapItemWriter_i.h>


//
// class DefaultBSwapItemWriter< PAtom >
//


namespace gkg
{


template <>
inline
DefaultBSwapItemWriter< PAtom >::DefaultBSwapItemWriter()
                                     : ItemWriter< PAtom >()
{
}


template <>
inline
DefaultBSwapItemWriter< PAtom >::~DefaultBSwapItemWriter()
{
}


template <>
inline
void 
DefaultBSwapItemWriter< PAtom >::write(
                                   std::ostream & os,
                                   const PAtom* pItem,
                                   size_t count ) const
{

  ItemWriter< uint8_t >& itemW_UInt8 =
    DefaultBSwapItemWriter< uint8_t >::getInstance();
  ItemWriter< std::vector< float > >& itemW_StdVectorFloat =
    StdVectorBSwapItemWriter< float >::getInstance();

  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_UInt8.write( os, pItem[ i ]->getType() );
    itemW_StdVectorFloat.write( os, pItem[ i ]->parameters );

  }

}


template <>
inline
void 
DefaultBSwapItemWriter< PAtom >::write(
                                   largefile_ofstream & os,
                                   const PAtom* pItem,
                                   size_t count ) const
{

  ItemWriter< uint8_t >& itemW_UInt8 =
    DefaultBSwapItemWriter< uint8_t >::getInstance();
  ItemWriter< std::vector< float > >& itemW_StdVectorFloat =
    StdVectorBSwapItemWriter< float >::getInstance();

  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_UInt8.write( os, pItem[ i ]->getType() );
    itemW_StdVectorFloat.write( os, pItem[ i ]->parameters );

  }

}


template <>
inline
void 
DefaultBSwapItemWriter< PAtom >::write(
                                   largefile_fstream & fs,
                                   const PAtom* pItem,
                                   size_t count ) const
{

  ItemWriter< uint8_t >& itemW_UInt8 =
    DefaultBSwapItemWriter< uint8_t >::getInstance();
  ItemWriter< std::vector< float > >& itemW_StdVectorFloat =
    StdVectorBSwapItemWriter< float >::getInstance();

  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_UInt8.write( fs, pItem[ i ]->getType() );
    itemW_StdVectorFloat.write( fs, pItem[ i ]->parameters );

  }

}


}


#endif

