#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorRandomSelection_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorRandomSelection_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorRandomSelection.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-algobase/RandomIndexVector.h>
#include <gkg-core-exception/Exception.h>


template < class L, class Compare >
inline
gkg::BundleMapOperatorRandomSelection< L, Compare >::
                                               BundleMapOperatorRandomSelection(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose )
                               : gkg::BundleMapOperator< L, Compare >( verbose )
{

  try
  {

    // sanity checks for string parameters
    if ( !stringParameters.empty()  )
    {

      throw std::runtime_error( "no string string parameter required" );

    }

    // sanity checks for scalar parameters
    if ( scalarParameters.size() != 1U )
    {

      throw std::runtime_error( "scalar parameters must have 1 argument" );

    }
    if ( ( scalarParameters[ 0 ] < 0.0 ) || ( scalarParameters[ 0 ] > 100.0 ) )
    {

      throw std::runtime_error(
                        "kept fiber percentage must be between 0.0 and 100.0" );

    }
    _keptFiberPercentage = scalarParameters[ 0 ];

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMapOperatorRandomSelection< L, Compare >::"
             "BundleMapOperatorRandomSelection( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template < class L, class Compare >
inline
gkg::BundleMapOperatorRandomSelection< L, Compare >::
                                             ~BundleMapOperatorRandomSelection()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorRandomSelection< L, Compare >::startContribution(
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


    // looping over input bundles
    typename gkg::BundleMap< L, Compare >::const_iterator
      ib = inputBundleMap->begin(),
      ibe = inputBundleMap->end();
    while ( ib != ibe )
    {

      // computing the number of kept curves (at least 1 per bundle)
      int32_t keptCurve3dCount = std::max(
                                 1,
                                 ( int32_t )( ib->second.getCurve3dCount() *
                                              _keptFiberPercentage / 100.0 ) );

      // computing the selection
      std::vector< bool > selection( ib->second.getCurve3dCount() );
      if( ib->second.getCurve3dCount() > 0 )
      {

        gkg::getRandomSelection( selection, keptCurve3dCount );

      }

      // collecting the selected fibers
      std::list< gkg::LightCurve3d< float > > fibers;
      std::vector< gkg::LightCurve3d< float > >::const_iterator
        f = ib->second.begin(),
        fe = ib->second.end();
      std::vector< bool >::const_iterator s = selection.begin();
      while ( f != fe )
      {

        if ( *s )
        {

          fibers.push_back( *f );

        }
        ++ f;
        ++ s;

      }

      // adding the kept fibers to the output bundle map
      outputBundleMap->addCurve3ds( ib->first, fibers );

      ++ ib;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorRandomSelection< L, Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorRandomSelection< L, Compare >::addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorRandomSelection< L, Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorRandomSelection< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorRandomSelection< L, Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorRandomSelection< L, Compare >::getName() const
{

  try
  {

    return 
      gkg::BundleMapOperatorRandomSelection< L, Compare >::getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorRandomSelection< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorRandomSelection< L, Compare >::getStaticName()
{

  try
  {

    return "random-selection";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorRandomSelection< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorRandomSelection< L, Compare >::getStringParameterHelp()
{

  try
  {

    return std::string( ".  N/A\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorRandomSelection< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorRandomSelection< L, Compare >::getScalarParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: kept fiber percentage between 0.0 and 100.0\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorRandomSelection< L, Compare >::"
             "getStringParameterHelp()" );

}


#endif
