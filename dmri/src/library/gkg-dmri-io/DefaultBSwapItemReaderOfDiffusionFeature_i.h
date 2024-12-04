#ifndef _gkg_dmri_io_DefaultBSwapItemReaderOfDiffusionFeature_i_h_
#define _gkg_dmri_io_DefaultBSwapItemReaderOfDiffusionFeature_i_h_


#include <gkg-core-io/DefaultBSwapItemReader_i.h>
#include <gkg-dmri-diffusion-feature/DiffusionFeature.h>
#include <gkg-core-io/StdVectorBSwapItemReader_i.h>


//
// class DefaultBSwapItemReader< DiffusionFeature >
//

namespace gkg
{

template <>
inline
DefaultBSwapItemReader< DiffusionFeature >::DefaultBSwapItemReader()
                                           : ItemReader< DiffusionFeature >()
{
}


template <>
inline
DefaultBSwapItemReader< DiffusionFeature >::~DefaultBSwapItemReader()
{
}


template <>
inline
void DefaultBSwapItemReader< DiffusionFeature >::read(
                                         std::istream & is,
                                         DiffusionFeature* pItem,
                                         size_t count ) const
{

  ItemReader< std::string >& itemR_String =
    DefaultBSwapItemReader< std::string >::getInstance();
  ItemReader< std::vector< float > >& itemR_DiffusionFeature =
    StdVectorBSwapItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::string basisTypeName;
    itemR_String.read( is, basisTypeName );
    pItem[ i ].setBasisType( basisTypeName );

    std::vector< float > values;
    itemR_DiffusionFeature.read( is, values );
    pItem[ i ].setValues( values );

  }

}


template <>
inline
void DefaultBSwapItemReader< DiffusionFeature >::read(
                                         largefile_ifstream & is,
                                         DiffusionFeature* pItem,
                                         size_t count ) const
{

  ItemReader< std::string >& itemR_String =
    DefaultBSwapItemReader< std::string >::getInstance();
  ItemReader< std::vector< float > >& itemR_DiffusionFeature =
    StdVectorBSwapItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::string basisTypeName;
    itemR_String.read( is, basisTypeName );
    pItem[ i ].setBasisType( basisTypeName );

    std::vector< float > values;
    itemR_DiffusionFeature.read( is, values );
    pItem[ i ].setValues( values );

  }

}


template <>
inline
void DefaultBSwapItemReader< DiffusionFeature >::read(
                                         largefile_fstream & fs,
                                         DiffusionFeature* pItem,
                                         size_t count ) const
{

  ItemReader< std::string >& itemR_String =
    DefaultBSwapItemReader< std::string >::getInstance();
  ItemReader< std::vector< float > >& itemR_DiffusionFeature =
    StdVectorBSwapItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::string basisTypeName;
    itemR_String.read( fs, basisTypeName );
    pItem[ i ].setBasisType( basisTypeName );

    std::vector< float > values;
    itemR_DiffusionFeature.read( fs, values );
    pItem[ i ].setValues( values );

  }

}


}


#endif
