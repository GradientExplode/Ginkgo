#ifndef _gkg_communication_X11_X11FileImplementation_h_
#define _gkg_communication_X11_X11FileImplementation_h_


#include <gkg-communication-sysinfo/FileImplementation.h>
#include <sys/stat.h>


namespace gkg
{


class X11FileImplementation : public FileImplementation
{

  public:

    X11FileImplementation( File* file, const std::string& name );
    ~X11FileImplementation();

    std::string getBaseName() const;
    std::string getDirectoryName() const;
    uint64_t getSize() const;

    time_t getLastAccessTime() const;
    time_t getLastModificationTime() const;
    time_t getLastStateChangeTime() const;

    bool isExisting() const;
    bool isRegular() const;
    bool isDirectory() const;
    bool isLink() const;
    bool isCharacterDevice() const;
    bool isBlockDevice() const;

    bool isReadable() const;
    bool isWritable() const;
    bool isExecutable() const;

    bool isUserReadable() const;
    bool isUserWritable() const;
    bool isUserExecutable() const;

    bool isGroupReadable() const;
    bool isGroupWritable() const;
    bool isGroupExecutable() const;

    bool isOtherReadable() const;
    bool isOtherWritable() const;
    bool isOtherExecutable() const;

    void remove();

    void update();

    static X11FileImplementation* getImplementation( File* file );

  private:

    struct stat _statistics;
    bool _isExisting;

};


}


#endif

