#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorFiberCountFilter_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorFiberCountFilter_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorFiberCountFilter.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-core-exception/Exception.h>




template < class L, class Compare >
inline
gkg::BundleMapOperatorFiberCountFilter< L, Compare >::
                                              BundleMapOperatorFiberCountFilter(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose )
                               : gkg::BundleMapOperator< L, Compare >( verbose )
{

  try
  {

    // sanity checks for string and scalar parameters
    if ( stringParameters.size() != 1U )
    {

      throw std::runtime_error( "one string parameter required" );

    }

    if ( ( scalarParameters.size() != 1U ) &&
         ( scalarParameters.size() != 2U ) )
    {

      throw std::runtime_error( "one or two scalar parameter required" );

    }

    if ( scalarParameters.size() == 1U )
    {

      _testFunction.reset(
        gkg::TestFunctionFactory< int32_t >::getInstance().createTestFunction( 
                                 stringParameters[ 0 ],
                                 ( int32_t )( scalarParameters[ 0 ] + 0.5 ),
                                 0 ) );

    }
    else
    {

      _testFunction.reset(
        gkg::TestFunctionFactory< int32_t >::getInstance().createTestFunction( 
                                 stringParameters[ 0 ],
                                 ( int32_t )( scalarParameters[ 0 ] + 0.5 ),
	                         ( int32_t )( scalarParameters[ 1 ] + 0.5 ) ) );

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMapOperatorFiberCountFilter< L, Compare >::"
             "BundleMapOperatorFiberCountFilter( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template < class L, class Compare >
inline
gkg::BundleMapOperatorFiberCountFilter< L, Compare >::
                                            ~BundleMapOperatorFiberCountFilter()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorFiberCountFilter< L, Compare >::startContribution(
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

      if ( _testFunction->getState( ib->second.getCurve3dCount() ) )
      {

        outputBundleMap->addCurve3ds( ib->first, ib->second.getCurve3ds() );

      }

      ++ ib;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorFiberCountFilter< L, Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorFiberCountFilter< L, Compare >::addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorFiberCountFilter< L, Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorFiberCountFilter< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorFiberCountFilter< L, Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorFiberCountFilter< L, Compare >::getName() const
{

  try
  {

    return gkg::BundleMapOperatorFiberCountFilter< L, Compare >::getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorFiberCountFilter< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorFiberCountFilter< L, Compare >::getStaticName()
{

  try
  {

    return "fiber-count-filter";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorFiberCountFilter< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorFiberCountFilter< L, Compare >::getStringParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: threshold mode, one of:\n"
      ".        - lt -> lower than\n"
      ".        - le -> lower or equal to\n"
      ".        - eq -> equal to\n"
      ".        - di -> different from\n"
      ".        - ge -> greater or equal to\n"
      ".        - gt -> greater than\n"
      ".        - be -> between or equal\n"
      ".        - bt -> between\n"
      ".        - oe -> outside or equal\n"
      ".        - ou -> outside\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorFiberCountFilter< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorFiberCountFilter< L, Compare >::getScalarParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: first threshold\n"
      ".  <P2>: second threshold (required for be/bt/oe/ou)\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorFiberCountFilter< L, Compare >::"
             "getStringParameterHelp()" );

}


#endif
