#ifndef _gkg_processing_plugin_functors_AffineTransform3d_AffineTransform3dCommand_h_
#define _gkg_processing_plugin_functors_AffineTransform3d_AffineTransform3dCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <vector>
#include <string>


namespace gkg
{


class AffineTransform3dCommand : public Command,
                                 public Creator2Arg< AffineTransform3dCommand,
                                                     Command,
                                                     int32_t,
                                                     char** >,
                                 public Creator1Arg< AffineTransform3dCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    AffineTransform3dCommand( int32_t argc,
                              char* argv[], 
                              bool loadPlugin = false,
                              bool removeFirst = true );
    AffineTransform3dCommand( const std::vector< float >& scalingParameters,
                              const std::vector< float >& shearingParameters,
                              const std::vector< float >& rotationParameters,
                              const std::vector< float >& translationParameters,
                              const std::string& transformName );
    AffineTransform3dCommand( const Dictionary& parameters );
    virtual ~AffineTransform3dCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< AffineTransform3dCommand, Command, 
                              int32_t, char** >;
    friend class Creator2Arg< AffineTransform3dCommand, Command,
                              const std::string&, const Dictionary& >;

    void parse();
    void execute( const std::vector< float >& scalingParameters,
                  const std::vector< float >& shearingParameters,
                  const std::vector< float >& rotationParameters,
                  const std::vector< float >& translationParameters,
                  const std::string& transformName );

};


}


#endif
