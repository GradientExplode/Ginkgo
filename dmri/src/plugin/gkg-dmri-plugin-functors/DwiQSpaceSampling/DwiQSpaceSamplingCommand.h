#ifndef _gkg_dmri_plugin_functors_DwiQSpaceSampling_DwiQSpaceSamplingCommand_h_
#define _gkg_dmri_plugin_functors_DwiQSpaceSampling_DwiQSpaceSamplingCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwiQSpaceSamplingCommand : public Command,
                                 public Creator2Arg< DwiQSpaceSamplingCommand,
                                                     Command,
                                                     int32_t,
                                                     char** >,
                                 public Creator1Arg< DwiQSpaceSamplingCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    DwiQSpaceSamplingCommand( int32_t argc,
                              char* argv[],
                              bool loadPlugin = false,
                              bool removeFirst = true );
    DwiQSpaceSamplingCommand(
                             const std::string& fileNameDW,
                             const std::vector< double >& scalarParameters,
                             const std::vector< std::string >& stringParameters,
                             const std::vector< float >& coefficients,
                             const std::string& fileNameGradientCharacteristics,
                             bool verbose );
    DwiQSpaceSamplingCommand( const Dictionary& parameters );
    virtual ~DwiQSpaceSamplingCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiQSpaceSamplingCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiQSpaceSamplingCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameDW,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  const std::vector< float >& coefficients,
                  const std::string& fileNameGradientCharacteristics,
                  bool verbose );

};


}


#endif
