#ifndef _gkg_communication_sysinfo_FileFinder_h_
#define _gkg_communication_sysinfo_FileFinder_h_


#include <string>


namespace gkg
{


class FileFinder
{

  public:

    FileFinder();
    virtual ~FileFinder();

    bool locateFromPath( const std::string& name );
    bool locateFromDirectory( const std::string& name, 
                              const std::string& directory );

    const std::string& getFullPath() const;

  private:

    std::string _fullPath;

};


}


#endif
