#ifndef _gkg_communication_sysinfo_Directory_h_
#define _gkg_communication_sysinfo_Directory_h_


#include <gkg-core-cppext/StdInt.h>
#include <set>
#include <string>


namespace gkg
{


class DirectoryImplementation;


class Directory
{

  public:

    Directory( const std::string& name = "." );
    virtual ~Directory();

    virtual bool isValid() const;
    virtual bool isDirectory( int32_t index ) const;

    virtual const std::string& getPath() const;
    virtual int32_t getCount() const;
    virtual std::string getName( int32_t index ) const;
    virtual int32_t getIndex( const std::string& name ) const;

    virtual std::set< std::string > getFiles() const;
    virtual std::set< std::string > getMatchingFiles( 
                                             const std::string& pattern ) const;
    virtual std::set< std::string > getSubDirectories(
                 const std::set< std::string >* unwantedDirectories = 0 ) const;

    virtual void chdir( const std::string& name );
    virtual void mkdir() const;
    virtual void remove();

    static bool match( const std::string& name, const std::string& pattern );

    DirectoryImplementation* getImplementation() const;

  private:

    DirectoryImplementation* _directoryImplementation;

    // not allowed
    Directory( const Directory& );
    Directory& operator =( const Directory& );

};


// this function is used to create system-dependant canonical directory form
// for a given path and should be implemented in the concrete library
// (in libgkg-communication-X11.so for instance )
std::string getCanonicalDirectory( const std::string& path );


// this function is used to get the separator pattern for directories
// and should be implemented in the concrete library
// (in libgkg-communication-X11.so for instance )
std::string getDirectorySeparator();

// this function is used to get the current directory
// and should be implemented in the concrete library
// (in libgkg-communication-X11.so for instance )
std::string getCurrentDirectory();

}


#endif
