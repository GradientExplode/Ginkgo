#include <gkg-fmri-statistics/ZScoreFunction.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


#define GKG_TINY  1e-15


gkg::ZScoreFunction::ZScoreFunction( int32_t effectiveScanCount,
                                     int32_t contrastCount )
                    : gkg::GLMStatisticalFunction( effectiveScanCount,
                                                   contrastCount )
{
}


gkg::ZScoreFunction::~ZScoreFunction()
{
}


void gkg::ZScoreFunction::process( const gkg::Vector& effects,
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

    // calculating z-score
    if ( _contrastCount == 1 )
    {

      double zscore = 0.0;
      if ( degreeOfFreedom > 0 )
      {

        if ( effectVariances( 0, 0 ) > GKG_TINY )
        {

          zscore = effects( 0 ) /
                   std::sqrt( effectVariances( 0, 0 ) *
                              ( ( double )_effectiveScanCount /
                                ( double )degreeOfFreedom ) );

        }
        double tdist = _factory->getTDistCdfP( zscore,
                                               ( double )degreeOfFreedom );
        if ( tdist >= 1.0 - 1e-15 )
        {

          tdist = 1.0 - 1e-15;

        }
        zscore = _factory->getGaussianCdfInverseP( tdist, 0.0, 1.0 );

      }
      statistics( 0 ) = zscore;

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
        double sumOfSquare = 0.0;
        for ( c = 0; c < _contrastCount; c++ )
        {

          statistics( c ) /= tmp;
          sumOfSquare += statistics( c ) * statistics( c );

        }
        if ( sumOfSquare > GKG_TINY )
        {

          double correction =
            _factory->getChi2CdfInverseP(
                 _factory->getFDistCdfP( sumOfSquare / ( double )_contrastCount,
                                         ( double )_contrastCount,
                                         ( double )degreeOfFreedom ),
                 ( double )_contrastCount );

          if ( correction == INFINITY )
          {

            correction = 
              _factory->getChi2CdfInverseP( 127, ( double )_contrastCount );

          }

          correction = std::sqrt( correction / sumOfSquare );

          for ( c = 0; c < _contrastCount; c++ )
          {

            statistics( c ) *= correction;
               
          }

        }
        else
        {

          statistics.setZero();

        }

      }

    }

  }
  GKG_CATCH( "void gkg::ZScoreFunction::process( "
             "const gkg::Vector& effects, "
             "const gkg::Matrix& effectVariances, "
             "gkg::Vector& statistics )" );

}

#undef GKG_TINY

