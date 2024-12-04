#ifndef _gkg_processing_plugin_functors_FIRFilter3d_FIRFilter3dCommand_h_
#define _gkg_processing_plugin_functors_FIRFilter3d_FIRFilter3dCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class FIRFilter3dCommand : public Command,
                     	   public Creator2Arg< FIRFilter3dCommand,
                     	   		       Command,
                     	   		       int32_t,
                     	   		       char** >,
                           public Creator1Arg< FIRFilter3dCommand,
                                               Command,
                                               const Dictionary& >
{

  public:

    FIRFilter3dCommand( int32_t argc,
                  	char* argv[], 
                  	bool loadPlugin = false,
                  	bool removeFirst = true );
    FIRFilter3dCommand( const std::string& fileNameIn,
    			const std::string& fileNameOut,
    			const std::string& filterType,
    			int32_t filterSize,
    			const std::vector< double >& frequencies,
    			const std::string& apodizationName,
    			const std::string& axes,
    			bool ascii,
    			const std::string& format,
    			bool verbose );
    FIRFilter3dCommand( const Dictionary& parameters );
    virtual ~FIRFilter3dCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< FIRFilter3dCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< FIRFilter3dCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
    		  const std::string& fileNameOut,
    		  const std::string& filterType,
    		  int32_t filterSize,
    		  const std::vector< double >& frequencies,
    		  const std::string& apodizationName,
    		  const std::string& axes,
    		  bool ascii,
    		  const std::string& format,
    		  bool verbose );

};


}


#endif
