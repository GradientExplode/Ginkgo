#ifndef _gkg_communication_sysinfo_DirectoryImplementation_h_
#define _gkg_communication_sysinfo_DirectoryImplementation_h_


#include <gkg-core-cppext/StdInt.h>
#include <string>


namespace gkg
{


class Directory;


class DirectoryImplementation
{

  public:


    virtual ~DirectoryImplementation();

    virtual Directory* getTarget() const;

    virtual const std::string& getPath() const;

    virtual std::string getName( int32_t index ) = 0;
    virtual int32_t getIndex( const std::string& name ) = 0;
    virtual int32_t getCount() = 0;

    virtual void chdir( const std::string& name ) = 0;
    virtual void mkdir() const = 0;
    virtual void remove() = 0;

    virtual bool isValid() const = 0;
    virtual bool isDirectory( int index ) = 0;

  protected:

    DirectoryImplementation( Directory* directory, const std::string& name );

    Directory* _target;

    std::string _name;
    int32_t _count;

};


}


#endif
