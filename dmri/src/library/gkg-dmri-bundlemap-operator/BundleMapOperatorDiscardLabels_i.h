#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorDiscardLabels_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorDiscardLabels_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorDiscardLabels.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/Exception.h>




template < class L, class Compare >
inline
gkg::BundleMapOperatorDiscardLabels< L, Compare >::
                                              BundleMapOperatorDiscardLabels(
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
             "gkg::BundleMapOperatorDiscardLabels< L, Compare >::"
             "BundleMapOperatorDiscardLabels( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}



namespace gkg
{


template <>
inline
BundleMapOperatorDiscardLabels< int16_t, std::less< int16_t > >::
                                              BundleMapOperatorDiscardLabels(
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
    std::vector< double >::const_iterator
      s = scalarParameters.begin(),
      se = scalarParameters.end();
    while ( s != se )
    {

      _discardedLabels.insert( ( int16_t )( *s + 0.5 ) );
      ++ s;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "BundleMapOperatorDiscardLabels< int16_t, "
             "std::less< int16_t > >::"
             "BundleMapOperatorDiscardLabels( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template <>
inline
BundleMapOperatorDiscardLabels< std::string, std::less< std::string > >::
                                              BundleMapOperatorDiscardLabels(
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
    std::vector< std::string >::const_iterator
      s = stringParameters.begin(),
      se = stringParameters.end();
    while ( s != se )
    {

      _discardedLabels.insert( *s );
      ++ s;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "BundleMapOperatorDiscardLabels< std::string, "
             "std::less< std::string > >::"
             "BundleMapOperatorDiscardLabels( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


}


template < class L, class Compare >
inline
gkg::BundleMapOperatorDiscardLabels< L, Compare >::
                                            ~BundleMapOperatorDiscardLabels()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorDiscardLabels< L, Compare >::startContribution(
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

      if ( _discardedLabels.find( ib->first ) == _discardedLabels.end() )
      {

        outputBundleMap->addCurve3ds( ib->first, ib->second.getCurve3ds() );

      }

      ++ ib;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorDiscardLabels< L, Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorDiscardLabels< L, Compare >::addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorDiscardLabels< L, Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorDiscardLabels< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorDiscardLabels< L, Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorDiscardLabels< L, Compare >::getName() const
{

  try
  {

    return gkg::BundleMapOperatorDiscardLabels< L, Compare >::getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorDiscardLabels< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorDiscardLabels< L, Compare >::getStaticName()
{

  try
  {

    return "discard-labels";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorDiscardLabels< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorDiscardLabels< L, Compare >::getStringParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: discarded labels if string type labelling\n"
      ".    .\n"
      ".    .\n"
      ".  <Pn>\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorDiscardLabels< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorDiscardLabels< L, Compare >::getScalarParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: discarded labels if int16_t type labelling\n"
      ".    .\n"
      ".    .\n"
      ".  <Pn>\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorDiscardLabels< L, Compare >::"
             "getStringParameterHelp()" );

}


#endif
