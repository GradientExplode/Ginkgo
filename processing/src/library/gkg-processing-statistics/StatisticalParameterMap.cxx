#include <gkg-processing-statistics/StatisticalParameterMap.h>
#include <gkg-core-exception/Exception.h>


gkg::StatisticalParameterMap::StatisticalParameterMap()
{
}


gkg::StatisticalParameterMap::StatisticalParameterMap(
                  const std::map< std::string, double >& statisticalParameters )
                             : _statisticalParameters( statisticalParameters )
{
}


gkg::StatisticalParameterMap::StatisticalParameterMap(
                                     const gkg::StatisticalParameterMap& other )
                             : _statisticalParameters(
                                                  other._statisticalParameters )
{
}


gkg::StatisticalParameterMap::~StatisticalParameterMap()
{
}


gkg::StatisticalParameterMap& 
gkg::StatisticalParameterMap::operator=(
                                     const gkg::StatisticalParameterMap& other )
{

  try
  {

    _statisticalParameters = other._statisticalParameters;

    return *this;

  }
  GKG_CATCH( "gkg::StatisticalParameterMap& "
             "gkg::StatisticalParameterMap::operator=( "
             "const gkg::StatisticalParameterMap& other )" );

}


gkg::StatisticalParameterMap::iterator gkg::StatisticalParameterMap::begin()
{

  return _statisticalParameters.begin();

}


gkg::StatisticalParameterMap::iterator gkg::StatisticalParameterMap::end()
{

  return _statisticalParameters.end();

}

gkg::StatisticalParameterMap::const_iterator 
gkg::StatisticalParameterMap::begin() const
{

  return _statisticalParameters.begin();

}


gkg::StatisticalParameterMap::const_iterator 
gkg::StatisticalParameterMap::end() const
{

  return _statisticalParameters.end();

}


void gkg::StatisticalParameterMap::setStatisticalParameter(
                                                        const std::string& name,
                                                        double value )
{

  try
  {

    _statisticalParameters[ name ] = value;

  }
  GKG_CATCH( "void gkg::StatisticalParameterMap::setStatisticalParameter( "
             "const std::string& name, double value )" );

}


double gkg::StatisticalParameterMap::getStatisticalParameter(
                                                 const std::string& name ) const
{

  try
  {

    std::map< std::string, double >::const_iterator
      m = _statisticalParameters.find( name );
    if ( m == _statisticalParameters.end() )
    {

      throw std::runtime_error( std::string( "'" ) + name +
                                "' attribute not present" );

    }
    return m->second;

  }
  GKG_CATCH( "double gkg::StatisticalParameterMap::getStatisticalParameter( "
             "const std::string& name ) const" );

}

    
std::set< std::string > 
gkg::StatisticalParameterMap::getStatisticalParameterNames() const
{

  try
  {

    std::set< std::string > statisticalParameterNames;
    std::map< std::string, double >::const_iterator
      m = _statisticalParameters.begin(),
      me = _statisticalParameters.end();
    while ( m != me )
    {

      statisticalParameterNames.insert( m->first );
      ++ m;

    }
    return statisticalParameterNames;

  }
  GKG_CATCH( "std::set< std::string > "
             "gkg::StatisticalParameterMap::getStatisticalParameterNames() "
             "const" );

}
