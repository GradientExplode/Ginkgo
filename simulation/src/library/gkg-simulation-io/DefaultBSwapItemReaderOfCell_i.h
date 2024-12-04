#ifndef _gkg_simulation_io_DefaultBSwapItemReaderOfCell_i_h_
#define _gkg_simulation_io_DefaultBSwapItemReaderOfCell_i_h_


#include <gkg-core-io/DefaultBSwapItemReader_i.h>
#include <gkg-simulation-io/DefaultAsciiItemReaderOfAtom_i.h>
#include <gkg-simulation-io/DefaultBinaryItemReaderOfAtom_i.h>
#include <gkg-simulation-io/DefaultBSwapItemReaderOfAtom_i.h>
#include <gkg-core-io/StdVectorBSwapItemReader_i.h>
#include <gkg-simulation-virtual-tissue/Cell.h>



//
// class DefaultBSwapItemReader< Cell >
//


namespace gkg
{


template <>
inline
DefaultBSwapItemReader< Cell >::DefaultBSwapItemReader()
                               : ItemReader< Cell >()
{
}


template <>
inline
DefaultBSwapItemReader< Cell >::~DefaultBSwapItemReader()
{
}


template <>
inline
void 
DefaultBSwapItemReader< Cell >::read(
                                   std::istream & is,
                                   Cell* pItem,
                                   size_t count ) const
{

  ItemReader< int32_t >& itemR_Int32 =
    DefaultBSwapItemReader< int32_t >::getInstance();

  ItemReader< std::vector< PAtom > >& itemR_VectorOfAtoms =
    StdVectorBSwapItemReader< PAtom >::getInstance();
  
  size_t i = 0U;
  int32_t id = 0;
  std::vector< Atom* > atoms;
  for ( i = 0; i < count; i++ )
  {

    itemR_Int32.read( is, id );
    pItem[ i ].setId( id );
    itemR_VectorOfAtoms.read( is, atoms );
    pItem[ i ].setAtoms( atoms );

  }

}


template <>
inline
void 
DefaultBSwapItemReader< Cell >::read(
                                   largefile_ifstream & is,
                                   Cell* pItem,
                                   size_t count ) const
{

  ItemReader< int32_t >& itemR_Int32 =
    DefaultBSwapItemReader< int32_t >::getInstance();

  ItemReader< std::vector< PAtom > >& itemR_VectorOfAtoms =
    StdVectorBSwapItemReader< PAtom >::getInstance();
  
  size_t i = 0U;
  int32_t id = 0;
  std::vector< Atom* > atoms;
  for ( i = 0; i < count; i++ )
  {

    itemR_Int32.read( is, id );
    pItem[ i ].setId( id );
    itemR_VectorOfAtoms.read( is, atoms );
    pItem[ i ].setAtoms( atoms );

  }

}


template <>
inline
void 
DefaultBSwapItemReader< Cell >::read(
                                   largefile_fstream & fs,
                                   Cell* pItem,
                                   size_t count ) const
{

  ItemReader< int32_t >& itemR_Int32 =
    DefaultBSwapItemReader< int32_t >::getInstance();

  ItemReader< std::vector< PAtom > >& itemR_VectorOfAtoms =
    StdVectorBSwapItemReader< PAtom >::getInstance();
  
  size_t i = 0U;
  int32_t id = 0;
  std::vector< Atom* > atoms;
  for ( i = 0; i < count; i++ )
  {

    itemR_Int32.read( fs, id );
    pItem[ i ].setId( id );
    itemR_VectorOfAtoms.read( fs, atoms );
    pItem[ i ].setAtoms( atoms );

  }

}


}


#endif

