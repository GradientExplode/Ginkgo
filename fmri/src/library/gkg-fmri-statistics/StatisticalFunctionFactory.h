#ifndef _gkg_fmri_statistics_StatisticalFunctionFactory_h_
#define _gkg_fmri_statistics_StatisticalFunctionFactory_h_


#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>

#include <list>
#include <string>
#include <map>
#include <vector>


namespace gkg
{


class StatisticalFunctionFactory : public Singleton< StatisticalFunctionFactory>
{

  public:

    typedef void ( *StatisticalFunction )( const int32_t,
                                           const int32_t,
                                           const Volume< float >&,
                                           const Volume< float >&,
                                           Volume< float >& );
    typedef void ( *StatisticalFunctionWithMask )(
                                const std::vector< Vector3d< int32_t > >& sites,
                                const int32_t,
                                const int32_t,
                                const Volume< float >&,
                                const Volume< float >&,
                                Volume< float >& );

    ~StatisticalFunctionFactory();

    bool registerStatisticalFunction(
                                     const std::string& statisticalFunctionName,
                                     StatisticalFunction statisticalFunction );
    bool registerStatisticalFunctionWithMask(
                              const std::string& statisticalFunctionName,
                              StatisticalFunctionWithMask statisticalFunction );
    std::list< std::string > getStatisticalFunctionNames() const;
    void applyFunction( const std::string& statisticalFunctionName,
                        const int32_t effectiveScanCount,
                        const int32_t contrastCount,
                        const Volume< float >& volumeOfEffects,
                        const Volume< float >& volumeOfEffectVariances,
                        Volume< float >& outputVolume ) const;
    void applyFunction( const std::string& statisticalFunctionName,
                        const std::vector< Vector3d< int32_t > >& sites,
                        const int32_t effectiveScanCount,
                        const int32_t contrastCount,
                        const Volume< float >& volumeOfEffects,
                        const Volume< float >& volumeOfEffectVariances,
                        Volume< float >& outputVolume ) const;

  private:

    friend class Singleton< StatisticalFunctionFactory >;

    StatisticalFunctionFactory();

    std::map< std::string, StatisticalFunction > _statisticalFunctions;
    std::map< std::string, StatisticalFunctionWithMask > 
                                                  _statisticalFunctionsWithMask;

};


}



#define RegisterStatisticalFunction( SUFFIX, NAME, FUNCTION )                 \
static bool initializeStatisticFunction##SUFFIX()                             \
{                                                                             \
                                                                              \
  gkg::StatisticalFunctionFactory::getInstance().registerStatisticalFunction( \
                                                                 NAME,        \
                                                                 FUNCTION );  \
  return true;                                                                \
                                                                              \
}                                                                             \
                                                                              \
static bool initialized_StatisticFunction##SUFFIX __attribute__((unused)) =   \
                                         initializeStatisticFunction##SUFFIX()



#define RegisterStatisticalFunctionWithMask( SUFFIX, NAME, FUNCTION )         \
static bool initializeStatisticFunctionWithMask##SUFFIX()                     \
{                                                                             \
                                                                              \
  gkg::StatisticalFunctionFactory::getInstance().                             \
                                         registerStatisticalFunctionWithMask( \
                                                                 NAME,        \
                                                                 FUNCTION );  \
  return true;                                                                \
                                                                              \
}                                                                             \
                                                                              \
static bool initialized_StatisticFunctionWithMask##SUFFIX                     \
                                 __attribute__((unused)) =                    \
                                 initializeStatisticFunctionWithMask##SUFFIX()


#endif
