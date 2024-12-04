#ifndef _gkg_core_io_DicomTagInfo_h_
#define _gkg_core_io_DicomTagInfo_h_


#include <gkg-core-cppext/StdInt.h>

#include <string>
#include <list>


namespace gkg
{


struct DicomTagInfo
{

  DicomTagInfo( int32_t theGroup = -2,
                std::string theTag = "Tag",
                std::string theVr = "VR",
                int32_t theVm = -1,
                int32_t theSize = -1,
                std::string theName = "Name",
                std::string theValue = "Value" );

  std::string group;
  std::string tag;
  std::string vr;
  std::string vm;
  std::string size;
  std::string name;
  std::string value;

  std::list< DicomTagInfo > tagList;

};


}


#endif
