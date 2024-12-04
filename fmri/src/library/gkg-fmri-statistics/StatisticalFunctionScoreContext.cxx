#include <gkg-fmri-statistics/StatisticalFunctionScoreContext.h>
#include <gkg-fmri-statistics/GLMStatisticalFunction.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-exception/Exception.h>


gkg::StatisticalFunctionScoreContext::StatisticalFunctionScoreContext(
                            gkg::GLMStatisticalFunction& statisticalFunction,
                            int32_t contrastCount,
                            const gkg::Volume< float >& volumeOfEffects,
                            const gkg::Volume< float >& volumeOfEffectVariances,
                            gkg::Volume< float >& statistics,
                            int32_t statisticCount )
                                    : gkg::LoopContext< int32_t >(),
                                      _statisticalFunction(
                                                          statisticalFunction ),
                                      _contrastCount( contrastCount ),
                                      _volumeOfEffects( volumeOfEffects ),
                                      _volumeOfEffectVariances( 
                                                      volumeOfEffectVariances ),
                                      _statistics( statistics ),
                                      _statisticCount( statisticCount )
{
}


void gkg::StatisticalFunctionScoreContext::doIt( int32_t startIndex,
                                                 int32_t count )
{

  try
  {

    int32_t stopIndex = startIndex + count;
    int32_t sizeX = _volumeOfEffects.getSizeX();
    int32_t sizeY = _volumeOfEffects.getSizeY();
    int32_t x, y, z, t, c, c1, c2;

    gkg::Vector scores( _contrastCount );
    gkg::Vector effects( _contrastCount );
    gkg::Matrix effectVariances( _contrastCount, _contrastCount );

    for ( z = startIndex; z < stopIndex; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          // copying back variance of contrast effects
          for ( c1 = 0, t = 0; c1 < _contrastCount; c1++ )
          {

            // copying back contrast effects
            effects( c1 ) = ( double )_volumeOfEffects( x, y, z, c1 );

            for ( c2 = 0; c2 < _contrastCount; c2++, t++ )
            {

              effectVariances( c1, c2 ) = ( double )_volumeOfEffectVariances(
                                                                   x, y, z, t );

            }

          }

          _statisticalFunction.process( effects, effectVariances, scores );

          for ( c = 0; c < _statisticCount; c++ )
          {

            _statistics( x, y, z, c ) = scores( c );

          }

        }

      }

    }

  }
  GKG_CATCH( "void gkg::StatisticalFunctionScoreContext::doIt( "
             "int32_t startIndex, int32_t count )" );

}
