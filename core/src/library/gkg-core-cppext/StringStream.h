#ifndef _gkg_core_cppext_StringStream_h_
#define _gkg_core_cppext_StringStream_h_


#if defined (__GNUC__) && (__GNUC__-0 == 2) && (__GNUC_MINOR__-0 < 96)


#include <strstream>


namespace std
{

  typedef istrstream istringstream;
  typedef ostrstream ostringstream;
  typedef strstream stringstream;

}


#else // gcc >= 2.96 and other compilers


#include <sstream>


#endif


#endif
