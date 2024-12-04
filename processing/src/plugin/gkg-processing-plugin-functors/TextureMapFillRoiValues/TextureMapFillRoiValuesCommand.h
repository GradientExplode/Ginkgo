#ifndef _gkg_processing_plugin_functors_TextureMapFillRoiValues_TextureMapFillRoiValuesCommand_h_
#define _gkg_processing_plugin_functors_TextureMapFillRoiValues_TextureMapFillRoiValuesCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class TextureMapFillRoiValuesCommand :
                             public Command,
                             public Creator2Arg< TextureMapFillRoiValuesCommand,
                                         	 Command,
                                         	 int32_t,
                                         	 char** >,
                             public Creator1Arg< TextureMapFillRoiValuesCommand,
                                                 Command,
                                                 const Dictionary& >
{

  public:

    TextureMapFillRoiValuesCommand( int32_t argc,
                  		    char* argv[], 
                  		    bool loadPlugin = false,
                  		    bool removeFirst = true );
    TextureMapFillRoiValuesCommand( const std::string& fileNameInputTexture,
    				    const std::string& fileNameOutputTexture,
    				    const std::vector< double >& labels,
    				    const std::vector< double >& values,
    				    double background,
    				    bool ascii,
    				    const std::string& format,
    				    bool verbose );
    TextureMapFillRoiValuesCommand( const Dictionary& parameters );
    virtual ~TextureMapFillRoiValuesCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< TextureMapFillRoiValuesCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< TextureMapFillRoiValuesCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameInputTexture,
    		  const std::string& fileNameOutputTexture,
    		  const std::vector< double >& labels,
    		  const std::vector< double >& values,
    		  double background,
    		  bool ascii,
    		  const std::string& format,
    		  bool verbose );

};


}


#endif
