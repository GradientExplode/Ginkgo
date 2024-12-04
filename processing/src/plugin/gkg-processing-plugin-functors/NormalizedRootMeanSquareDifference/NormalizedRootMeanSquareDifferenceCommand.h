#ifndef _gkg_processing_plugin_functors_NormalizedRootMeanSquareDifference_NormalizedRootMeanSquareDifferenceCommand_h_
#define _gkg_processing_plugin_functors_NormalizedRootMeanSquareDifference_NormalizedRootMeanSquareDifferenceCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/RCPointer.h>

#include <string>
#include <vector>


namespace gkg
{

class NormalizedRootMeanSquareDifferenceCommand : public Command,
                  public Creator2Arg< NormalizedRootMeanSquareDifferenceCommand,
                                  	  Command,
                                  	  int32_t,
                                  	  char** >,
                  public Creator1Arg< NormalizedRootMeanSquareDifferenceCommand,
                                      Command,
                                      const Dictionary& >
{

  public:

    NormalizedRootMeanSquareDifferenceCommand( int32_t argc,
                                        	     char* argv[], 
                                        	     bool loadPlugin = false,
                                        	     bool removeFirst = true );
    NormalizedRootMeanSquareDifferenceCommand( 
                                  const std::vector< std::string >& fileNameIns,
                  		     		    const std::string& fileNameMask,
                            	    const std::string& fileNameOut,
                  		      		  bool verbose );
    NormalizedRootMeanSquareDifferenceCommand( const Dictionary& parameters );
    virtual ~NormalizedRootMeanSquareDifferenceCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< NormalizedRootMeanSquareDifferenceCommand, 
                              Command, int32_t, char** >;
    friend class Creator1Arg< NormalizedRootMeanSquareDifferenceCommand, 
                              Command, const Dictionary& >;

    void parse();
    void execute( const std::vector< std::string >& fileNameIns,
                  const std::string& fileNameMask,
                  const std::string& fileNameOut,
                  bool verbose );

};



}

#endif