#ifndef _gkg_graphics_QtGL_plugin_functors_Abacus_AbacusCommand_h_
#define _gkg_graphics_QtGL_plugin_functors_Abacus_AbacusCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


class QApplication;


namespace gkg
{


class AbacusCommand : public Command,
                      public Creator2Arg< AbacusCommand,
                                          Command,
                                          int32_t,
                                          char** >,
                      public Creator1Arg< AbacusCommand,
                                          Command,
                                          const Dictionary& >
{

  public:

    AbacusCommand( int32_t argc,
                   char* argv[], 
                   bool loadPlugin = false,
                   bool removeFirst = true );
    AbacusCommand( const std::string& fileName );
    AbacusCommand( const Dictionary& parameters );
    virtual ~AbacusCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< AbacusCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< AbacusCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileName );

  private:

    QApplication* _qtApplication;

};


}


#endif
