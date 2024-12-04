#include <gkg-qmri-plugin-functors/MyelinWaterFractionMapper/VFASPGRBasedMyelinWaterFractionMCMCFunction.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


gkg::VFASPGRBasedMyelinWaterFractionMCMCFunction::
                                    VFASPGRBasedMyelinWaterFractionMCMCFunction(
                         const std::vector< gkg::Vector >& echoTimes,
                         const gkg::Vector& flipAngles,
                         const gkg::Vector& repetitionTimes,
                         const std::vector< double >& t2NoiseStandardDeviations,
                         double t1NoiseStandardDeviation )
                                     : gkg::MyelinWaterFractionMCMCFunction(
                                                     echoTimes,
                                                     t2NoiseStandardDeviations,
                                                     repetitionTimes.getSize(),
                                                     t1NoiseStandardDeviation ),
                                       _echoTimes( echoTimes ),
                                       _flipAngles( flipAngles ),
                                       _repetitionTimes( repetitionTimes ),
                                       _b1Scaling( 1.0f )
{
}


gkg::VFASPGRBasedMyelinWaterFractionMCMCFunction::
                                  ~VFASPGRBasedMyelinWaterFractionMCMCFunction()
{
}


void gkg::VFASPGRBasedMyelinWaterFractionMCMCFunction::setB1Scaling(
                                                               float b1Scaling )
{

  try
  {

    _b1Scaling = b1Scaling;

  }
  GKG_CATCH( "void gkg::VFASPGRBasedMyelinWaterFractionMCMCFunction::"
             "setB1Scaling( float b1SCaling )" );

}


void gkg::VFASPGRBasedMyelinWaterFractionMCMCFunction::getValuesAt(
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

    int32_t t1WeightingCount = _repetitionTimes.getSize();
    int32_t t1Index = 0;
    float expMinusTROverT1 = 0.0f;
    float correctedFlipAngle = 0.0f;
    for ( t1Index = 0; t1Index < t1WeightingCount; t1Index++ )
    {

      expMinusTROverT1 = 
                   ( float )std::exp( -_repetitionTimes( t1Index ) *
                                      ( ( 1.0f - csfFraction ) *
                                        (  myelinWaterFraction / myelinWaterT1 +
                                           ( 1.0f - myelinWaterFraction ) /
                                           grayWhiteT1 ) +
                                        csfFraction / csfT1 ) );
      correctedFlipAngle =  _flipAngles( t1Index ) * _b1Scaling * M_PI / 180.0;
      values( _globalT2WeightedCount + t1Index ) =
          ( protonDensity * protonDensityScaling *
            ( 1.0f - expMinusTROverT1 ) *
            ( float )std::sin( correctedFlipAngle ) ) /
          ( 1.0f - expMinusTROverT1 * ( float )std::cos( correctedFlipAngle ) );

    }

  }
  GKG_CATCH( "void gkg::VFASPGRBasedMyelinWaterFractionMCMCFunction::"
             "getValuesAt( "
             "const gkg::Vector& parameters, "
             "gkg::Vector& values ) const" );

}
