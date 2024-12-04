#ifndef _gkg_graphics_QtGL_plugin_functors_DicomDump_DicomDumpCommand_h_
#define _gkg_graphics_QtGL_plugin_functors_DicomDump_DicomDumpCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


class QApplication;


namespace gkg
{


class DicomDumpCommand : public Command,
                         public Creator2Arg< DicomDumpCommand,
                                             Command,
                                             int32_t,
                                             char** >,
                         public Creator1Arg< DicomDumpCommand,
                                             Command,
                                             const Dictionary& >
{

  public:

    DicomDumpCommand( int32_t argc,
                      char* argv[], 
                      bool loadPlugin = false,
                      bool removeFirst = true );
    DicomDumpCommand( const std::string& fileNameIn,
                      const std::string& fileNameOut,
                      const std::string& tagName );
    DicomDumpCommand( const Dictionary& parameters );
    virtual ~DicomDumpCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DicomDumpCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DicomDumpCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& tagName );

  private:

    QApplication* _qtApplication;

};


}


#endif
