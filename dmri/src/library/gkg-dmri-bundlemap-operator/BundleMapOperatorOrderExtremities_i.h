#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorOrderExtremities_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorOrderExtremities_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorOrderExtremities.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-exception/Exception.h>


template < class L, class Compare >
inline
gkg::BundleMapOperatorOrderExtremities< L, Compare >::
                                              BundleMapOperatorOrderExtremities(
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
    if ( scalarParameters.size() != 1U )
    {

      throw std::runtime_error( "scalar parameters must have 1 argument" );

    }
    if ( scalarParameters[ 0 ] <= 0.0 )
    {

      throw std::runtime_error(
         "resampling point count must be strictly positive" );

    }
    _resamplingPointCount = ( int32_t )( scalarParameters[ 0 ] + 0.5 );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMapOperatorOrderExtremities< L, Compare >::"
             "BundleMapOperatorOrderExtremities( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template < class L, class Compare >
inline
gkg::BundleMapOperatorOrderExtremities< L, Compare >::
                                              ~BundleMapOperatorOrderExtremities()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorOrderExtremities< L, Compare >::startContribution(
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

      gkg::LightCurve3d< float > referenceFiber = 
                                f->getEquidistantCurve( _resamplingPointCount );
      fibers.push_back( *f );
      ++ f;


      while ( f != fe )
      {

        gkg::LightCurve3d< float > fiber =
                                f->getEquidistantCurve( _resamplingPointCount );

        gkg::LightCurve3d< float >::const_iterator
          referenceSite = referenceFiber.begin();

        gkg::LightCurve3d< float >::const_reverse_iterator
          reverseSite = fiber.rbegin();
        gkg::LightCurve3d< float >::const_iterator site = fiber.begin(),
                                                   siteEnd = fiber.end();

        float directDistance = 0.0;
        float reverseDistance = 0.0;
        while ( site != siteEnd )
        {

          directDistance += ( *site - *referenceSite ).getNorm2();
          reverseDistance += ( *reverseSite - *referenceSite ).getNorm2();

          ++ site;
          ++ referenceSite;
          ++ reverseSite;


        }

        gkg::LightCurve3d< float > currentFiber =  *f;

        if ( reverseDistance < directDistance )
        {

          currentFiber.reverse();

        }

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
             "void gkg::BundleMapOperatorOrderExtremities< L, Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorOrderExtremities< L, Compare >::addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorOrderExtremities< L, Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorOrderExtremities< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorOrderExtremities< L, Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorOrderExtremities< L, Compare >::getName() const
{

  try
  {

    return 
      gkg::BundleMapOperatorOrderExtremities< L, Compare >::getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorOrderExtremities< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorOrderExtremities< L, Compare >::getStaticName()
{

  try
  {

    return "order-extremities";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorOrderExtremities< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorOrderExtremities< L, Compare >::getStringParameterHelp()
{

  try
  {

    return std::string( ".  N/A\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorOrderExtremities< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorOrderExtremities< L, Compare >::getScalarParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: resampling point count to compute cross-fiber distance\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorOrderExtremities< L, Compare >::"
             "getStringParameterHelp()" );

}


#endif
