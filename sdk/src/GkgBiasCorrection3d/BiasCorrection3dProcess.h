#ifndef _gkg_processing_bias_BiasCorrection3dProcess_h_
#define _gkg_processing_bias_BiasCorrection3dProcess_h_


#include <gkg-processing-process/Process.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


class BiasCorrection3dProcess : public Process
{


  public:

    BiasCorrection3dProcess( const std::string& theFileNameOutputVolume,
                             const std::string& theFileNameCorrectionVolume,
                             const Vector3d< int32_t >& theCorrectionStride,
                             const int32_t& theHistogramBinCount,
                             const int32_t& theInputCompressionLevelCount,
                             const double& theLowerThreshold,
                             const int32_t& theGridCount,
                             const int32_t& theGibbsSamplerIncrementCount,
                             const double& theGibbsSamplerIncrementMagnitude,
                             const double& theGibbsSamplerIncrementGeometry,
                             const double& theEntropyWeight,
                             const double& theRegularizationWeight,
                             const double& theRegularizationWeightGeometry,
                             const double& theDataAttachmentWeight,
                             const bool& theAscii,
                             const std::string& theFormat,
                             const bool& theVerbose );

    const std::string& fileNameOutputVolume;
    const std::string& fileNameCorrectionVolume;
    const Vector3d< int32_t >& correctionStride;
    const int32_t& histogramBinCount;
    const int32_t& inputCompressionLevelCount;
    const double& lowerThreshold;
    const int32_t& gridCount;
    const int32_t& gibbsSamplerIncrementCount;
    const double& gibbsSamplerIncrementMagnitude;
    const double& gibbsSamplerIncrementGeometry;
    const double& entropyWeight;
    const double& regularizationWeight;
    const double& regularizationWeightGeometry;
    const double& dataAttachmentWeight;
    const bool& ascii;
    const std::string& format;
    const bool& verbose;

  private:

    template < class T >
    static void correctBias3d( Process& process,
                               const std::string& fileNameInputVolume,
                               const AnalyzedObject&,
                               const std::string& );


    static void computeCorrectedVolume(
                                 const Volume< uint16_t >& compressedInputVolume,
                                 const Vector3d< int32_t >& inpuSize,
                                 const Volume< float >& correctionVolume,
                                 const Vector3d< int32_t >& correctionStride,
                                 const int32_t& inputCompressionLevelCount,
                                 const uint16_t& compressedLowerThreshold,
                                 Volume< uint16_t >& correctedVolume );
    static void computeEntropy( const Volume< uint16_t >& correctedVolume,
                                const Vector3d< int32_t >& inputSize,
                                const int32_t& inputCompressionLevelCount,
                                const uint16_t& compressedLowerThreshold,
                                float& entropy );
    static void computeCorrectionVolumeRegularity(
                                      const Volume< float >& correctionVolume,
                                      const Vector3d< int32_t >& correctionSize,
                                      float& correctionVolumeRegularity );
    static void computeDataAttachment(
                            const Volume< uint16_t >& compressedInputVolume,
                            const Volume< uint16_t >& correctedVolume,
                            float& discrepancyBetweenCorrectedAndInputVolumes );

};


}



#endif
