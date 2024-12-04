#ifndef _gkg_processing_plugin_functors_AffineTransform3dSplitter_AffineTransform3dSplitterCommand_h_
#define _gkg_processing_plugin_functors_AffineTransform3dSplitter_AffineTransform3dSplitterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <vector>
#include <string>


namespace gkg
{


class AffineTransform3dSplitterCommand : public Command,
                                         public Creator2Arg< 
                                               AffineTransform3dSplitterCommand,
                                               Command,
                                               int32_t,
                                               char** >,
                                         public Creator1Arg<
                                               AffineTransform3dSplitterCommand,
                                               Command,
                                               const Dictionary& >
{

  public:

    AffineTransform3dSplitterCommand( int32_t argc,
                                      char* argv[], 
                                      bool loadPlugin = false,
                                      bool removeFirst = true );
    AffineTransform3dSplitterCommand( 
                               const std::string& transformNameIn,
                               const std::string& transformNameOut,
                               const std::vector< std::string >& keptParameters,
                               bool verbose );
    AffineTransform3dSplitterCommand( const Dictionary& parameters );
    virtual ~AffineTransform3dSplitterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< AffineTransform3dSplitterCommand, Command, 
                              int32_t, char** >;
    friend class Creator2Arg< AffineTransform3dSplitterCommand, Command,
                              const std::string&, const Dictionary& >;

    void parse();
    void execute( const std::string& transformNameIn,
                  const std::string& transformNameOut,
                  const std::vector< std::string >& keptParameters,
                  bool verbose );

};


}


#endif
