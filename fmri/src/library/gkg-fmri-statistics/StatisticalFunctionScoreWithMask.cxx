#include <gkg-fmri-statistics/StatisticalFunctionFactory.h>
#include <gkg-fmri-statistics/StatisticalFunctionScoreMaskContext.h>
#include <gkg-fmri-statistics/PValueFunction.h>
#include <gkg-fmri-statistics/TScoreFunction.h>
#include <gkg-fmri-statistics/ZScoreFunction.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>


////////////////////////////////////////////////////////////////////////////////
// score volume
////////////////////////////////////////////////////////////////////////////////

template < class T >
void statisticalFunctionScoreWithMask(
                           const std::vector< gkg::Vector3d< int32_t > >& sites,
                           const int32_t effectiveScanCount,
                           const int32_t contrastCount,
                           const gkg::Volume< float >& volumeOfEffects,
                           const gkg::Volume< float >& volumeOfEffectVariances,
                           gkg::Volume< float >& statistics )
{

  try
  {

    int32_t sizeX = volumeOfEffects.getSizeX();
    int32_t sizeY = volumeOfEffects.getSizeY();
    int32_t sizeZ = volumeOfEffects.getSizeZ();
    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
    volumeOfEffects.getResolution( resolution );

    statistics.reallocate( sizeX, sizeY, sizeZ, contrastCount );
    statistics.setResolution( resolution );
    statistics.fill( 0.0f );

    T scoreFunction( effectiveScanCount, contrastCount );

    gkg::StatisticalFunctionScoreMaskContext context( scoreFunction,
                                                      sites,
                                                      contrastCount,
                                                      volumeOfEffects,
                                                      volumeOfEffectVariances,
                                                      statistics,
                                                      contrastCount );
    gkg::ThreadedLoop< int32_t > threadedLoop( &context, 0, sites.size() );

    threadedLoop.launch();

  }
  GKG_CATCH( "template < class T > void statisticalFunctionScoreWithMask( "
             "const std::vector< Vector3d< int32_t > >& sites, "
             "const int32_t effectiveScanCount, "
             "const int32_t contrastCount, "
             "const gkg::Volume< float >& volumeOfEffects, "
             "const gkg::Volume< float >& volumeOfEffectVariances, "
             "gkg::Volume< float >& statistics )" );

}


template <>
void statisticalFunctionScoreWithMask< gkg::PValueFunction >(
                           const std::vector< gkg::Vector3d< int32_t > >& sites,
                           const int32_t effectiveScanCount,
                           const int32_t contrastCount,
                           const gkg::Volume< float >& volumeOfEffects,
                           const gkg::Volume< float >& volumeOfEffectVariances,
                           gkg::Volume< float >& statistics )
{

  try
  {

    int32_t sizeX = volumeOfEffects.getSizeX();
    int32_t sizeY = volumeOfEffects.getSizeY();
    int32_t sizeZ = volumeOfEffects.getSizeZ();
    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
    volumeOfEffects.getResolution( resolution );

    statistics.reallocate( sizeX, sizeY, sizeZ, 1 );
    statistics.setResolution( resolution );
    statistics.fill( 0.0f );

    gkg::PValueFunction scoreFunction( effectiveScanCount, contrastCount );

    gkg::StatisticalFunctionScoreMaskContext context( scoreFunction,
                                                      sites,
                                                      contrastCount,
                                                      volumeOfEffects,
                                                      volumeOfEffectVariances,
                                                      statistics,
                                                      1 );
    gkg::ThreadedLoop< int32_t > threadedLoop( &context, 0, sites.size() );

    threadedLoop.launch();

  }
  GKG_CATCH( "void statisticalFunctionScoreWithMask< gkg::PValueFunction >( "
             "const std::vector< Vector3d< int32_t > >& sites, "
             "const int32_t effectiveScanCount, "
             "const int32_t contrastCount, "
             "const gkg::Volume< float >& volumeOfEffects, "
             "const gkg::Volume< float >& volumeOfEffectVariances, "
             "gkg::Volume< float >& statistics )" );

}


////////////////////////////////////////////////////////////////////////////////
// score functions registration
////////////////////////////////////////////////////////////////////////////////

RegisterStatisticalFunctionWithMask(
                      PVALUE, 
                      "pvalue",
                      statisticalFunctionScoreWithMask< gkg::PValueFunction > );
RegisterStatisticalFunctionWithMask(
                      TSCORE,
                      "tscore", 
                      statisticalFunctionScoreWithMask< gkg::TScoreFunction > );
RegisterStatisticalFunctionWithMask(
                      ZSCORE,
                      "zscore", 
                      statisticalFunctionScoreWithMask< gkg::ZScoreFunction > );
