#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorMergeLabels_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorMergeLabels_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorMergeLabels.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/Exception.h>




template < class L, class Compare >
inline
gkg::BundleMapOperatorMergeLabels< L, Compare >::
                                              BundleMapOperatorMergeLabels(
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
             "gkg::BundleMapOperatorMergeLabels< L, Compare >::"
             "BundleMapOperatorMergeLabels( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}



namespace gkg
{


template <>
inline
BundleMapOperatorMergeLabels< int16_t, std::less< int16_t > >::
                                              BundleMapOperatorMergeLabels(
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
    if ( scalarParameters.size() != 1U )
    {

      throw std::runtime_error( "only one scalar parameter required" );

    }
    _mergedLabel = ( int16_t )( scalarParameters[ 0 ] + 0.5 );

  }
  GKG_CATCH( "template <> "
             "inline "
             "BundleMapOperatorMergeLabels< int16_t, "
             "std::less< int16_t > >::"
             "BundleMapOperatorMergeLabels( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template <>
inline
BundleMapOperatorMergeLabels< std::string, std::less< std::string > >::
                                              BundleMapOperatorMergeLabels(
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
    if ( !scalarParameters.empty() )
    {

      throw std::runtime_error( "no scalar parameter required" );

    }
    if ( stringParameters.size() != 1U )
    {

      throw std::runtime_error( "only one string parameter required" );

    }
    _mergedLabel = stringParameters[ 0 ];

  }
  GKG_CATCH( "template <> "
             "inline "
             "BundleMapOperatorMergeLabels< std::string, "
             "std::less< std::string > >::"
             "BundleMapOperatorMergeLabels( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


}


template < class L, class Compare >
inline
gkg::BundleMapOperatorMergeLabels< L, Compare >::~BundleMapOperatorMergeLabels()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorMergeLabels< L, Compare >::startContribution(
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


    typename gkg::BundleMap< L, Compare >::const_iterator
      ib = inputBundleMap->begin(),
      ibe = inputBundleMap->end();
    while ( ib != ibe )
    {

      outputBundleMap->addCurve3ds( _mergedLabel, ib->second.getCurve3ds() );
      ++ ib;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorMergeLabels< L, Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorMergeLabels< L, Compare >::addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorMergeLabels< L, Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorMergeLabels< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorMergeLabels< L, Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorMergeLabels< L, Compare >::getName() const
{

  try
  {

    return gkg::BundleMapOperatorMergeLabels< L, Compare >::getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorMergeLabels< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorMergeLabels< L, Compare >::getStaticName()
{

  try
  {

    return "merge-labels";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorMergeLabels< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorMergeLabels< L, Compare >::getStringParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: label of merged bundles if string labelling\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorMergeLabels< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorMergeLabels< L, Compare >::getScalarParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: label of merged bundles if int16_t labelling\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorMergeLabels< L, Compare >::"
             "getStringParameterHelp()" );

}


#endif
