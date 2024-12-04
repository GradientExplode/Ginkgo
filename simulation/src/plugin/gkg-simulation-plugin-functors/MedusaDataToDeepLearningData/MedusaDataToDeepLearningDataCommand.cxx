#include <gkg-simulation-plugin-functors/MedusaDataToDeepLearningData/MedusaDataToDeepLearningDataCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-cppext/UncommentCounterInputFileStream.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-object/TypedObject_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-dmri-pdf-dti/Tensor.h>
#include <gkg-dmri-qspace-sampling/CustomSingleShellQSpaceSampling.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/DictionaryHelpers.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-io/MatrixItemIOFactory.h>
#include <gkg-processing-transform/Rotation3d.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-communication-thread/LoopContext.h>
#include <gkg-communication-thread/ThreadGauge.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <fstream>
#include <iomanip>


#define MRI_SEQUENCE_JSON_FILENAME  "mri-sequences.json"
#define GEOMETRY_JSON_FILENAME  "geometry.json"
#define MRI_OUTPUT_DIRECTORY_NAME "MRIOutput"

#define DTI_PARAMETER_COUNT  4

#define POPULATION_COUNT_OFFSET 0
#define POPULATION_GENERATIVE_PARAMETER_COUNT 6
#define AXON_DIAMETER_OFFSET 1
#define GLOBAL_ANGULAR_DISPERSION_OFFSET 2
#define VOLUME_FRACTION_OFFSET 3
#define MEAN_ORIENTATION_X_OFFSET 4
#define MEAN_ORIENTATION_Y_OFFSET 5
#define MEAN_ORIENTATION_Z_OFFSET 6


//
// class MedusaDataToDeepLearningDataGauge
//

class MedusaDataToDeepLearningDataGauge : public gkg::ThreadGauge< int64_t >
{

  public: 
  
    MedusaDataToDeepLearningDataGauge( int64_t maxCount ); 
    
    void add( int64_t value ); 
    void reset(); 
    
  private: 
  
    void display(); 
    
    int64_t _count; 
    int64_t _maxCount; 
    bool _alreadyDisplayed; 

};


MedusaDataToDeepLearningDataGauge::MedusaDataToDeepLearningDataGauge(
                                                              int64_t maxCount )
                                  : _count( 0 ),
                                    _maxCount( maxCount ),
                                    _alreadyDisplayed( false )
{
}


void MedusaDataToDeepLearningDataGauge::add( int64_t value )
{

  try
  {
   
    _count += value; 
    
    display();
   
  }
  GKG_CATCH( "MedusaDataToDeepLearningDataGauge::add( int64_t value )" );
  
}


void MedusaDataToDeepLearningDataGauge::reset()
{

  try
  {
   
    _count = 0; 
   
  }
  GKG_CATCH( "MedusaDataToDeepLearningDataGauge::reset()" );
  
}


void MedusaDataToDeepLearningDataGauge::display()
{

  try
  {
    
    if ( _alreadyDisplayed )
    {
    
      std::cout << gkg::Eraser( 27 );
    
    }
    
    std::cout << "[ "
              << std::setw( 10 ) << _count
              << " / "
              << std::setw( 10 ) << _maxCount
              << " ]" 
              << std::flush;

    _alreadyDisplayed = true; 
  
  }
  GKG_CATCH( "void MedusaDataToDeepLearningDataGauge::display()" ); 

}


//
// class MedusaDataToDeepLearningDataContext
//


class MedusaDataToDeepLearningDataContext : public gkg::LoopContext< int64_t >
{

  public:

    MedusaDataToDeepLearningDataContext(
      MedusaDataToDeepLearningDataGauge& gauge,
      gkg::Matrix& deepLearningMatrix,
      const std::vector< gkg::RCPointer< gkg::Volume< float > > >& t2Volumes,
      const std::vector< gkg::RCPointer< gkg::Volume< float > > >& dwVolumes,
      const std::vector< std::vector< float > >& bValueSets,
      const std::vector< gkg::OrientationSet >& orientationSets,
      const std::vector< int64_t >& populationCounts,
      const std::vector< std::vector< gkg::Vector3d< float > > >&
                                                              meanOrientations,
      const std::vector< std::vector< float > >& axonMeanDiameters,
      const std::vector< std::vector< float > >& globalAngularDispersions,
      const std::vector< std::vector< float > >& volumeFractions,
      const int64_t& rotationCount,
      const double& noiseStandardDeviation,
      const int64_t& maximumPopulationCount,
      const int64_t& outputGenerativeParameterCount,
      const int64_t& outputColumnCount,
      const int64_t& mriSequenceCount,
      const int64_t& sphericalHarmonicsCoefficientCount,
      const double& regularizationLCurveFactor,
      bool verbose );

    void doIt( int64_t startIndex, int64_t count );

  protected:

    gkg::Matrix& _deepLearningMatrix;
    const std::vector< gkg::RCPointer< gkg::Volume< float > > >& _t2Volumes;
    const std::vector< gkg::RCPointer< gkg::Volume< float > > >& _dwVolumes;
    const std::vector< std::vector< float > >& _bValueSets;
    const std::vector< gkg::OrientationSet >& _orientationSets;

    // structure(s) corresponding to the generative parameters of interest
    const std::vector< int64_t >& _populationCounts;
    const std::vector< std::vector< gkg::Vector3d< float > > >& 
                                                              _meanOrientations;
    const std::vector< std::vector< float > >& _axonMeanDiameters;
    const std::vector< std::vector< float > >& _globalAngularDispersions;
    const std::vector< std::vector< float > >& _volumeFractions;

    const int64_t& _rotationCount;
    const double& _noiseStandardDeviation;
    const int64_t& _maximumPopulationCount;
    const int64_t& _outputGenerativeParameterCount;
    const int64_t& _outputColumnCount;
    const int64_t& _mriSequenceCount;
    const int64_t& _sphericalHarmonicsCoefficientCount;
    const double& _regularizationLCurveFactor;
    bool _verbose;

};


MedusaDataToDeepLearningDataContext::MedusaDataToDeepLearningDataContext( 
       MedusaDataToDeepLearningDataGauge& gauge,
       gkg::Matrix& deepLearningMatrix,
       const std::vector< gkg::RCPointer< gkg::Volume< float > > >& t2Volumes,
       const std::vector< gkg::RCPointer< gkg::Volume< float > > >& dwVolumes,
       const std::vector< std::vector< float > >& bValueSets,
       const std::vector< gkg::OrientationSet >& orientationSets,
       const std::vector< int64_t >& populationCounts,
       const std::vector< std::vector< gkg::Vector3d< float > > >&
                                                               meanOrientations,
       const std::vector< std::vector< float > >& axonMeanDiameters,
       const std::vector< std::vector< float > >& globalAngularDispersions,
       const std::vector< std::vector< float > >& volumeFractions,
       const int64_t& rotationCount,
       const double& noiseStandardDeviation,
       const int64_t& maximumPopulationCount,
       const int64_t& outputGenerativeParameterCount,
       const int64_t& outputColumnCount,
       const int64_t& mriSequenceCount,
       const int64_t& sphericalHarmonicsCoefficientCount,
       const double& regularizationLCurveFactor,
       bool verbose )
                                    : gkg::LoopContext< int64_t >( &gauge ),
                                      _deepLearningMatrix( deepLearningMatrix ),
                                      _t2Volumes( t2Volumes ),
                                      _dwVolumes( dwVolumes ),
                                      _bValueSets( bValueSets ),
                                      _orientationSets( orientationSets ),
                                      _populationCounts( populationCounts ),
                                      _meanOrientations( meanOrientations ),
                                      _axonMeanDiameters( axonMeanDiameters ),
                                      _globalAngularDispersions(
                                                     globalAngularDispersions ),
                                      _volumeFractions( volumeFractions ),
                                      _rotationCount( rotationCount ),
                                      _noiseStandardDeviation(
                                                       noiseStandardDeviation ),
                                      _maximumPopulationCount(
                                                       maximumPopulationCount ),
                                      _outputGenerativeParameterCount(
                                               outputGenerativeParameterCount ),
                                      _outputColumnCount( outputColumnCount ),
                                      _mriSequenceCount( mriSequenceCount ),
                                      _sphericalHarmonicsCoefficientCount(
                                           sphericalHarmonicsCoefficientCount ),
                                      _regularizationLCurveFactor(
                                                   regularizationLCurveFactor ),
                                      _verbose( verbose )
{
}


void MedusaDataToDeepLearningDataContext::doIt( int64_t startIndex,
                                                int64_t count )
{

  try
  {


    // getting pointer to the numerical analysis implementation factory
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    // allocating random generator and resetting random generator seed with time
    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );
    randomGenerator.setSeedWithTime();


    // allocating the target output matrix used for deep learning
    int64_t outputSampleCount = count * _rotationCount;
    gkg::Matrix localDeepLearningMatrix( outputSampleCount,
                                         _outputColumnCount );

    ///////////////////////////////////////////////////////////////////////////
    // looping over input samples to generate noisy and rotated samples
    ///////////////////////////////////////////////////////////////////////////

    int64_t inputSampleIndex = 0;
    int64_t globalInputSampleIndex = 0;
    int64_t rotationIndex = 0;
    int64_t outputSampleIndex = 0;
    double realNoise = 0.0;
    double imagNoise = 0.0;
    gkg::Vector3d< float > rotatedMeanOrientation;
    gkg::Vector3d< int32_t > nulVector3d( 0, 0, 0 );
    int64_t mriSequenceIndex = 0;
    for ( inputSampleIndex = 0, globalInputSampleIndex = startIndex ; 
          inputSampleIndex < count;
          inputSampleIndex++, globalInputSampleIndex++ )
    {

      if ( _verbose )
      {

        lock();
        gaugeAdd( _rotationCount );
        unlock();

      }

      // looping over rotation(s)
      for ( rotationIndex = 0; rotationIndex < _rotationCount; rotationIndex++ )
      {

        ////////////////////////////////////////////////////////////////////////
        // computing the output sampling index
        ////////////////////////////////////////////////////////////////////////

        outputSampleIndex = inputSampleIndex * _rotationCount + rotationIndex;


        ////////////////////////////////////////////////////////////////////////
        // computing a random rotation (the same for all the sequence(s))
        ////////////////////////////////////////////////////////////////////////

        gkg::Vector3d< float > 
          rotationAngles( 
                    ( float )factory->getUniformRandomNumber( randomGenerator,
                                                              0.0, 2 * M_PI ),
                    ( float )factory->getUniformRandomNumber( randomGenerator,
                                                              0.0, 2 * M_PI ),
                    ( float )factory->getUniformRandomNumber( randomGenerator,
                                                              0.0, 2 * M_PI ) );
        gkg::Rotation3d< float > rotation3d( rotationAngles );


        ////////////////////////////////////////////////////////////////////////
        // first, filling the generative parameters (first columns of matrix)
        ////////////////////////////////////////////////////////////////////////

        localDeepLearningMatrix( outputSampleIndex, 
                                 POPULATION_COUNT_OFFSET ) =
                          ( double )_populationCounts[ globalInputSampleIndex ];

        int32_t populationIndex = 0;
        for ( populationIndex = 0;
              populationIndex < _populationCounts[ globalInputSampleIndex ];
              populationIndex++ )
        {

          localDeepLearningMatrix(
            outputSampleIndex, 
            populationIndex * POPULATION_GENERATIVE_PARAMETER_COUNT +
            AXON_DIAMETER_OFFSET ) =
            ( double )_axonMeanDiameters[ globalInputSampleIndex ]
                                        [ populationIndex ];
          localDeepLearningMatrix(
            outputSampleIndex, 
            populationIndex * POPULATION_GENERATIVE_PARAMETER_COUNT +
            GLOBAL_ANGULAR_DISPERSION_OFFSET ) =
            ( double )_globalAngularDispersions[ globalInputSampleIndex ]
                                               [ populationIndex ];
          localDeepLearningMatrix(
            outputSampleIndex, 
            populationIndex * POPULATION_GENERATIVE_PARAMETER_COUNT +
            VOLUME_FRACTION_OFFSET ) =
            ( double )_volumeFractions[ globalInputSampleIndex ]
                                      [ populationIndex ];

          rotation3d.getDirect( _meanOrientations[ globalInputSampleIndex ]
                                                 [ populationIndex ],
                                rotatedMeanOrientation );

          localDeepLearningMatrix(
            outputSampleIndex, 
            populationIndex * POPULATION_GENERATIVE_PARAMETER_COUNT +
            MEAN_ORIENTATION_X_OFFSET ) =
            ( double )rotatedMeanOrientation.x;
          localDeepLearningMatrix(
            outputSampleIndex, 
            populationIndex * POPULATION_GENERATIVE_PARAMETER_COUNT +
            MEAN_ORIENTATION_Y_OFFSET ) =
            ( double )rotatedMeanOrientation.y;
          localDeepLearningMatrix(
            outputSampleIndex, 
            populationIndex * POPULATION_GENERATIVE_PARAMETER_COUNT +
            MEAN_ORIENTATION_Z_OFFSET ) =
            ( double )rotatedMeanOrientation.z;
                             

        }

        // filling the further population columns with 0
        for ( populationIndex = _populationCounts[ globalInputSampleIndex ];
              populationIndex < _maximumPopulationCount;
              populationIndex++ )
        {

          localDeepLearningMatrix(
            outputSampleIndex, 
            populationIndex * POPULATION_GENERATIVE_PARAMETER_COUNT +
            AXON_DIAMETER_OFFSET ) = 0.0;
          localDeepLearningMatrix(
            outputSampleIndex, 
            populationIndex * POPULATION_GENERATIVE_PARAMETER_COUNT +
            GLOBAL_ANGULAR_DISPERSION_OFFSET ) = 0.0;
          localDeepLearningMatrix(
            outputSampleIndex, 
            populationIndex * POPULATION_GENERATIVE_PARAMETER_COUNT +
            VOLUME_FRACTION_OFFSET ) = 0.0;
          localDeepLearningMatrix(
            outputSampleIndex, 
            populationIndex * POPULATION_GENERATIVE_PARAMETER_COUNT +
            MEAN_ORIENTATION_X_OFFSET ) = 0.0;
          localDeepLearningMatrix(
            outputSampleIndex, 
            populationIndex * POPULATION_GENERATIVE_PARAMETER_COUNT +
            MEAN_ORIENTATION_Y_OFFSET ) = 0.0;
          localDeepLearningMatrix(
            outputSampleIndex, 
            populationIndex * POPULATION_GENERATIVE_PARAMETER_COUNT +
            MEAN_ORIENTATION_Z_OFFSET ) = 0.0;

        }


        ////////////////////////////////////////////////////////////////////////
        // second, filling the diffusion MRI parameters (last columns of matrix)
        ////////////////////////////////////////////////////////////////////////

        int64_t outputParameterOffset = _outputGenerativeParameterCount;

        int64_t volumeIndex = 0;
        for ( mriSequenceIndex = 0;
              mriSequenceIndex < _mriSequenceCount;
              mriSequenceIndex++ )
        {

          //////////////////////////////////////////////////////////////////////
          // computing the volume index in the global volume structure(s)
          //////////////////////////////////////////////////////////////////////

          volumeIndex = globalInputSampleIndex * _mriSequenceCount +
                        mriSequenceIndex;


          //////////////////////////////////////////////////////////////////////
          // collecting references to diffusion-weighted volumes, mask,
          // orientation and b-value set(s)
          //////////////////////////////////////////////////////////////////////

          const gkg::RCPointer< gkg::Volume< float > >&
            t2Volume = _t2Volumes[ volumeIndex ];
          const gkg::RCPointer< gkg::Volume< float > >&
            dwVolume = _dwVolumes[ volumeIndex ];
          const gkg::OrientationSet& 
            orientationSet = _orientationSets[ volumeIndex ];
          const std::vector< float >& 
            bValues = _bValueSets[ volumeIndex ];


          //////////////////////////////////////////////////////////////////////
          // creating the q-space information
          //////////////////////////////////////////////////////////////////////

          gkg::Dictionary gradientCharacteristics;
          gradientCharacteristics[ "type" ] = std::string( "unknown" );

          gkg::CustomSingleShellQSpaceSampling
            qSpaceSampling( bValues,
                            orientationSet.getOrientations(),
                            rotation3d.getComposition(),
                            gradientCharacteristics );


          //////////////////////////////////////////////////////////////////////
          // creating a noise-corrupted DW volume
          //////////////////////////////////////////////////////////////////////

          gkg::Volume< float > noiseCorruptedDwVolume = *dwVolume;
          gkg::Volume< float >::iterator
            v = noiseCorruptedDwVolume.begin(),
            ve = noiseCorruptedDwVolume.end();
          while ( v != ve )
          {

            realNoise = factory->getUniformRandomNumber(
                                       randomGenerator,
                                       0.0, ( double )_noiseStandardDeviation );
            imagNoise = factory->getUniformRandomNumber(
                                       randomGenerator,
                                       0.0, ( double )_noiseStandardDeviation );
            *v = ( float )std::sqrt( ( ( double )*v + realNoise ) *
                                     ( ( double )*v + realNoise ) +
                                     ( imagNoise * imagNoise ) );
            ++ v;

          }


          //////////////////////////////////////////////////////////////////////
          // adding the q-space information to the noise-corrupted DW volume
          //////////////////////////////////////////////////////////////////////

          qSpaceSampling.addInformationToHeader( noiseCorruptedDwVolume );


          //////////////////////////////////////////////////////////////////////
          // computing the associate tensor feature(s)
          //////////////////////////////////////////////////////////////////////

          gkg::Vector3d< int32_t > cartesianGridSize;
          gkg::Vector3d< double > cartesianGridResolution;
          noiseCorruptedDwVolume.getSize( cartesianGridSize );
          noiseCorruptedDwVolume.getResolution( cartesianGridResolution );

          gkg::Tensor tensor( &cartesianGridSize,
                              &cartesianGridResolution,
                              *t2Volume,
                              noiseCorruptedDwVolume,
                              nulVector3d,
                              gkg::Tensor::RobustPositiveDefinite );

          // adding ADC, FA, LambdaParallel and LambdaTransverse to the
          // deep learning matrix
          localDeepLearningMatrix( outputSampleIndex, outputParameterOffset ) =
            ( double )tensor.getApparentDiffusionCoefficient();
          ++ outputParameterOffset;

          localDeepLearningMatrix( outputSampleIndex, outputParameterOffset ) =
            ( double )tensor.getFractionalAnisotropy();
          ++ outputParameterOffset;

          localDeepLearningMatrix( outputSampleIndex, outputParameterOffset ) =
            ( double )tensor.getLambdaParallel();
          ++ outputParameterOffset;

          localDeepLearningMatrix( outputSampleIndex, outputParameterOffset ) =
            ( double )tensor.getLambdaTransverse();
          ++ outputParameterOffset;


          //////////////////////////////////////////////////////////////////////
          // computing the associate SH decomposition
          //////////////////////////////////////////////////////////////////////

          // computing the normalized signal
          int32_t orientationCount = orientationSet.getCount();
          gkg::Vector normalizedSignal( orientationCount );
          int32_t orientationIndex = 0;
          for ( orientationIndex = 0; orientationIndex < orientationCount;
                orientationIndex++ )
          {

            normalizedSignal( orientationIndex ) = 
                            noiseCorruptedDwVolume( 0, 0, 0, orientationIndex );

          }

          // computing the SH basis matrix
          gkg::Matrix B( orientationCount,
                         _sphericalHarmonicsCoefficientCount );
          gkg::Vector3d< float > orientation;
          double radial = 0.0;
          double theta = 0.0;
          double phi = 0.0;
          int32_t sphericalHarmonicsCoefficientIndex = 0;
          for ( orientationIndex = 0;
                orientationIndex < orientationCount; orientationIndex++ )
          {

            rotation3d.getDirect( orientationSet.getOrientation(
                                                             orientationIndex ),
                                  orientation );
            factory->getCartesianToSphericalCoordinates(
                                                        ( double )orientation.x,
                                                        ( double )orientation.y,
                                                        ( double )orientation.z,
                                                        radial,
                                                        theta,
                                                        phi );
            for ( sphericalHarmonicsCoefficientIndex = 0;
                  sphericalHarmonicsCoefficientIndex < 
                  _sphericalHarmonicsCoefficientCount; 
                  sphericalHarmonicsCoefficientIndex++ )
            {

              B( orientationIndex, sphericalHarmonicsCoefficientIndex ) =
                 factory->getSymmetricalSphericalHarmonic( 
                                             sphericalHarmonicsCoefficientIndex,
                                             phi,
                                             theta );

            }

          }

          // building Laplace-Beltrami smoothness matrix for regularization
          gkg::Matrix L( _sphericalHarmonicsCoefficientCount,
                         _sphericalHarmonicsCoefficientCount );
          L.setZero();
          double sphericalLaplacian = 0;
          int32_t l = 0;
          int32_t m = 0;
          for ( sphericalHarmonicsCoefficientIndex = 0;
                sphericalHarmonicsCoefficientIndex <
                _sphericalHarmonicsCoefficientCount; 
                sphericalHarmonicsCoefficientIndex++ )
          {

            factory->getSymmetricalSphericalHarmonicLM( 
              sphericalHarmonicsCoefficientIndex, l, m );
            sphericalLaplacian = - l * ( l + 1 );
            L( sphericalHarmonicsCoefficientIndex,
               sphericalHarmonicsCoefficientIndex ) = sphericalLaplacian *
                                                      sphericalLaplacian;

          }

          // computing the SH decomposition matrix
          gkg::Matrix Bplus( _sphericalHarmonicsCoefficientCount,
                             orientationCount );
          factory->getThikonovPseudoInverse( B, L,
                                             _regularizationLCurveFactor,
                                             Bplus );
          // computing the SH decomposition of the normalized signal
          gkg::Vector 
            sphericalHarmonicsDecomposition = Bplus.getComposition(
                                                             normalizedSignal );

          // storing the decomposition to the output deep learning matrix
          for ( sphericalHarmonicsCoefficientIndex = 0;
                sphericalHarmonicsCoefficientIndex < 
                _sphericalHarmonicsCoefficientCount;
                sphericalHarmonicsCoefficientIndex++ )
          {

            localDeepLearningMatrix( outputSampleIndex,
                                     outputParameterOffset ) =
              sphericalHarmonicsDecomposition(
                                           sphericalHarmonicsCoefficientIndex );
            ++ outputParameterOffset;

          }

        }

      }

    }

    // copying local deep learning matrix to global deep learning matrix
    
    lock();

    inputSampleIndex = 0;
    int64_t outputColumnIndex = 0;
    int64_t outputSampleOffset = startIndex * _rotationCount;
    for ( outputSampleIndex = 0; 
          outputSampleIndex < outputSampleCount;
          outputSampleIndex++ )
    {

      for ( outputColumnIndex = 0; 
            outputColumnIndex < _outputColumnCount;
            outputColumnIndex++ )
      {

        _deepLearningMatrix( outputSampleOffset + outputSampleIndex,
                             outputColumnIndex ) =
          localDeepLearningMatrix( outputSampleIndex, outputColumnIndex );

      }

    }

    unlock();

  }
  GKG_CATCH( "void MedusaDataToDeepLearningDataContext::doIt( "
             "int64_t startIndex, "
             "int64_t count )" );

}


//
// class MedusaDataToDeepLearningDataCommand
//

gkg::MedusaDataToDeepLearningDataCommand::MedusaDataToDeepLearningDataCommand(
                                                              int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                         : gkg::Command( argc, argv,
                                                         loadPlugin,
                                                         removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::MedusaDataToDeepLearningDataCommand::"
             "MedusaDataToDeepLearningDataCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}

gkg::MedusaDataToDeepLearningDataCommand::MedusaDataToDeepLearningDataCommand(
                       const std::string& medusaSimulationCampaignDirectoryName,
                       const std::string& outputDeepLearningMatrixFileName,
                       int64_t rotationCount,
                       float noiseStandardDeviation,
                       int64_t sphericalHarmonicsOrder,
                       double regularizationLCurveFactor,
                       bool csv,
                       bool verbose )
{

  try
  {

    execute( medusaSimulationCampaignDirectoryName,
             outputDeepLearningMatrixFileName,
             rotationCount,
             noiseStandardDeviation,
             sphericalHarmonicsOrder,
             regularizationLCurveFactor,
             csv,
             verbose );

  }
  GKG_CATCH( "gkg::MedusaDataToDeepLearningDataCommand::"
             "MedusaDataToDeepLearningDataCommand( "
             "const std::string& medusaSimulationCampaignDirectoryName, "
             "const std::string& outputDeepLearningMatrixFileName, "
             "int64_t rotationCount, "
             "float noiseStandardDeviation, "
             "int64_t sphericalHarmonicsOrder, "
             "double regularizationLCurveFactor, "
             "bool csv, "
             "bool verbose )" );

}


gkg::MedusaDataToDeepLearningDataCommand::MedusaDataToDeepLearningDataCommand(
                                             const gkg::Dictionary& parameters )
                                         : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string,
                              medusaSimulationCampaignDirectoryName );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              outputDeepLearningMatrixFileName );
    DECLARE_INTEGER_PARAMETER( parameters, int64_t, rotationCount );
    DECLARE_FLOATING_PARAMETER( parameters, float, noiseStandardDeviation );
    DECLARE_INTEGER_PARAMETER( parameters, int64_t, sphericalHarmonicsOrder );
    DECLARE_FLOATING_PARAMETER( parameters, double,
                                regularizationLCurveFactor );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, csv );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( medusaSimulationCampaignDirectoryName,
             outputDeepLearningMatrixFileName,
             rotationCount,
             noiseStandardDeviation,
             sphericalHarmonicsOrder,
             regularizationLCurveFactor,
             csv,
             verbose );

  }
  GKG_CATCH( "gkg::MedusaDataToDeepLearningDataCommand::"
             "MedusaDataToDeepLearningDataCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::MedusaDataToDeepLearningDataCommand::~MedusaDataToDeepLearningDataCommand()
{
}


std::string gkg::MedusaDataToDeepLearningDataCommand::getStaticName()
{

  try
  {

    return "MedusaDataToDeepLearningData";

  }
  GKG_CATCH( "std::string "
             "gkg::MedusaDataToDeepLearningDataCommand::"
             "getStaticName()" );

}


void gkg::MedusaDataToDeepLearningDataCommand::parse()
{

  try
  {

    std::string medusaSimulationCampaignDirectoryName;
    std::string outputDeepLearningMatrixFileName;
    int64_t rotationCount = 0;
    float noiseStandardDeviation = 0.0f;
    int64_t sphericalHarmonicsOrder = 4;
    double regularizationLCurveFactor = 0.006;
    bool csv = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Tool to generate a learning database from "
                                  "a MEDUSA simulation campaign",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input MEDUSA simulation campaign "
                                  "directory name ",
                                  medusaSimulationCampaignDirectoryName );
    application.addSingleOption( "-o",
                                 "Output deep learning matrix filename",
                                  outputDeepLearningMatrixFileName );
    application.addSingleOption( "-r",
                                 "Rotation count",
                                 rotationCount );
    application.addSingleOption( "-n",
                                 "Noise standard deviation betwen 0.0 and 1.0",
                                 noiseStandardDeviation );
    application.addSingleOption( "-sh",
                                 "Spherical harmonics order (defaut=4)",
                                 sphericalHarmonicsOrder,
                                 true );
    application.addSingleOption( "-lb",
                                 "Laplace-Beltrami regularization L-curve "
                                 "factor (defaut=0.006)",
                                 regularizationLCurveFactor,
                                 true );
    application.addSingleOption( "-csv",
                                 "Use CSV format for output matrix instead "
                                 "binary matrix format",
                                 csv, 
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( medusaSimulationCampaignDirectoryName,
             outputDeepLearningMatrixFileName,
             rotationCount,
             noiseStandardDeviation,
             sphericalHarmonicsOrder,
             regularizationLCurveFactor,
             csv,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void "
                     "gkg::MedusaDataToDeepLearningDataCommand::"
                     "parse()" );

}


void gkg::MedusaDataToDeepLearningDataCommand::execute(
                       const std::string& medusaSimulationCampaignDirectoryName,
                       const std::string& outputDeepLearningMatrixFileName,
                       int64_t rotationCount,
                       float noiseStandardDeviation,
                       int64_t sphericalHarmonicsOrder,
                       double regularizationLCurveFactor,
                       bool csv,
                       bool verbose )
{
 
  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // exploring the simulation campaign directory content
    ////////////////////////////////////////////////////////////////////////////

    if (verbose)
    {

      std::cout << "exploring simulation campaign directory : " << std::endl;

    }
    gkg::Directory
      simulationCampaignDirectory( medusaSimulationCampaignDirectoryName );

    if ( !simulationCampaignDirectory.isValid() )
    {

      throw std::runtime_error( "not a valid simulation campaign directory!" );

    }

    // collecting sub-directory names
    std::set< std::string > unwantedSubDirectoryNames;
    unwantedSubDirectoryNames.insert( "." );
    unwantedSubDirectoryNames.insert( ".." );
    std::set< std::string >
      subDirectoryNames = simulationCampaignDirectory.getSubDirectories(
                                                   &unwantedSubDirectoryNames );

    // collecting the input sample count
    int64_t inputSampleCount = ( int64_t )subDirectoryNames.size();

    // computing the target output rotated sample count
    int64_t outputSampleCount = inputSampleCount * rotationCount;

    if ( verbose )
    {

      std::cout << " - input sample count : " << inputSampleCount << std::endl;
      std::cout << " - rotation count : " << rotationCount << std::endl;
      std::cout << " - output sample count : " << outputSampleCount
                << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // collecting the common MRI sequence information from the first sample
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "collecting the common MRI sequence information from the "
                << "first sample : "
                << std::endl;

    }

    // opening and reading the MRI dictionray from the first sample
    std::string 
      commonMRISequenceJsonFileName = medusaSimulationCampaignDirectoryName +
                                      gkg::getDirectorySeparator() +
                                      *subDirectoryNames.begin() +
                                      gkg::getDirectorySeparator() +
                                      MRI_SEQUENCE_JSON_FILENAME;
    gkg::UncommentCounterInputFileStream 
      mriUcifs( commonMRISequenceJsonFileName.c_str() );

    if ( !mriUcifs )
    {

      throw std::runtime_error( "could not open MRI sequence JSON file" );

    }
    gkg::BaseObjectReader mriBaseObjectReader;
    gkg::TypedObject< gkg::Dictionary > typedMriDictionary;
    mriBaseObjectReader.read( mriUcifs, typedMriDictionary );
    mriUcifs.close();
    const gkg::Dictionary& mriDictionary = typedMriDictionary.getTypedValue();

    // collecting the sequence name(s)
    std::list< std::string > mriSequenceNames;
    gkg::Dictionary::const_iterator
      msn  = mriDictionary.begin(),
      msne = mriDictionary.end();
      
    int64_t mriSequenceCount = 0;
    while ( msn != msne )
    {

      mriSequenceNames.push_back( msn->first );
      ++ mriSequenceCount;
      ++ msn;

    }

    if ( verbose )
    {

      std::cout << " - sequence name(s) : " << std::flush;
      std::list< std::string >::const_iterator
        n = mriSequenceNames.begin(),
        ne = mriSequenceNames.end();
      while ( n != ne )
      {

        std::cout << "    " << *n << std::endl;
        ++ n;

      }

    }

    // computing the total number of volume(s)
    int64_t volumeCount =  mriSequenceCount * inputSampleCount;
    if ( verbose )
    {

      std::cout << " - total input volume count : "
                << volumeCount
                << std::endl;

    }


    ///////////////////////////////////////////////////////////////////////////
    // allocating generative and diffusion MRI intermediate structure(s)
    ///////////////////////////////////////////////////////////////////////////

    // struture(s) corresponding to the diffusion MRI signature(s)
    std::vector< gkg::RCPointer< gkg::Volume< float > > >
      t2Volumes( volumeCount );
    std::vector< gkg::RCPointer< gkg::Volume< float > > >
      dwVolumes( volumeCount );
    std::vector< std::vector< float > >
      bValueSets( volumeCount );
    std::vector< gkg::OrientationSet > 
      orientationSets( volumeCount );

    // structure(s) corresponding to the generative parameters of interest
    std::vector< int64_t >
      populationCounts( volumeCount );
    std::vector< std::vector< gkg::Vector3d< float > > >
      meanOrientations( volumeCount );
    std::vector< std::vector< float > > 
      axonMeanDiameters( volumeCount );
    std::vector< std::vector< float > > 
      globalAngularDispersions( volumeCount );
    std::vector< std::vector< float > > 
      volumeFractions( volumeCount );


    ///////////////////////////////////////////////////////////////////////////
    // looping over sample directories
    ///////////////////////////////////////////////////////////////////////////


    if ( verbose )
    {

      std::cout << "analyzing the input sample directories : " << std::flush;

    }
    
    int64_t maximumPopulationCount = 0;
    int64_t inputSampleIndex = 0;
    std::set< std::string >::const_iterator
      s = subDirectoryNames.begin(),
      se = subDirectoryNames.end();

    while ( s != se )
    {

      if ( verbose )
      {

        if ( inputSampleIndex != 0 )
        {

          std::cout << gkg::Eraser( 28 );

        }
        std::cout << " [ " << std::setw( 10 ) << inputSampleIndex + 1 
                  << " / " << std::setw( 10 ) << inputSampleCount
                  << " ]" << std::flush;

      }

      gkg::Directory sampleDirectory( medusaSimulationCampaignDirectoryName +
                                      gkg::getDirectorySeparator() + 
                                      *s );


      //////////////////////////////////////////////////////////////////////////
      // collecting the geometry information
      //////////////////////////////////////////////////////////////////////////

      std::string geometryJsonFileName = sampleDirectory.getPath() +
                                         gkg::getDirectorySeparator() + 
                                         GEOMETRY_JSON_FILENAME;
      gkg::UncommentCounterInputFileStream 
        geometryUcis( geometryJsonFileName.c_str() );
              
      gkg::BaseObjectReader geometryBaseObjectReader;
      gkg::TypedObject< gkg::Dictionary > typedGeometryDictionary;
      geometryBaseObjectReader.read( geometryUcis, typedGeometryDictionary );
      geometryUcis.close();

      const gkg::Dictionary geometryDictionary = 
                                        typedGeometryDictionary.getTypedValue();
            
      // collecting the population dictionary
      gkg::Dictionary 
        populationDictionary = gkg::getDictionaryValue( geometryDictionary,
                                                        "populations",
                                                        true );

      // computing the population count and storing it
      int64_t populationCount = ( int64_t )populationDictionary.size();
      populationCounts[ inputSampleIndex ] = populationCount;
      if ( populationCount > maximumPopulationCount )
      {

        maximumPopulationCount = populationCount;

      }

      // resizing the generative parameter vectors accordingly
      meanOrientations[ inputSampleIndex ].resize( populationCount );
      axonMeanDiameters[ inputSampleIndex ].resize( populationCount );
      globalAngularDispersions[ inputSampleIndex ].resize( populationCount );
      volumeFractions[ inputSampleIndex ].resize( populationCount );

      // looping over population(s)
      int64_t populationIndex = 0;
      gkg::Dictionary::const_iterator
        p = populationDictionary.begin(),
        pe = populationDictionary.end();
      while ( p != pe )
      {
            
        const gkg::Dictionary& 
          populationDictionary = p->second->getValue< gkg::Dictionary >();

        // accessing parameters
        gkg::Dictionary
          populationParameters = gkg::getDictionaryValue( populationDictionary,
                                                          "parameters",
                                                          true );

        // accessing axon dictionary parameters
        gkg::Dictionary
          axonParameters = gkg::getDictionaryValue( populationParameters,
                                                    "axon",
                                                    true );

        // accessing diameter distribution parameters
        gkg::Dictionary 
          axonDiameterDistributionParameters = 
                                gkg::getDictionaryValue( axonParameters,
                                                        "diameter_distribution",
                                                        true );

        // accessing parameters dictionnary from population axon
        gkg::Dictionary 
          axonDiameterParameters = gkg::getDictionaryValue(
                                             axonDiameterDistributionParameters,
                                             "parameters",
                                             true );

        // accessing generatives parameters 
        meanOrientations[ inputSampleIndex ][ populationIndex ] =
                               gkg::getVector3dOfFloatValue( axonParameters,
                                                             "mean_orientation",
                                                             true );
        axonMeanDiameters[ inputSampleIndex ][ populationIndex ] =
                                     gkg::getFloatValue( axonDiameterParameters,
                                                         "mean",
                                                         true );
        globalAngularDispersions[ inputSampleIndex ][ populationIndex ] =
                     gkg::getFloatValue( axonParameters,
                                         "global_angular_dispersion_in_degrees",
                                         true );
 
        volumeFractions[ inputSampleIndex ][ populationIndex ] =
                                       gkg::getFloatValue( populationParameters,
                                                           "volume_fraction",
                                                           true );

        ++ populationIndex;
        ++ p;

      }


      //////////////////////////////////////////////////////////////////////////
      // collecting the diffusion MRI information
      //////////////////////////////////////////////////////////////////////////

      std::string diffusionMRIDirectoryName = sampleDirectory.getPath() +
                                              gkg::getDirectorySeparator() + 
                                              MRI_OUTPUT_DIRECTORY_NAME;

      // looping over MRI sequences
      int64_t mriSequenceIndex = 0;
      int64_t volumeIndex = 0;
      std::list< std::string >::const_iterator
        ms = mriSequenceNames.begin(),
        mse = mriSequenceNames.end();
      while ( ms != mse )
      {

        // computing the volume index in the global volume structure(s)
        volumeIndex = inputSampleIndex * mriSequenceCount + mriSequenceIndex;

        // reading T2 volume
        gkg::RCPointer< gkg::Volume< float > >& 
          t2Volume = t2Volumes[ volumeIndex ];
        t2Volume.reset( new gkg::Volume< float > );
        gkg::TypedVolumeReaderProcess< float > 
          t2VolumeReaderProcess( *t2Volume );
        t2VolumeReaderProcess.execute( diffusionMRIDirectoryName +
                                       gkg::getDirectorySeparator() +
                                       "volume_attenuations_" +
                                       *ms +
                                       "_t2.ima" );

        // reading DW volume
        gkg::RCPointer< gkg::Volume< float > >& 
          dwVolume = dwVolumes[ volumeIndex ];
        dwVolume.reset( new gkg::Volume< float > );
        gkg::TypedVolumeReaderProcess< float > 
          dwVolumeReaderProcess( *dwVolume );
        dwVolumeReaderProcess.execute( diffusionMRIDirectoryName +
                                       gkg::getDirectorySeparator() +
                                       "volume_attenuations_" +
                                       *ms +
                                       "_dw.ima" );

        // reading the orientation set
        gkg::OrientationSet& orientationSet = orientationSets[ volumeIndex ];
        orientationSet = gkg::OrientationSet( diffusionMRIDirectoryName +
                                              gkg::getDirectorySeparator() +
                                              "volume_attenuations_" +
                                              *ms +
                                              "_dw.bvec" );
        int64_t orientationCount = ( int64_t )orientationSet.getCount();


        // reading the b-value set
        std::ifstream is( ( diffusionMRIDirectoryName +
                            gkg::getDirectorySeparator() +
                            "volume_attenuations_" +
                            *ms +
                            "_dw.bval" ).c_str() );
        std::vector< float >& bValues = bValueSets[ volumeIndex ];
        bValues.reserve( orientationCount );
        float bValue = 0.0f;
        is >> bValue;
        do
        {

          is >> bValue;
          bValues.push_back( bValue );

        }
        while ( !is.eof() );
        if ( ( int64_t )bValues.size() != orientationCount )
        {

          throw std::runtime_error(
                                "inconsistent b-value and orientation counts" );

        }
        is.close();


        ++ mriSequenceIndex;
        ++ ms;

      }

      ++ inputSampleIndex;
      ++ s;

    }

    if ( verbose )
    {

      std::cout << std::endl;

    }


    ///////////////////////////////////////////////////////////////////////////
    // allocating an output matrix for deep learning
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "allocating an output matrix for deep learning : "
                << std::flush;

    }

    // computing the output generative parameter count
    int64_t outputGenerativeParameterCount =
             1 + maximumPopulationCount * POPULATION_GENERATIVE_PARAMETER_COUNT;

    // computing the output diffusion MRI parameter count
    int64_t sphericalHarmonicsCoefficientCount =
          ( sphericalHarmonicsOrder + 1 ) * ( sphericalHarmonicsOrder + 2 ) / 2;
    int64_t outputDiffusionMRIParameterCount =
                      mriSequenceCount * ( DTI_PARAMETER_COUNT +
                                           sphericalHarmonicsCoefficientCount );

    // computing the total column count of the output matrix
    int64_t outputColumnCount = outputGenerativeParameterCount +
                                outputDiffusionMRIParameterCount;

    // allocating the target output matrix used for deep learning
    gkg::Matrix deepLearningMatrix( outputSampleCount, outputColumnCount );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ///////////////////////////////////////////////////////////////////////////
    // looping a second time over samples to generate noisy and rotated samples
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "generating output samples : " << std::flush;

    }
    
    MedusaDataToDeepLearningDataGauge gauge( inputSampleCount * rotationCount );

    MedusaDataToDeepLearningDataContext
      context( gauge,
               deepLearningMatrix,
               t2Volumes,
               dwVolumes,
               bValueSets,
               orientationSets,
               populationCounts,
               meanOrientations,
               axonMeanDiameters,
               globalAngularDispersions,
               volumeFractions,
               rotationCount,
               noiseStandardDeviation,
               maximumPopulationCount,
               outputGenerativeParameterCount,
               outputColumnCount,
               mriSequenceCount,
               sphericalHarmonicsCoefficientCount,
               regularizationLCurveFactor,
               verbose );
    gkg::ThreadedLoop< int64_t > threadedLoop( &context, 0, inputSampleCount );
    threadedLoop.launch();

    if ( verbose )
    {

      std::cout << std::endl;

    }


    ///////////////////////////////////////////////////////////////////////////
    // writing output deep learning matrix
    ///////////////////////////////////////////////////////////////////////////

    if ( csv )
    {

      if ( verbose )
      {

        std::cout << "writing output '"
                  << outputDeepLearningMatrixFileName
                  << "' csv deep learning matrix : "
                  << std::flush;

      }

      std::ofstream os( outputDeepLearningMatrixFileName.c_str() );

      int64_t outputSampleIndex = 0;
      int64_t outputColumnIndex = 0;
      for ( outputSampleIndex = 0;
            outputSampleIndex < outputSampleCount;
            outputSampleIndex++ )
      {

        for ( outputColumnIndex = 0;
              outputColumnIndex < outputColumnCount;
              outputColumnIndex++ )
        {

          os << deepLearningMatrix( outputSampleIndex, outputColumnIndex );
          if ( outputColumnIndex == outputColumnCount - 1 )
          {

            os << std::endl;

          }
          else
          {

            os << " ";

          }

        }

      }

      os.close();

      if ( verbose )
      {

        std::cout << "done" << std::endl;


      }

    }
    else
    {

      if ( verbose )
      {

        std::cout << "writing output '"
                  << outputDeepLearningMatrixFileName
                  << "' deep learning binary matrix : "
                  << std::flush;

      }

      gkg::ItemWriter< gkg::Matrix >& matrixW =
        gkg::MatrixItemIOFactory::getInstance().getWriter( false, false );
      std::ofstream os( outputDeepLearningMatrixFileName.c_str() );
      matrixW.write( os, deepLearningMatrix );
      os.close();

      if ( verbose )
      {

        std::cout << "done" << std::endl;


      }

    }

  }
  GKG_CATCH( "void gkg::MedusaDataToDeepLearningDataCommand::execute( "
             "const std::string& medusaSimulationCampaignDirectoryName, "
             "const std::string& outputDeepLearningMatrixFileName, "
             "int64_t rotationCount, "
             "float noiseStandardDeviation, "
             "int64_t sphericalHarmonicsOrder, "
             "double regularizationLCurveFactor, "
             "bool csv, "
             "bool verbose )" );

}


RegisterCommandCreator(
   MedusaDataToDeepLearningDataCommand,
   DECLARE_LIST_OF_STRINGS_PARAMETER_HELP(
                                       medusaSimulationCampaignDirectoryName ) +
   DECLARE_LIST_OF_STRINGS_PARAMETER_HELP( outputDeepLearningMatrixFileName ) +
   DECLARE_INTEGER_PARAMETER_HELP( rotationCount ) +
   DECLARE_FLOATING_PARAMETER_HELP( noiseStandardDeviation ) +
   DECLARE_INTEGER_PARAMETER_HELP( sphericalHarmonicsOrder ) +
   DECLARE_FLOATING_PARAMETER_HELP( regularizationLCurveFactor ) +
   DECLARE_BOOLEAN_PARAMETER_HELP( csv ) +
   DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );


#undef MRI_SEQUENCE_JSON_FILENAME
#undef GEOMETRY_JSON_FILENAME
#undef MRI_OUTPUT_DIRECTORY_NAME
#undef DTI_PARAMETER_COUNT

#undef POPULATION_COUNT_OFFSET
#undef POPULATION_GENERATIVE_PARAMETER_COUNT
#undef AXON_DIAMETER_OFFSET
#undef GLOBAL_ANGULAR_DISPERSION_OFFSET
#undef VOLUME_FRACTION_OFFSET
#undef MEAN_ORIENTATION_X_OFFSET
#undef MEAN_ORIENTATION_Y_OFFSET
#undef MEAN_ORIENTATION_Z_OFFSET

