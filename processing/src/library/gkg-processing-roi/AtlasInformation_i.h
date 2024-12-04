#ifndef _gkg_processing_roi_AtlasInformation_i_h_
#define _gkg_processing_roi_AtlasInformation_i_h_

#include <gkg-processing-roi/AtlasInformation.h>
#include <gkg-processing-statistics/StructureBasedStatisticalMap_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>

template < class S, class Compare >
inline
gkg::AtlasInformation< S, Compare >::AtlasInformation(
                                  const gkg::HeaderedObject& headeredObject,
                                  const std::string& structureDictionaryName,
                                  const std::string& statisticalDictionaryName )
                                    : gkg::HeaderedObject(),
                                      _labelledRoiDictionary(
                                                     headeredObject,
                                                     structureDictionaryName ),
                                      _structureBasedStatisticalMap(
                                                     headeredObject,
                                                     statisticalDictionaryName )
{

  try
  {

    // sanity check
    // labelledRoiDictionary has one more roi than structureBasedStatisticalMap
    // which is the background
    if ( _labelledRoiDictionary.getRoiCount() !=
         _structureBasedStatisticalMap.getStructureCount() + 1 )
    {

      throw std::runtime_error( "inconsistent labelled ROI dictionary"
                                " and structure based statistical map sizes" );

    }

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

    addSemantic( *this );

    // adding attributes to header
    _header.addAttribute( "object_type", std::string( "AtlasInformation" ) );

    // getting data base count
    double data_base_count = 0.0;
    if ( headeredObject.getHeader().hasAttribute( "data_base_count" ) )
    {

      headeredObject.getHeader().getAttribute( "data_base_count",
                                               data_base_count );
      _dataBaseCount = ( int32_t )( data_base_count + 0.5 );

    }
    else
    {

      throw std::runtime_error( "'data_base_count' attribute not found" );

    }

    // getting label to rank dictionary
    gkg::Dictionary labelToRankLut;

    if ( headeredObject.getHeader().hasAttribute( "label_to_rank_lut" ) )
    {

      headeredObject.getHeader().getAttribute( "label_to_rank_lut",
                                               labelToRankLut );

    }
    else
    {

      throw std::runtime_error( "'label_to_rank_lut' attribute not found" );

    }

    // building label from/to rank lut(s)
    gkg::Dictionary::const_iterator
      d = labelToRankLut.begin(),
      de = labelToRankLut.end();
    int16_t label;

    int16_t labelCount = 0;
    int32_t rankCount = 0;  

    while ( d != de )
    {

      std::istringstream iss( d->first );
      iss >> label;
      int32_t rank = ( int32_t )( d->second->getValue< double >() + 0.5 );
      _labelToRankLut[ label ] = rank;
      _rankToLabelLut[ rank ] = label;

      if ( label > labelCount )
      {

        labelCount = label;

      }

      if ( rank > rankCount )
      {

        rankCount = rank;

      }

      ++ d;

    }

    _labelToRankVector.resize( labelCount + 1 );
    _rankToLabelVector.resize( rankCount + 1 );

    // building label from/to rank vector(s)
    std::map< int16_t, int32_t >::iterator
      l = _labelToRankLut.begin(),
      le = _labelToRankLut.end();
    while( l != le )
    {

      _labelToRankVector[ l->first ] = l->second;
      _rankToLabelVector[ l->second ] = l->first;

      ++ l;

    }

    // reading Talairach's AC coordinates
    gkg::GenericObjectList genericObjectList;
    if ( headeredObject.getHeader().hasAttribute( "talairach_AC_coordinates" ) )
    {

      try
      {
 
        headeredObject.getHeader().getAttribute( "talairach_AC_coordinates",
                                                 genericObjectList );
        _integerTalairachAnteriorCommissure.x =
          ( int32_t )( genericObjectList[ 0 ]->getScalar() + 0.5 );
        _integerTalairachAnteriorCommissure.y =
          ( int32_t )( genericObjectList[ 1 ]->getScalar() + 0.5 );
        _integerTalairachAnteriorCommissure.z =
          ( int32_t )( genericObjectList[ 2 ]->getScalar() + 0.5 );

       }
       catch( std:: exception& )
       {

         std::vector< int32_t > tmp;
         headeredObject.getHeader().getAttribute( "talairach_AC_coordinates",
                                                  tmp );
         _integerTalairachAnteriorCommissure.x = tmp[ 0 ];
         _integerTalairachAnteriorCommissure.y = tmp[ 1 ];
         _integerTalairachAnteriorCommissure.z = tmp[ 2 ];

       }

    }
    else
    {

      throw std::runtime_error(
                            "'talairach_AC_coordinates' attribute not found" );

    }


  }
  GKG_CATCH( "template < class S, class Compare > "
             "inline "
             "gkg::AtlasInformation< S, Compare >::AtlasInformation( "
             "const gkg::HeaderedObject& headeredObject, "
             "const std::string& structureDictionaryName, "
             "const std::string& statisticalDictionaryName )" );

}


template < class S, class Compare >
inline
gkg::AtlasInformation< S, Compare >::AtlasInformation(
                              const gkg::AtlasInformation< S, Compare >& other )
                                    : gkg::HeaderedObject( other ),
                                      _labelledRoiDictionary(
                                                 other._labelledRoiDictionary ),
                                      _structureBasedStatisticalMap(
                                           other._structureBasedStatisticalMap ),
                                      _dataBaseCount( other._dataBaseCount ),
                                      _labelToRankLut( other._labelToRankLut ),
                                      _rankToLabelLut( other._rankToLabelLut ),
                                      _labelToRankVector(
                                                     other._labelToRankVector ),
                                      _rankToLabelVector(
                                                     other._rankToLabelVector ),
                                      _integerTalairachAnteriorCommissure( 
                                    other._integerTalairachAnteriorCommissure )

{

  try
  {

    // adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
      gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

    addSemantic( *this );

    // adding attributes to header
    _header.addAttribute( "object_type", std::string( "AtlasInformation" ) );

  }
  GKG_CATCH( "template < class S, class Compare > "
             "inline "
             "gkg::AtlasInformation< S, Compare >::AtlasInformation( "
             "const gkg::AtlasInformation& other )" );

}


template < class S, class Compare >
inline
gkg::AtlasInformation< S, Compare >::~AtlasInformation()
{
}


template < class S, class Compare >
inline
gkg::AtlasInformation< S, Compare >& 
gkg::AtlasInformation< S, Compare >::operator=(
                              const gkg::AtlasInformation< S, Compare >& other )
{

  try
  {

    this->HeaderedObject::operator=( other );

    _labelledRoiDictionary = other._labelledRoiDictionary;
    _structureBasedStatisticalMap =  other._structureBasedStatisticalMap;
    _dataBaseCount = other._dataBaseCount;
    _labelToRankLut = other._labelToRankLut;
    _rankToLabelLut = other._rankToLabelLut;
    _labelToRankVector = other._labelToRankVector;
    _rankToLabelVector = other._rankToLabelVector;
    _integerTalairachAnteriorCommissure =
      other._integerTalairachAnteriorCommissure;

    return *this;

  }
  GKG_CATCH( "template < class S, class Compare > "
             "inline "
             "gkg::AtlasInformation& "
             "gkg::AtlasInformation< S, Compare >::operator=( "
             "const gkg::AtlasInformation& other )" );

}


template < class S, class Compare >
inline
const gkg::LabelledRoiDictionary& 
gkg::AtlasInformation< S, Compare >::getLabelledRoiDictionary() const
{

  return _labelledRoiDictionary;

}


template < class S, class Compare >
inline
const gkg::StructureBasedStatisticalMap< S, Compare >& 
gkg::AtlasInformation< S, Compare >::getStructureBasedStatisticalMap() const
{

  return _structureBasedStatisticalMap;

}


template < class S, class Compare >
inline
int32_t gkg::AtlasInformation< S, Compare >::getStructureCount() const
{

  try
  {

    return _structureBasedStatisticalMap.getStructureCount();

  }
  GKG_CATCH( "template < class S, class Compare > "
             "inline "
             "int32_t gkg::AtlasInformation< S, Compare >::"
             "getStructureCount() const" );

}


template < class S, class Compare >
inline
int32_t gkg::AtlasInformation< S, Compare >::getRank( int16_t label ) const
{
 
  try
  {
     
    if ( label >= ( int32_t )_labelToRankVector.size() )
    {

      throw std::runtime_error( "bad label" );

    }
    return _labelToRankVector[ label ];

  }
  GKG_CATCH( "template < class S, class Compare > "
             "inline "
             "int32_t gkg::AtlasInformation< S, Compare >::getRank( "
             "int16_t& label ) const" );

}


template < class S, class Compare >
inline
int16_t gkg::AtlasInformation< S, Compare >::getLabel( int32_t rank ) const
{

  try
  {

    if ( rank >= ( int32_t )_rankToLabelVector.size() )
    {

      throw std::runtime_error( "bad rank" );

    }
    return _rankToLabelVector[ rank ];

  }
  GKG_CATCH( "template < class S, class Compare > "
             "inline "
             "int16_t gkg::AtlasInformation< S, Compare >::getLabel( "
             "int32_t& rank ) const" );

}


template < class S, class Compare >
inline
const gkg::Vector3d< int32_t > 
gkg::AtlasInformation< S,
                       Compare >::getIntegerTalairachAnteriorCommissure() const
{

  return _integerTalairachAnteriorCommissure;

}

template < class S, class Compare >
inline
int32_t gkg::AtlasInformation< S, Compare >::getDataBaseCount() const
{

  return _dataBaseCount;

}

template < class S, class Compare >
inline
void gkg::AtlasInformation< S, Compare >::addSemantic(
                                     gkg::HeaderedObject& headeredObject ) const
{

  try
  {

    headeredObject.addSemantic(
            "__atlas_information__",
            "talairach_AC_coordinates",
            gkg::Semantic( gkg::TypeOf< std::vector< int32_t > >::getName() ) );
    headeredObject.addSemantic(
            "__atlas_information__",
            "data_base_count",
            gkg::Semantic( gkg::TypeOf< int32_t >::getName() ) );
    headeredObject.addSemantic(
            "__atlas_information__",
            "label_to_rank_lut",
            gkg::Semantic( gkg::TypeOf< gkg::Dictionary >::getName() ) );
    headeredObject.addSemantic(
            "__atlas_information__",
            _labelledRoiDictionary.getName(),
            gkg::Semantic( gkg::TypeOf< gkg::Dictionary >::getName() ) );
    headeredObject.addSemantic(
            "__atlas_information__",
            _structureBasedStatisticalMap.getName(),
            gkg::Semantic( gkg::TypeOf< gkg::Dictionary >::getName() ) );

  }
  GKG_CATCH( "template < class S, class Compare > "
             "inline "
             "void gkg::AtlasInformation< S, Compare >::addSemantic( "
             "gkg::HeaderedObject& headeredObject )" );

}


template < class S, class Compare >
inline
void gkg::AtlasInformation< S, Compare >::addToHeaderedObject(
                                     gkg::HeaderedObject& headeredObject ) const
{

  try
  {

    // adding semantic
    addSemantic( headeredObject );

    // adding labelled ROI dictionary
    _labelledRoiDictionary.addToHeaderedObject( headeredObject );

    // adding structure based statistical map
    _structureBasedStatisticalMap.addToHeaderedObject( headeredObject );

    // adding data base count
    headeredObject.getHeader().addAttribute( "data_base_count",
                                             _dataBaseCount );

    // converting label/rank structures to dictionaries on the fly
    gkg::Dictionary labelToRankLut;
    std::map< int16_t, int32_t >::const_iterator 
      i = _labelToRankLut.begin(),
      ie = _labelToRankLut.end();
    while ( i != ie )
    {

      labelToRankLut[ gkg::StringConverter::toString( i->first ) ] = i->second;
      ++ i;

    } 

    headeredObject.getHeader().addAttribute( "label_to_rank_lut",
                                             labelToRankLut );

    // adding integer Talairach coordinates
    std::vector< int32_t > ac( 3 );
    ac[ 0 ] = _integerTalairachAnteriorCommissure.x;
    ac[ 1 ] = _integerTalairachAnteriorCommissure.y;
    ac[ 2 ] = _integerTalairachAnteriorCommissure.z;
    headeredObject.getHeader().addAttribute( "talairach_AC_coordinates", ac ); 


  }
  GKG_CATCH( "template < class S, class Compare > "
             "inline "
             "void gkg::AtlasInformation< S, Compare >::addToHeaderedObject( "
             "gkg::HeaderedObject& headeredObject )" );

}


#endif

