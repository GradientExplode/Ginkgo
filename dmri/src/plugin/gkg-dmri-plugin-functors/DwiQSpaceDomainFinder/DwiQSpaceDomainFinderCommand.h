#ifndef _gkg_dmri_plugin_functors_DwiQSpaceDomainFinder_DwiQSpaceDomainFinderCommand_h_
#define _gkg_dmri_plugin_functors_DwiQSpaceDomainFinder_DwiQSpaceDomainFinderCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiQSpaceDomainFinderCommand : public Command,
                         public Creator2Arg< DwiQSpaceDomainFinderCommand,
                                             Command,
                                             int32_t,
                                             char** >,
                         public Creator1Arg< DwiQSpaceDomainFinderCommand,
                                             Command,
                                             const Dictionary& >
{

  public:

    DwiQSpaceDomainFinderCommand( int32_t argc,
                                  char* argv[],
                                  bool loadPlugin = false,
                                  bool removeFirst = true );
    DwiQSpaceDomainFinderCommand( float maximumEchoTime,
                                  float maximumGradientMagnitude,
                                  float timeExcitation,
                                  float timeExcitationToRamp,
                                  float timeRamp,
                                  float timeRampToRefocusing,
                                  float timeRefocusing,
                                  float timeRampToRead,
                                  float timeRead,
                                  int32_t stepCountEchoTime,
                                  int32_t stepCountGradientMagnitude,
                                  int32_t stepCountLittleDelta,
                                  int32_t stepCountBigDelta,
                                  float minimumBValue,
                                  float maximumBValue,
                                  const std::string& qSpaceDomainFileName,
                                  const std::string& bValueFileName,
                                  const std::string& pythonListFileName,
                                  bool verbose );
    DwiQSpaceDomainFinderCommand( const Dictionary& parameters );
    virtual ~DwiQSpaceDomainFinderCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiQSpaceDomainFinderCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiQSpaceDomainFinderCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( float maximumEchoTime,
                  float maximumGradientMagnitude,
                  float timeExcitation,
                  float timeExcitationToRamp,
                  float timeRamp,
                  float timeRampToRefocusing,
                  float timeRefocusing,
                  float timeRampToRead,
                  float timeRead,
                  int32_t stepCountEchoTime,
                  int32_t stepCountGradientMagnitude,
                  int32_t stepCountLittleDelta,
                  int32_t stepCountBigDelta,
                  float minimumBValue,
                  float maximumBValue,
                  const std::string& qSpaceDomainFileName,
                  const std::string& bValueFileName,
                  const std::string& pythonListFileName,
                  bool verbose );

};


}


#endif
