#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorLengthNormalization_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorLengthNormalization_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorLengthNormalization.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-algobase/AverageFilter_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>


template < class L, class Compare >
inline
gkg::BundleMapOperatorLengthNormalization< L, Compare >::
                                   BundleMapOperatorLengthNormalization(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose )
                               : gkg::BundleMapOperator< L, Compare >( verbose )
{

  try
  {

    // sanity checks for string parameters
    if ( !stringParameters.empty() )
    {

      throw std::runtime_error( "there are no string parameters" );

    }

    // sanity checks for scalar parameters
    if ( scalarParameters.size() != 2U )
         
    {

      throw std::runtime_error( "scalar parameters must have 2 argument" );

    }

    // collecting the target bundle length
    _targetLength = ( float )scalarParameters[ 0 ];
    _keepOriginalCenterOfMass = ( scalarParameters[ 1 ] > 0 ? true : false );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMapOperatorLengthNormalization< L, Compare >::"
             "BundleMapOperatorLengthNormalization( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template < class L, class Compare >
inline
gkg::BundleMapOperatorLengthNormalization< L, Compare >::
                                 ~BundleMapOperatorLengthNormalization()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorLengthNormalization< L, Compare >::startContribution(
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
    float normalizationFactor = 1.0f;

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
      // looping over fibers to compute their average length
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
      
      // the normalization factor for the current bundle
      // corresponds to the ratio between the target length 
      // and the average length of the current bundle
      normalizationFactor = _targetLength / averageLength;
      
      // computing the normalized fiber(s)
      std::list< gkg::LightCurve3d< float> > targetFibers;
      
      // computing the center of mass of the bundle
      gkg::Vector3d< float > centerOfMass( 0.0f, 0.0f, 0.0f );
      f = ib->second.begin();
      while ( f != fe )
      {
      
        centerOfMass += f->getCenterOfMass();
        ++ f;
      
      }
      centerOfMass /= ( float )bundle.getCurve3dCount();
      
      // applying the normalization to the bundle
      gkg::Vector3d< float > newCenterOfMass( _targetLength,
                                              _targetLength,
                                              _targetLength );
      gkg::Vector3d< float > normalizedPoint;          
      f = ib->second.begin();
      while ( f != fe )
      {
      
        gkg::LightCurve3d< float > normalizedFiber = *f;
        
        gkg::LightCurve3d< float >::iterator
          p = normalizedFiber.begin(),
          pe = normalizedFiber.end();
        if ( _keepOriginalCenterOfMass )
        {
        
          while ( p != pe )
          {
        
            *p = centerOfMass + 
                 ( *p - centerOfMass ) * normalizationFactor;
            ++ p;
        
          }
          
        }
        else
        {
        
          while ( p != pe )
          {
        
            *p = newCenterOfMass + 
                 ( *p - centerOfMass ) * normalizationFactor;
            ++ p;
        
          }
          
        }
        targetFibers.push_back( normalizedFiber );
        ++ f;
      
      }
      
      // filling the output bundle map with the normalized bundle
      outputBundleMap->addCurve3ds( label, targetFibers );

      ++ ib;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorLengthNormalization< L, "
             "Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorLengthNormalization< L, Compare >::addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorLengthNormalization< L, "
             "Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorLengthNormalization< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorLengthNormalization< L, "
             "Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorLengthNormalization< L, Compare >::getName() const
{

  try
  {

    return  gkg::BundleMapOperatorLengthNormalization< L, Compare >:: getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorLengthNormalization< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorLengthNormalization< L, Compare >::getStaticName()
{

  try
  {

    return "length-normalization";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorLengthNormalization< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorLengthNormalization< L, Compare >::getStringParameterHelp()
{

  try
  {

    return std::string( ".  N/A\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorLengthNormalization< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorLengthNormalization< L, Compare >::getScalarParameterHelp()
{

  try
  {

    return std::string( ".  <P1>: target normalization length in mm\n"
                        ".  <P2>: keep normalized bundles at their original "
                        "center of mass" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorLengthNormalization< L, Compare >::"
             "getStringParameterHelp()" );

}


#endif
