#ifndef _gkg_dmri_plugin_functors_DwiSyntheticCorpusCallosumMidSagittalPlane_DwiSyntheticCorpusCallosumMidSagittalPlaneCommand_h_
#define _gkg_dmri_plugin_functors_DwiSyntheticCorpusCallosumMidSagittalPlane_DwiSyntheticCorpusCallosumMidSagittalPlaneCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>
#include <list>


namespace gkg
{


class DwiSyntheticCorpusCallosumMidSagittalPlaneCommand : public Command,
          public Creator2Arg< DwiSyntheticCorpusCallosumMidSagittalPlaneCommand,
                              Command,
                              int32_t,
                              char** >,
          public Creator1Arg< DwiSyntheticCorpusCallosumMidSagittalPlaneCommand,
                              Command,
                              const Dictionary& >
{

  public:

    DwiSyntheticCorpusCallosumMidSagittalPlaneCommand( int32_t argc,
                           char* argv[],
                           bool loadPlugin = false,
                           bool removeFirst = true );
    DwiSyntheticCorpusCallosumMidSagittalPlaneCommand( 
                                      const std::string& fileNameMask,
                                      const std::string& fileNameTrainingMatrix,
                                      int32_t trainingSampleCount,
                                      const std::string& fileNameOut,
                                      bool verbose );
    DwiSyntheticCorpusCallosumMidSagittalPlaneCommand(
                                                 const Dictionary& parameters );
    virtual ~DwiSyntheticCorpusCallosumMidSagittalPlaneCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiSyntheticCorpusCallosumMidSagittalPlaneCommand,
                              Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiSyntheticCorpusCallosumMidSagittalPlaneCommand,
                              Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameMask,
                  const std::string& fileNameTrainingMatrix,
                  int32_t trainingSampleCount,
                  const std::string& fileNameOut,
                  bool verbose );

};


}


#endif
