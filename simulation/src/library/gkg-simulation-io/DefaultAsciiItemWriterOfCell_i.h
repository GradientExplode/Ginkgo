#ifndef _gkg_simulation_io_DefaultAsciiItemWriterOfCell_i_h_
#define _gkg_simulation_io_DefaultAsciiItemWriterOfCell_i_h_


#include <gkg-core-io/DefaultAsciiItemWriter_i.h>
#include <gkg-simulation-io/DefaultAsciiItemWriterOfAtom_i.h>
#include <gkg-simulation-io/DefaultBinaryItemWriterOfAtom_i.h>
#include <gkg-simulation-io/DefaultBSwapItemWriterOfAtom_i.h>
#include <gkg-core-io/StdVectorAsciiItemWriter_i.h>
#include <gkg-simulation-virtual-tissue/Cell.h>


//
// class DefaultAsciiItemWriter< Cell >
//


namespace gkg
{


template <>
inline
DefaultAsciiItemWriter< Cell >::DefaultAsciiItemWriter()
                               : ItemWriter< Cell >()
{
}


template <>
inline
DefaultAsciiItemWriter< Cell >::~DefaultAsciiItemWriter()
{
}


template <>
inline
void 
DefaultAsciiItemWriter< Cell >::write(
                                   std::ostream & os,
                                   const Cell* pItem,
                                   size_t count ) const
{

  ItemWriter< int32_t >& itemW_Int32 =
    DefaultAsciiItemWriter< int32_t >::getInstance();

  ItemWriter< std::vector< PAtom > >& itemW_VectorOfAtoms =
    StdVectorAsciiItemWriter< PAtom >::getInstance();
  
  size_t i = 0U;
  for ( i = 0; i < count; i++ )
  {

    itemW_Int32.write( os, pItem[ i ].getId() );
    itemW_VectorOfAtoms.write( os, pItem[ i ].getAtoms() );

  }

}


template <>
inline
void 
DefaultAsciiItemWriter< Cell >::write(
                                   largefile_ofstream & os,
                                   const Cell* pItem,
                                   size_t count ) const
{

  ItemWriter< int32_t >& itemW_Int32 =
    DefaultAsciiItemWriter< int32_t >::getInstance();

  ItemWriter< std::vector< PAtom > >& itemW_VectorOfAtoms =
    StdVectorAsciiItemWriter< PAtom >::getInstance();
  
  size_t i = 0U;
  for ( i = 0; i < count; i++ )
  {

    itemW_Int32.write( os, pItem[ i ].getId() );
    itemW_VectorOfAtoms.write( os, pItem[ i ].getAtoms() );

  }

}


template <>
inline
void 
DefaultAsciiItemWriter< Cell >::write(
                                   largefile_fstream & fs,
                                   const Cell* pItem,
                                   size_t count ) const
{

  ItemWriter< int32_t >& itemW_Int32 =
    DefaultAsciiItemWriter< int32_t >::getInstance();

  ItemWriter< std::vector< PAtom > >& itemW_VectorOfAtoms =
    StdVectorAsciiItemWriter< PAtom >::getInstance();
  
  size_t i = 0U;
  for ( i = 0; i < count; i++ )
  {

    itemW_Int32.write( fs, pItem[ i ].getId() );
    itemW_VectorOfAtoms.write( fs, pItem[ i ].getAtoms() );

  }

}


}


#endif

