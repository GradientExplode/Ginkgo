#ifndef _gkg_dmri_plugin_functors_DwiDirectionFieldDiscrepancy_DwiDirectionFieldDiscrepancyCommand_h_
#define _gkg_dmri_plugin_functors_DwiDirectionFieldDiscrepancy_DwiDirectionFieldDiscrepancyCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiDirectionFieldDiscrepancyCommand :
                        public Command,
                        public Creator2Arg< DwiDirectionFieldDiscrepancyCommand,
                                            Command,
                                            int32_t,
                                            char** >,
                        public Creator1Arg< DwiDirectionFieldDiscrepancyCommand,
                                            Command,
                                            const Dictionary& >
{

  public:

    DwiDirectionFieldDiscrepancyCommand( int32_t argc,
                                         char* argv[],
                                         bool loadPlugin = false,
                                         bool removeFirst = true );
    DwiDirectionFieldDiscrepancyCommand( 
                                    const std::string& fileNameDirectionField1,
                                    const std::string& fileNameDirectionField2,
                                    const std::string& fileNameAngleDiscrepancy,
                                    bool degree,
                                    const std::string format,
                                    bool ascii,
                                    bool verbose );
    DwiDirectionFieldDiscrepancyCommand( const Dictionary& parameters );
    virtual ~DwiDirectionFieldDiscrepancyCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiDirectionFieldDiscrepancyCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiDirectionFieldDiscrepancyCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameDirectionField1,
                  const std::string& fileNameDirectionField2,
                  const std::string& fileNameAngleDiscrepancy,
                  bool degree,
                  const std::string format,
                  bool ascii,
                  bool verbose );

};


}


#endif
