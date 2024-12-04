#ifndef _gkg_communication_log_InfoWriter_h_
#define _gkg_communication_log_InfoWriter_h_


#include <string>
#include <fstream>


namespace gkg
{


class InfoWriter
{

  public:

    InfoWriter( const std::string& name );
    virtual ~InfoWriter();

    virtual void initialize( const std::string& applicationName );

  protected:

    void checkAndCreateDirectory();
    void clearOldFiles();

    std::string _name;
    std::string _applicationName;
    std::string _directoryName;
    std::string _timeString;
    std::string _fileName;

};


}


#endif
