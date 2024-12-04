#include <gkg-core-io/DicomTagInfo.h>

#include <sstream>
#include <iomanip>


gkg::DicomTagInfo::DicomTagInfo( int32_t theGroup,
                                 std::string theTag,
                                 std::string theVr,
                                 int32_t theVm,
                                 int32_t theSize,
                                 std::string theName,
                                 std::string theValue )
                 : group( "" ),
                   tag( theTag ),
                   vr( theVr ),
                   vm( "" ),
                   size( "" ),
                   name( theName ),
                   value( theValue )
{

  if ( theGroup >= 0 )
  {

    std::ostringstream groupStr;
    groupStr << std::hex << std::setw( 4 ) << std::setfill( '0' ) << theGroup;
    group = groupStr.str();

    if ( theTag.empty() )
    {

      tag = std::string( "Group " ) + group;

    }

  }

  if ( theGroup == -2 )
  {

    vm = "VM";
    size = "Size";

  }
  else
  {

    if ( theVm >= 0 )
    {

      std::ostringstream vmStr;
      vmStr << theVm;
      vm = vmStr.str();

    }

    if ( theSize >= 0 )
    {

      std::ostringstream sizeStr;
      sizeStr << theSize;
      size = sizeStr.str();

    }

  }

}
