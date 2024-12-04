#ifndef _gkg_simulation_io_DefaultBinaryItemReaderOfCell_i_h_
#define _gkg_simulation_io_DefaultBinaryItemReaderOfCell_i_h_


#include <gkg-core-io/DefaultBinaryItemReader_i.h>
#include <gkg-simulation-io/DefaultAsciiItemReaderOfAtom_i.h>
#include <gkg-simulation-io/DefaultBinaryItemReaderOfAtom_i.h>
#include <gkg-simulation-io/DefaultBSwapItemReaderOfAtom_i.h>
#include <gkg-core-io/StdVectorBinaryItemReader_i.h>
#include <gkg-simulation-virtual-tissue/Cell.h>


//
// class DefaultBinaryItemReader< Cell >
//


namespace gkg
{


template <>
inline
DefaultBinaryItemReader< Cell >::DefaultBinaryItemReader()
                               : ItemReader< Cell >()
{
}


template <>
inline
DefaultBinaryItemReader< Cell >::~DefaultBinaryItemReader()
{
}


template <>
inline
void 
DefaultBinaryItemReader< Cell >::read(
                                   std::istream & is,
                                   Cell* pItem,
                                   size_t count ) const
{

  ItemReader< int32_t >& itemR_Int32 =
    DefaultBinaryItemReader< int32_t >::getInstance();

  ItemReader< std::vector< PAtom > >& itemR_VectorOfAtoms =
    StdVectorBinaryItemReader< PAtom >::getInstance();
  
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
DefaultBinaryItemReader< Cell >::read(
                                   largefile_ifstream & is,
                                   Cell* pItem,
                                   size_t count ) const
{

  ItemReader< int32_t >& itemR_Int32 =
    DefaultBinaryItemReader< int32_t >::getInstance();

  ItemReader< std::vector< PAtom > >& itemR_VectorOfAtoms =
    StdVectorBinaryItemReader< PAtom >::getInstance();
  
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
DefaultBinaryItemReader< Cell >::read(
                                   largefile_fstream & fs,
                                   Cell* pItem,
                                   size_t count ) const
{

  ItemReader< int32_t >& itemR_Int32 =
    DefaultBinaryItemReader< int32_t >::getInstance();

  ItemReader< std::vector< PAtom > >& itemR_VectorOfAtoms =
    StdVectorBinaryItemReader< PAtom >::getInstance();
  
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

