#ifndef _gkg_simulation_io_DefaultBinaryItemWriterOfCell_i_h_
#define _gkg_simulation_io_DefaultBinaryItemWriterOfCell_i_h_


#include <gkg-core-io/DefaultBinaryItemWriter_i.h>
#include <gkg-simulation-io/DefaultAsciiItemWriterOfAtom_i.h>
#include <gkg-simulation-io/DefaultBinaryItemWriterOfAtom_i.h>
#include <gkg-simulation-io/DefaultBSwapItemWriterOfAtom_i.h>
#include <gkg-core-io/StdVectorBinaryItemWriter_i.h>
#include <gkg-simulation-virtual-tissue/Cell.h>


//
// class DefaultBinaryItemWriter< Cell >
//


namespace gkg
{


template <>
inline
DefaultBinaryItemWriter< Cell >::DefaultBinaryItemWriter()
                               : ItemWriter< Cell >()
{
}


template <>
inline
DefaultBinaryItemWriter< Cell >::~DefaultBinaryItemWriter()
{
}


template <>
inline
void 
DefaultBinaryItemWriter< Cell >::write(
                                   std::ostream & os,
                                   const Cell* pItem,
                                   size_t count ) const
{

  ItemWriter< int32_t >& itemW_Int32 =
    DefaultBinaryItemWriter< int32_t >::getInstance();

  ItemWriter< std::vector< PAtom > >& itemW_VectorOfAtoms =
    StdVectorBinaryItemWriter<PAtom >::getInstance();
  
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
DefaultBinaryItemWriter< Cell >::write(
                                   largefile_ofstream & os,
                                   const Cell* pItem,
                                   size_t count ) const
{

  ItemWriter< int32_t >& itemW_Int32 =
    DefaultBinaryItemWriter< int32_t >::getInstance();

  ItemWriter< std::vector< PAtom > >& itemW_VectorOfAtoms =
    StdVectorBinaryItemWriter< PAtom >::getInstance();
  
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
DefaultBinaryItemWriter< Cell >::write(
                                   largefile_fstream & fs,
                                   const Cell* pItem,
                                   size_t count ) const
{

  ItemWriter< int32_t >& itemW_Int32 =
    DefaultBinaryItemWriter< int32_t >::getInstance();

  ItemWriter< std::vector< PAtom > >& itemW_VectorOfAtoms =
    StdVectorBinaryItemWriter< PAtom >::getInstance();
  
  size_t i = 0U;
  for ( i = 0; i < count; i++ )
  {

    itemW_Int32.write( fs, pItem[ i ].getId() );
    itemW_VectorOfAtoms.write( fs, pItem[ i ].getAtoms() );

  }

}


}


#endif

