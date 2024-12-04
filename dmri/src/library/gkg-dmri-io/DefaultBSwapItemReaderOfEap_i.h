#ifndef _gkg_dmri_io_DefaultBSwapItemReaderOfEap_i_h_
#define _gkg_dmri_io_DefaultBSwapItemReaderOfEap_i_h_


#include <gkg-core-io/DefaultBSwapItemReader_i.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-io/StdVectorBSwapItemReader_i.h>


//
// class DefaultBSwapItemReader< OrientationDistributionFunction >
//

namespace gkg
{

template <>
inline
DefaultBSwapItemReader< OrientationDistributionFunction
                      >::DefaultBSwapItemReader()
                        : ItemReader< OrientationDistributionFunction >()
{
}


template <>
inline
DefaultBSwapItemReader< OrientationDistributionFunction
                      >::~DefaultBSwapItemReader()
{
}


template <>
inline
void DefaultBSwapItemReader< OrientationDistributionFunction >::read(
                                         std::istream & is,
                                         OrientationDistributionFunction* pItem,
                                         size_t count ) const
{

  ItemReader< std::string >& itemR_String =
    DefaultBSwapItemReader< std::string >::getInstance();
  ItemReader< std::vector< float > >& itemR_Eap =
    StdVectorBSwapItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::string basisTypeName;
    itemR_String.read( is, basisTypeName );
    pItem[ i ].setBasisType( basisTypeName );

    std::vector< float > values;
    itemR_Eap.read( is, values );
    pItem[ i ].setValues( values );

  }

}


template <>
inline
void DefaultBSwapItemReader< OrientationDistributionFunction >::read(
                                         largefile_ifstream & is,
                                         OrientationDistributionFunction* pItem,
                                         size_t count ) const
{

  ItemReader< std::string >& itemR_String =
    DefaultBSwapItemReader< std::string >::getInstance();
  ItemReader< std::vector< float > >& itemR_Eap =
    StdVectorBSwapItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::string basisTypeName;
    itemR_String.read( is, basisTypeName );
    pItem[ i ].setBasisType( basisTypeName );

    std::vector< float > values;
    itemR_Eap.read( is, values );
    pItem[ i ].setValues( values );

  }

}


template <>
inline
void DefaultBSwapItemReader< OrientationDistributionFunction >::read(
                                         largefile_fstream & fs,
                                         OrientationDistributionFunction* pItem,
                                         size_t count ) const
{

  ItemReader< std::string >& itemR_String =
    DefaultBSwapItemReader< std::string >::getInstance();
  ItemReader< std::vector< float > >& itemR_Eap =
    StdVectorBSwapItemReader< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    std::string basisTypeName;
    itemR_String.read( fs, basisTypeName );
    pItem[ i ].setBasisType( basisTypeName );

    std::vector< float > values;
    itemR_Eap.read( fs, values );
    pItem[ i ].setValues( values );

  }

}


}


#endif
