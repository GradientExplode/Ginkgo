#ifndef _gkg_dmri_plugin_functors_DwiPostMortemBlockToWholeImageComposer_DwiPostMortemBlockToWholeImageComposerCommand_h_
#define _gkg_dmri_plugin_functors_DwiPostMortemBlockToWholeImageComposer_DwiPostMortemBlockToWholeImageComposerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/BoundingBox.h>

#include <string>
#include <vector>


namespace gkg
{


template < class T > class HomogeneousTransform3d;


class DwiPostMortemBlockToWholeImageComposerCommand :
              public Command,
              public Creator2Arg< DwiPostMortemBlockToWholeImageComposerCommand,
                                  Command,
                                  int32_t,
                                  char** >,
              public Creator1Arg< DwiPostMortemBlockToWholeImageComposerCommand,
                                  Command,
                                  const Dictionary& >
{

  public:

    DwiPostMortemBlockToWholeImageComposerCommand( int32_t argc,
                                                   char* argv[],
                                                   bool loadPlugin = false,
                                                   bool removeFirst = true );
    DwiPostMortemBlockToWholeImageComposerCommand(
           const std::vector< std::string >& fileNameT2Fovs,
           const std::vector< std::string >& fileNameOutputCompositionMask,
           const std::vector< std::string >& fileNameDWFovs,
           const std::vector< std::string >& fileNameBvalFovs,
           const std::vector< std::string >& fileNameBvecFovs,
           const std::vector< std::string >& fileNameFlippingFovs,
           const std::vector< std::string >& fileNameFovToReferenceTransform3ds,
           const std::vector< std::string >& fovToReferenceTransform3dTypes,
           const std::string& fileNameOutputT2Volume,
           const std::string& fileNameOutputDWVolume,
           const std::string& fileNameOutputBVal,
           const std::string& fileNameOutputBVec,
           const std::vector< double >& fovScalings,
           const Vector3d< int32_t >& referenceSizes,
           const Vector3d< double >& referenceResolutions,
           double deltaBValue,
           const std::vector< int32_t >& sphericalHarmonicOrders,
           const std::vector< double >& laplaceBeltramiRegularizationFactors,
           int32_t resamplingOrder,
           double background,
           const BoundingBox< int32_t >& outputBoundingBox,
           const std::string& format,
           bool verbose );
    DwiPostMortemBlockToWholeImageComposerCommand(
                                                 const Dictionary& parameters );
    virtual ~DwiPostMortemBlockToWholeImageComposerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiPostMortemBlockToWholeImageComposerCommand,
                              Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiPostMortemBlockToWholeImageComposerCommand,
                              Command,
                              const Dictionary& >;

    void parse();
    void execute(
           const std::vector< std::string >& fileNameT2Fovs,
           const std::vector< std::string >& fileNameOutputCompositionMask,
           const std::vector< std::string >& fileNameDWFovs,
           const std::vector< std::string >& fileNameBvalFovs,
           const std::vector< std::string >& fileNameBvecFovs,
           const std::vector< std::string >& fileNameFlippingFovs,
           const std::vector< std::string >& fileNameFovToReferenceTransform3ds,
           const std::vector< std::string >& fovToReferenceTransform3dTypes,
           const std::string& fileNameOutputT2Volume,
           const std::string& fileNameOutputDWVolume,
           const std::string& fileNameOutputBVal,
           const std::string& fileNameOutputBVec,
           const std::vector< double >& fovScalings,
           const Vector3d< int32_t >& referenceSizes,
           const Vector3d< double >& referenceResolutions,
           double deltaBValue,
           const std::vector< int32_t >& sphericalHarmonicOrders,
           const std::vector< double >& laplaceBeltramiRegularizationFactors,
           int32_t resamplingOrder,
           double background,
           const BoundingBox< int32_t >& outputBoundingBox,
           const std::string& format,
           bool verbose );

};


}


#endif
