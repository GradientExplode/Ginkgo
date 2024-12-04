#ifndef _gkg_dmri_io_DefaultBinaryItemWriterOfEap_i_h_
#define _gkg_dmri_io_DefaultBinaryItemWriterOfEap_i_h_


#include <gkg-core-io/DefaultBinaryItemWriter_i.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-io/StdVectorBinaryItemWriter_i.h>


//
// class DefaultBinaryItemWriter< OrientationDistributionFunction >
//

namespace gkg
{

template <>
inline
DefaultBinaryItemWriter< OrientationDistributionFunction
                       >::DefaultBinaryItemWriter()
                         : ItemWriter< OrientationDistributionFunction >()
{
}


template <>
inline
DefaultBinaryItemWriter< OrientationDistributionFunction
                       >::~DefaultBinaryItemWriter()
{
}


template <>
inline
void 
DefaultBinaryItemWriter< OrientationDistributionFunction >::write(
                                   std::ostream & os,
                                   const OrientationDistributionFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< std::string >& itemW_String =
    DefaultBinaryItemWriter< std::string >::getInstance();
  ItemWriter< std::vector< float > >& itemW_Eap =
    StdVectorBinaryItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_String.write( os, pItem[ i ].getBasisTypeName() );
    itemW_Eap.write( os, pItem[ i ].getValues() );

  }

}


template <>
inline
void 
DefaultBinaryItemWriter< OrientationDistributionFunction >::write(
                                   largefile_ofstream & os,
                                   const OrientationDistributionFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< std::string >& itemW_String =
    DefaultBinaryItemWriter< std::string >::getInstance();

  ItemWriter< std::vector< float > >& itemW_Eap =
    StdVectorBinaryItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_String.write( os, pItem[ i ].getBasisTypeName() );
    itemW_Eap.write( os, pItem[ i ].getValues() );

  }

}


template <>
inline
void
DefaultBinaryItemWriter< OrientationDistributionFunction >::write(
                                   largefile_fstream & fs,
                                   const OrientationDistributionFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< std::string >& itemW_String =
    DefaultBinaryItemWriter< std::string >::getInstance();
  ItemWriter< std::vector< float > >& itemW_Eap =
    StdVectorBinaryItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_String.write( fs, pItem[ i ].getBasisTypeName() );
    itemW_Eap.write( fs, pItem[ i ].getValues() );

  }

}


}


#endif
