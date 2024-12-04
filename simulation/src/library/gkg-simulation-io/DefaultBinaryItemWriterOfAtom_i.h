#ifndef _gkg_simulation_io_DefaultBinaryItemWriterOfAtom_i_h_
#define _gkg_simulation_io_DefaultBinaryItemWriterOfAtom_i_h_


#include <gkg-core-io/DefaultBinaryItemWriter_i.h>
#include <gkg-simulation-virtual-tissue/Atom.h>
#include <gkg-core-io/StdVectorBinaryItemWriter_i.h>


//
// class DefaultBinaryItemWriter< PAtom >
//


namespace gkg
{


template <>
inline
DefaultBinaryItemWriter< PAtom >::DefaultBinaryItemWriter()
                                 : ItemWriter< PAtom >()
{
}


template <>
inline
DefaultBinaryItemWriter< PAtom >::~DefaultBinaryItemWriter()
{
}


template <>
inline
void 
DefaultBinaryItemWriter< PAtom >::write(
                                   std::ostream & os,
                                   const PAtom* pItem,
                                   size_t count ) const
{

  ItemWriter< uint8_t >& itemW_UInt8 =
    DefaultBinaryItemWriter< uint8_t >::getInstance();
  ItemWriter< std::vector< float > >& itemW_StdVectorFloat =
    StdVectorBinaryItemWriter< float >::getInstance();

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
DefaultBinaryItemWriter< PAtom >::write(
                                   largefile_ofstream & os,
                                   const PAtom* pItem,
                                   size_t count ) const
{

  ItemWriter< uint8_t >& itemW_UInt8 =
    DefaultBinaryItemWriter< uint8_t >::getInstance();
  ItemWriter< std::vector< float > >& itemW_StdVectorFloat =
    StdVectorBinaryItemWriter< float >::getInstance();

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
DefaultBinaryItemWriter< PAtom >::write(
                                   largefile_fstream & fs,
                                   const PAtom* pItem,
                                   size_t count ) const
{

  ItemWriter< uint8_t >& itemW_UInt8 =
    DefaultBinaryItemWriter< uint8_t >::getInstance();
  ItemWriter< std::vector< float > >& itemW_StdVectorFloat =
    StdVectorBinaryItemWriter< float >::getInstance();

  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_UInt8.write( fs, pItem[ i ]->getType() );
    itemW_StdVectorFloat.write( fs, pItem[ i ]->parameters );

  }

}


}


#endif

