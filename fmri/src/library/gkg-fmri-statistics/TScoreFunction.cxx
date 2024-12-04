#include <gkg-fmri-statistics/TScoreFunction.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


#define GKG_TINY  1e-300


gkg::TScoreFunction::TScoreFunction( int32_t effectiveScanCount,
                                     int32_t contrastCount )
                    : gkg::GLMStatisticalFunction( effectiveScanCount,
                                                   contrastCount )
{
}


gkg::TScoreFunction::~TScoreFunction()
{
}


void gkg::TScoreFunction::process( const gkg::Vector& effects,
                                   const gkg::Matrix& effectVariances,
                                   gkg::Vector& statistics )
{

  try
  {

    // sanity check
    if ( ( effects.getSize() != _contrastCount ) ||
         ( effectVariances.getSize1() != _contrastCount ) ||
         ( effectVariances.getSize2() != _contrastCount ) )
    {

      throw std::runtime_error(
                             "inconsistent effect and contrast count size(s)" );

    }

    // processing degree of freedom
    int32_t degreeOfFreedom = _effectiveScanCount - _contrastCount;
    
    statistics.reallocate( _contrastCount );

    // calculating t-score
    if ( _contrastCount == 1 )
    {

      double tscore = 0.0;
      if ( degreeOfFreedom > 0 )
      {

        if ( effectVariances( 0, 0 ) > GKG_TINY )
        {

          tscore = effects( 0 ) /
                   std::sqrt( effectVariances( 0, 0 ) *
                              ( ( double )_effectiveScanCount /
                                ( double )degreeOfFreedom ) );

        }

      }
      statistics( 0 ) = tscore;

    }
    else
    {

      statistics.setZero();
      int32_t c;
      if ( degreeOfFreedom > 0 )
      {

        gkg::Matrix effectVariancesInvSqrt;
        _factory->getPower( effectVariances, -0.5, effectVariancesInvSqrt );
        statistics = effectVariancesInvSqrt.getComposition( effects );
        double tmp = std::sqrt( ( double )_effectiveScanCount /
                                ( double )degreeOfFreedom );
        for ( c = 0; c < _contrastCount; c++ )
        {

          statistics( c ) /= tmp;

        }

      }

    }

  }
  GKG_CATCH( "void gkg::TScoreFunction::process( "
             "const gkg::Vector& effects, "
             "const gkg::Matrix& effectVariances, "
             "gkg::Vector& statistics )" );

}

#undef GKG_TINY

