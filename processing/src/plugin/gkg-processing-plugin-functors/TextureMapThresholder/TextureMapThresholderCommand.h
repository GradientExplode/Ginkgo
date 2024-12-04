#ifndef _gkg_processing_plugin_functors_TextureMapThresholder_TextureMapThresholderCommand_h_
#define _gkg_processing_plugin_functors_TextureMapThresholder_TextureMapThresholderCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class TextureMapThresholderCommand :
                               public Command,
                               public Creator2Arg< TextureMapThresholderCommand,
                                         	   Command, 
                                         	   int32_t, 
                                         	   char** > ,
                               public Creator1Arg< TextureMapThresholderCommand,
                                                   Command,
                                                   const Dictionary& >
{

  public:

    TextureMapThresholderCommand( int32_t argc,
                  		  char* argv[], 
                  		  bool loadPlugin = false,
                  		  bool removeFirst = true );
    TextureMapThresholderCommand( const std::string& fileNameInputTexture,
                                  const std::string& fileNameInputMesh,
                                  const std::string& fileNameOutputTexture,
                                  const std::string& fileNameOutputSurface,
                                  const std::string& mode,
                                  double threshold1,
                                  double threshold2,
                                  double background,
                                  bool ascii,
                                  const std::string& format,
                                  bool verbose );
    TextureMapThresholderCommand( const Dictionary& parameters );
    virtual ~TextureMapThresholderCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< TextureMapThresholderCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< TextureMapThresholderCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameInputTexture,
                  const std::string& fileNameInputMesh,
                  const std::string& fileNameOutputTexture,
                  const std::string& fileNameOutputSurface,
                  const std::string& mode,
                  double threshold1,
                  double threshold2,
                  double background,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
