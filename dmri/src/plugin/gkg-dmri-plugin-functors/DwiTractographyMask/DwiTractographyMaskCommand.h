#ifndef _gkg_dmri_plugin_functors_DwiTractographyMask_DwiTractographyMaskCommand_h_
#define _gkg_dmri_plugin_functors_DwiTractographyMask_DwiTractographyMaskCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwiTractographyMaskCommand :
                                 public Command,
                                 public Creator2Arg< DwiTractographyMaskCommand,
                                                     Command,
                                                     int32_t,
                                                     char** >,
                                 public Creator1Arg< DwiTractographyMaskCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    DwiTractographyMaskCommand( int32_t argc,
                                char* argv[],
                                bool loadPlugin = false,
                                bool removeFirst = true );
    DwiTractographyMaskCommand( const std::string& fileNameT2,
                                const std::string& fileNameFA,
                                const std::string& fileNameWhiteMask,
                                float faThreshold,
                                const std::vector< float >& t2StdDevFactors,
                                float erosionRadius,
                                float dilationRadius,
                                const std::string& intermediate,
                                bool verbose );
    DwiTractographyMaskCommand( const Dictionary& parameters );
    virtual ~DwiTractographyMaskCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiTractographyMaskCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiTractographyMaskCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameT2,
                  const std::string& fileNameFA,
                  const std::string& fileNameWhiteMask,
                  float faThreshold,
                  const std::vector< float >& t2StdDevFactors,
                  float erosionRadius,
                  float dilationRadius,
                  const std::string& intermediate,
                  bool verbose );

};


}


#endif
