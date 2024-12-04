#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorCutExtremities_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorCutExtremities_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorCutExtremities.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-exception/Exception.h>


template < class L, class Compare >
inline
gkg::BundleMapOperatorCutExtremities< L, Compare >::
                                               BundleMapOperatorCutExtremities(
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
    if ( scalarParameters[ 0 ] <= 0.0 )
    {

      throw std::runtime_error(
         "point count to be removed at extremities must be strictly positive" );

    }
    _pointCountToBeRemoved = ( int32_t )( scalarParameters[ 0 ] + 0.5 );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMapOperatorCutExtremities< L, Compare >::"
             "BundleMapOperatorCutExtremities( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template < class L, class Compare >
inline
gkg::BundleMapOperatorCutExtremities< L, Compare >::
                                              ~BundleMapOperatorCutExtremities()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorCutExtremities< L, Compare >::startContribution(
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

        int32_t p = 0;
        int32_t pointCount = f->getPointCount();
        std::list< gkg::Vector3d< float > > points;
        if ( pointCount >= ( 2 * _pointCountToBeRemoved + 2 ) )
        {

          for ( p = _pointCountToBeRemoved;
                p < pointCount - _pointCountToBeRemoved; p++ )
          {

            points.push_back( f->getPoint( p ) );

          }
          fibers.push_back( gkg::LightCurve3d< float >( points ) );

        }
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
             "void gkg::BundleMapOperatorCutExtremities< L, Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorCutExtremities< L, Compare >::addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorCutExtremities< L, Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorCutExtremities< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorCutExtremities< L, Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorCutExtremities< L, Compare >::getName() const
{

  try
  {

    return 
      gkg::BundleMapOperatorCutExtremities< L, Compare >::getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorCutExtremities< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorCutExtremities< L, Compare >::getStaticName()
{

  try
  {

    return "cut-extremities";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorCutExtremities< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorCutExtremities< L, Compare >::getStringParameterHelp()
{

  try
  {

    return std::string( ".  N/A\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorCutExtremities< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorCutExtremities< L, Compare >::getScalarParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: point count to be removed at extremities of fibers\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorCutExtremities< L, Compare >::"
             "getStringParameterHelp()" );

}


#endif
