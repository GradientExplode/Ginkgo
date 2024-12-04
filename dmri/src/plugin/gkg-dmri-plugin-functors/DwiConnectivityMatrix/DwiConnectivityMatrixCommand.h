#ifndef _gkg_dmri_plugin_functors_DwiConnectivityMatrix_DwiConnectivityMatrixCommand_h_
#define _gkg_dmri_plugin_functors_DwiConnectivityMatrix_DwiConnectivityMatrixCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>
#include <list>


namespace gkg
{


class DwiConnectivityMatrixCommand :
                               public Command,
                               public Creator2Arg< DwiConnectivityMatrixCommand,
                                                   Command,
                                                   int32_t,
                                                   char** >,
                               public Creator1Arg< DwiConnectivityMatrixCommand,
                                                   Command,
                                                   const Dictionary& >
{

  public:

    DwiConnectivityMatrixCommand( int32_t argc,
                                  char* argv[],
                                  bool loadPlugin = false,
                                  bool removeFirst = true );
    DwiConnectivityMatrixCommand(
                         const std::list< std::string >& fileNameBundleMaps,
                         const std::string& fileNameVolumeROIs1,
                         const std::string& fileNameMeshMapROIs1,
                         const std::string& fileNameTextureMapROIs1,
                         const std::string& fileNameROIs1ToBundleMapTransform3d,
                         const std::string& fileNameVolumeROIs2,
                         const std::string& fileNameMeshMapROIs2,
                         const std::string& fileNameTextureMapROIs2,
                         const std::string& fileNameROIs2ToBundleMapTransform3d,
                         const std::vector< double >& scalarParameters,
                         const std::vector< std::string >& stringParameters,
                         const std::list< std::string >& functorNames,
                         const std::list< std::string >& outputFileNames,
                         const std::string& outputBundleMapFormat,
                         const std::string& outputTextureMapFormat,
                         bool doNotCheckLabelTypes,
                         bool ascii,
                         bool verbose );
    DwiConnectivityMatrixCommand( const Dictionary& parameters );
    virtual ~DwiConnectivityMatrixCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiConnectivityMatrixCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiConnectivityMatrixCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::list< std::string >& fileNameBundleMaps,
                  const std::string& fileNameVolumeROIs1,
                  const std::string& fileNameMeshMapROIs1,
                  const std::string& fileNameTextureMapROIs1,
                  const std::string& fileNameROIs1ToBundleMapTransform3d,
                  const std::string& fileNameVolumeROIs2,
                  const std::string& fileNameMeshMapROIs2,
                  const std::string& fileNameTextureMapROIs2,
                  const std::string& fileNameROIs2ToBundleMapTransform3d,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  const std::list< std::string >& functorNames,
                  const std::list< std::string >& outputFileNames,
                  const std::string& outputBundleMapFormat,
                  const std::string& outputTextureMapFormat,
                  bool doNotCheckLabelTypes,
                  bool ascii,
                  bool verbose );

};


}


#endif
