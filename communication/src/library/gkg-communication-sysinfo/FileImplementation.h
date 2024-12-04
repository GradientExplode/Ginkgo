#ifndef _gkg_communication_sysinfo_FileImplementation_h_
#define _gkg_communication_sysinfo_FileImplementation_h_


#include <gkg-core-cppext/StdInt.h>
#include <string>


namespace gkg
{


class File;


class FileImplementation
{

  public:

    virtual ~FileImplementation();

    File* getTarget() const;

    virtual std::string getName() const;
    virtual std::string getBaseName() const = 0;
    virtual std::string getDirectoryName() const = 0;
    virtual uint64_t getSize() const = 0;

    virtual time_t getLastAccessTime() const = 0;
    virtual time_t getLastModificationTime() const = 0;
    virtual time_t getLastStateChangeTime() const = 0;

    virtual bool isExisting() const = 0;
    virtual bool isRegular() const = 0;
    virtual bool isDirectory() const = 0;
    virtual bool isLink() const = 0;
    virtual bool isCharacterDevice() const = 0;
    virtual bool isBlockDevice() const = 0;

    virtual bool isReadable() const = 0;
    virtual bool isWritable() const = 0;
    virtual bool isExecutable() const = 0;

    virtual bool isUserReadable() const = 0;
    virtual bool isUserWritable() const = 0;
    virtual bool isUserExecutable() const = 0;

    virtual bool isGroupReadable() const = 0;
    virtual bool isGroupWritable() const = 0;
    virtual bool isGroupExecutable() const = 0;

    virtual bool isOtherReadable() const = 0;
    virtual bool isOtherWritable() const = 0;
    virtual bool isOtherExecutable() const = 0;

    virtual void remove() = 0;

    virtual void update() = 0;

  protected:

    FileImplementation( File* file, const std::string& name );

    File* _target;

    std::string _name;

};


}


#endif
