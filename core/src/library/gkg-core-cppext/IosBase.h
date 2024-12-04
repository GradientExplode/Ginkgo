#ifndef _gkg_core_cppext_IosBase_h_
#define _gkg_core_cppext_IosBase_h_


#include <iostream>


namespace gkg
{


struct IosBase
{

#if defined (__GNUC__) && (__GNUC__-0 < 3)

  typedef std::ios Ios;
  typedef std::ios::seek_dir seekdir;

  static const seekdir beg = std::ios::beg;
  static const seekdir cur = std::ios::cur;
  static const seekdir end = std::ios::end;

#else

  typedef std::ios_base Ios;
  typedef std::ios::seekdir seekdir;

  static const seekdir beg = std::ios_base::beg;
  static const seekdir cur = std::ios_base::cur;
  static const seekdir end = std::ios_base::end;

#endif

};


}


#endif
