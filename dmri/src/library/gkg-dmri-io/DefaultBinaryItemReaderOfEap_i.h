#ifndef _gkg_dmri_io_DefaultBinaryItemReaderOfEap_i_h_
#define _gkg_dmri_io_DefaultBinaryItemReaderOfEap_i_h_


#include <gkg-core-io/DefaultBinaryItemReader_i.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-io/StdVectorBinaryItemReader_i.h>


//
// class DefaultBinaryItemReader< OrientationDistributionFunction >
//

namespace gkg
{

template <>
inline
DefaultBinaryItemReader< OrientationDistributionFunction
                       >::DefaultBinaryItemReader()
                        : ItemReader< OrientationDistributionFunction >()
{
}


template <>
inline
DefaultBinaryItemReader< OrientationDistributionFunction
                       >::~DefaultBinaryItemReader()
{
}


template <>
inline
void DefaultBinaryItemReader< OrientationDistributionFunction >::read(
                                         std::istream & is,
                                         OrientationDistributionFunction* pItem,
                                         size_t count ) const
{

  ItemReader< std::string >& itemR_String =
    DefaultBinaryItemReader< std::string >::getInstance();
  ItemReader< std::vector< float > >& itemR_Eap =
    StdVectorBinaryItemReader< float >::getInstance();
  
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
void DefaultBinaryItemReader< OrientationDistributionFunction >::read(
                                         largefile_ifstream & is,
                                         OrientationDistributionFunction* pItem,
                                         size_t count ) const
{

  ItemReader< std::string >& itemR_String =
    DefaultBinaryItemReader< std::string >::getInstance();
  ItemReader< std::vector< float > >& itemR_Eap =
    StdVectorBinaryItemReader< float >::getInstance();
  
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
void DefaultBinaryItemReader< OrientationDistributionFunction >::read(
                                         largefile_fstream & fs,
                                         OrientationDistributionFunction* pItem,
                                         size_t count ) const
{

  ItemReader< std::string >& itemR_String =
    DefaultBinaryItemReader< std::string >::getInstance();
  ItemReader< std::vector< float > >& itemR_Eap =
    StdVectorBinaryItemReader< float >::getInstance();
  
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
