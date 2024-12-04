#ifndef _gkg_dmri_io_DefaultBSwapItemWriterOfEap_i_h_
#define _gkg_dmri_io_DefaultBSwapItemWriterOfEap_i_h_


#include <gkg-core-io/DefaultBSwapItemWriter_i.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-io/StdVectorBSwapItemWriter_i.h>


//
// class DefaultBSwapItemWriter< OrientationDistributionFunction >
//

namespace gkg
{

template <>
inline
DefaultBSwapItemWriter< OrientationDistributionFunction
                      >::DefaultBSwapItemWriter()
                        : ItemWriter< OrientationDistributionFunction >()
{
}


template <>
inline
DefaultBSwapItemWriter< OrientationDistributionFunction
                      >::~DefaultBSwapItemWriter()
{
}


template <>
inline
void 
DefaultBSwapItemWriter< OrientationDistributionFunction >::write(
                                   std::ostream & os,
                                   const OrientationDistributionFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< std::string >& itemW_String =
    DefaultBSwapItemWriter< std::string >::getInstance();
  ItemWriter< std::vector< float > >& itemW_Eap =
    StdVectorBSwapItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_String.write( os, pItem[i].getBasisTypeName() );
    itemW_Eap.write( os, pItem[ i ].getValues() );

  }

}


template <>
inline
void 
DefaultBSwapItemWriter< OrientationDistributionFunction >::write(
                                   largefile_ofstream & os,
                                   const OrientationDistributionFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< std::string >& itemW_String =
    DefaultBSwapItemWriter< std::string >::getInstance();
  ItemWriter< std::vector< float > >& itemW_Eap =
    StdVectorBSwapItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_String.write( os, pItem[i].getBasisTypeName() );
    itemW_Eap.write( os, pItem[ i ].getValues() );

  }

}


template <>
inline
void
DefaultBSwapItemWriter< OrientationDistributionFunction >::write(
                                   largefile_fstream & fs,
                                   const OrientationDistributionFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< std::string >& itemW_String =
    DefaultBSwapItemWriter< std::string >::getInstance();
  ItemWriter< std::vector< float > >& itemW_Eap =
    StdVectorBSwapItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_String.write( fs, pItem[i].getBasisTypeName() );
    itemW_Eap.write( fs, pItem[ i ].getValues() );

  }

}


}


#endif
