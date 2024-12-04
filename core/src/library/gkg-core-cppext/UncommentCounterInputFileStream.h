#ifndef _gkg_core_cppext_UncommentCounterInputFileStream_h_
#define _gkg_core_cppext_UncommentCounterInputFileStream_h_

#if defined (__GNUC__) && (__GNUC__-0 < 3)

#include <streambuf.h>

#else

#include <streambuf>

#endif

#include <fstream>


namespace gkg
{


//
// class FilteredInputStreamBuffer
//

template < class F >
class FilteredInputStreamBuffer : public std::streambuf
{

  public:

    FilteredInputStreamBuffer( std::streambuf* source,
                               const F& filter,
                               bool autodelete = false );
    FilteredInputStreamBuffer( std::streambuf* source,
                               bool autodelete = false );
    virtual ~FilteredInputStreamBuffer();

    F& getFilter();

  protected:

    virtual int underflow();
    virtual int sync();
    virtual std::streambuf* setbuf( char* p, int length );

  private:

    std::streambuf* _source;
    F _filter;
    char _buffer;
    bool _autodelete;

};


//
// class UncommentFilter
//

class UncommentFilter
{

  public:

    UncommentFilter( char commentCharacter = '#' );

    int operator()( std::streambuf& source );

  private:

    char _commentCharacter;

};


//
// class CounterFilter
//

class CounterFilter
{

  public:

    CounterFilter( int line = 1 );

    int operator()( std::streambuf& source );
    int getLine() const;
    int reset( int line = 1 );

  private:

    int _line;
    bool _newLine;

};


//
// class UncommentCounterInputFileStream
//

class UncommentCounterInputFileStream : public std::istream
{

  public:

    UncommentCounterInputFileStream();
    UncommentCounterInputFileStream( const char* name );
    virtual ~UncommentCounterInputFileStream();

    virtual void open( const char* name );
    virtual void close();
    virtual bool operator!() const;
    virtual bool is_open() const;

#if defined (__GNUC__) && (__GNUC__-0 < 3)

    void specialSeekg( streamoff offset,
                       std::ios::seek_dir dir );

#else

    void specialSeekg( std::istream::off_type offset,
                       std::ios_base::seekdir dir );

#endif

    int getLine() const;

  protected:

    virtual void init_buffers();
    virtual void destroy_buffers();

    mutable std::ifstream _base;
    FilteredInputStreamBuffer< UncommentFilter >* _uncomment;
    FilteredInputStreamBuffer< CounterFilter >* _counter;

};


}


#endif
