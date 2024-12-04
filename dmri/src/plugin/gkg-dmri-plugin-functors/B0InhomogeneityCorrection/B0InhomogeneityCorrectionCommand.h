#ifndef _gkg_dmri_plugin_functors_B0InhomogeneityCorrection_B0InhomogeneityCorrectionCommand_h_
#define _gkg_dmri_plugin_functors_B0InhomogeneityCorrection_B0InhomogeneityCorrectionCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class B0InhomogeneityCorrectionCommand :
                           public Command,
                           public Creator2Arg< B0InhomogeneityCorrectionCommand,
                                               Command,
                                               int32_t,
                                               char** >,
                           public Creator1Arg< B0InhomogeneityCorrectionCommand,
                                               Command,
                                               const Dictionary& >
{

  public:

    B0InhomogeneityCorrectionCommand( int32_t argc,
                                      char* argv[],
                                      bool loadPlugin = false,
                                      bool removeFirst = true );
    B0InhomogeneityCorrectionCommand(
               const std::string& fileNameDistortedVolume,
               const std::string& fileNamePhaseDifferenceVolume,
               const std::string& fileNamePhaseDifferenceToDistortedTransform3d,
               const std::string& fileNameUndistortedVolume,
               const std::string& fileNameShiftVolume,
               const std::string& phaseAxis,
               const std::vector< float >& phaseToPixelFactorInformation,
               bool ascii,
               const std::string& format,
               bool verbose );
    B0InhomogeneityCorrectionCommand( const Dictionary& parameters );
    virtual ~B0InhomogeneityCorrectionCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< B0InhomogeneityCorrectionCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< B0InhomogeneityCorrectionCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute(
               const std::string& fileNameDistortedVolume,
               const std::string& fileNamePhaseDifferenceVolume,
               const std::string& fileNamePhaseDifferenceToDistortedTransform3d,
               const std::string& fileNameUndistortedVolume,
               const std::string& fileNameShiftVolume,
               const std::string& phaseAxis,
               const std::vector< float >& phaseToPixelFactorInformation,
               bool ascii,
               const std::string& format,
               bool verbose );

};


}


#endif
