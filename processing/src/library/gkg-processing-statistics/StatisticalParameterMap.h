#ifndef _gkg_processing_statistics_StatisticalParameterMap_h_
#define _gkg_processing_statistics_StatisticalParameterMap_h_


#include <gkg-core-cppext/StdInt.h>
#include <string>
#include <map>
#include <set>


namespace gkg
{


class StatisticalParameterMap
{
      
  public:

    typedef std::map< std::string, double >::iterator iterator;
    typedef std::map< std::string, double >::const_iterator const_iterator;

    StatisticalParameterMap();
    StatisticalParameterMap( const std::map< std::string,
                                             double >& statisticalParameters );
    StatisticalParameterMap( const StatisticalParameterMap& other );
    virtual ~StatisticalParameterMap();

    StatisticalParameterMap& operator=( const StatisticalParameterMap& other );

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    void setStatisticalParameter( const std::string& name, double value );
    double getStatisticalParameter( const std::string& name ) const;

    std::set< std::string > getStatisticalParameterNames() const;

  protected:

    std::map< std::string, double > _statisticalParameters;


};


}


#endif
