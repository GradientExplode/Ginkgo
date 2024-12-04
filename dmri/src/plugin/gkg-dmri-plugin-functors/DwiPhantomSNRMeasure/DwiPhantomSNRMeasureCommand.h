#ifndef _gkg_dmri_plugin_functors_DwiPhantomSNRMeasure_DwiPhantomSNRMeasureCommand_h_
#define _gkg_dmri_plugin_functors_DwiPhantomSNRMeasure_DwiPhantomSNRMeasureCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwiPhantomSNRMeasureCommand :
                                public Command,
                                public Creator2Arg< DwiPhantomSNRMeasureCommand,
                                                    Command,
                                                    int32_t,
                                                    char** >,
                                public Creator1Arg< DwiPhantomSNRMeasureCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    DwiPhantomSNRMeasureCommand( int32_t argc,
                                 char* argv[],
                                 bool loadPlugin = false,
                                 bool removeFirst = true );
    DwiPhantomSNRMeasureCommand( const std::string& fileNameDwi,
                                 const std::string& fileNameWeight,
                                 const std::vector< int32_t >& sBox,
                                 const std::vector< int32_t >& nBox,
                                 const std::string& fileNameOut,
                                 bool verbose );
    DwiPhantomSNRMeasureCommand( const Dictionary& parameters );
    virtual ~DwiPhantomSNRMeasureCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiPhantomSNRMeasureCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiPhantomSNRMeasureCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameDwi,
                  const std::string& fileNameWeight,
                  const std::vector< int32_t >& sBox,
                  const std::vector< int32_t >& nBox,
                  const std::string& fileNameOut,
                  bool verbose );

};


}


#endif
