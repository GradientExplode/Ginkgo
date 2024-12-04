#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorResampler_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorResampler_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorResampler.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>


template < class L, class Compare >
inline
gkg::BundleMapOperatorResampler< L, Compare >::BundleMapOperatorResampler(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose )
                               : gkg::BundleMapOperator< L, Compare >( verbose )
{

  try
  {

    // sanity checks for string parameters
    if ( stringParameters.empty() )
    {

      _hasConstantStep = true;
      throw std::runtime_error( "no string parameter required" );

    }
    else
    {

      if ( stringParameters.size() != 1U )
      {

        throw std::runtime_error( "string parameters must have 1 argument" );

      }

      if ( ( stringParameters[ 0 ] != "constant-step" ) &&
           ( stringParameters[ 0 ] != "constant-point-count" ) )
      {

        throw std::runtime_error( "resampling type must be 'constant-step' or "
                                  "'constant-point-count'" );

      }
      if ( stringParameters[ 0 ] == "constant-step" )
      {

        _hasConstantStep = true;

      }
      else
      {

        _hasConstantStep = false;

      }

    }

    // sanity checks for scalar parameters
    if ( scalarParameters.size() != 1U )
    {

      throw std::runtime_error( "scalar parameters must have 1 argument" );

    }
    if ( _hasConstantStep )
    {

      _resamplingStep = ( float )scalarParameters[ 0 ];
      _resamplingPointCount = 0;
      if ( _resamplingStep <= 0.0 )
      {

        throw std::runtime_error( "resampling step must be strictly positive" );

      }

    }
    else
    {

      _resamplingStep = 0.0f;
      _resamplingPointCount = ( int32_t )( scalarParameters[ 0 ] + 0.5 );

      if ( _resamplingPointCount < 2 )
      {

        throw std::runtime_error( 
                              "resampling point count must be greater than 1" );

      }

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMapOperatorResampler< L, Compare >::"
             "BundleMapOperatorResampler( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template < class L, class Compare >
inline
gkg::BundleMapOperatorResampler< L, Compare >::~BundleMapOperatorResampler()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorResampler< L, Compare >::startContribution(
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
    int32_t fiberIndex = 0;
    typename gkg::BundleMap< L, Compare >::const_iterator
      ib = inputBundleMap->begin(),
      ibe = inputBundleMap->end();
    while ( ib != ibe )
    {

      fiberIndex = 0;

      // collecting the selected fibers
      gkg::Vector3d< float > tp;
      std::list< gkg::LightCurve3d< float > > fibers;
      std::vector< gkg::LightCurve3d< float > >::const_iterator
        f = ib->second.begin(),
        fe = ib->second.end();
      while ( f != fe )
      {

        if ( _hasConstantStep )
        {

          float fiberLength = f->getLength();
          int32_t resamplingPointCount = std::max( 2,
                                                   ( int32_t )( fiberLength /
                                                   _resamplingStep + 0.5 ) );
          fibers.push_back( f->getEquidistantCurve( resamplingPointCount ) );

        }
        else
        {

          fibers.push_back( f->getEquidistantCurve( _resamplingPointCount ) );

        }
        ++ fiberIndex;
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
             "void gkg::BundleMapOperatorResampler< L, "
             "Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorResampler< L, Compare >::addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorResampler< L, "
             "Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorResampler< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorResampler< L, "
             "Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorResampler< L, Compare >::getName() const
{

  try
  {

    return  gkg::BundleMapOperatorResampler< L, Compare >:: getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorResampler< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorResampler< L, Compare >::getStaticName()
{

  try
  {

    return "resampler";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorResampler< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorResampler< L, Compare >::getStringParameterHelp()
{

  try
  {

    return std::string( ".  <P1>: resampling mode constant-step/"
                        "constant-point-count (default=constant-step)\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorResampler< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorResampler< L, Compare >::getScalarParameterHelp()
{

  try
  {

    return std::string( ".  <P1>: resampling step in mm in case of "
                        "'constant-step' mode or resampling point count "
                        "in case of 'constant-point-count' mode\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorResampler< L, Compare >::"
             "getStringParameterHelp()" );

}


#endif
