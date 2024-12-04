#ifndef _gkg_communication_plugin_functors_CudaDeviceList_CudaDeviceListCommand_h_
#define _gkg_communication_plugin_functors_CudaDeviceList_CudaDeviceListCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class CudaDeviceListCommand : public Command,
                              public Creator2Arg< CudaDeviceListCommand,
                                                  Command,
                                                  int32_t,
                                                  char** >,
                              public Creator1Arg< CudaDeviceListCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    CudaDeviceListCommand( int32_t argc,
                           char* argv[], 
                           bool loadPlugin = false,
                           bool removeFirst = true );
    CudaDeviceListCommand();
    CudaDeviceListCommand( const gkg::Dictionary& parameters );
    virtual ~CudaDeviceListCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< CudaDeviceListCommand, Command, int32_t, char** >;
    friend class Creator1Arg< CudaDeviceListCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute();

};


}


#endif
