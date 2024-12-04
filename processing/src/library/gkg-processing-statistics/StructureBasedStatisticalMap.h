#ifndef _gkg_processing_statistics_StructureBasedStatisticalMap_h_
#define _gkg_processing_statistics_StructureBasedStatisticalMap_h_

#include <gkg-core-object/HeaderedObject.h>
#include <gkg-processing-statistics/StatisticalParameterMap.h>
#include <map>
#include <string>


namespace gkg
{


template < class S, class Compare = std::less< S > >
class StructureBasedStatisticalMap : public HeaderedObject
{

  public:
  
    StructureBasedStatisticalMap( const std::string& name );
    StructureBasedStatisticalMap(
                      const StructureBasedStatisticalMap< S, Compare >& other );
    StructureBasedStatisticalMap( const HeaderedObject& headeredObject,
                                  const std::string& name );
    virtual ~StructureBasedStatisticalMap();

    StructureBasedStatisticalMap< S, Compare >&
      operator=( const StructureBasedStatisticalMap< S, Compare >& other );

    const std::string& getName() const;

    void addStatisticalParameterMap(
                       const S& structure, 
                       const std::string& measureName, 
                       const StatisticalParameterMap& statisticalParameterMap );
    const StatisticalParameterMap&
      getStatisticalParameterMap( const S& structure,
                                  const std::string& measureName ) const; 

    int32_t getStructureCount() const;
    std::set< S, Compare > getStructures() const;
    std::set< std::string > getMeasureNames( const S& structure ) const;

    void addSemantic( HeaderedObject& headeredObject ) const;
    void addToHeaderedObject( HeaderedObject& headeredObject ) const;

  protected:

    std::string toString( const S& structure ) const;
    S toStructure( const std::string& name ) const;

    std::string _name;
    std::map< S,
              std::map< std::string,
                        StatisticalParameterMap >,
              Compare > _statisticalParameterMaps;

};


}


#endif
