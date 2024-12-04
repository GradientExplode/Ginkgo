#ifndef _gkg_core_object_Dictionary_h_
#define _gkg_core_object_Dictionary_h_


#include <gkg-core-object/GenericObject.h>
#include <map>
#include <vector>


namespace gkg
{


typedef std::map< std::string, GenericObject > Dictionary;
typedef std::map< int32_t, GenericObject > IntDictionary;
typedef std::map< float, GenericObject > FloatDictionary;
typedef std::map< double, GenericObject > DoubleDictionary;
typedef std::map< GenericObject, GenericObject > RCDictionary;
typedef std::vector< GenericObject > GenericObjectList;


}


#endif
