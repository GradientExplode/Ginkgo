#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorSplitLabels_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorSplitLabels_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorSplitLabels.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>




template < class L, class Compare >
inline
gkg::BundleMapOperatorSplitLabels< L, Compare >::BundleMapOperatorSplitLabels(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose )
                               : gkg::BundleMapOperator< L, Compare >( verbose )
{

  try
  {

    throw std::runtime_error( "should not enter this code" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMapOperatorSplitLabels< L, Compare >::"
             "BundleMapOperatorSplitLabels( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}



namespace gkg
{


template <>
inline
BundleMapOperatorSplitLabels< int16_t, std::less< int16_t > >::
                                              BundleMapOperatorSplitLabels(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose )
                               : BundleMapOperator< int16_t,
                                                    std::less< int16_t > >(
                                                                       verbose )
{

  try
  {

    // sanity checks for string and scalar parameters
    if ( !stringParameters.empty() )
    {

      throw std::runtime_error( "no string parameter required" );

    }
    if ( !scalarParameters.empty() )
    {

      throw std::runtime_error( "no scalar parameter required" );

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "BundleMapOperatorSplitLabels< int16_t, "
             "std::less< int16_t > >::"
             "BundleMapOperatorSplitLabels( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template <>
inline
BundleMapOperatorSplitLabels< std::string, std::less< std::string > >::
                                              BundleMapOperatorSplitLabels(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose )
                           : gkg::BundleMapOperator< std::string,
                                                     std::less< std::string > >(
                                                                       verbose )
{

  try
  {

    // sanity checks for string and scalar parameters
    if ( !stringParameters.empty() )
    {

      throw std::runtime_error( "no string parameter required" );

    }
    if ( !scalarParameters.empty() )
    {

      throw std::runtime_error( "no scalar parameter required" );

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "BundleMapOperatorSplitLabels< std::string, "
             "std::less< std::string > >::"
             "BundleMapOperatorSplitLabels( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


}


template < class L, class Compare >
inline
gkg::BundleMapOperatorSplitLabels< L, Compare >::~BundleMapOperatorSplitLabels()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorSplitLabels< L, Compare >::startContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    // sanity checks
    if ( inputBundleMap.isNull() )
    {

      throw std::runtime_error( "input pointer is empty" );

    }
    if ( outputBundleMap.isNull() )
    {

      throw std::runtime_error( "output pointer is empty" );

    }

    // collecting output base filename
    std::string splitOutputBaseName = "";
    if ( inputBundleMap->getHeader().hasAttribute( "split_output_base_name" ) )
    {

      inputBundleMap->getHeader().getAttribute( "split_output_base_name",
                                                splitOutputBaseName );

    }

    // collecting output bundle map format
    std::string splitOutputFormat = "";
    if ( inputBundleMap->getHeader().hasAttribute( "split_output_format" ) )
    {

      inputBundleMap->getHeader().getAttribute( "split_output_format",
                                                splitOutputFormat );

    }

    // collecting output ascii mode
    bool ascii = false;
    if ( inputBundleMap->getHeader().hasAttribute( "split_ascii" ) )
    {

      double split_ascii = 0.0;
      inputBundleMap->getHeader().getAttribute( "split_ascii",
                                                split_ascii );
      if ( ( int32_t )( split_ascii + 0.5 ) )
      {

        ascii = true;

      }

    }

    typename gkg::BundleMap< L, Compare >::const_iterator
      ib = inputBundleMap->begin(),
      ibe = inputBundleMap->end();
    while ( ib != ibe )
    {

      outputBundleMap->addBundle( ib->first, ib->second );

      gkg::Writer::getInstance().write(
        splitOutputBaseName + "_" + gkg::StringConverter::toString( ib->first ),
        *outputBundleMap,
        ascii,
        splitOutputFormat );

      outputBundleMap->clear();

      ++ ib;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorSplitLabels< L, Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorSplitLabels< L, Compare >::addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorSplitLabels< L, Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorSplitLabels< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorSplitLabels< L, Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorSplitLabels< L, Compare >::getName() const
{

  try
  {

    return gkg::BundleMapOperatorSplitLabels< L, Compare >::getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorSplitLabels< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
bool 
gkg::BundleMapOperatorSplitLabels< L, Compare >::saveOutputBundleMap() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "bool "
             "gkg::BundleMapOperatorSplitLabels< L, Compare >::"
             "saveOutputBundleMap() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorSplitLabels< L, Compare >::getStaticName()
{

  try
  {

    return "split-labels";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorSplitLabels< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorSplitLabels< L, Compare >::getStringParameterHelp()
{

  try
  {

    return std::string( ".  N/A\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorSplitLabels< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorSplitLabels< L, Compare >::getScalarParameterHelp()
{

  try
  {

    return std::string( ".  N/A\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorSplitLabels< L, Compare >::"
             "getStringParameterHelp()" );

}


#endif
