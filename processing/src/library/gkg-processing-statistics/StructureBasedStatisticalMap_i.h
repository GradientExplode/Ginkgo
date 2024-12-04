#ifndef _gkg_processing_statistics_StructureBasedStatisticalMap_i_h_
#define _gkg_processing_statistics_StructureBasedStatisticalMap_i_h_


#include <gkg-processing-statistics/StructureBasedStatisticalMap.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>


template < class S, class Compare >
inline
gkg::StructureBasedStatisticalMap< S, Compare >::StructureBasedStatisticalMap(
                                                    const std::string& name )
                                                : gkg::HeaderedObject(),
                                                  _name( name )
{

  try
  {
  
    //adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type",
                          std::string( "StructureBasedStatisticalMap" ) );

  }
  GKG_CATCH( "template < class S, class Compare > "
             "inline "
             "gkg::StructureBasedStatisticalMap< S, Compare >::"
             "StructureBasedStatisticalMap( const std::string& name )" );

}


template < class S, class Compare >
inline
gkg::StructureBasedStatisticalMap< S, Compare >::StructureBasedStatisticalMap(
                  const gkg::StructureBasedStatisticalMap< S, Compare >& other )
                                                : gkg::HeaderedObject( other ),
                                                  _name( other._name ),
                                                  _statisticalParameterMaps(
                                               other._statisticalParameterMaps )
{

  try
  {

    //adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

   // adding attributes to header
   _header.addAttribute( "object_type",
                         std::string( "StructureBasedStatisticalMap" ) );

  }
  GKG_CATCH( "template < class S, class Compare > "
             "inline "
             "gkg::StructureBasedStatisticalMap< S, Compare >::"
             "StructureBasedStatisticalMap( "
             "const gkg::StructureBasedStatisticalMap< S, Compare >& other )" );

}


template < class S, class Compare >
inline
gkg::StructureBasedStatisticalMap< S, Compare >::StructureBasedStatisticalMap(
                                     const gkg::HeaderedObject& headeredObject,
                                     const std::string& name )
                                               : gkg::HeaderedObject(),
                                                 _name( name )
{

  try
  {

    //adding a generic syntax set
    _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

    // adding attributes to header
    _header.addAttribute( "object_type",
                          std::string( "StructureBasedStatisticalMap" ) );

    // filling statistical parameter map(s)
    if ( !headeredObject.getHeader().hasAttribute( _name ) )
    {

      throw std::runtime_error( std::string( "'" ) + _name +
                                " attribute not found" );

    }

    // reading structure dictionary
    gkg::Dictionary structureDictionary;
    headeredObject.getHeader().getAttribute( _name, structureDictionary );

    // looping over structure(s)
    gkg::Dictionary::const_iterator
      s = structureDictionary.begin(),
      se = structureDictionary.end();
    while ( s != se )
    {

      // obtaining structure key
      S structure = toStructure( s->first );

      // obtaining the dictionary of measure(s)
      gkg::Dictionary 
        measureDictionary = s->second->getValue< gkg::Dictionary >();

      // looping over measure(s)
      gkg::Dictionary::const_iterator
        m = measureDictionary.begin(),
        me = measureDictionary.end();
      while ( m != me )
      {

        // obtaining measure name
        std::string measureName = m->first;

        // obtaining parameter map dictionary
        gkg::Dictionary
          parameterDictionary = m->second->getValue< gkg::Dictionary >();

        // creating statistical parameter map
        gkg::StatisticalParameterMap statisticalParameterMap;
        gkg::Dictionary::const_iterator
          p = parameterDictionary.begin(),
          pe = parameterDictionary.end();
        while ( p != pe )
        {

          statisticalParameterMap.setStatisticalParameter(
                                                       p->first,
                                                       p->second->getScalar() );
          ++ p;

        }
        addStatisticalParameterMap( structure,
                                    measureName,
                                    statisticalParameterMap );
        ++ m;

      }
      ++ s;

    }

  }
  GKG_CATCH( "template < class S, class Compare > "
             "inline "
             "gkg::StructureBasedStatisticalMap< S, Compare >::"
             "StructureBasedStatisticalMap( "
             "const gkg::HeaderedObject& headeredObject, "
             "const std::string& name )" );

}


template < class S, class Compare >
inline
gkg::StructureBasedStatisticalMap< S, Compare >::~StructureBasedStatisticalMap()
{
}


template < class S, class Compare >
inline
gkg::StructureBasedStatisticalMap< S, Compare >&
gkg::StructureBasedStatisticalMap< S, Compare >::operator=(
                  const gkg::StructureBasedStatisticalMap< S, Compare >& other )
{

  try
  {

    this->gkg::HeaderedObject::operator=( other );
    _name = other._name;
    _statisticalParameterMaps = other._statisticalParameterMaps;

    return *this;

  }
  GKG_CATCH( "template < class S, class Compare > "
             "inline "
             "gkg::StructureBasedStatisticalMap< S, Compare >& "
             "gkg::StructureBasedStatisticalMap< S, Compare >::operator=( "
             "const gkg::StructureBasedStatisticalMap< S, Compare >& other )" );

}


template < class S, class Compare >
inline
const std::string& 
gkg::StructureBasedStatisticalMap< S, Compare >::getName() const
{

  return _name;

}


template < class S, class Compare >
inline
void gkg::StructureBasedStatisticalMap< S, Compare >::
                                                     addStatisticalParameterMap(
                   const S& structure, 
                   const std::string& measureName, 
                   const gkg::StatisticalParameterMap& statisticalParameterMap )
{

  try
  {

    _statisticalParameterMaps[ structure ][ measureName ] = 
      statisticalParameterMap;

  }
  GKG_CATCH( "template < class S, class Compare > "
             "inline "
             "void gkg::StructureBasedStatisticalMap< S, Compare >::"
             "addStatisticalParameterMap( "
             "const S& structure, "
             "const std::string& measureName, "
             "const gkg::StatisticalParameterMap& statisticalParameterMap )" );

}


template < class S, class Compare >
inline
const gkg::StatisticalParameterMap&
gkg::StructureBasedStatisticalMap< S, Compare >::getStatisticalParameterMap(
                                          const S& structure,
                                          const std::string& measureName ) const
{

  try
  {

    typename std::map< S,
                       std::map< std::string,
                                 gkg::StatisticalParameterMap >,
                       Compare >::const_iterator
      spm = _statisticalParameterMaps.find( structure );

    if ( spm == _statisticalParameterMaps.end() )
    {

      throw std::runtime_error( "structure key not found" );

    }

    std::map< std::string, gkg::StatisticalParameterMap >::const_iterator
      pm = spm->second.find( measureName );

    if ( pm == spm->second.end() )
    {

      throw std::runtime_error( std::string( "'" ) + measureName +
                                "' measure not found" );

    }
    return pm->second;

  }
  GKG_CATCH( "template < class S, class Compare > "
             "inline "
             "const gkg::StatisticalParameterMap& "
             "gkg::StructureBasedStatisticalMap< S, Compare >::"
             "getStatisticalParameterMap( "
             "const S& structure, "
             "const std::string& measureName ) const" );

}


template < class S, class Compare >
inline
int32_t 
gkg::StructureBasedStatisticalMap< S, Compare >::getStructureCount() const
{

  try
  {

    return ( int32_t )_statisticalParameterMaps.size();

  }
  GKG_CATCH( "template < class S, class Compare > "
             "inline "
             "int32_t  "
             "gkg::StructureBasedStatisticalMap< S, Compare >::"
             "getStructureCount() const" );

}


template < class S, class Compare >
inline
std::set< S, Compare > 
gkg::StructureBasedStatisticalMap< S, Compare >::getStructures() const
{

  try
  {

    std::set< S, Compare > structures;
    typename std::map< S,
                       std::map< std::string,
                                 gkg::StatisticalParameterMap >,
                       Compare >::const_iterator
      s = _statisticalParameterMaps.begin(),
      se = _statisticalParameterMaps.end();
    while ( s != se )
    {


      structures.insert( s->first );
      ++ s;

    }
    return structures;

  }
  GKG_CATCH( "template < class S, class Compare > "
             "inline "
             "std::set< S, Compare >  "
             "gkg::StructureBasedStatisticalMap< S, Compare >::"
             "getStructures() const" );

}


template < class S, class Compare >
inline
std::set< std::string > 
gkg::StructureBasedStatisticalMap< S, Compare >::getMeasureNames(
                                                      const S& structure ) const
{

  try
  {

    typename std::map< S,
                       std::map< std::string,
                                 gkg::StatisticalParameterMap >,
                       Compare >::const_iterator
      spm = _statisticalParameterMaps.find( structure );

    if ( spm == _statisticalParameterMaps.end() )
    {

      throw std::runtime_error( "structure key not found" );

    }

    std::set< std::string > measureNames;
    std::map< std::string, gkg::StatisticalParameterMap >::const_iterator
      pm = spm->second.begin(),
      pme = spm->second.end();
    while ( pm != pme )
    {

      measureNames.insert( pm->first );
      ++ pm;

    }
    return measureNames;

  }
  GKG_CATCH( "template < class S, class Compare > "
             "inline "
             "std::set< std::string >  "
             "gkg::StructureBasedStatisticalMap< S, Compare >::"
             "getMeasureNames( const S& structure ) const" );

}


template < class S, class Compare >
inline
void gkg::StructureBasedStatisticalMap< S, Compare >::addSemantic(
                                    gkg::HeaderedObject& headeredObject ) const
{

  try
  {

    headeredObject.addSemantic( "__structure_based_statistical_map__",
                                _name,
                                gkg::Semantic( 
                                gkg::TypeOf< gkg::Dictionary >::getName() ) );

  }
  GKG_CATCH( "template < class S, class Compare > "
             "inline "
             "void gkg::StructureBasedStatisticalMap< S, Compare >::"
             "addSemantic( "
             "gkg::HeaderedObject& headeredObject ) const" );

}


template < class S, class Compare >
inline
void gkg::StructureBasedStatisticalMap< S, Compare >::addToHeaderedObject(
                                    gkg::HeaderedObject& headeredObject ) const
{

  try
  {

    addSemantic( headeredObject );
    if ( headeredObject.getHeader().hasAttribute( _name ) )
    {

      throw std::runtime_error( std::string( "'" ) + _name +
                                "' attribute already present" );

    }

    gkg::Dictionary structureDictionary;
    typename std::map< S,
                       std::map< std::string,
                                 gkg::StatisticalParameterMap >,
                       Compare >::const_iterator
      s = _statisticalParameterMaps.begin(),
      se = _statisticalParameterMaps.end();

    // looping over structure(s)
    while ( s != se )
    {

      // looping over measure(s)
      gkg::Dictionary measureDictionary;
      std::map< std::string, gkg::StatisticalParameterMap >::const_iterator
        pm = s->second.begin(),
        pme = s->second.end();

      while ( pm != pme )
      {

        gkg::Dictionary parameterDictionary;

        // looping over parameter(s)
        gkg::StatisticalParameterMap::const_iterator
          p = pm->second.begin(),
          pe = pm->second.end();
        while ( p != pe )
        {

          parameterDictionary[ p->first ] = p->second;
          ++ p;

        }

        measureDictionary[ pm->first ] = parameterDictionary;

        ++ pm;

      }
      structureDictionary[ toString( s->first ) ] = measureDictionary;
      ++ s;

    }

    headeredObject.getHeader().addAttribute( _name, structureDictionary );

  }
  GKG_CATCH( "template < class S, class Compare > "
             "inline "
             "void gkg::StructureBasedStatisticalMap< S, Compare >::"
             "addToHeaderedObject( "
             "gkg::HeaderedObject& headeredObject ) const" );

}


template < class S, class Compare >
inline
std::string gkg::StructureBasedStatisticalMap< S, Compare >::toString(
                                                      const S& structure ) const
{

  try
  {

    return gkg::StringConverter::toString( structure );

  }
  GKG_CATCH( "template < class S, class Compare > "
             "inline "
             "std::string gkg::StructureBasedStatisticalMap< S, Compare >::"
             "getName( const S& structure ) const" );

}


template < class S, class Compare >
inline
S gkg::StructureBasedStatisticalMap< S, Compare >::toStructure(
                                                  const std::string& name ) const
{

  try
  {

    S structure;
    bool ok = gkg::StringConverter::toScalar( name, structure );
    if ( !ok )
    {

      throw std::runtime_error( "bad string to structure convertion" );

    }
    return structure;

  }
  GKG_CATCH( "template < class S, class Compare > "
             "inline "
             "S gkg::StructureBasedStatisticalMap< S, Compare >::toStructure( "
             "const std::string& name ) const" );

}


#endif
