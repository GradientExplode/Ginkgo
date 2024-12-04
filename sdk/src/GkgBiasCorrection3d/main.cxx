#include <gkg-communication-getopt/Application.h>
#include <gkg-core-exception/Exception.h>
#include <string>
#include "BiasCorrection3dProcess.h"


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::string fileNameInputVolume;
    std::string fileNameOutputVolume;
    std::string fileNameCorrectionVolume;
    std::vector< int32_t > correctionStrideVector;
    int32_t histogramBinCount = 256;
    int32_t inputCompressionLevelCount = 64;
    double lowerThreshold = 10.0;
    int32_t gridCount = 2;
    int32_t gibbsSamplerIncrementCount = 2;
    double gibbsSamplerIncrementMagnitude = 1.03;
    double gibbsSamplerIncrementGeometry = 0.99;
    double entropyWeight = 1.0;
    double regularizationWeight = 20.0;
    double regularizationWeightGeometry = 0.25;
    double dataAttachmentWeight = 0.5;

    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( argc, argv,
                                  "3D bias correction" );
    application.addSingleOption( "-i",
                                 "Input volume file name",
                                 fileNameInputVolume );
    application.addSingleOption( "-o",
                                 "Output volume file name",
                                 fileNameOutputVolume );
    application.addSingleOption( "-c",
                                 "Correction field volume file name",
                                 fileNameCorrectionVolume,
                                 true );
    application.addSeriesOption( "-correctionStride",
                                 "Stride along x, y, and z direction(s)",
                                 correctionStrideVector,
                                 3, 3 );
    application.addSingleOption( "-histogramBinCount",
                                 "Histogram bin count (default=256)",
                                 histogramBinCount,
                                 true );
    application.addSingleOption( "-inputCompressionLevelCount",
                                 "Input volume compression level count "
                                 " (default=64)",
                                 inputCompressionLevelCount,
                                 true );
    application.addSingleOption( "-lowerThreshold",
                                 "Do not correct for voxels below a lower "
                                 "threshold (default=10)",
                                 lowerThreshold,
                                 true );
    application.addSingleOption( "-gridCount",
                                 "Multi-grid strategy: number of level steps"
                                 " (default=2)",
                                 gridCount,
                                 true );
    application.addSingleOption( "-gibbsSamplerIncrementCount",
                                 "Gibb's sampler increment count "
                                 " (default=2)",
                                 gibbsSamplerIncrementCount,
                                 true );
    application.addSingleOption( "-gibbsSamplerIncrementMagnitude",
                                 "Gibb's sampler increment magnitude "
                                 " (default=1.03)",
                                 gibbsSamplerIncrementMagnitude,
                                 true );
    application.addSingleOption( "-gibbsSamplerIncrementGeometry",
                                 "Gibb's sampler increment geometry factor "
                                 " (default=0.99)",
                                 gibbsSamplerIncrementGeometry,
                                 true );
    application.addSingleOption( "-regularizationEntropy",
                                 "Weight of the entropy energy term "
                                 " (default=1.0)",
                                 entropyWeight,
                                 true );
    application.addSingleOption( "-regularizationWeight",
                                 "Weight of the membrane regularization energy "
                                 "term (default=20.0)",
                                 regularizationWeight,
                                 true );
    application.addSingleOption( "-regularizationWeightGeometry",
                                 "Geometry factor of evolution of the "
                                 "regularization weight accord to grid level "
                                 "(default=0.25)",
                                 regularizationWeightGeometry,
                                 true );
    application.addSingleOption( "-regularizationDataAttachment",
                                 "Weight of the data attachment energy term "
                                 " (default=0.5)",
                                 dataAttachmentWeight,
                                 true );

    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();


    gkg::Vector3d< int32_t > correctionStride( correctionStrideVector[ 0 ],
                                               correctionStrideVector[ 1 ],
                                               correctionStrideVector[ 2 ] );
                                               

    gkg::BiasCorrection3dProcess
      biasCorrection3dProcess( fileNameOutputVolume,
                               fileNameCorrectionVolume,
                               correctionStride,
                               histogramBinCount,
                               inputCompressionLevelCount,
                               lowerThreshold,
                               gridCount,
                               gibbsSamplerIncrementCount,
                               gibbsSamplerIncrementMagnitude,
                               gibbsSamplerIncrementGeometry,
                               entropyWeight,
                               regularizationWeight,
                               regularizationWeightGeometry,
                               dataAttachmentWeight,
                               ascii,
                               format,
                               verbose );
    biasCorrection3dProcess.execute( fileNameInputVolume );
                                                          


  }
  GKG_CATCH_COMMAND( result );

  return result;


}

