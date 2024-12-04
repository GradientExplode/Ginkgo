#ifndef _gkg_processing_roi_AtlasInformation_h_
#define _gkg_processing_roi_AtlasInformation_h_

#include <gkg-core-object/HeaderedObject.h>
#include <gkg-processing-roi/LabelledRoiDictionary.h>
#include <gkg-processing-statistics/StructureBasedStatisticalMap.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <map>
#include <string>
#include <vector>


namespace gkg
{


template < class S, class Compare = std::less< S > >
class AtlasInformation : public HeaderedObject
{

  public:

    AtlasInformation( const HeaderedObject& headeredObject,
                      const std::string& structureDictionaryName,
                      const std::string& statisticalDictionaryName );
    AtlasInformation( const AtlasInformation< S, Compare >& other );
    virtual ~AtlasInformation();

    AtlasInformation& operator=( const AtlasInformation< S, Compare >& other );

    const LabelledRoiDictionary& getLabelledRoiDictionary() const;
    const StructureBasedStatisticalMap< S, Compare >&
      getStructureBasedStatisticalMap() const;

    int32_t getStructureCount() const;
    int32_t getRank( int16_t label ) const;
    int16_t getLabel( int32_t rank ) const;

    const Vector3d< int32_t > getIntegerTalairachAnteriorCommissure() const;
    int32_t getDataBaseCount() const;

    void addSemantic( HeaderedObject& headeredObject ) const;
    void addToHeaderedObject( HeaderedObject& headeredObject ) const;

  protected:

    LabelledRoiDictionary _labelledRoiDictionary;
    StructureBasedStatisticalMap< S, Compare > _structureBasedStatisticalMap;
    int32_t _dataBaseCount;
    std::map< int16_t, int32_t > _labelToRankLut;
    std::map< int32_t, int16_t > _rankToLabelLut;
    std::vector< int32_t > _labelToRankVector;
    std::vector< int16_t > _rankToLabelVector;
    Vector3d< int32_t > _integerTalairachAnteriorCommissure;

};


} 


#endif
