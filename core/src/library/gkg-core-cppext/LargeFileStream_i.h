#ifndef _gkg_core_cppext_LargeFileStream_i_h_
#define _gkg_core_cppext_LargeFileStream_i_h_


#include <gkg-core-cppext/LargeFileStream.h>
#include <gkg-core-cppext/StringStream.h>


//
// class BasicLargeFileIFStream
//

template < class T >
inline
gkg::BasicLargeFileIFStream& 
gkg::BasicLargeFileIFStream::operator>>( T& /*thing*/ )
{


  // code to be written
  return *this;

}


//
// class BasicLargeFileOFStream
//

template < class T >
inline
gkg::BasicLargeFileOFStream& 
gkg::BasicLargeFileOFStream::operator<<( const T& thing )
{

  std::ostringstream oss;
  oss << thing;
  std::string s = oss.str();
  if ( !s.empty() )
  {

    write( s.c_str(), s.length() );

  }
  return *this;

}


//
// class BasicLargeFileFStream
//

template < class T >
inline
gkg::BasicLargeFileFStream& 
gkg::BasicLargeFileFStream::operator>>( T& /*thing*/ )
{

  // code to be written
  return *this;

}


template < class T >
inline
gkg::BasicLargeFileFStream&
gkg::BasicLargeFileFStream::operator<<( const T& thing )
{

  std::ostringstream oss;
  oss << thing;
  std::string s = oss.str();
  if ( !s.empty() )
  {

    write( s.c_str(), s.length() );

  }
  return *this;

}


#endif
