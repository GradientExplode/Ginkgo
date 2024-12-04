#ifndef _gkg_communication_X11_X11DirectoryImplementation_h_
#define _gkg_communication_X11_X11DirectoryImplementation_h_


// BSD tends to have things in <sys/dir.h>, System V uses <dirent.h>;
// so far as I can tell, POSIX went with <dirent.h>;  Ultrix <dirent.h>
// includes <sys/dir.h>, which is silly because <sys/dir.h> needs <sys/types.h>


#include <gkg-communication-sysinfo/DirectoryImplementation.h>
#include <gkg-core-cppext/StdInt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <vector>

#ifdef apollo

#include <sys/dir.h>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#if _G_HAVE_SYS_SOCKET

#include <sys/socket.h>

#endif

#if _G_HAVE_SYS_RESOURCE

#include <sys/time.h>
#include <sys/resource.h>

#endif

#else

#include <dirent.h>

#endif


namespace gkg
{


class X11DirectoryImplementation : public DirectoryImplementation
{

  public:

    struct Entry
    {

      std::string name;
      struct stat info;

      bool operator < ( const Entry& other ) const;

    };

    X11DirectoryImplementation( Directory* directory, const std::string& name );
    ~X11DirectoryImplementation();

    std::string getName( int32_t index );
    int32_t getIndex( const std::string& name );
    int32_t getCount();

    void chdir( const std::string& name );
    void mkdir() const;
    void remove();

    bool isValid() const;
    bool isDirectory( int32_t index );

    static std::string getCanonical( const std::string& name );
    static bool hasDotSlash( const std::string& path, int32_t position );
    static bool hasDotDotSlash( const std::string& path,int32_t position );
    static std::string getHome( const std::string& name );
    static std::string eliminateDot( const std::string& path );
    static bool collapsedDotDotSlash( const std::string& path, int32_t& start );
    static std::string eliminateDotDot( const std::string& path );
    static std::string interpretSlashSlash( const std::string& path );
    static std::string interpretTilde( const std::string& path );
    static std::string expandTilde( const std::string& tilde );
    static std::string realPath( const std::string& path );
    static bool ifDirectory( const std::string& path );

    static X11DirectoryImplementation*
      getImplementation( Directory* directory );

  private:

    void doFill();
    void doClear();

    DIR* _dir;
    std::vector< Entry > _entries;
    bool _filled;

};


}


#endif
