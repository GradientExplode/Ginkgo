#ifndef _gkg_mri_reconstruction_plugin_functors_DwBalancedSSFPPhaseOffsetCombiner_DwBalancedSSFPPhaseOffsetCombinerCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_DwBalancedSSFPPhaseOffsetCombiner_DwBalancedSSFPPhaseOffsetCombinerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwBalancedSSFPPhaseOffsetCombinerCommand : public Command,
                  public Creator2Arg< DwBalancedSSFPPhaseOffsetCombinerCommand,
                                      Command,
                                      int32_t,
                                      char** >,
                  public Creator1Arg< DwBalancedSSFPPhaseOffsetCombinerCommand,
                                      Command,
                                      const Dictionary& >
{

  public:

    DwBalancedSSFPPhaseOffsetCombinerCommand( int32_t argc,
                                               char* argv[], 
                                               bool loadPlugin = false,
                                               bool removeFirst = true );
    DwBalancedSSFPPhaseOffsetCombinerCommand( 
                                  const std::vector< std::string >& inputNames,
                                  const std::string& outputName,
                                  const std::string& axis,
                                  bool ascii,
                                  const std::string& format,
                                  bool verbose );
    DwBalancedSSFPPhaseOffsetCombinerCommand( const Dictionary& parameters );
    virtual ~DwBalancedSSFPPhaseOffsetCombinerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwBalancedSSFPPhaseOffsetCombinerCommand,
                              Command, int32_t, char** >;
    friend class Creator1Arg< DwBalancedSSFPPhaseOffsetCombinerCommand,
                              Command, const Dictionary& >;

    void parse();
    void execute( const std::vector< std::string >& inputNames,
                  const std::string& outputName,
                  const std::string& axis,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
