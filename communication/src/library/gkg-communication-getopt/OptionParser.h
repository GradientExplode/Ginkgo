#ifndef _gkg_communication_getopt_OptionParser_h_
#define _gkg_communication_getopt_OptionParser_h_


#include <gkg-communication-getopt/Option.h>


namespace gkg
{


class OptionParser
{

  public:

    OptionParser( int32_t argc, char* argv[], bool commandLauncher = false );
    virtual ~OptionParser();

    template < class T >
    void addSingleOption( const std::string& name,
                          const std::string& information,
                          T& value,
                          bool optional = false );
    template < class T >
    void addSeriesOption( const std::string& name,
                          const std::string& information,
                          T& series,
                          size_t minItemCount = 0U,
                          size_t maxItemCount = 0U );

    void alias( const std::string& from, const std::string& to );
    void parse();
    void check();

  protected:

    std::deque< std::string > _arguments;
    std::deque< Option* > _options;
    bool _commandLauncher;

};


}


#endif
