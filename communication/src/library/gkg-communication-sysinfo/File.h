#ifndef _gkg_communication_sysinfo_File_h_
#define _gkg_communication_sysinfo_File_h_


#include <gkg-core-cppext/StdInt.h>
#include <string>
#include <ctime>


namespace gkg
{


class FileImplementation;


class File
{

  public:

    File( const std::string& name );
    virtual ~File();

    std::string getName() const;
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

    FileImplementation* getImplementation() const;

  protected:

    FileImplementation* _fileImplementation;

};


}


#endif
