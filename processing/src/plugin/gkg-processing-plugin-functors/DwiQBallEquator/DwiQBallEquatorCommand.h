#ifndef _gkg_processing_plugin_functors_DwiQBallEquator_DwiQBallEquatorCommand_h_
#define _gkg_processing_plugin_functors_DwiQBallEquator_DwiQBallEquatorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwiQBallEquatorCommand : public Command,
                               public Creator2Arg< DwiQBallEquatorCommand,
                                        	   Command,
                                        	   int32_t,
                                        	   char** >,
                               public Creator1Arg< DwiQBallEquatorCommand,
                                                   Command,
                                                   const Dictionary& >
{

  public:

    DwiQBallEquatorCommand( int32_t argc,
                  	    char* argv[], 
                  	    bool loadPlugin = false,
                  	    bool removeFirst = true );
    DwiQBallEquatorCommand( const std::vector< double >& normalizedOrientation,
    			    int32_t pointCount,
    			    const std::string& fileNameOut,
    			    double bValue,
    			    bool verbose );
    DwiQBallEquatorCommand( const Dictionary& parameters );
    virtual ~DwiQBallEquatorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiQBallEquatorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiQBallEquatorCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::vector< double >& normalizedOrientation,
    		  int32_t pointCount,
    		  const std::string& fileNameOut,
    		  double bValue,
    		  bool verbose );

};


}


#endif
