#ifndef _gkg_graphics_X11_plugin_functors_BitmapEditor_BitmapEditorCommand_h_
#define _gkg_graphics_X11_plugin_functors_BitmapEditor_BitmapEditorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class BitmapEditorCommand : public Command,
                    	    public Creator2Arg< BitmapEditorCommand,
                    	   			Command,
                    	   			int32_t,
                    	   			char** >,
                            public Creator1Arg< BitmapEditorCommand,
                                                Command,
                                                const Dictionary& >
{

  public:

    BitmapEditorCommand( int32_t argc,
                 	 char* argv[], 
                 	 bool loadPlugin = false,
                 	 bool removeFirst = true );
    BitmapEditorCommand();
    BitmapEditorCommand( const Dictionary& parameters );
    virtual ~BitmapEditorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< BitmapEditorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< BitmapEditorCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute();

};


}


#endif
