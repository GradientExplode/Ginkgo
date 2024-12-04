#ifndef _gkg_processing_plugin_functors_ConvexHull_ConvexHullCommand_h_
#define _gkg_processing_plugin_functors_ConvexHull_ConvexHullCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class ConvexHullCommand : public Command,
                          public Creator2Arg< ConvexHullCommand,
                    		   	      Command,
                                              int32_t,
                                              char** >,
                          public Creator1Arg< ConvexHullCommand,
                                              Command,
                                              const Dictionary& >
{

  public:

    ConvexHullCommand( int32_t argc,
                       char* argv[], 
                       bool loadPlugin = false,
                       bool removeFirst = true );
    ConvexHullCommand( const std::string& fileNameIn,
                       const std::string& fileNameOut,
                       bool addSymmetrical,
                       bool ascii,
                       const std::string& format,
                       bool verbose );
    ConvexHullCommand( const Dictionary& parameters );
    virtual ~ConvexHullCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< ConvexHullCommand, Command, int32_t, char** >;
    friend class Creator1Arg< ConvexHullCommand, Command, const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  bool addSymmetrical,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
