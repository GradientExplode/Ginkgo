#include <gkg-fmri-statistics/PValueFunction.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


gkg::PValueFunction::PValueFunction( int32_t effectiveScanCount,
                                     int32_t contrastCount )
                    : gkg::GLMStatisticalFunction( effectiveScanCount,
                                                   contrastCount ),
                      _zScoreFunction( effectiveScanCount, contrastCount )
{
}


gkg::PValueFunction::~PValueFunction()
{
}


void gkg::PValueFunction::process( const gkg::Vector& effects,
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

    // processing z-score(s)
    gkg::Vector zscores( _contrastCount );
    _zScoreFunction.process( effects, effectVariances, zscores );
    
    statistics.reallocate( 1 );

    if ( _contrastCount == 1 )
    {

      statistics( 0 ) = 1.0 - _factory->getGaussianCdfP( zscores( 0 ), 
                                                         0.0, 1.0 );
    }
    else
    {

      int32_t c;
      double sumOfSquare = 0.0;
      for ( c = 0; c < _contrastCount; c++ )
      {

        sumOfSquare += zscores( c ) * zscores( c ); 

      }
      statistics( 0 ) = 1.0 - _factory->getChi2CdfP( sumOfSquare,
                                                     ( double )_contrastCount );

    }

  }
  GKG_CATCH( "void gkg::PValueFunction::process( "
             "const gkg::Vector& effects, "
             "const gkg::Matrix& effectVariances, "
             "gkg::Vector& statistics )" );

}

