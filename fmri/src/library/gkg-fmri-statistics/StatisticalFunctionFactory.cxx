#include <gkg-fmri-statistics/StatisticalFunctionFactory.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


gkg::StatisticalFunctionFactory::StatisticalFunctionFactory()
{
}


gkg::StatisticalFunctionFactory::~StatisticalFunctionFactory()
{
}


bool gkg::StatisticalFunctionFactory::registerStatisticalFunction(
      const std::string& statisticalFunctionName,
      gkg::StatisticalFunctionFactory::StatisticalFunction statisticalFunction )
{

  try
  {

    if ( statisticalFunction )
    {

      std::map< std::string, 
        gkg::StatisticalFunctionFactory::StatisticalFunction >::const_iterator
        f = _statisticalFunctions.find( statisticalFunctionName );

      if ( f == _statisticalFunctions.end() )
      {

        _statisticalFunctions[ statisticalFunctionName ] = statisticalFunction;
        return true;

      }
      else
      {

        std::cerr << "gkg::StatisticalFunctionFactory::"
                  << "registerStatisticalFunction:"
                  << statisticalFunctionName
                  << " already exists"
                  << std::endl;
        return false;

      }

    }
    return false;

  }
  GKG_CATCH( "bool gkg::StatisticalFunctionFactory::"
             "registerStatisticalFunction( "
             "const std::string& statisticalFunctionName, "
             "gkg::StatisticalFunctionFactory::StatisticalFunction "
             "statisticalFunction )" );

}


bool gkg::StatisticalFunctionFactory::registerStatisticalFunctionWithMask(
      const std::string& statisticalFunctionName,
      gkg::StatisticalFunctionFactory::StatisticalFunctionWithMask
                                                           statisticalFunction )
{

  try
  {

    if ( statisticalFunction )
    {

      std::map< std::string, 
        gkg::StatisticalFunctionFactory::StatisticalFunctionWithMask 
                                                               >::const_iterator
        f = _statisticalFunctionsWithMask.find( statisticalFunctionName );

      if ( f == _statisticalFunctionsWithMask.end() )
      {

        _statisticalFunctionsWithMask[ statisticalFunctionName ] = 
                                                            statisticalFunction;
        return true;

      }
      else
      {

        std::cerr << "gkg::StatisticalFunctionFactory::"
                  << "registerStatisticalFunctionWithMask:"
                  << statisticalFunctionName
                  << " already exists"
                  << std::endl;
        return false;

      }

    }
    return false;

  }
  GKG_CATCH( "bool gkg::StatisticalFunctionFactory::"
             "registerStatisticalFunctionWithMask( "
             "const std::string& statisticalFunctionName, "
             "gkg::StatisticalFunctionFactory::StatisticalFunctionWithMask "
             "statisticalFunction )" );

}


void gkg::StatisticalFunctionFactory::applyFunction(
                            const std::string& statisticalFunctionName,
                            const int32_t effectiveScanCount,
                            const int32_t contrastCount,
                            const gkg::Volume< float >& volumeOfEffects,
                            const gkg::Volume< float >& volumeOfEffectVariances,
                            gkg::Volume< float >& outputVolume ) const
{

  try
  {

    std::map< std::string, 
      gkg::StatisticalFunctionFactory::StatisticalFunction >::const_iterator
      f = _statisticalFunctions.find( statisticalFunctionName );

    if ( f != _statisticalFunctions.end() )
    {

      ( f->second )( effectiveScanCount,
                     contrastCount,
                     volumeOfEffects,
                     volumeOfEffectVariances,
                     outputVolume );

    }
    else
    {

      std::cerr << statisticalFunctionName
                << " statistical function not found"
                << std::endl;

    }

  }
  GKG_CATCH( "void gkg::StatisticalFunctionFactory::applyFunction( "
             "const std::string& statisticalFunctionName, "
             "const int32_t effectiveScanCount, "
             "const int32_t contrastCount, "
             "gkg::Volume< float >& volumeOfEffects, "
             "gkg::Volume< float >& volumeOfEffectVariances, "
             "gkg::Volume< float >& outputVolume ) const" );

}


void gkg::StatisticalFunctionFactory::applyFunction(
                           const std::string& statisticalFunctionName,
                           const std::vector< gkg::Vector3d< int32_t > >& sites,
                           const int32_t effectiveScanCount,
                           const int32_t contrastCount,
                           const gkg::Volume< float >& volumeOfEffects,
                           const gkg::Volume< float >& volumeOfEffectVariances,
                           gkg::Volume< float >& outputVolume ) const
{

  try
  {

    std::map< std::string, 
      gkg::StatisticalFunctionFactory::StatisticalFunctionWithMask
                                                               >::const_iterator
      f = _statisticalFunctionsWithMask.find( statisticalFunctionName );

    if ( f != _statisticalFunctionsWithMask.end() )
    {

      ( f->second )( sites,
                     effectiveScanCount,
                     contrastCount,
                     volumeOfEffects,
                     volumeOfEffectVariances,
                     outputVolume );

    }
    else
    {

      std::cerr << statisticalFunctionName
                << " statistical function not found"
                << std::endl;

    }

  }
  GKG_CATCH( "void gkg::StatisticalFunctionFactory::applyFunction( "
             "const std::string& statisticalFunctionName, "
             "const std::vector< gkg::Vector3d< int32_t > >& sites, "
             "const int32_t effectiveScanCount, "
             "const int32_t contrastCount, "
             "gkg::Volume< float >& volumeOfEffects, "
             "gkg::Volume< float >& volumeOfEffectVariances, "
             "gkg::Volume< float >& outputVolume ) const" );

}


std::list< std::string > 
gkg::StatisticalFunctionFactory::getStatisticalFunctionNames() const
{

  try
  {

    std::list< std::string > statisticalFunctionNames;

    std::map< std::string, 
      gkg::StatisticalFunctionFactory::StatisticalFunction >::const_iterator
      f = _statisticalFunctions.begin(),
      fe = _statisticalFunctions.end();

    while ( f != fe )
    {

      statisticalFunctionNames.push_back( f->first );
      ++ f;

    }
    return statisticalFunctionNames;

  }
  GKG_CATCH( "std::list< std::string > "
             "gkg::StatisticalFunctionFactory::getStatisticalFunctionNames() "
             "const" );

}
