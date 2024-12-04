#ifndef _gkg_communication_getopt_Application_h_
#define _gkg_communication_getopt_Application_h_


#include <gkg-communication-getopt/OptionParser_i.h>
#include <gkg-communication-getopt/SingleOption_i.h>
#include <gkg-communication-getopt/SeriesOption_i.h>
#include <gkg-core-exception/HelpOptionException.h>


namespace gkg
{


class Application : public OptionParser
{

  public:

    Application( int32_t argc,
                 char* argv[], 
                 const std::string& documentation,
                 bool loadPlugin = true,
                 bool commandLauncher = false );
    virtual ~Application();

    void initialize();

    const std::string& getName() const;

  private:

    std::string _name;
    std::string _documentation;
    bool _loadPlugin;
    bool _commandLauncher;

};


}


#endif
