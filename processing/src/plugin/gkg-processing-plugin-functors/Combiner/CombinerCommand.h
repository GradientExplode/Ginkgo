#ifndef _gkg_processing_plugin_functors_Combiner_CombinerCommand_h_
#define _gkg_processing_plugin_functors_Combiner_CombinerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <vector>
#include <string>


namespace gkg
{


class CombinerCommand : public Command,
                        public Creator2Arg< CombinerCommand,
                        		    Command,
                        		    int32_t,
                        		    char** >,
                        public Creator1Arg< CombinerCommand,
                                            Command,
                                            const Dictionary& >
{

  public:

    CombinerCommand( int32_t argc,
                     char* argv[], 
                     bool loadPlugin = false,
                     bool removeFirst = true );
    CombinerCommand( const std::vector< std::string >& fileNameIns,
                     const std::string& fileNameOut,
    		     std::vector< std::string >& functor1s,
    		     std::vector< std::string >& functor2s,
    		     std::vector< double >& numerator1s,
    		     std::vector< double >& denominator1s,
    		     std::vector< double >& numerator2s,
    		     std::vector< double >& denominator2s,
    		     std::vector< std::string >& operators,
    		     const std::string& fileNameMask,
    		     const std::string& mode,
    		     double threshold1,
    		     double threshold2,
    		     double background,
    		     const std::string& outputType,
    		     bool ascii,
    		     const std::string& format,
    		     bool verbose );
    CombinerCommand( const Dictionary& parameters );
    virtual ~CombinerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< CombinerCommand, Command, int32_t, char** >;
    friend class Creator1Arg< CombinerCommand, Command, const Dictionary& >;

    void parse();
    void execute( const std::vector< std::string >& fileNameIns,
                  const std::string& fileNameOut,
    		  std::vector< std::string >& functor1s,
    		  std::vector< std::string >& functor2s,
    		  std::vector< double >& numerator1s,
    		  std::vector< double >& denominator1s,
    		  std::vector< double >& numerator2s,
    		  std::vector< double >& denominator2s,
    		  std::vector< std::string >& operators,
    		  const std::string& fileNameMask,
    		  const std::string& mode,
    		  double threshold1,
    		  double threshold2,
    		  double background,
    		  const std::string& outputType,
    		  bool ascii,
    		  const std::string& format,
    		  bool verbose );

};


}


#endif
