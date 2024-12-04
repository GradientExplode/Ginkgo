#ifndef _gkg_processing_plugin_functors_GetSliceMask_GetSliceMaskCommand_h_
#define _gkg_processing_plugin_functors_GetSliceMask_GetSliceMaskCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class GetSliceMaskCommand : public Command,
                            public Creator2Arg< GetSliceMaskCommand,
                                                Command,
                                                int32_t,
                                                char** >,
                            public Creator1Arg< GetSliceMaskCommand,
                                                Command,
                                                const Dictionary& >
{

  public:

    GetSliceMaskCommand( int32_t argc,
                         char* argv[], 
                         bool loadPlugin = false,
                         bool removeFirst = true );
    GetSliceMaskCommand( const std::string& fileNameIn,
    			 const std::string& fileNameOut,
    			 int32_t xSliceIndex,
    			 int32_t ySliceIndex,
    			 int32_t zSliceIndex,
    			 const std::string& outType,
    			 const std::string& mode,
    			 double threshold1,
    			 double threshold2,
    			 double foreground,
    			 double background,
    			 bool ascii,
    			 const std::string& format,
    			 bool verbose );
    GetSliceMaskCommand( const Dictionary& parameters );
    virtual ~GetSliceMaskCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< GetSliceMaskCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< GetSliceMaskCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
    		  const std::string& fileNameOut,
    		  int32_t xSliceIndex,
    		  int32_t ySliceIndex,
    		  int32_t zSliceIndex,
    		  const std::string& outType,
    		  const std::string& mode,
    		  double threshold1,
    		  double threshold2,
    		  double foreground,
    		  double background,
    		  bool ascii,
    		  const std::string& format,
    		  bool verbose );

};


}


#endif
