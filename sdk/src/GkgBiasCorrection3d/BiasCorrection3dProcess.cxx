#include "BiasCorrection3dProcess.h"
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-histogram/HistogramAnalyzer.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-morphology/Neighborhood3d.h>
#include <gkg-processing-algobase/AverageFilter_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-processing-algobase/Math.h>
#include <iostream>
#include <iomanip>


gkg::BiasCorrection3dProcess::BiasCorrection3dProcess(
                            const std::string& theFileNameOutputVolume,
                            const std::string& theFileNameCorrectionVolume,
                            const gkg::Vector3d< int32_t >& theCorrectionStride,
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
                            const bool& theVerbose )
                             : gkg::Process( "Volume" ),
                               fileNameOutputVolume( theFileNameOutputVolume ),
                               fileNameCorrectionVolume(
                                                  theFileNameCorrectionVolume ),
                               correctionStride( theCorrectionStride ),
                               histogramBinCount( theHistogramBinCount ),
                               inputCompressionLevelCount(
                                                theInputCompressionLevelCount ),
                               lowerThreshold( theLowerThreshold ),
                               gridCount( theGridCount ),
                               gibbsSamplerIncrementCount(
                                                theGibbsSamplerIncrementCount ),
                               gibbsSamplerIncrementMagnitude(
                                            theGibbsSamplerIncrementMagnitude ),
                               gibbsSamplerIncrementGeometry(
                                             theGibbsSamplerIncrementGeometry ),
                               entropyWeight( theEntropyWeight ),
                               regularizationWeight( theRegularizationWeight ),
                               regularizationWeightGeometry(
                                              theRegularizationWeightGeometry ),
                               dataAttachmentWeight( theDataAttachmentWeight ),
                               ascii( theAscii ),
                               format( theFormat ),
                               verbose( theVerbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &BiasCorrection3dProcess::correctBias3d< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &BiasCorrection3dProcess::correctBias3d< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &BiasCorrection3dProcess::correctBias3d< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &BiasCorrection3dProcess::correctBias3d< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &BiasCorrection3dProcess::correctBias3d< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &BiasCorrection3dProcess::correctBias3d< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &BiasCorrection3dProcess::correctBias3d< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &BiasCorrection3dProcess::correctBias3d< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &BiasCorrection3dProcess::correctBias3d< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &BiasCorrection3dProcess::correctBias3d< double > );

  }
  GKG_CATCH( "gkg::BiasCorrection3dProcess::BiasCorrection3dProcess( "
             "const std::string& theFileNameOutputVolume, "
             "const std::string& theFileNameBiasVolume )" );

}


template < class T >
void gkg::BiasCorrection3dProcess::correctBias3d( gkg::Process& process,
                                         const std::string& fileNameInputVolume,
                                         const gkg::AnalyzedObject&,
                                         const std::string& )
{

  try
  {


    gkg::BiasCorrection3dProcess&
      biasCorrection3dProcess = static_cast< gkg::BiasCorrection3dProcess& >(
                                                                      process );

    
    const std::string& fileNameOutputVolume =
                                   biasCorrection3dProcess.fileNameOutputVolume;
    const std::string& fileNameCorrectionVolume =
                               biasCorrection3dProcess.fileNameCorrectionVolume;
    const gkg::Vector3d< int32_t >& correctionStride =
                                       biasCorrection3dProcess.correctionStride;
    const int32_t& histogramBinCount =
                                      biasCorrection3dProcess.histogramBinCount;
    const int32_t& inputCompressionLevelCount =
                             biasCorrection3dProcess.inputCompressionLevelCount;
    const double& lowerThreshold = biasCorrection3dProcess.lowerThreshold;

    const int32_t& gridCount = biasCorrection3dProcess.gridCount;
    const int32_t& gibbsSamplerIncrementCount =
                             biasCorrection3dProcess.gibbsSamplerIncrementCount;
    const double& gibbsSamplerIncrementMagnitude =
                         biasCorrection3dProcess.gibbsSamplerIncrementMagnitude;
    const double& gibbsSamplerIncrementGeometry =
                          biasCorrection3dProcess.gibbsSamplerIncrementGeometry;
    const double& entropyWeight = biasCorrection3dProcess.entropyWeight;
    const double& regularizationWeight =
                                   biasCorrection3dProcess.regularizationWeight;
    const double& regularizationWeightGeometry =
                           biasCorrection3dProcess.regularizationWeightGeometry;
    const double& dataAttachmentWeight =
                                   biasCorrection3dProcess.dataAttachmentWeight;

    const bool& ascii = biasCorrection3dProcess.ascii;
    const std::string& format = biasCorrection3dProcess.format;
    const bool& verbose = biasCorrection3dProcess.verbose;



    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( ( correctionStride.x < 1 ) ||
         ( correctionStride.y < 1 ) ||
         ( correctionStride.y < 1 ) )
    {

      throw std::runtime_error( "some strides are below 1" );

    }
         
    if ( histogramBinCount <= 1 )
    {

      throw std::runtime_error( "histogram bin count must be > 1" );

    }

    if ( inputCompressionLevelCount < 16 )
    {

      throw std::runtime_error(
                        "input compression level count should be at least 16" );

    }

    if ( gridCount < 1 )
    {

      throw std::runtime_error( "grid count must be >= 1" );

    }


    if ( gibbsSamplerIncrementCount < 1 )
    {

      throw std::runtime_error(
                               "Gibb's samplier increment count must be >= 1" );

    }

    if ( gibbsSamplerIncrementMagnitude <= 1.0 )
    {

      throw std::runtime_error(
                          "Gibb's samplier increment magnitude must be > 1.0" );

    }

    if ( ( gibbsSamplerIncrementGeometry <= 0.0 ) ||
         ( gibbsSamplerIncrementGeometry > 1.0 ) )
    {

      throw std::runtime_error(
                "Gibb's samplier increment geometry must be within (0.0;1.0]" );

    }

    if ( entropyWeight <= 0 )
    {

      throw std::runtime_error( "entropy weight must be > 0.0" );

    }

    if ( regularizationWeight <= 0 )
    {

      throw std::runtime_error( "regularization weight must be > 0.0" );

    }

    if ( ( regularizationWeightGeometry <= 0.0 ) ||
         ( regularizationWeightGeometry > 1.0 ) )
    {

      throw std::runtime_error( "regularization weight geometry must be "
                                "within (0.0;1.0]" );

    }

    if ( dataAttachmentWeight <= 0 )
    {

      throw std::runtime_error( "data attachment weight must be > 0.0" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading the intput volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameInputVolume << "' : " << std::flush;


    }

    gkg::Volume< T > inputVolume;
    gkg::Reader::getInstance().read( fileNameInputVolume, inputVolume );

    std::cout << "managed to read volume of type "
              << gkg::TypeOf< T >::getName()
              << std::endl;

    gkg::Vector3d< int32_t > inputSize;
    int32_t inputSizeT = 0;
    inputVolume.getSize( inputSize, inputSizeT );

    gkg::Vector3d< double > inputResolution;
    inputVolume.getResolution( inputResolution );


    if ( verbose )
    {

      std::cout << "done" << std::endl;


    }

    ////////////////////////////////////////////////////////////////////////////
    // allocating the correction field
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "allocating the correction field : " << std::flush;

    }

    gkg::Vector3d< int32_t > correctionSize;
    correctionSize.x = inputSize.x / correctionStride.x +
                       ( ( inputSize.x % correctionStride.x ) ? 1 : 0 );
    correctionSize.y = inputSize.y / correctionStride.y +
                       ( ( inputSize.y % correctionStride.y ) ? 1 : 0 );
    correctionSize.z = inputSize.z / correctionStride.z +
                       ( ( inputSize.z % correctionStride.z ) ? 1 : 0 );


    gkg::Vector3d< double > correctionResolution;
    correctionResolution.x = inputResolution.x * correctionStride.x;
    correctionResolution.y = inputResolution.y * correctionStride.y;
    correctionResolution.z = inputResolution.z * correctionStride.z;


    gkg::Volume< float > correctionVolume( correctionSize );
    correctionVolume.setResolution( correctionResolution );

    if ( verbose )
    {

      std::cout << "done" << std::endl;


    }


    ////////////////////////////////////////////////////////////////////////////
    // looping over time(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< uint16_t > compressedInputVolume( inputSize );
    compressedInputVolume.setResolution( inputResolution );

    gkg::Volume< uint16_t > correctedVolume( inputSize );
    correctedVolume.setResolution( inputResolution );


    int32_t t = 0;
    for ( t = 0; t < inputSizeT; t++ )
    {


      if ( verbose )
      {

        std::cout << "correcting volume of index " << t << "..." << std::endl;

      }

      //////////////////////////////////////////////////////////////////////////
      // resetting the bias volume
      //////////////////////////////////////////////////////////////////////////

      correctionVolume.fill( 1.0f );

      
      //////////////////////////////////////////////////////////////////////////
      // computing the 95% maximum value of the histogram of the input volume
      //////////////////////////////////////////////////////////////////////////

      if ( verbose )
      {

        std::cout << "95% histogram boundaries : " << std::flush;

      }


      T inputLowerBoundary = ( T )0;
      T inputUpperBoundary = ( T )0;
      gkg::HistogramAnalyzer::getInstance().getPercentileUpperBoundaries(
                                                       inputVolume,
                                                       histogramBinCount,
                                                       95.0,
                                                       inputLowerBoundary,
                                                       inputUpperBoundary );

      if ( verbose )
      {

        std::cout << inputLowerBoundary
                  << " -> "
                  << inputUpperBoundary
                  << std::endl;

      }



      //////////////////////////////////////////////////////////////////////////
      // computing compressed lower threshold
      //////////////////////////////////////////////////////////////////////////

      if ( verbose )
      {

        std::cout << "compressed lower threshold : " << std::flush;

      }
      uint16_t compressedLowerThreshold = lowerThreshold;
      if ( lowerThreshold > ( double )inputUpperBoundary )
      {

        throw std::runtime_error(
                std::string( "you must choose a lower threshold between " ) +
                gkg::StringConverter::toString( ( double )inputLowerBoundary,
                                                'f', 7, 2 ) + " and " +
                gkg::StringConverter::toString( ( double )inputUpperBoundary,
                                                'f', 7, 2 ) );

      }
      else
      {

        compressedLowerThreshold =
               ( uint16_t )( ( lowerThreshold / ( double )inputUpperBoundary ) *
                             ( double )( inputCompressionLevelCount - 1 ) );

      }

      if ( verbose )
      {

        std::cout << lowerThreshold
                  << " -> "
                  << ( double )compressedLowerThreshold
                  << std::endl;

      }



      //////////////////////////////////////////////////////////////////////////
      // compressing input volume
      //////////////////////////////////////////////////////////////////////////

      if ( verbose )
      {

        std::cout << "compressing input volume : " << std::flush;

      }

      typename gkg::Volume< T >::const_iterator i = inputVolume.begin(),
                                                ie = inputVolume.end();
      gkg::Volume< uint16_t >::iterator ci = compressedInputVolume.begin();
      while ( i != ie )
      {


        if ( *i > inputUpperBoundary )
        {

          *ci = ( uint16_t )( inputCompressionLevelCount - 1 );

        }
        else
        {

          *ci = ( uint16_t )( ( ( double )*i / ( double )inputUpperBoundary ) *
                             ( double )( inputCompressionLevelCount - 1 ) );

        }
        ++ i;
        ++ ci;

      }



      gkg::Writer::getInstance().write( "/tmp/step1.ima",
                                        compressedInputVolume,
                                        ascii,
                                        format );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }



      //////////////////////////////////////////////////////////////////////////
      // looping over multigrid levels
      //////////////////////////////////////////////////////////////////////////

      if ( verbose )
      {

        std::cout << "looping over multi-grid levels : " << std::endl;

      }

      int32_t gridLevel = gridCount;
      gkg::Vector3d< int32_t > currentCorrectionStride( correctionStride );

      int32_t currentGibbsSamplerIncrementCount = gibbsSamplerIncrementCount;
      double currentGibbsSamplerIncrementMagnitude =
                                                 gibbsSamplerIncrementMagnitude;
      double currentRegularizationWeight = regularizationWeight;

      for ( gridLevel = gridCount; gridLevel > 0; gridLevel-- )
      {



        if ( verbose )
        {

          if ( gridLevel != gridCount )
          {

             std::cout << gkg::Eraser( 17 ) << std::flush;

          }
          std::cout << "  grid level " << std::setw( 1 ) << gridLevel << " : "
                    << std::flush;

        }

        ////////////////////////////////////////////////////////////////////////
        // allocating the current correction bias field
        ////////////////////////////////////////////////////////////////////////

        if ( verbose )
        {

          std::cout << "    - allocating the correction field : " << std::flush;

        }

        gkg::Vector3d< int32_t > currentCorrectionSize;
        currentCorrectionSize.x = inputSize.x / currentCorrectionStride.x +
                        ( ( inputSize.x % currentCorrectionStride.x ) ? 1 : 0 );
        currentCorrectionSize.y = inputSize.y / currentCorrectionStride.y +
                        ( ( inputSize.y % currentCorrectionStride.y ) ? 1 : 0 );
        currentCorrectionSize.z = inputSize.z / currentCorrectionStride.z +
                        ( ( inputSize.z % currentCorrectionStride.z ) ? 1 : 0 );


        gkg::Vector3d< double > currentCorrectionResolution;
        currentCorrectionResolution.x = inputResolution.x *
                                        currentCorrectionStride.x;
        currentCorrectionResolution.y = inputResolution.y *
                                        currentCorrectionStride.y;
        currentCorrectionResolution.z = inputResolution.z *
                                        currentCorrectionStride.z;

        gkg::Volume< float > currentCorrectionVolume( currentCorrectionSize );
        currentCorrectionVolume.setResolution( currentCorrectionResolution );

        if ( verbose )
        {

          std::cout << currentCorrectionSize.x << " x "
                    << currentCorrectionSize.y << " x "
                    << currentCorrectionSize.z << " at "
                    << currentCorrectionResolution.x << "mm x "
                    << currentCorrectionResolution.y << "mm x "
                    << currentCorrectionResolution.z << "mm"
                    << std::endl;

        }


        ////////////////////////////////////////////////////////////////////////
        // computing bias field for the current grid level
        ////////////////////////////////////////////////////////////////////////

        if ( verbose )
        {

          std::cout << "    - computing the histogram of compressed input "
                       "volume : "
                    << std::flush;

        }


        ////////////////////////////////////////////////////////////////////////
        // updating correction strides, Gibb's sampler parameters, weightings...
        ////////////////////////////////////////////////////////////////////////


        currentCorrectionStride.x = std::max( currentCorrectionStride.x / 2,
                                              1 );
        currentCorrectionStride.y = std::max( currentCorrectionStride.y / 2,
                                              1 );
        currentCorrectionStride.z = std::max( currentCorrectionStride.z / 2,
                                              1 );


        currentGibbsSamplerIncrementCount = std::max(
              ( int32_t )(  ( double )gibbsSamplerIncrementCount * 
                           ( ( double )( gridLevel - 1 )  /
                             ( double )gridCount ) + 0.5 ),
              1 );
        currentGibbsSamplerIncrementMagnitude =
                        1.0 + ( currentGibbsSamplerIncrementMagnitude - 1.0 ) * 
                        gibbsSamplerIncrementGeometry;

        currentRegularizationWeight *= regularizationWeightGeometry;

        if ( verbose )
        {

          std::cout << std::endl;

        }


      }




////////////////////////////////////////////////////////////////////////////////

      //////////////////////////////////////////////////////////////////////////
      // computing the corrected volume
      //////////////////////////////////////////////////////////////////////////

      computeCorrectedVolume( compressedInputVolume,
                              inputSize,
                              correctionVolume,
                              correctionStride,
                              inputCompressionLevelCount,
                              compressedLowerThreshold,
                              correctedVolume );


      //////////////////////////////////////////////////////////////////////////
      // computing the entropy of the corrected volume
      //////////////////////////////////////////////////////////////////////////

      float entropy = 0.0f;
      computeEntropy( correctedVolume,
                      inputSize,
                      inputCompressionLevelCount,
                      compressedLowerThreshold,
                      entropy );

      std::cout << "entropy = " << entropy << std::endl;


      //////////////////////////////////////////////////////////////////////////
      // computing the regularity of the correction field using membrane energy
      //////////////////////////////////////////////////////////////////////////

      float correctionVolumeRegularity = 0.0;
      computeCorrectionVolumeRegularity( correctionVolume,
                                         correctionSize,
                                         correctionVolumeRegularity );
      std::cout << "correctionVolumeRegularity = "
                << correctionVolumeRegularity << std::endl;


      //////////////////////////////////////////////////////////////////////////
      // computing the discrepancy between corrected and input compressed volume
      //////////////////////////////////////////////////////////////////////////

      float discrepancyBetweenCorrectedAndInputVolumes = 0.0f;
      computeDataAttachment( compressedInputVolume,
                             correctedVolume,
                             discrepancyBetweenCorrectedAndInputVolumes );
      std::cout << "discrepancyBetweenCorrectedAndInputVolumes = "
                << discrepancyBetweenCorrectedAndInputVolumes << std::endl;

////////////////////////////////////////////////////////////////////////////////


    }


  }
  GKG_CATCH( "template < class T > "
             "void gkg::BiasCorrection3dProcess::correctBias3d( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );






}



void gkg::BiasCorrection3dProcess::computeCorrectedVolume(
                           const gkg::Volume< uint16_t >& compressedInputVolume,
                           const gkg::Vector3d< int32_t >& inputSize,
                           const gkg::Volume< float >& correctionVolume,
                           const gkg::Vector3d< int32_t >& correctionStride,
                           const int32_t& inputCompressionLevelCount,
                           const uint16_t& compressedLowerThreshold,
                           gkg::Volume< uint16_t >& correctedVolume )
{

  try
  {



    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;

    float originalValue = 0.0f;
    float correctedValue = 0.0f;
    for ( z = 0; z < inputSize.z; z++ )
    {

      for ( y = 0; y < inputSize.y; y++ )
      {

        for ( x = 0; x < inputSize.x; x++ )
        {

          originalValue = ( float )compressedInputVolume( x, y, z );

          if ( originalValue > compressedLowerThreshold )
          {

            correctedValue = correctionVolume( x / correctionStride.x,
                                               y / correctionStride.y,
                                               z / correctionStride.z ) *
                             originalValue;
            if ( correctedValue >= ( float )( inputCompressionLevelCount - 1 ) )
            {

              correctedVolume( x, y, z ) =
                                 ( uint16_t )( inputCompressionLevelCount - 1 );

            }
            else
            {

              correctedVolume( x, y, z ) = ( uint16_t )correctedValue;

            }

          }
          else
          {

            correctedVolume( x, y, z ) = ( uint16_t )originalValue;

          }

        }

      }

    }
    gkg::Writer::getInstance().write( "/tmp/step2.ima",
                                      correctedVolume );



  }
  GKG_CATCH( "void gkg::BiasCorrection3dProcess::computeCorrectedVolume( "
             "const gkg::Volume< uint16_t >& compressedInputVolume, "
             "const gkg::Vector3d< int32_t >& inputSize, "
             "const gkg::Volume< float >& correctionVolume, "
             "const gkg::Vector3d< int32_t >& correctionStride, "
             "const int32_t& inputCompressionLevelCount, "
             "const uint16_t& compressedLowerThreshold, "
             "gkg::Volume< uint16_t >& correctedVolume )" );

}


void gkg::BiasCorrection3dProcess::computeEntropy(
                                 const gkg::Volume< uint16_t >& correctedVolume,
                                 const gkg::Vector3d< int32_t >& inputSize,
                                 const int32_t& inputCompressionLevelCount,
                                 const uint16_t& compressedLowerThreshold,
                                 float& entropy )
{

  try
  {

    std::vector< float > histogram( inputCompressionLevelCount, 0.0f );


    float normalizedCost = 1.0f / ( ( float )inputSize.x * 
                                    ( float )inputSize.y * 
                                    ( float )inputSize.z );

    gkg::Volume< uint16_t >::const_iterator
      v = correctedVolume.begin(),
      ve = correctedVolume.end();

    float count = 0.0;
    while ( v != ve )
    {

      if ( *v > compressedLowerThreshold )
      {

        histogram[ *v ] += 1.0f;
        count += 1.0f;

      }
      ++ v;

    }

    std::cout << count << "voxels " << std::endl;

    entropy = 0.0f;
    std::vector< float >::const_iterator h = histogram.begin(),
                                         he = histogram.end();
    while ( h != he )
    {

      if ( *h > 0.0f )
      {

        entropy -= ( *h / count ) * std::log( *h / count );

      }
      ++ h;

    }

    entropy /= ( float )std::log( 2.0 );

  }
  GKG_CATCH( "void gkg::BiasCorrection3dProcess::computeEntropy( "
             "const gkg::Volume< float >& correctedVolume, "
             "const gkg::Vector3d< int32_t >& inputSize, "
             "const int32_t& inputCompressionLevelCount, "
             "const uint8_t& compressedLowerThreshold, "
             "float& entropy )" );

}


void gkg::BiasCorrection3dProcess::computeCorrectionVolumeRegularity(
                                 const gkg::Volume< float >& correctionVolume,
                                 const gkg::Vector3d< int32_t >& correctionSize,
                                 float& correctionVolumeRegularity )
{

  try
  {

    gkg::BoundingBox< int32_t > boundingBox( correctionVolume );

    std::list< gkg::Vector3d< int32_t > >
      neighborOffsets =
         gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                                        gkg::Neighborhood3d::Neighborhood3d_6 );

    std::list< gkg::Vector3d< int32_t > >::const_iterator
      o = neighborOffsets.begin(),
      oe = neighborOffsets.end();

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    gkg::Vector3d< int32_t > neighbor;

    correctionVolumeRegularity = 0.0;

    double theLog = 0.0;

    for ( z = 0; z < correctionSize.z; z++ )
    {

      for ( y = 0; y < correctionSize.y; y++ )
      {

        for ( x = 0; x < correctionSize.x; x++ )
        {

          o = neighborOffsets.begin();
          while ( o != oe )
          {

            neighbor.x = x + o->x;
            neighbor.y = y + o->y;
            neighbor.z = z + o->z;

            if ( boundingBox.contains( neighbor ) )
            {

              theLog = std::log( correctionVolume( x, y, z ) /
                                 correctionVolume( neighbor ) );
              
              correctionVolumeRegularity += theLog * theLog;

            }
            ++ o;

          }

        }

      }

    }


  }
  GKG_CATCH( "void gkg::BiasCorrection3dProcess::"
             "computeCorrectionVolumeRegularity( "
             "const gkg::Volume< float >& correctionVolume, "
             "const gkg::Vector3d< int32_t >& correctionSize, "
             "float& correctionVolumeRegularity )" );

}


void gkg::BiasCorrection3dProcess::computeDataAttachment(
                           const gkg::Volume< uint16_t >& compressedInputVolume,
                           const gkg::Volume< uint16_t >& correctedVolume,
                           float& discrepancyBetweenCorrectedAndInputVolumes )
{

  try
  {

    gkg::AverageFilter< gkg::Volume< uint16_t >, float > averageFilter;

    float meanCompressedInputVolume = 0.0;
    averageFilter.filter( compressedInputVolume, meanCompressedInputVolume );

    float meanCorrectedVolume = 0.0;
    averageFilter.filter( correctedVolume, meanCorrectedVolume );

    float discrepancy = meanCorrectedVolume - meanCompressedInputVolume;

    discrepancyBetweenCorrectedAndInputVolumes = discrepancy * discrepancy;

  }
  GKG_CATCH( "void gkg::BiasCorrection3dProcess::computeDataAttachment( "
             "const gkg::Volume< uint16_t >& compressedInputVolume, "
             "const gkg::Vector3d< int32_t >& intpuSize, "
             "const gkg::Volume< uint16_t >& correctedVolume, "
             "float& discrepancyBetweenCorrectedAndInputVolumes )" );

}
