#ifndef _gkg_simulation_io_DefaultAsciiItemReaderOfCell_i_h_
#define _gkg_simulation_io_DefaultAsciiItemReaderOfCell_i_h_


#include <gkg-core-io/DefaultAsciiItemReader_i.h>
#include <gkg-simulation-io/DefaultAsciiItemReaderOfAtom_i.h>
#include <gkg-simulation-io/DefaultBinaryItemReaderOfAtom_i.h>
#include <gkg-simulation-io/DefaultBSwapItemReaderOfAtom_i.h>
#include <gkg-core-io/StdVectorAsciiItemReader_i.h>
#include <gkg-simulation-virtual-tissue/Cell.h>



//
// class DefaultAsciiItemReader< Cell >
//


namespace gkg
{


template <>
inline
DefaultAsciiItemReader< Cell >::DefaultAsciiItemReader()
                               : ItemReader< Cell >()
{
}


template <>
inline
DefaultAsciiItemReader< Cell >::~DefaultAsciiItemReader()
{
}


template <>
inline
void 
DefaultAsciiItemReader< Cell >::read(
                                   std::istream & is,
                                   Cell* pItem,
                                   size_t count ) const
{

  ItemReader< int32_t >& itemR_Int32 =
    DefaultAsciiItemReader< int32_t >::getInstance();

  ItemReader< std::vector< PAtom > >& itemR_VectorOfAtoms =
    StdVectorAsciiItemReader< PAtom >::getInstance();
  
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
DefaultAsciiItemReader< Cell >::read(
                                   largefile_ifstream & is,
                                   Cell* pItem,
                                   size_t count ) const
{

  ItemReader< int32_t >& itemR_Int32 =
    DefaultAsciiItemReader< int32_t >::getInstance();

  ItemReader< std::vector< PAtom > >& itemR_VectorOfAtoms =
    StdVectorAsciiItemReader< PAtom >::getInstance();
  
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
DefaultAsciiItemReader< Cell >::read(
                                   largefile_fstream & fs,
                                   Cell* pItem,
                                   size_t count ) const
{

  ItemReader< int32_t >& itemR_Int32 =
    DefaultAsciiItemReader< int32_t >::getInstance();

  ItemReader< std::vector< PAtom > >& itemR_VectorOfAtoms =
    StdVectorAsciiItemReader< PAtom >::getInstance();
  
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

