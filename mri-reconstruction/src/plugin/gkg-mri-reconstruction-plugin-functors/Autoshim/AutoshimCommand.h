#ifndef _gkg_mri_reconstruction_plugin_functors_Autoshim_AutoshimCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_Autoshim_AutoshimCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class AutoshimCommand : public Command,
                        public Creator2Arg< AutoshimCommand,
                                            Command,
                                            int32_t,
                                            char** >,
                        public Creator1Arg< AutoshimCommand,
                                            Command,
                                            const Dictionary& >
{

  public:

    AutoshimCommand( int32_t argc,
                     char* argv[],
                     bool loadPlugin = false,
                     bool removeFirst = true );
    AutoshimCommand( const std::string& fileNameDeltaPhase,
                     const std::string& fileNameMagnitude,
                     float radius,
                     int32_t entry,
                     int32_t position,
                     int16_t xfull,
                     int16_t yfull,
                     int16_t zfull,
                     float xfs,
                     float yfs,
                     float zfs,
                     int16_t oldShimX,
                     int16_t oldShimY,
                     int16_t oldShimZ );
    AutoshimCommand( const Dictionary& parameters );
    virtual ~AutoshimCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< AutoshimCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< AutoshimCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameDeltaPhase,
                  const std::string& fileNameMagnitude,
                  float radius,
                  int32_t entry,
                  int32_t position,
                  int16_t xfull,
                  int16_t yfull,
                  int16_t zfull,
                  float xfs,
                  float yfs,
                  float zfs,
                  int16_t oldShimX,
                  int16_t oldShimY,
                  int16_t oldShimZ );

};


}


#endif
