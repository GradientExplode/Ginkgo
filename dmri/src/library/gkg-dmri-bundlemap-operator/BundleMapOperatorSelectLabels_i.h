#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorSelectLabels_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorSelectLabels_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorSelectLabels.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/Exception.h>




template < class L, class Compare >
inline
gkg::BundleMapOperatorSelectLabels< L, Compare >::
                                              BundleMapOperatorSelectLabels(
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
             "gkg::BundleMapOperatorSelectLabels< L, Compare >::"
             "BundleMapOperatorSelectLabels( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}



namespace gkg
{


template <>
inline
BundleMapOperatorSelectLabels< int16_t, std::less< int16_t > >::
                                              BundleMapOperatorSelectLabels(
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

      _selectedLabels.insert( ( int16_t )( *s + 0.5 ) );
      ++ s;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "BundleMapOperatorSelectLabels< int16_t, "
             "std::less< int16_t > >::"
             "BundleMapOperatorSelectLabels( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template <>
inline
BundleMapOperatorSelectLabels< std::string, std::less< std::string > >::
                                              BundleMapOperatorSelectLabels(
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

      _selectedLabels.insert( *s );
      _regularExpressions.push_back( gkg::RCPointer< gkg::RegularExpression >(
                                           new gkg::RegularExpression( *s ) ) );
      ++ s;

    }

  }
  GKG_CATCH( "template <> "
             "inline "
             "BundleMapOperatorSelectLabels< std::string, "
             "std::less< std::string > >::"
             "BundleMapOperatorSelectLabels( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


}


template < class L, class Compare >
inline
gkg::BundleMapOperatorSelectLabels< L, Compare >::
                                            ~BundleMapOperatorSelectLabels()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorSelectLabels< L, Compare >::startContribution(
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

      if ( this->isMatching( ib->first ) )
      {

        outputBundleMap->addBundle( ib->first, ib->second );

      }

      ++ ib;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorSelectLabels< L, Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorSelectLabels< L, Compare >::addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorSelectLabels< L, Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorSelectLabels< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorSelectLabels< L, Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorSelectLabels< L, Compare >::getName() const
{

  try
  {

    return gkg::BundleMapOperatorSelectLabels< L, Compare >::getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorSelectLabels< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorSelectLabels< L, Compare >::getStaticName()
{

  try
  {

    return "select-labels";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorSelectLabels< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorSelectLabels< L, Compare >::getStringParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: selected labels if string type labelling\n"
      ".    .\n"
      ".    .\n"
      ".  <Pn>\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorSelectLabels< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorSelectLabels< L, Compare >::getScalarParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: selected labels if int16_t type labelling\n"
      ".    .\n"
      ".    .\n"
      ".  <Pn>\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorSelectLabels< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
bool gkg::BundleMapOperatorSelectLabels< L, Compare >::isMatching(
                                                const L& label ) const
{

  try
  {

    throw std::runtime_error( "should not enter this code" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "bool gkg::BundleMapOperatorSelectLabels< L, Compare >::"
             "isMatching( "
             "const L& label ) const" );

}


namespace gkg
{


template <>
inline
bool BundleMapOperatorSelectLabels< int16_t, std::less< int16_t > >::isMatching(
                                                const int16_t& /*label*/ ) const
{

  try
  {

    return false;

  }
  GKG_CATCH( "template <> "
             "inline "
             "bool BundleMapOperatorSelectLabels< int16_t, "
             "std::less< int16_t > >::isMatching( "
             "const int16_t& /*label*/ ) const" );

}


template <>
inline
bool BundleMapOperatorSelectLabels< std::string,
                                    std::less< std::string > >::isMatching(
                                                const std::string& label ) const
{

  try
  {

    if ( _selectedLabels.find( label ) != _selectedLabels.end() )
    {

      return true;

    }


    std::list< RCPointer< RegularExpression > >::const_iterator
      r = _regularExpressions.begin(),
      re = _regularExpressions.end();
    while ( r != re )
    {

      if ( ( *r )->match( label, 0 ) >= 0 )
      {

        return true;

      } 
      ++ r;

    }

    return false;

  }
  GKG_CATCH( "template <> "
             "inline "
             "bool BundleMapOperatorSelectLabels< std::string, "
             "std::less< std::string > >::isMatching( "
             "const std::string& label ) const" );

}


}


#endif
