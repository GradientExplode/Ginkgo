#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorLengthSelection_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorLengthSelection_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorLengthSelection.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-algobase/TestFunction.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-processing-algobase/AverageFilter_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>


template < class L, class Compare >
inline
gkg::BundleMapOperatorLengthSelection< L, Compare >::
                                   BundleMapOperatorLengthSelection(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose )
                               : gkg::BundleMapOperator< L, Compare >( verbose )
{

  try
  {

    // sanity checks for string parameters
    if ( stringParameters.size() != 1U )
    {

      throw std::runtime_error( "string parameters must have 1 argument" );

    }

    // sanity checks for scalar parameters
    if ( ( scalarParameters.size() != 1U ) &&
         ( scalarParameters.size() != 2U ) )
         
    {

      throw std::runtime_error( 
                            "scalar parameters  must have 1 or 2 argument(s)" );

    }

    // creating the adequate test function

    if ( scalarParameters.size() == 1U )
    {

      _testFunction.reset( gkg::TestFunctionFactory< float >::getInstance().
                                createTestFunction(
                                          stringParameters[ 0 ],
                                          ( float )scalarParameters[ 0 ] ) );

    }
    else
    {

      _testFunction.reset( gkg::TestFunctionFactory< float >::getInstance().
                                createTestFunction(
                                          stringParameters[ 0 ],
                                          ( float )scalarParameters[ 0 ],
                                          ( float )scalarParameters[ 1 ] ) );

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMapOperatorLengthSelection< L, Compare >::"
             "BundleMapOperatorLengthSelection( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template < class L, class Compare >
inline
gkg::BundleMapOperatorLengthSelection< L, Compare >::
                                 ~BundleMapOperatorLengthSelection()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorLengthSelection< L, Compare >::startContribution(
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

    // creating an avearge filter
    gkg::AverageFilter< std::vector< float >, float > averageFilter;
    float averageLength = 0.0f;

    // looping over input bundles
    typename gkg::BundleMap< L, Compare >::const_iterator
      ib = inputBundleMap->begin(),
      ibe = inputBundleMap->end();
    while ( ib != ibe )
    {

      const L& label = ib->first;
      const typename gkg::BundleMap< L, Compare >::Bundle&
        bundle = ib->second;

      std::vector< float > lengths( bundle.getCurve3dCount() );
      std::vector< float >::iterator l = lengths.begin();
      // looping over fibers
      std::vector< gkg::LightCurve3d< float > >::const_iterator
        f = ib->second.begin(),
        fe = ib->second.end();
      while ( f != fe )
      {

        const gkg::LightCurve3d< float >& fiber = *f;

        *l = fiber.getLength();
        ++ f;
        ++ l;

      }
      averageFilter.filter( lengths, averageLength );

      if ( _testFunction->getState( averageLength ) )
      {

        // adding the kept fibers to the output bundle map
        outputBundleMap->addBundle( label, bundle );

      }

      ++ ib;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorLengthSelection< L, "
             "Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorLengthSelection< L, Compare >::addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorLengthSelection< L, "
             "Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorLengthSelection< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorLengthSelection< L, "
             "Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorLengthSelection< L, Compare >::getName() const
{

  try
  {

    return  gkg::BundleMapOperatorLengthSelection< L, Compare >:: getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorLengthSelection< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorLengthSelection< L, Compare >::getStaticName()
{

  try
  {

    return "length-selection";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorLengthSelection< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorLengthSelection< L, Compare >::getStringParameterHelp()
{

  try
  {

    return std::string( ".  <P1>: selection mode, one of\n"
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
             "gkg::BundleMapOperatorLengthSelection< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorLengthSelection< L, Compare >::getScalarParameterHelp()
{

  try
  {

    return std::string( ".  <P1>: 1st threshold\n"
                        ".  <P2>: 2nd threshold (optional)\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorLengthSelection< L, Compare >::"
             "getStringParameterHelp()" );

}


#endif
