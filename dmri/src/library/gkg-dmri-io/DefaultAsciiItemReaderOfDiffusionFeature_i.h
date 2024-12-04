#ifndef _gkg_dmri_io_DefaultAsciiItemReaderOfDiffusionFeature_i_h_
#define _gkg_dmri_io_DefaultAsciiItemReaderOfDiffusionFeature_i_h_


#include <gkg-core-io/DefaultAsciiItemReader_i.h>
#include <gkg-dmri-diffusion-feature/DiffusionFeature.h>
#include <gkg-core-io/StdVectorAsciiItemReader_i.h>


//
// class DefaultAsciiItemReader< DiffusionFeature >
//

namespace gkg
{

template <>
inline
DefaultAsciiItemReader< DiffusionFeature
                      >::DefaultAsciiItemReader()
                        : ItemReader< DiffusionFeature >()
{
}


template <>
inline
DefaultAsciiItemReader< DiffusionFeature
                      >::~DefaultAsciiItemReader()
{
}


template <>
inline
void DefaultAsciiItemReader< DiffusionFeature >::read(
                                         std::istream & is,
                                         DiffusionFeature* pItem,
                                         size_t count ) const
{

  ItemReader< std::string >& itemR_String =
    DefaultAsciiItemReader< std::string >::getInstance();
  ItemReader< std::vector< float > >& itemR_DiffusionFeature =
    StdVectorAsciiItemReader< float >::getInstance();
  
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
void DefaultAsciiItemReader< DiffusionFeature >::read(
                                         largefile_ifstream & is,
                                         DiffusionFeature* pItem,
                                         size_t count ) const
{

  ItemReader< std::string >& itemR_String =
    DefaultAsciiItemReader< std::string >::getInstance();
  ItemReader< std::vector< float > >& itemR_DiffusionFeature =
    StdVectorAsciiItemReader< float >::getInstance();
  
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
void DefaultAsciiItemReader< DiffusionFeature >::read(
                                         largefile_fstream & fs,
                                         DiffusionFeature* pItem,
                                         size_t count ) const
{

  ItemReader< std::string >& itemR_String =
    DefaultAsciiItemReader< std::string >::getInstance();
  ItemReader< std::vector< float > >& itemR_DiffusionFeature =
    StdVectorAsciiItemReader< float >::getInstance();
  
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
