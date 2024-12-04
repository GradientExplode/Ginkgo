#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorChangeLabels_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorChangeLabels_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorChangeLabels.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/Exception.h>




template < class L, class Compare >
inline
gkg::BundleMapOperatorChangeLabels< L, Compare >::
                                              BundleMapOperatorChangeLabels(
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
             "gkg::BundleMapOperatorChangeLabels< L, Compare >::"
             "BundleMapOperatorChangeLabels( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}



namespace gkg
{


template <>
inline
BundleMapOperatorChangeLabels< int16_t, std::less< int16_t > >::
                                              BundleMapOperatorChangeLabels(
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
    int16_t oldLabel;
    int16_t newLabel;

    std::vector< double >::const_iterator
      s = scalarParameters.begin(),
      se = scalarParameters.end();
    while ( s != se )
    {

      oldLabel = ( int16_t )( *s + 0.5 );
      ++ s;
      newLabel = ( int16_t )( *s + 0.5 );
      ++ s;

      _labelChanges[ oldLabel ] = newLabel;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "BundleMapOperatorChangeLabels< int16_t, "
             "std::less< int16_t > >::"
             "BundleMapOperatorChangeLabels( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template <>
inline
BundleMapOperatorChangeLabels< std::string, std::less< std::string > >::
                                              BundleMapOperatorChangeLabels(
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

    std::string oldLabel;
    std::string newLabel;

    std::vector< std::string >::const_iterator
      s = stringParameters.begin(),
      se = stringParameters.end();
    while ( s != se )
    {

      oldLabel = *s;
      ++ s;
      newLabel = *s;
      ++ s;

      _labelChanges[ oldLabel ] = newLabel;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "BundleMapOperatorChangeLabels< std::string, "
             "std::less< std::string > >::"
             "BundleMapOperatorChangeLabels( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


}


template < class L, class Compare >
inline
gkg::BundleMapOperatorChangeLabels< L, Compare >::
                                            ~BundleMapOperatorChangeLabels()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorChangeLabels< L, Compare >::startContribution(
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

      typename std::map< L, L, Compare >::const_iterator
        c = _labelChanges.find( ib->first );
      if ( c == _labelChanges.end() )
      {

        outputBundleMap->addCurve3ds( ib->first, ib->second.getCurve3ds() );

      }
      else
      {

        outputBundleMap->addCurve3ds( c->second, ib->second.getCurve3ds() );

      }

      ++ ib;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorChangeLabels< L, Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorChangeLabels< L, Compare >::addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorChangeLabels< L, Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorChangeLabels< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorChangeLabels< L, Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorChangeLabels< L, Compare >::getName() const
{

  try
  {

    return gkg::BundleMapOperatorChangeLabels< L, Compare >::getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorChangeLabels< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorChangeLabels< L, Compare >::getStaticName()
{

  try
  {

    return "change-labels";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorChangeLabels< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorChangeLabels< L, Compare >::getStringParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: series of [ old-label new-label ] if string type labelling\n"
      ".    .\n"
      ".    .\n"
      ".  <Pn>\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorChangeLabels< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorChangeLabels< L, Compare >::getScalarParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: series of [ old-label new-label ] if int16_t type labelling\n"
      ".    .\n"
      ".    .\n"
      ".  <Pn>\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorChangeLabels< L, Compare >::"
             "getStringParameterHelp()" );

}


#endif
