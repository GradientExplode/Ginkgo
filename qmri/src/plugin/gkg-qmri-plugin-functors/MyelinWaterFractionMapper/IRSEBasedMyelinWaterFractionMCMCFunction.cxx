#include <gkg-qmri-plugin-functors/MyelinWaterFractionMapper/IRSEBasedMyelinWaterFractionMCMCFunction.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


#include <fstream>

gkg::IRSEBasedMyelinWaterFractionMCMCFunction::
                                       IRSEBasedMyelinWaterFractionMCMCFunction(
                         const std::vector< gkg::Vector >& echoTimes,
                         const gkg::Vector& inversionTimes,
                         const std::vector< double >& t2NoiseStandardDeviations,
                         double t1NoiseStandardDeviation )
                                     : gkg::MyelinWaterFractionMCMCFunction(
                                                     echoTimes,
                                                     t2NoiseStandardDeviations,
                                                     inversionTimes.getSize(),
                                                     t1NoiseStandardDeviation ),
                                       _echoTimes( echoTimes ),
                                       _inversionTimes( inversionTimes )
{
}


gkg::IRSEBasedMyelinWaterFractionMCMCFunction::
                                     ~IRSEBasedMyelinWaterFractionMCMCFunction()
{
}


void gkg::IRSEBasedMyelinWaterFractionMCMCFunction::getValuesAt(
                                                  const gkg::Vector& parameters,
                                                  gkg::Vector& values ) const
{

  try
  {

    float myelinWaterFraction = ( float )parameters( 0 );
    float myelinWaterT1 = ( float )parameters( 1 );
    float myelinWaterT2 = ( float )parameters( 2 );
    float grayWhiteT1 = ( float )parameters( 3 );
    float grayWhiteT2 = ( float )parameters( 4 );
    float csfFraction = ( float )parameters( 5 );
    float csfT1 = ( float )parameters( 6 );
    float csfT2 = ( float )parameters( 7 );
    float protonDensity = ( float )parameters( 8 );
    float protonDensityScaling = ( float )parameters( 9 );
    std::vector< float > t2WeightedScalings( _t2WeightedVolumeCount );

    t2WeightedScalings[ 0 ] = 1.0f;
    int32_t t2WeightedVolumeIndex = 0;
    for ( t2WeightedVolumeIndex = 0;
          t2WeightedVolumeIndex < _t2WeightedVolumeCount - 1;
          t2WeightedVolumeIndex++ )
    {

      t2WeightedScalings[ t2WeightedVolumeIndex + 1 ] =
                              ( float )parameters( 10 + t2WeightedVolumeIndex );

    }


    int32_t globalT2Index = 0;
    int32_t t2Index = 0;
    for ( t2WeightedVolumeIndex = 0;
          t2WeightedVolumeIndex < _t2WeightedVolumeCount;
          t2WeightedVolumeIndex++ )
    {

      for ( t2Index = 0; t2Index < _t2WeightedCounts[ t2WeightedVolumeIndex ];
            t2Index++ )
      {

         values( globalT2Index ) =
                            t2WeightedScalings[ t2WeightedVolumeIndex ] *
                            protonDensity *
                            ( ( 1.0f - csfFraction ) *
                              ( myelinWaterFraction *
                                std::exp( -_echoTimes[ t2WeightedVolumeIndex ](
                                                                     t2Index ) /
                                          myelinWaterT2 ) +
                                ( 1.0f - myelinWaterFraction ) *
                                std::exp( -_echoTimes[ t2WeightedVolumeIndex ](
                                                                     t2Index ) /
                                          grayWhiteT2 ) ) +
                              csfFraction *
                              std::exp( -_echoTimes[ t2WeightedVolumeIndex ](
                                                                     t2Index ) /
                                        csfT2 ) );

        ++ globalT2Index;

      }

    }


    int32_t t1WeightingCount = _inversionTimes.getSize();
    int32_t t1Index = 0;
    for ( t1Index = 0; t1Index < t1WeightingCount; t1Index++ )
    {

      values( _globalT2WeightedCount + t1Index ) = std::fabs( 
                 protonDensity * protonDensityScaling *
                 ( 1.0f - 2.0f *
                   ( float )std::exp( -_inversionTimes( t1Index ) *
                                      ( ( 1.0f - csfFraction ) *
                                        (  myelinWaterFraction / myelinWaterT1 +
                                           ( 1.0f - myelinWaterFraction ) /
                                           grayWhiteT1 ) +
                                        csfFraction / csfT1 ) ) ) );

    }

  }
  GKG_CATCH( "void gkg::IRSEBasedMyelinWaterFractionMCMCFunction::"
             "getValuesAt( "
             "const gkg::Vector& parameters, "
             "gkg::Vector& values ) const" );

}
