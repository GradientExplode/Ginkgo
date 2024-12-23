#ifndef _gkg_core_cppext_LargeFileStream_h_
#define _gkg_core_cppext_LargeFileStream_h_


#include <gkg-core-cppext/IosBase.h>
#include <cstdio>
#include <fcntl.h>
#include <string>
#include <iostream>


namespace gkg
{


//
// class BasicLargeFileIFStream
//

class BasicLargeFileIFStream
{

  public:

    typedef off_t off_type;

    BasicLargeFileIFStream();
    BasicLargeFileIFStream( const std::string& name );
    virtual ~BasicLargeFileIFStream();


    void open( const std::string& name );
    bool isOpened() const;
    void close();

    void read( char* pointer, size_t count ); 
    void seek( off_type offset, IosBase::seekdir direction );

    template < class T >
    BasicLargeFileIFStream& operator>>( T& thing );

  private:

    int _fileDescriptor;

};


//
// class BasicLargeFileOFStream
//

class BasicLargeFileOFStream
{

  public:

    typedef off_t off_type;

    BasicLargeFileOFStream();
    BasicLargeFileOFStream( const std::string& name );
    virtual ~BasicLargeFileOFStream();


    void open( const std::string& name );
    bool isOpened() const;
    void close();

    void write( const char* pointer, size_t count ); 
    void seek( off_type offset, IosBase::seekdir direction );
    void truncate( off_type length );

    template < class T >
    BasicLargeFileOFStream& operator<<( const T& thing );

  private:

    int _fileDescriptor;

};


//
// class BasicLargeFileFStream
//

class BasicLargeFileFStream
{

  public:

    typedef off_t off_type;

    BasicLargeFileFStream();
    BasicLargeFileFStream( const std::string& name );
    virtual ~BasicLargeFileFStream();


    void open( const std::string& name );
    bool isOpened() const;
    void close();

    void read( char* pointer, size_t count ); 
    void write( const char* pointer, size_t count ); 
    void seek( off_type offset, IosBase::seekdir direction );
    void truncate( off_type length );

    template < class T >
    BasicLargeFileFStream& operator>>( T& thing );
    template < class T >
    BasicLargeFileFStream& operator<<( const T& thing );

  private:

    int _fileDescriptor;

};


typedef BasicLargeFileIFStream largefile_ifstream;
typedef BasicLargeFileOFStream largefile_ofstream;
typedef BasicLargeFileFStream largefile_fstream;


}


#endif
