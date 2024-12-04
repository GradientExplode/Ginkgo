#include <gkg-fmri-statistics/StatisticalFunctionScoreMaskContext.h>
#include <gkg-fmri-statistics/GLMStatisticalFunction.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::StatisticalFunctionScoreMaskContext::StatisticalFunctionScoreMaskContext(
                           gkg::GLMStatisticalFunction& statisticalFunction,
                           const std::vector< gkg::Vector3d< int32_t > >& sites,
                           int32_t contrastCount,
                           const gkg::Volume< float >& volumeOfEffects,
                           const gkg::Volume< float >& volumeOfEffectVariances,
                           gkg::Volume< float >& statistics,
                           int32_t statisticCount )
                                    : gkg::LoopContext< int32_t >(),
                                      _statisticalFunction(
                                                          statisticalFunction ),
                                      _sites( sites ),
                                      _contrastCount( contrastCount ),
                                      _volumeOfEffects( volumeOfEffects ),
                                      _volumeOfEffectVariances( 
                                                      volumeOfEffectVariances ),
                                      _statistics( statistics ),
                                      _statisticCount( statisticCount )
{
}


void gkg::StatisticalFunctionScoreMaskContext::doIt( int32_t startIndex,
                                                     int32_t count )
{

  try
  {

    int32_t t, c, c1, c2;
    std::vector< gkg::Vector3d< int32_t > >::const_iterator
      s = _sites.begin() + startIndex,
      se = s + count;

    gkg::Vector scores( _contrastCount );
    gkg::Vector effects( _contrastCount );
    gkg::Matrix effectVariances( _contrastCount, _contrastCount );

    while ( s != se )
    {

      // copying back variance of contrast effects
      for ( c1 = 0, t = 0; c1 < _contrastCount; c1++ )
      {

        // copying back contrast effects
    	effects( c1 ) = ( double )_volumeOfEffects( *s, c1 );

    	for ( c2 = 0; c2 < _contrastCount; c2++, t++ )
    	{

    	  effectVariances( c1, c2 ) = ( double )_volumeOfEffectVariances(
    							                *s, t );

    	}

      }

      _statisticalFunction.process( effects, effectVariances, scores );

      for ( c = 0; c < _statisticCount; c++ )
      {

    	_statistics( *s, c ) = scores( c );

      }

      ++s;

    }

  }
  GKG_CATCH( "void gkg::StatisticalFunctionScoreMaskContext::doIt( "
             "int32_t startIndex, int32_t count )" );

}
