#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorReverseFibers_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorReverseFibers_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorReverseFibers.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-exception/Exception.h>


template < class L, class Compare >
inline
gkg::BundleMapOperatorReverseFibers< L, Compare >::
                                              BundleMapOperatorReverseFibers(
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

      throw std::runtime_error( "no string parameter required" );

    }

    // sanity checks for scalar parameters
    if ( !scalarParameters.empty()  )
    {

      throw std::runtime_error( "no scalar parameter required" );

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMapOperatorReverseFibers< L, Compare >::"
             "BundleMapOperatorReverseFibers( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template < class L, class Compare >
inline
gkg::BundleMapOperatorReverseFibers< L, Compare >::
                                              ~BundleMapOperatorReverseFibers()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorReverseFibers< L, Compare >::startContribution(
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

      // collecting the selected fibers
      std::list< gkg::LightCurve3d< float > > fibers;
      std::vector< gkg::LightCurve3d< float > >::const_iterator
        f = ib->second.begin(),
        fe = ib->second.end();
      while ( f != fe )
      {

        gkg::LightCurve3d< float > currentFiber =  *f;
        currentFiber.reverse();
        fibers.push_back( currentFiber );

        ++ f;

      }

      if ( !fibers.empty() )
      {

        // adding the kept fibers to the output bundle map
        outputBundleMap->addCurve3ds( ib->first, fibers );

      }

      ++ ib;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorReverseFibers< L, Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorReverseFibers< L, Compare >::addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorReverseFibers< L, Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorReverseFibers< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorReverseFibers< L, Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorReverseFibers< L, Compare >::getName() const
{

  try
  {

    return 
      gkg::BundleMapOperatorReverseFibers< L, Compare >::getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorReverseFibers< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorReverseFibers< L, Compare >::getStaticName()
{

  try
  {

    return "reverse-fibers";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorReverseFibers< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorReverseFibers< L, Compare >::getStringParameterHelp()
{

  try
  {

    return std::string( ".  N/A\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorReverseFibers< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorReverseFibers< L, Compare >::getScalarParameterHelp()
{

  try
  {

    return std::string( ".  N/A\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorReverseFibers< L, Compare >::"
             "getStringParameterHelp()" );

}


#endif
