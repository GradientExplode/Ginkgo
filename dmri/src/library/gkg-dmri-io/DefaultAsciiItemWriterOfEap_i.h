#ifndef _gkg_dmri_io_DefaultAsciiItemWriterOfEap_i_h_
#define _gkg_dmri_io_DefaultAsciiItemWriterOfEap_i_h_


#include <gkg-core-io/DefaultAsciiItemWriter_i.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-io/StdVectorAsciiItemWriter_i.h>


//
// class DefaultAsciiItemWriter< OrientationDistributionFunction >
//

namespace gkg
{

template <>
inline
DefaultAsciiItemWriter< OrientationDistributionFunction
                      >::DefaultAsciiItemWriter()
                        : ItemWriter< OrientationDistributionFunction >()
{
}


template <>
inline
DefaultAsciiItemWriter< OrientationDistributionFunction
                      >::~DefaultAsciiItemWriter()
{
}


template <>
inline
void 
DefaultAsciiItemWriter< OrientationDistributionFunction >::write(
                                   std::ostream & os,
                                   const OrientationDistributionFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< std::string >& itemW_String =
    DefaultAsciiItemWriter< std::string >::getInstance();
  ItemWriter< std::vector< float > >& itemW_Eap =
    StdVectorAsciiItemWriter< float >::getInstance();
  
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
DefaultAsciiItemWriter< OrientationDistributionFunction >::write(
                                   largefile_ofstream & os,
                                   const OrientationDistributionFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< std::string >& itemW_String =
    DefaultAsciiItemWriter< std::string >::getInstance();
  ItemWriter< std::vector< float > >& itemW_Eap =
    StdVectorAsciiItemWriter< float >::getInstance();
  
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
DefaultAsciiItemWriter< OrientationDistributionFunction >::write(
                                   largefile_fstream & fs,
                                   const OrientationDistributionFunction* pItem,
                                   size_t count ) const
{

  ItemWriter< std::string >& itemW_String =
    DefaultAsciiItemWriter< std::string >::getInstance();
  ItemWriter< std::vector< float > >& itemW_Eap =
    StdVectorAsciiItemWriter< float >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_String.write( fs, pItem[ i ].getBasisTypeName() );
    itemW_Eap.write( fs, pItem[ i ].getValues() );

  }

}


}


#endif
