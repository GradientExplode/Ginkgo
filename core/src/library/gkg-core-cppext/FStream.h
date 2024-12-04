#ifndef _gkg_core_cppext_FStream_h_
#define _gkg_core_cppext_FStream_h_


#ifdef __GNUC__


#if __GNUG__ >= 3


#include <ext/stdio_filebuf.h>


namespace gkg
{


class FStream : public __gnu_cxx::stdio_filebuf< char, 
                                                 std::char_traits< char > >
{

  public:

    enum OpenMode
    {

      In,
      Out,

    };

    FStream( int fd, OpenMode mode, int size )  // open mode not used
#if ( __GNUC__-0 >= 4 || __GNUC__-0 == 3 && __GNUC_MINOR__-0 >= 4 )
          : __gnu_cxx::stdio_filebuf< char,
                                      std::char_traits< char > >(
                                  fd,
                                  ( mode == In ? std::ios::in : std::ios::out ),
                                  size )
#else
          : __gnu_cxx::stdio_filebuf< char,
                                      std::char_traits< char > >(
                                  fd,
                                  ( mode == In ? std::ios::in : std::ios::out ),
                                  true,
                                  size )
#endif
    { }
    virtual ~FStream()
    { }
      
};


}


#else


#include <streambuf.h>


namespace gkg
{


class FStream : public std::filebuf
{

  public:

    enum OpenMode
    {

      In,
      Out,

    };

    FStream( int fd, OpenMode, int size )  // open mode not used
          : std::filebuf( fd )
    {

      _buffer = new char[ size ];
      setbuf( _buffer, size );

    }
    virtual ~FStream()
    {

      delete [] _buffer;

    }

  private:

    char* _buffer;
      
};


}


#endif


#else    // non __GNUC__

// code to be written for non GNU compiler(s)

#endif


#endif
