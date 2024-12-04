#include <gkg-qmri-plugin-functors/RelaxometryT1CorticalLaminationMapper/RelaxometryT1CorticalLaminationMapperCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/LevenbergMarquardtFunction.h>
#include <gkg-processing-numericalanalysis/PowellNewUOAFunction.h>
#include <gkg-processing-histogram/HistogramAnalyzer.h>
#include <gkg-processing-numericalanalysis/ConstrainedGaussianMixtureFunction.h>
#include <gkg-processing-numericalanalysis/GaussianMixtureFunction.h>
#include <gkg-processing-algobase/MedianFilter_i.h>
#include <gkg-processing-algobase/MaximumIndexFilter_i.h>
#include <gkg-processing-algobase/SumFilter_i.h>
#include <gkg-processing-morphology/Neighborhood3d.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>


#define UNCORRECTED_LABEL 1000
#define CORRECTED_LABEL 2000


///////////////////////////////////////////////////////////////////////////////
// case of Inversion Recovery Spin Echo pulse sequence with variable TI
//   -> rho/T1 mapping
// As magnetization may be negative with inversion recovery pulses:
// the EPI sequence takes the absolute value
///////////////////////////////////////////////////////////////////////////////

//
// class MultipleRhoT1MagnetizationFunction
//

class MultipleRhoT1MagnetizationFunction :
                                         public gkg::LevenbergMarquardtFunction,
                                         public gkg::PowellNewUOAFunction
{

  public:

    MultipleRhoT1MagnetizationFunction( const gkg::Vector& parameters,
                                        bool constrainedNewUOA = false );
    ~MultipleRhoT1MagnetizationFunction();

    // for Levenberg-Marquardt function
    double getValueWithParametersAt( const gkg::Vector& x,
                                     const gkg::Vector& p ) const;
    gkg::Vector getDerivativeWithParametersAt( const gkg::Vector& x,
                                               const gkg::Vector& p ) const;

    void setMeasurements( const gkg::Vector& measurements );
    void setInversionTimes( const gkg::Vector& echoTimes );

    // for Powell's NewUOA function
    double getValueAt( const double* parameters, int32_t count ) const;

  protected:

    gkg::Vector _measurements;
    gkg::Vector _inversionTimes;
    bool _constrainedNewUOA;  

};


MultipleRhoT1MagnetizationFunction::MultipleRhoT1MagnetizationFunction(
                                                  const gkg::Vector& parameters,
                                                  bool constrainedNewUOA )
                           : gkg::LevenbergMarquardtFunction( parameters ),
                             gkg::PowellNewUOAFunction( parameters.getSize() ),
                             _constrainedNewUOA( constrainedNewUOA )
{
}


MultipleRhoT1MagnetizationFunction::~MultipleRhoT1MagnetizationFunction()
{
}


double MultipleRhoT1MagnetizationFunction::getValueWithParametersAt(
                                                    const gkg::Vector& x,
                                                    const gkg::Vector& p ) const
{

  try
  {

    // the vector of parameters is organized as follows:
    //   p = [ rho1 rho2 .... rhoN T1_1 T1_2 .... T1_N ]
    int32_t componentCount = ( int32_t )( this->_parameterCount / 2 );

    const double* rho = &p( 0 );
    const double* t1 = &p( componentCount );

    int32_t c = 0;
    double value = 0.0;
    for ( c = 0; c < componentCount; c ++ )
    {

      value += rho[ c ] *
               std::fabs( 1.0 - 2.0 * std::exp( - x( 0 ) / t1[ c ] ) );

    }
            
    return value;

  }
  GKG_CATCH( "double MultipleRhoT1MagnetizationFunction::"
             "getValueWithParametersAt( "
             "const gkg::Vector& x, "
             "const gkg::Vector& p ) const" );

}


gkg::Vector MultipleRhoT1MagnetizationFunction::getDerivativeWithParametersAt(
                                                    const gkg::Vector& x,
                                                    const gkg::Vector& p ) const
{

  try
  {

    int32_t componentCount = ( int32_t )( this->_parameterCount / 2 );

    double threshold = x( 0 ) / std::log( 2 );

    const double* rho = &p( 0 );
    const double* t1 = &p( componentCount );

    int32_t c = 0;
    gkg::Vector derivative( this->_parameterCount );
    for ( c = 0; c < componentCount; c++ )
    {
         
      if ( t1[ c ] < threshold )
      {

        derivative( c ) = 1.0 - 2.0 * std::exp( - x( 0 ) / t1[ c ] );

        derivative( c + componentCount ) = - 2.0 * rho[ c ] * 
                                           ( x( 0 ) / ( t1[ c ] *
                                           t1[ c ] ) ) *
                                           std::exp( - x( 0 ) / t1[ c ] );

      } 
      else
      {

        derivative( c ) = 2.0 * std::exp( - x( 0 ) / t1[ c ] ) - 1.0;
           
        derivative( c + componentCount ) = 2.0 * rho[ c ] * 
                                           ( x( 0 ) / ( t1[ c ] *
                                           t1[ c ] ) ) *
                                           std::exp( - x( 0 ) / t1[ c ] );

      }

    }
    return derivative;

  }
  GKG_CATCH( "gkg::Vector MultipleRhoT1MagnetizationFunction::"
             "getDerivativeWithParametersAt( "
             "const gkg::Vector& x, "
             "const gkg::Vector& p ) const" );

}


void MultipleRhoT1MagnetizationFunction::setMeasurements(
                                               const gkg::Vector& measurements )
{

  try
  {

    _measurements = measurements;

  }
  GKG_CATCH( "void MultipleRhoT1MagnetizationFunction::setMeasurements( "
             "const gkg::Vector& measurements )" );

}


void MultipleRhoT1MagnetizationFunction::setInversionTimes(
                                             const gkg::Vector& inversionTimes )
{

  try
  {

    _inversionTimes = inversionTimes;

  }
  GKG_CATCH( "void MultipleRhoT1MagnetizationFunction::setInversionTimes( "
             "const gkg::Vector& inversionTimes )" );

}


double MultipleRhoT1MagnetizationFunction::getValueAt(
                                                     const double* parameters,
                                                     int32_t /* count */ ) const
{

  try
  {

    double squareError = 0.0; 

    int32_t inversionTimeCount = _measurements.getSize();
    int32_t componentCount = ( int32_t )( this->_parameterCount / 2 );

    if ( _inversionTimes.getSize() != inversionTimeCount )
    {

      throw std::runtime_error(
                   "inconsistent measurement and inversion time vector size" );

    }

    std::vector< double > constrainedParameters;
    const double* rho = 0;
    const double* t1 = 0;
    if ( _constrainedNewUOA )
    {

      constrainedParameters.resize( this->_parameterCount );

      this->applyConstraints( parameters,
                              this->_parameterCount,
                              &constrainedParameters[ 0 ] );

      rho = &constrainedParameters[ 0 ];
      t1 = &constrainedParameters[ componentCount ];

    }
    else
    {

      rho = &parameters[ 0 ];
      t1 = &parameters[ componentCount ];

    }

    int32_t ti = 0;
    int32_t c = 0;
    double error = 0.0;
    double value = 0.0;
    for ( ti = 0; ti < inversionTimeCount; ti++ )
    {

      value = 0.0;
      for ( c = 0; c < componentCount; c++ )
      {

        value += std::fabs( rho[ c ] *
               ( 1.0 - 2.0 * std::exp( -_inversionTimes( ti ) / t1[ c ] ) ) );

      }
  
      error = _measurements( ti ) - value;
      squareError += error * error; 

    }

    return squareError;

  }
  GKG_CATCH( "double MultipleRhoT1MagnetizationFunction::getValueAt( "
             "const gkg::Vector& parameters ) const" );

}


///////////////////////////////////////////////////////////////////////////////
// file time(s) reading
///////////////////////////////////////////////////////////////////////////////


static gkg::Vector readTimeFile( const std::string& fileName, int32_t trialCount )
{

  try
  {

    gkg::Vector times( trialCount );

    std::ifstream is( fileName.c_str() );

    int32_t t;
    for ( t = 0; t < trialCount; t++ )
    {

      is >> times( t );

    }

    is.close();

    return times;

  }
  GKG_CATCH( "gkg::Vector readTimeFile( const std::string& fileName, "
             "int2_t trialCount )" );

}


//
//  class RelaxometryT1CorticalLaminationMapperCommand
//


gkg::RelaxometryT1CorticalLaminationMapperCommand::
                RelaxometryT1CorticalLaminationMapperCommand( int32_t argc,
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
  GKG_CATCH( "gkg::RelaxometryT1CorticalLaminationMapperCommand::"
             "RelaxometryT1CorticalLaminationMapperCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::RelaxometryT1CorticalLaminationMapperCommand::
                                   RelaxometryT1CorticalLaminationMapperCommand(
                               const std::string& fileNameIn, 
                               const std::string& fileNameTimes,
                               const std::string& fileNameMask,
                               const std::string& fileNameOut,
                               const std::string& fileNameFittedData,
                               const std::string& fileNameHistogram,
                               int32_t componentCount,
                               const std::string& optimizerType,
                               const std::vector< double >& optimizerParameters,
                               const std::vector< double >& lowerBoundaries,
                               const std::vector< double >& upperBoundaries,
                               const std::vector< double >& histogramParameters,
                               bool ascii,
                               const std::string& format,
                               bool verbose )
                                                  : gkg::Command()
{

  try
  {

    execute( fileNameIn,
             fileNameTimes,
             fileNameMask,
             fileNameOut,
             fileNameFittedData,
             fileNameHistogram,
             componentCount,
             optimizerType,
             optimizerParameters,
             lowerBoundaries,
             upperBoundaries,
             histogramParameters,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::RelaxometryT1CorticalLaminationMapperCommand::"
             "RelaxometryT1CorticalLaminationMapperCommand( "
             "const std::string& fileNameIn,  "
             "const std::string& fileNameTimes, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameOut, "
             "const std::string& fileNameFittedData, "
             "const std::string& fileNameHistogram, "
             "int32_t componentCount, "
             "const std::string& optimizerType, "
             "const std::vector< double >& optimizerParameters, "
             "const std::vector< double >& lowerBoundaries, "
             "const std::vector< double >& upperBoundaries, "
             "const std::vector< double >& histogramParameters, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


gkg::RelaxometryT1CorticalLaminationMapperCommand::
                                   RelaxometryT1CorticalLaminationMapperCommand(
                                             const gkg::Dictionary& parameters )
                             : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameTimes );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameFittedData );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameHistogram );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, componentCount );
    DECLARE_STRING_PARAMETER( parameters, std::string, optimizerType );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           optimizerParameters );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           lowerBoundaries );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           upperBoundaries );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           histogramParameters );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn,
             fileNameTimes,
             fileNameMask,
             fileNameOut,
             fileNameFittedData,
             fileNameHistogram,
             componentCount,
             optimizerType,
             optimizerParameters,
             lowerBoundaries,
             upperBoundaries,
             histogramParameters,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::RelaxometryT1CorticalLaminationMapperCommand::"
             "RelaxometryT1CorticalLaminationMapperCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::RelaxometryT1CorticalLaminationMapperCommand::
                                 ~RelaxometryT1CorticalLaminationMapperCommand()
{
}


std::string gkg::RelaxometryT1CorticalLaminationMapperCommand::getStaticName()
{

  try
  {

    return "RelaxometryT1CorticalLaminationMapper";

  }
  GKG_CATCH( "std::string "
             "gkg::RelaxometryT1CorticalLaminationMapperCommand::"
             "getStaticName()" );

}



bool gkg::RelaxometryT1CorticalLaminationMapperCommand::sortingT1(
                                         const std::pair< int32_t, double >& a,
                                         const std::pair< int32_t, double >& b )
{

  try
  {

    return ( a.second < b.second );


  }
  GKG_CATCH( "bool "
             "gkg::RelaxometryT1CorticalLaminationMapperCommand::sortingT1( "
             "const std::pair< int32_t, double >& a, "
             "const std::pair< int32_t, double >& b )" );

}


void gkg::RelaxometryT1CorticalLaminationMapperCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameTimes;
    std::string fileNameMask;
    std::string fileNameOut;
    std::string fileNameFittedData;
    std::string fileNameHistogram;
    int32_t componentCount = 7;
    std::string optimizerType = "levenberg-marquardt";
    std::vector< double > optimizerParameters;
    std::vector< double > lowerBoundaries;
    std::vector< double > upperBoundaries;
    std::vector< double > histogramParameters;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Relaxometry T1-based cortical lamination "
                                  "mapper",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-t",
                                 "Input TI times file name",
                                 fileNameTimes );
    application.addSingleOption( "-m",
                                 "Mask file name",
                                 fileNameMask );
    application.addSingleOption( "-o",
                                 "Output relaxometry / cortical lamination "
                                 "file name",
                                 fileNameOut );
    application.addSingleOption( "-fittedData",
                                 "Output volume file name of the fitted data",
                                 fileNameFittedData,
                                 true );
    application.addSingleOption( "-histogram",
                                 "Output histogram file name",
                                 fileNameHistogram,
                                 true );
    application.addSingleOption( "-componentCount",
                                 "Component count eg number of compartments "
                                 "in the multicompartmental model (default=7)",
                                 componentCount,
                                 true );
    application.addSingleOption( "-optimizerType",
                                 "Optimizer type:\n"
                                 " - levenberg-marquardt\n"
                                 " - powell-newuoa\n"
                                 "(default=levenberg-marquardt)",
                                 optimizerType,
                                 true );
    application.addSeriesOption( "-optimizerParameters",
                                 "Parameters used during optimization and "
                                 "depending on the optmizer type:\n"
                                 "- in case of levenberg-marquardt,\n"
                                 ".  <P1>: initial Rho value 1\n"
                                 ".  ... \n"
                                 ".  <PN>: initial Rho value N\n"
                                 ".  <PN+1>: initial T1 value 1\n"
                                 ".  ... \n"
                                 ".  <P2N>: initial T1 value N\n"
                                 ".  <P2N+1>: maximum iteration count\n"
                                 ".  <P2N+2>: absolute error\n"
                                 ".  <P2N+3>: relative error\n\n"
                                 "- in case of powell-newuoa,\n"
                                 ".  <P1>: initial Rho value 1\n"
                                 ".  ... \n"
                                 ".  <PN>: initial Rho value N\n"
                                 ".  <PN+1>: initial T1 value 1\n"
                                 ".  ... \n"
                                 ".  <P2N>: initial T1 value N\n"
                                 ".  <P2N+1>: initial trust region radius\n"
                                 ".  <P2N+2>: maximum iteration count\n"
                                 ".  <P2N+3>: maximum step size\n"
                                 ".  <P2N+4>: 0->wo constraint 1->with "
                                 "constraints",
                                 optimizerParameters,
                                 5 );
    application.addSeriesOption( "-lowerBoundaries",
                                 "Lower boundary of multiple rho and "
                                 "T1 values\n"
                                 "lower_rho_1 ... lower_rho_N "
                                 "lower_T1_1 ... lower_T1_N",
                                 lowerBoundaries,
                                 2 );
    application.addSeriesOption( "-upperBoundaries",
                                 "Upper boundary of multiple rho and "
                                  "T1 values\n"
                                 "upper_rho_1 ... upper_rho_N "
                                 "upper_T1_1 ... upper_T1_N",
                                 upperBoundaries,
                                 2 );

    application.addSeriesOption( "-histogramParameters",
                                 "Histogram Gaussian mixture parameters:\n"
                                 ".  <P1>: histogram level count\n"
                                 ".  <P2>: Parzen window width\n"
                                 ".  <P3>: smallest trusty T1 value\n"
                                 ".  <P4>: largest trusty T1 value\n"
                                 ".  <P5>: minimum Gaussian count for Gaussian "
                                 "mixture modeling\n"
                                 ".  <P6>: maximum Gaussian count for Gaussian "
                                 "mixture modeling\n"
                                 ".  <P7>: maximum iteration of Levenberg-"
                                 "Marquardt fit to get Gaussian mixture\n"
                                 ".  <P8>: absolute error of Levenberg-"
                                 "Marquardt fit to get Gaussian mixture\n"
                                 ".  <P9>: relative error of Levenberg-"
                                 "Marquardt fit to get Gaussian mixture\n"
                                 ". <P10>: 1-> histogram normalized 0->no"
                                 "normalization\n"
                                 ". <P11>: lower slice index along z\n"
                                 ". <P12>: upper slice index along z",
                                 histogramParameters,
                                 12, 12 );

    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn,
             fileNameTimes,
             fileNameMask,
             fileNameOut,
             fileNameFittedData,
             fileNameHistogram,
             componentCount,
             optimizerType,
             optimizerParameters,
             lowerBoundaries,
             upperBoundaries,
             histogramParameters,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void "
                     "gkg::RelaxometryT1CorticalLaminationMapperCommand::"
                     "parse()" );

}


void gkg::RelaxometryT1CorticalLaminationMapperCommand::execute(
                               const std::string& fileNameIn, 
                               const std::string& fileNameTimes,
                               const std::string& fileNameMask,
                               const std::string& fileNameOut,
                               const std::string& fileNameFittedData,
                               const std::string& fileNameHistogram,
                               int32_t componentCount,
                               const std::string& optimizerType,
                               const std::vector< double >& optimizerParameters,
                               const std::vector< double >& lowerBoundaries,
                               const std::vector< double >& upperBoundaries,
                               const std::vector< double >& histogramParameters,
                               bool /* ascii */,
                               const std::string& /* format */,
                               bool verbose )
{

  try
  {

    ///////////////////////////////////////////////////////////////////////////
    // sanity checks
    ///////////////////////////////////////////////////////////////////////////

    if ( ( optimizerType != "levenberg-marquardt" ) &&
         ( optimizerType != "powell-newuoa" ) )
    {

      throw std::runtime_error( "unknown optimizer type" );

    }


    if ( ( ( optimizerType == "levenberg-marquardt" ) &&
           ( ( int32_t )optimizerParameters.size() != 2 * componentCount + 3 ) )
         ||
         ( ( optimizerType == "powell-newuoa" ) &&
           ( ( int32_t )optimizerParameters.size() != 2 * componentCount + 4 ) )
        )
    {

      throw std::runtime_error( "bad optimizer parameter count" );

    }

    if ( ( int32_t )lowerBoundaries.size() != 2 * componentCount )
    {

      throw std::runtime_error( "bad lower boundary count" );

    }

    if ( ( int32_t )upperBoundaries.size() != 2 * componentCount )
    {

      throw std::runtime_error( "bad upper boundary count" );

    }

    ///////////////////////////////////////////////////////////////////////////
    // getting numerical analysis implementation factory
    ///////////////////////////////////////////////////////////////////////////

    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();


    ///////////////////////////////////////////////////////////////////////////
    // reading input data
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading input data : " << std::flush;

    }
    gkg::Volume< float > inputVolume;
    gkg::TypedVolumeReaderProcess< float >
      inputVolumeReaderProcess( inputVolume );
    inputVolumeReaderProcess.execute( fileNameIn );

    // collecting size of the input volume
    gkg::Vector3d< int32_t > inputSize;
    int32_t inputSizeT;
    inputVolume.getSize( inputSize, inputSizeT );

    // collecting resolution of the input volume
    gkg::Vector3d< double > inputResolution;
    double inputResolutionT = 1.0;
    inputVolume.getResolution( inputResolution, inputResolutionT );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ///////////////////////////////////////////////////////////////////////////
    // reading time file
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading time file : " << std::flush;

    }
    gkg::Vector inversionTimes = readTimeFile( fileNameTimes, inputSizeT );

    if ( verbose )
    {

      std::cout << inputSizeT << " inversion times " << std::endl;
      std::cout << "TI -> ";
      int32_t ti;
      for ( ti = 0; ti < inputSizeT; ti++ )
      {

        std::cout << inversionTimes( ti ) << "ms ";

      }
      std::cout << std::endl;

    }


    ///////////////////////////////////////////////////////////////////////////
    // reading mask
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading mask : " << std::flush;

    }
    gkg::Volume< int16_t > mask;
    gkg::TypedVolumeReaderProcess< int16_t > maskVolumeReaderProcess( mask );
    maskVolumeReaderProcess.execute( fileNameMask );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ///////////////////////////////////////////////////////////////////////////
    // allocating output volume
    ///////////////////////////////////////////////////////////////////////////

    int32_t parameterCount = 2 * componentCount;
    int32_t outputSizeT = parameterCount;
    bool constrainedNewUOA = false;
    std::vector< gkg::RCPointer< gkg::OptimizerConstraint > >
                                              constraints( 2 * componentCount );
    if ( optimizerType == "levenberg-marquardt" )
    {

      // we also save the standard deviations, the chi square per degree
      // of freedom and the outlier mask
      outputSizeT += parameterCount + 2;

    } 
    else if ( optimizerType == "powell-newuoa" )
    {

      // we also save the outlier mask
      outputSizeT += 1;
      constrainedNewUOA = ( ( int32_t )( optimizerParameters.back() + 0.5 ) ?
                            true : false );

      if ( constrainedNewUOA )
      {

        int32_t p = 0;
        for ( p = 0; p < parameterCount; p++ )
        {

          constraints[ p ].reset( new gkg::ClosedIntervalOptimizerConstraint(
                                                       lowerBoundaries[ p ],
                                                       upperBoundaries[ p ] ) );

        }


      }

    } 


    gkg::Volume< float > outputVolume( inputSize, outputSizeT );
    outputVolume.fill( 0.0f );
    outputVolume.getHeader().addAttribute( "resolutionX", inputResolution.x );
    outputVolume.getHeader().addAttribute( "resolutionY", inputResolution.y );
    outputVolume.getHeader().addAttribute( "resolutionZ", inputResolution.z );


    ///////////////////////////////////////////////////////////////////////////
    // allocating T1 volume
    ///////////////////////////////////////////////////////////////////////////

    gkg::Volume< float > t1Volume( inputSize, componentCount );
    t1Volume.fill( 0.0f );
    t1Volume.getHeader().addAttribute( "resolutionX", inputResolution.x );
    t1Volume.getHeader().addAttribute( "resolutionY", inputResolution.y );
    t1Volume.getHeader().addAttribute( "resolutionZ", inputResolution.z );


    ///////////////////////////////////////////////////////////////////////////
    // fitting data
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "fitting multiple Rho/T1 : " << std::flush;

    }

    int32_t voxelCountInMask = 0;
    gkg::Vector3d< int32_t > voxel;
    int32_t p = 0;
    for ( voxel.z = 0; voxel.z < inputSize.z; voxel.z++ )
    {

      if ( verbose )
      {

        if ( voxel.z )
        {

          std::cout << gkg::Eraser( 16 );

        }
        std::cout << "[ " << std::setw( 4 ) << voxel.z + 1 
                  << " / " << std::setw( 4 ) << inputSize.z
                  << " ] " << std::flush;

      }

      for ( voxel.y = 0; voxel.y < inputSize.y; voxel.y++ )
      {

        for ( voxel.x = 0; voxel.x < inputSize.x; voxel.x++ )
        {

          if ( mask( voxel ) )
          {

            ++ voxelCountInMask;

            // preparing initial parameters
            gkg::Vector initialParameters( parameterCount );
            for ( p = 0; p < parameterCount; p++ )
            {

              initialParameters( p ) = optimizerParameters[ p ];

            }

            // allocating magnetization function
            MultipleRhoT1MagnetizationFunction
                        multipleRhoT1MagnetizationFunction( initialParameters,
                                                            constrainedNewUOA );

            //////////////////////// LEVENBERG-MARQUARDT ///////////////////////
            if ( optimizerType == "levenberg-marquardt" )
            {

              // collecting measurements
              gkg::Vector Y( inputSizeT );
              gkg::Vector W( inputSizeT );

              int32_t ti = 0;
              for ( ti = 0; ti < inputSizeT; ti++ )
              {

                Y( ti ) = ( double )inputVolume( voxel, ti );
                W( ti ) = 1.0;

              }

              // performing the optimization using the Levenberg-Marquardt
              // algorithm
              int32_t maximumIterationCount = 
                ( int32_t )( optimizerParameters[ optimizerParameters.size() - 
                                                  3U ] + 0.5 );
              double absoluteError = 
                          optimizerParameters[ optimizerParameters.size() - 2 ];
              double relativeError = 
                          optimizerParameters[ optimizerParameters.size() - 1 ];

              gkg::Matrix covariance;
              double chiSquarePerDegreeOfFreedom = 0.0;
              factory->getLevenbergMarquardtFit(
                                             inversionTimes.toMatrix(),
                                             Y,
                                             W,
                                             multipleRhoT1MagnetizationFunction,
                                             covariance,
                                             chiSquarePerDegreeOfFreedom,
                                             maximumIterationCount,
                                             absoluteError,
                                             relativeError );

              // storing back the results into the output volume
              bool isAnOutlier = false;
              for ( p = 0; p < parameterCount; p++ )
              {

                const double& currentParameter = 
                        multipleRhoT1MagnetizationFunction.getParameters()( p );
                if ( ( currentParameter < lowerBoundaries[ p ] ) ||
                     ( currentParameter > upperBoundaries[ p ] ) )
                {

                  isAnOutlier = true;
                  break;

                }
                else
                { 
   
                  outputVolume( voxel, p ) =  ( float )currentParameter;
                  outputVolume( voxel, p + parameterCount ) = 
                                       ( float )std::sqrt( covariance( p, p ) );
                  // filling the volume containing the T1 components
                  if ( p >= componentCount )
                  {

                    t1Volume( voxel, p - componentCount ) =
                                                      ( float )currentParameter;

                  }

                }

              }
              outputVolume( voxel, 2 * parameterCount ) =
                                           ( float )chiSquarePerDegreeOfFreedom;

              // in case it is an outlier, reset everything, and store a 
              // specific label in the outlier mask
              if ( isAnOutlier )
              {

                for ( p = 0; p < 2 * parameterCount + 1; p++ )
                {

                  outputVolume( voxel, p ) = 0.0f;

                }
                outputVolume( voxel, 2 * parameterCount + 1 ) =
                                                     ( float )UNCORRECTED_LABEL;

              }

            }
            /////////////////////////// POWELL-NEWUOA //////////////////////////
            else
            {

              // collecting measurements
              gkg::Vector Y( inputSizeT );

              int32_t ti = 0;
              for ( ti = 0; ti < inputSizeT; ti++ )
              {

                Y( ti ) = ( double )inputVolume( voxel, ti );

              }

              multipleRhoT1MagnetizationFunction.setMeasurements( Y );
              multipleRhoT1MagnetizationFunction.setInversionTimes(
                                                               inversionTimes );

              // performing the optimization using the Powell's NEWUOA
              // algorithm
              double radius = 
                         optimizerParameters[ optimizerParameters.size() - 4U ];
              int32_t maximumIterationCount = 
                ( int32_t )( optimizerParameters[ optimizerParameters.size() - 
                                                  3U ] + 0.5 );
              double maximumTestSize = 
                         optimizerParameters[ optimizerParameters.size() - 2U ];

              if ( constrainedNewUOA )
              {

                multipleRhoT1MagnetizationFunction.addConstraints(
                                                                  constraints );

              }

              factory->getPowellNewUOAOptimumParameters( 
                                             initialParameters,
                                             multipleRhoT1MagnetizationFunction,
                                             radius,
                                             maximumIterationCount,
                                             maximumTestSize,
                                             false );

              gkg::Vector constrainedParameters( parameterCount );
              if ( constrainedNewUOA )
              {

                multipleRhoT1MagnetizationFunction.getConstrainedParameters(
                                                        initialParameters,
                                                        constrainedParameters );
                initialParameters = constrainedParameters;

              }

              // storing back the results into the output volume
              bool isAnOutlier = false;
              for ( p = 0; p < parameterCount; p++ )
              {

                const double& currentParameter = initialParameters( p );
                if ( !constrainedNewUOA &&
                     ( ( currentParameter < lowerBoundaries[ p ] ) ||
                       ( currentParameter > upperBoundaries[ p ] ) ) )
                {

                  isAnOutlier = true;
                  break;

                }
                else
                { 
   
                  outputVolume( voxel, p ) =  ( float )currentParameter;
                  // filling the volume containing the T1 components
                  if ( p >= componentCount )
                  {

                    t1Volume( voxel, p - componentCount ) =
                                                      ( float )currentParameter;

                  }

                }

              }

              // in case it is an outlier, reset everything, and store a 
              // specific label in the outlier mask
              if ( isAnOutlier )
              {

                for ( p = 0; p < 2 * parameterCount + 1; p++ )
                {

                  outputVolume( voxel, p ) = 0.0f;

                }
                outputVolume( voxel, 2 * parameterCount ) =
                                                     ( float )UNCORRECTED_LABEL;

              }

            }

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << std::endl;

    }


    ///////////////////////////////////////////////////////////////////////////
    // correcting points where fitting failed
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "correcting points where fitting failed : " << std::flush;

    }

    std::list< gkg::Vector3d< int32_t > >
      offsets = gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                              gkg::Neighborhood3d::Neighborhood3d_26 );
    gkg::BoundingBox< int32_t > boundingBox( inputVolume );
    gkg::Vector3d< int32_t > neighbor;
    gkg::MedianFilter< std::vector< float >, float > medianFilter;
    for ( voxel.z = 0; voxel.z < inputSize.z; voxel.z++ )
    {

      if ( verbose )
      {

        if ( voxel.z != 0 )
        {

          std::cout << gkg::Eraser( 16 );

        }
        std::cout << "[ " << std::setw( 4 ) << voxel.z + 1 
                  << " / " << std::setw( 4 ) << inputSize.z
                  << " ] " << std::flush;

      }

      for ( voxel.y = 0; voxel.y < inputSize.y; voxel.y++ )
      {

        for ( voxel.x = 0; voxel.x < inputSize.x; voxel.x++ )
        {

          bool atLeastOneValidNeighborFound = false;
          if ( ( mask( voxel ) ) &&
               ( outputVolume( voxel, outputSizeT - 1  ) > 0 ) )
          {

            std::vector< std::vector< float > > values( outputSizeT );
            for ( p = 0; p < outputSizeT; p++ )
            {

              values[ p ].reserve( 26U );

            }

            std::list< gkg::Vector3d< int32_t > >::const_iterator
              o = offsets.begin(),
              oe = offsets.end();
            while ( o != oe )
            {

              neighbor = voxel + *o;
              if ( boundingBox.contains( neighbor ) )
              {

                if ( mask( neighbor ) )
                {

                  if ( ( int32_t )outputVolume( neighbor,
                                                outputSizeT - 1 ) == 0 )
                  {

                    atLeastOneValidNeighborFound = true;
                    for ( p = 0; p < outputSizeT; p++ )
                    {

                      values[ p ].push_back( outputVolume( neighbor, p ) );

                    }

                  }

                }

              }
              ++ o;

            }

            if ( atLeastOneValidNeighborFound )
            {

              for ( p = 0; p < outputSizeT; p++ )
              {

                medianFilter.filter( values[ p ], outputVolume( voxel, p ) );

              }
              outputVolume( voxel, outputSizeT - 1 ) = ( float )CORRECTED_LABEL;

              for ( p = 0; p < componentCount; p++ )
              {

                t1Volume( voxel, p ) = outputVolume( voxel,
                                                     p + componentCount );

              }

            }

          }

        }

      }

    }
    if ( verbose )
    {

      std::cout << std::endl;

    }


    ///////////////////////////////////////////////////////////////////////////
    // creating and saving fitted data
    ///////////////////////////////////////////////////////////////////////////

    if ( !fileNameFittedData.empty() )
    {

      if ( verbose )
      {

        std::cout << "processing volume of fitted data : " << std::flush;

      }
      gkg::Volume< float > fittedVolume( inputSize, inputSizeT );
      fittedVolume.getHeader().addAttribute( "resolutionX", inputResolution.x );
      fittedVolume.getHeader().addAttribute( "resolutionY", inputResolution.y );
      fittedVolume.getHeader().addAttribute( "resolutionZ", inputResolution.z );
      fittedVolume.getHeader().addAttribute( "resolutionT", inputResolutionT );
      fittedVolume.fill( 0.0f );

      for ( voxel.z = 0; voxel.z < inputSize.z; voxel.z++ )
      {

        if ( verbose )
        {

          if ( voxel.z != 0 )
          {

            std::cout << gkg::Eraser( 16 );

          }
          std::cout << "[ " << std::setw( 4 ) << voxel.z + 1
                    << " / " << std::setw( 4 ) << inputSize.z
                    << " ] " << std::flush;

        }

        int32_t ti = 0;
        for ( voxel.y = 0; voxel.y < inputSize.y; voxel.y++ )
        {

          for ( voxel.x = 0; voxel.x < inputSize.x; voxel.x++ )
          {

            if ( mask( voxel ) )
            {

              gkg::Vector fittedParameters( parameterCount );
              for ( ti = 0; ti < parameterCount; ti++ )
              {

                fittedParameters( ti ) = outputVolume( voxel, ti );

              }

              MultipleRhoT1MagnetizationFunction
                multipleRhoT1MagnetizationFunction( fittedParameters );
              multipleRhoT1MagnetizationFunction.setInversionTimes(
                                                               inversionTimes );

              gkg::Vector inversionTime( 1 ); 
              for ( ti = 0; ti < inputSizeT; ti++ )
              {

                inversionTime( 0 ) = inversionTimes( ti );
                fittedVolume( voxel, ti ) =
                  ( float )multipleRhoT1MagnetizationFunction.
                                                      getValueWithParametersAt(
                                                             inversionTime,
                                                             fittedParameters );

              }

            }
 
          }

        }

      }
      if ( verbose )
      {

        std::cout << std::endl;

      }

      if ( verbose )
      {

        std::cout << "writing fitted data : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameFittedData, fittedVolume );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ///////////////////////////////////////////////////////////////////////////
    // computing the histogram of T1 on all the image
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "computing the histogram of quantitative T1 : "
                << std::flush;

    }


    int32_t lowerSliceIndexAlongZ = ( int32_t )( histogramParameters[ 10 ] +
                                                 0.5 );
    int32_t upperSliceIndexAlongZ = ( int32_t )( histogramParameters[ 11 ] +
                                                 0.5 );

    gkg::Volume< int16_t > maskHisto( mask );
    for ( voxel.z = 0; voxel.z < inputSize.z; voxel.z++ )
    {

      for ( voxel.y = 0; voxel.y < inputSize.y; voxel.y++ )
      {

        for ( voxel.x = 0; voxel.x < inputSize.x; voxel.x++ )
        {

          if ( ( voxel.z < lowerSliceIndexAlongZ ) ||
               ( voxel.z > upperSliceIndexAlongZ ) )
          {

            maskHisto( voxel ) = 0;

          }

        }

      }

    }




    float lowerT1Boundary = 0.0f;
    float upperT1Boundary = 0.0f;

    

    int32_t histogramLevelCount = ( int32_t )( histogramParameters[ 0 ] + 0.5 );
    std::vector< double >
      quantitativeT1Histogram( histogramLevelCount, 0.0f );

    gkg::HistogramAnalyzer::getInstance().getHistogram(
                                                       t1Volume,
                                                       maskHisto,
                                                       quantitativeT1Histogram,
                                                       lowerT1Boundary,
                                                       upperT1Boundary,
                                                       histogramParameters[ 1 ],
                                                       false );

    gkg::Vector histogramAbscissas( histogramLevelCount );
    gkg::Vector histogramOrdinates( histogramLevelCount );
    double smallestT1Value = histogramParameters[ 2 ];
    double largestT1Value = histogramParameters[ 3 ];

    double incrementT1Value = ( ( double )upperT1Boundary - 
                                ( double )lowerT1Boundary ) /
                              ( ( double )histogramLevelCount + 1 );

    int32_t h = 0;
    double sum = 0.0;
    for ( h = 0; h < histogramLevelCount; h++ )
    {

      histogramAbscissas( h ) = lowerT1Boundary + ( h + 1 ) * incrementT1Value;
      if ( ( histogramAbscissas( h ) < smallestT1Value ) ||
           ( histogramAbscissas( h ) > largestT1Value ) )
      {

        histogramOrdinates( h ) = 0.0;

      }
      else
      {

        histogramOrdinates( h ) = quantitativeT1Histogram[ h ];
        sum += quantitativeT1Histogram[ h ];

      }

    }

    bool normalizedHistogram = ( ( int32_t )( histogramParameters[ 9 ] + 0.5 ) ?
                                 true : false );
    if ( normalizedHistogram )
    {

      for ( h = 0; h < histogramLevelCount; h++ )
      {

        histogramOrdinates( h ) /= sum;

      }

    }

    std::ofstream os( "/tmp/histo.txt" );
    os << "X=[";
    for ( h = 0; h < histogramLevelCount - 1; h++ )
    {

      os << histogramAbscissas( h ) << ", ";
      os << histogramOrdinates( h ) << "; ";

    }
    os << histogramAbscissas( h ) << ", ";
    os << histogramOrdinates( h ) << " ];";

    os.close();

    if ( verbose )
    {

      std::cout << "done" << std::endl;
      std::cout << "lower quantitative T1 boundary : "
                << lowerT1Boundary
                << " ms" << std::endl;
      std::cout << "upper quantitative T1 boundary : "
                << upperT1Boundary
                << " ms" << std::endl;

    }

  
    ///////////////////////////////////////////////////////////////////////////
    // computing the histogram of T1 on all the image
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "fitting the T1 histogram with a Gaussian mixture : "
                << std::flush;

    }

    gkg::Vector weights( histogramLevelCount );
    weights.fill( 1.0 );

    int32_t minimumGaussianCount = ( int32_t )( histogramParameters[ 4 ] +
                                                0.5 );
    int32_t maximumGaussianCount = ( int32_t )( histogramParameters[ 5 ] +
                                                0.5 );

    // looping over various Gaussian count
    int32_t optimalGaussianCount = 0;
    double minimumBayesianInformationCriterion = 1e38;
    gkg::Vector optimalGaussianParameters;
    gkg::Vector optimalFittedHistogramOrdinates;

    int32_t gaussianCount = 0;
    for ( gaussianCount = minimumGaussianCount;
          gaussianCount <= maximumGaussianCount;
          gaussianCount++ )
    {

      // initializing the mean/stdDev and scales of Gaussian parameters
      gkg::Vector meanT1s( gaussianCount );
      gkg::Vector stdDevT1s( gaussianCount );
      gkg::Vector scales( gaussianCount );

      double averageScale = ( normalizedHistogram ? 1.0 :
                              ( double )voxelCountInMask ) /
                            ( double )gaussianCount;

      int32_t g = 0;
      for ( g = 0; g < gaussianCount; g++ )
      {

        scales( g ) = averageScale;
        meanT1s( g ) = 350.0 + ( double )g * ( 700.0 - 350.0 ) /
                               ( double )( gaussianCount - 1 );
        stdDevT1s( g ) = 10.0;

      }

      // computing constrained parameters
      int32_t gaussianParameterCount = 3 * gaussianCount;
      gkg::Vector contrainedGaussianParameters( gaussianParameterCount ); 
      for ( g = 0; g < gaussianCount; g++ )
      {

        contrainedGaussianParameters( 3 * g + 0 ) = std::sqrt( scales( g ) );
        contrainedGaussianParameters( 3 * g + 1 ) = std::sqrt( meanT1s( g ) );
        contrainedGaussianParameters( 3 * g + 2 ) = std::sqrt( stdDevT1s( g ) );

      }

      // performing a Levenberg-Marquardt optimization to get the optimal
      // parameters for the mixture of Gaussian distributions
      gkg::ConstrainedGaussianMixtureFunction
             constrainedGaussianMixtureFunction( contrainedGaussianParameters );
      gkg::Matrix covariance;
      double chiSquarePerDegreeOfFreedom = 0.0; 
      int32_t maximumIterationCount =
                                  ( int32_t )( histogramParameters[ 6 ] + 0.5 );
      double absoluteError = ( int32_t )( histogramParameters[ 7 ] + 0.5 );
      double relativeError = ( int32_t )( histogramParameters[ 8 ] + 0.5 );

      //int32_t iterationCount = 
      factory->getLevenbergMarquardtFit( histogramAbscissas.toMatrix(),
                                         histogramOrdinates,
                                         weights,
                                         constrainedGaussianMixtureFunction,
                                         covariance,
                                         chiSquarePerDegreeOfFreedom,
                                         maximumIterationCount,
                                         absoluteError,
                                         relativeError );

      // computing unconstrained parameters from optimal constrained ones
      gkg::Vector gaussianParameters( gaussianParameterCount );
      double value = 0;
      for ( p = 0; p < gaussianParameterCount; p++ )
      {

        value = constrainedGaussianMixtureFunction.getParameters()( p );
        gaussianParameters( p ) = value * value;

      }

      // creating the unconstrained Gaussian Levenberg-Marquardt function
      gkg::GaussianMixtureFunction
        gaussianMixtureFunction( gaussianParameters );

      // computing the fitted histogram values
      gkg::Vector fittedHistogramOrdinates( histogramLevelCount );
      gkg::Vector histogramAbscissa( 1 );
      for ( h = 0; h < histogramLevelCount; h++ )
      {

        histogramAbscissa( 0 ) = histogramAbscissas( h );
        fittedHistogramOrdinates( h ) =
          gaussianMixtureFunction.getValueWithParametersAt(
                                                           histogramAbscissa,
                                                           gaussianParameters );
      }

      double bayesianInformationCriterion = 
                                          this->getBayesianInformationCriterion(
                                                       histogramOrdinates,
                                                       fittedHistogramOrdinates,
                                                       gaussianCount );
/*
      for ( g = 0; g < gaussianCount; g++ )
      {

        if ( ( gaussianParameters( 3 * g + 1 ) < smallestT1Value ) ||
             ( gaussianParameters( 3 * g + 1 ) > largestT1Value ) )
        {

          bayesianInformationCriterion += 100.0;

        }

      }
*/

      if ( verbose )
      {

        std::cout << gaussianCount << "(" << bayesianInformationCriterion
                  << ") "
                  << std::flush;

      }

      if ( bayesianInformationCriterion < minimumBayesianInformationCriterion )
      {

        optimalGaussianCount = gaussianCount;
        minimumBayesianInformationCriterion = bayesianInformationCriterion;
        optimalGaussianParameters = gaussianParameters;
        optimalFittedHistogramOrdinates = fittedHistogramOrdinates;

      }

    }
    if ( verbose )
    {

      std::cout << std::endl;

    }

    ///////////////////////////////////////////////////////////////////////////
    // ordering Gaussian distributions with respect to T1 mean
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "computing the histogram of quantitative T1 : "
                << std::flush;

    }

    std::vector< std::pair< int32_t, double > >
      indexToT1Lut( optimalGaussianCount );

    int32_t index = 0;
    int32_t g = 0;
    for ( g = 0; g < optimalGaussianCount; g++ )
    {

      index = 3 * g + 1;
      indexToT1Lut[ g ] = std::make_pair( index,
                                          optimalGaussianParameters( index ) );

    }

    std::sort( indexToT1Lut.begin(), indexToT1Lut.end(),
               gkg::RelaxometryT1CorticalLaminationMapperCommand::sortingT1 );

    gkg::Vector sortedOptimalGaussianParameters( optimalGaussianCount * 3 );
    for ( g = 0; g < optimalGaussianCount; g++ )
    {

      index = 3 * g;
      sortedOptimalGaussianParameters( index ) = 
                       optimalGaussianParameters( indexToT1Lut[ g ].first - 1 );
      sortedOptimalGaussianParameters( index + 1 ) = 
                       indexToT1Lut[ g ].second;
      sortedOptimalGaussianParameters( index + 2 ) = 
                       optimalGaussianParameters( indexToT1Lut[ g ].first + 1 );

      if ( verbose )
      {

        std::cout << "( " << sortedOptimalGaussianParameters( index )
                  << ", " << sortedOptimalGaussianParameters( index + 1 )
                  << ", " << sortedOptimalGaussianParameters( index + 2 )
                  << " ) "
                  << std::flush;

      }

    }
    if ( verbose )
    {

      std::cout << std::endl;

    }

    if ( verbose )
    {

      std::cout << std::endl;
      std::cout << "optimum number of components in Gaussian mixture : "
                << optimalGaussianCount
                << " ( ";

      for ( g = 0; g < optimalGaussianCount - 1; g++ )
      {

        std::cout << sortedOptimalGaussianParameters( 3 * g + 1 ) << ", ";

      }
      std::cout << sortedOptimalGaussianParameters( 3 * g + 1 ) << " )"
                << std::endl;

    }


    ///////////////////////////////////////////////////////////////////////////
    // constructing individual profiles of all Gaussian distributions
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "constructing individual Gaussian distributions : "
                << std::flush;

    }

    // storing in independent Gaussian parameter set(s)
    std::vector< gkg::Vector >
      vectorOfGaussianParameters( optimalGaussianCount );
    for ( g = 0; g < optimalGaussianCount; g++ )
    {

      gkg::Vector gaussianParameters( sortedOptimalGaussianParameters );

      int32_t gPrime = 0;
      for ( gPrime = 0; gPrime < optimalGaussianCount; gPrime++ )
      {

        if ( gPrime != g )
        {

          gaussianParameters( 3 * gPrime ) = 0.0;

        }

      }

      vectorOfGaussianParameters[ g ] = gaussianParameters;

    }

    // extracting individual profile of each Gaussian distribution
    gkg::GaussianMixtureFunction
      gaussianMixtureFunction( sortedOptimalGaussianParameters );
    gkg::Matrix individualGaussianHistogramOrdinates( optimalGaussianCount,
                                                      histogramLevelCount );

    for ( g = 0; g < optimalGaussianCount; g++ )
    {

      gkg::Vector histogramAbscissa( 1 );
      for ( h = 0; h < histogramLevelCount; h++ )
      {

        histogramAbscissa( 0 ) = histogramAbscissas( h );
        individualGaussianHistogramOrdinates( g, h ) =
          gaussianMixtureFunction.getValueWithParametersAt(
                                              histogramAbscissa,
                                              vectorOfGaussianParameters[ g ] );

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ///////////////////////////////////////////////////////////////////////////
    // exporting the measured and fitted histogram to matplotlib
    ///////////////////////////////////////////////////////////////////////////

    if ( !fileNameHistogram.empty() )
    {

      if ( verbose )
      {

        std::cout << "exporting the measured/fitted histogram to matplotlib : "
                  << std::flush;

      }

      std::ofstream os( fileNameHistogram.c_str() );
      os << "import numpy as np" << std::endl;
      os << "import matplotlib.pyplot as plt" << std::endl;
      os << std::endl;

      os << "T1 = [ " << std::flush;
      for ( h = 0; h < histogramLevelCount - 1; h++ )
      {

        os << histogramAbscissas( h ) << ", ";

      }
      os << histogramAbscissas( h ) << " ]" << std::endl;

      os << "hm = [ " << std::flush;
      for ( h = 0; h < histogramLevelCount - 1; h++ )
      {

        os << histogramOrdinates( h ) << ", ";

      }
      os << histogramOrdinates( h ) << " ]" << std::endl;

      os << "hf = [ " << std::flush;
      for ( h = 0; h < histogramLevelCount - 1; h++ )
      {

        os << optimalFittedHistogramOrdinates( h ) << ", ";

      }
      os << optimalFittedHistogramOrdinates( h ) << " ]" << std::endl;


      for ( g = 0; g < optimalGaussianCount; g++ )
      {

        os << "hi_" << g << " = [ " << std::flush;
        for ( h = 0; h < histogramLevelCount - 1; h++ )
        {

          os << individualGaussianHistogramOrdinates( g, h ) << ", ";

        }
        os << individualGaussianHistogramOrdinates( g, h ) << " ]" << std::endl;


      }

      // plotting curve(s)
      os << "fig = plt.figure()" << std::endl;
      os << std::endl;

      os << "markers = [ '.', '-' ]" << std::endl;
      os << "colors = [ 'r', 'b', 'g' ]" << std::endl;
      os << std::endl;

      os << "plt.plot( T1, " << std::endl;
      os << "          hm, " << std::endl;
      os << "          color=colors[ 0 ], " << std::endl;
      os << "          marker=markers[ 0 ], " << std::endl;
      os << "          linestyle='-', " << std::endl;
      os << "          label='measured T1 histogram' ) " << std::endl;
      os << "plt.plot( T1, " << std::endl;
      os << "          hf, " << std::endl;
      os << "          color=colors[ 1 ], " << std::endl;
      os << "          linestyle='-', " << std::endl;
      os << "          label='fitted T1 histogram' ) " << std::endl;
      for ( g = 0; g < optimalGaussianCount; g++ )
      {

        os << "plt.plot( T1, " << std::endl;
        os << "          hi_" << g << ", " << std::endl;
        os << "          color=colors[ 2 ], " << std::endl;
        os << "          linestyle='-', " << std::endl;
        os << "          label='Gaussian dist. @T1=" 
           << sortedOptimalGaussianParameters( 3 * g + 1 )
           << "ms' ) "
           << std::endl;

      }
      os << std::endl;
      os << "plt.grid( True )" << std::endl;
      os << "plt.legend()" << std::endl;
      os << std::endl;
      os << "plt.xlabel( 'time (ms)' )" << std::endl;
      os << "plt.ylabel( 'voxels' )" << std::endl;
      os << std::endl;
      os << "plt.show()" << std::endl;
      os << std::endl;

      os.close();

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }


    }

    ///////////////////////////////////////////////////////////////////////////
    // computing layer probablity map(s)
    ///////////////////////////////////////////////////////////////////////////

    if ( componentCount  == 1 )
    {

      if ( verbose )
      {

        std::cout << "computing layer probablity map(s) : "
                  << std::flush;

      }

      // first reallocating the output volume to the adequate size
      outputVolume.reallocate( inputSize,
                               outputSizeT + optimalGaussianCount + 3 );

      // computing layer probabilities
      std::vector< float > layerProbabilities( optimalGaussianCount );
      float sumOfScales = 0.0f;
      for ( g = 0; g < optimalGaussianCount; g++ )
      {

        sumOfScales += ( float )sortedOptimalGaussianParameters( 3 * g );

      }
      std::cout << "sumOfScales=" << sumOfScales << std::endl;
      for ( g = 0; g < optimalGaussianCount; g++ )
      {

        layerProbabilities[ g ] = ( float )sortedOptimalGaussianParameters(
                                                          3 * g ) / sumOfScales;

      }

      gkg::Vector t1Value( 1 );
      float probabilityOfT1 = 0.0f;
      float probabilityOfT1KnowingLayer = 0.0f;
      std::vector< float > probabilities( optimalGaussianCount );
      gkg::MaximumIndexFilter< std::vector< float >, int32_t >
        maximumIndexFilter;
      int32_t clusterIndex = 0;
      gkg::SumFilter< std::vector< float >, float > sumFilter;
      float sumOfProbabilities = 0.0f;
      for ( voxel.z = 0; voxel.z < inputSize.z; voxel.z++ )
      {

        if ( verbose )
        {

          if ( voxel.z != 0 )
          {

            std::cout << gkg::Eraser( 16 );

          }
          std::cout << "[ " << std::setw( 4 ) << voxel.z + 1
                    << " / " << std::setw( 4 ) << inputSize.z
                    << " ] " << std::flush;

        }

        for ( voxel.y = 0; voxel.y < inputSize.y; voxel.y++ )
        {

          for ( voxel.x = 0; voxel.x < inputSize.x; voxel.x++ )
          {

            if ( mask( voxel ) )
            {

              t1Value( 0 ) = t1Volume( voxel );
              probabilityOfT1 = 
                      ( float )gaussianMixtureFunction.getValueWithParametersAt(
                                              t1Value,
                                              sortedOptimalGaussianParameters );
              for ( g = 0; g < optimalGaussianCount; g++ )
              {

                
                probabilityOfT1KnowingLayer =
                      ( float )gaussianMixtureFunction.getValueWithParametersAt(
                                              t1Value,
                                              vectorOfGaussianParameters[ g ] );
                probabilities[ g ] = 
                     ( probabilityOfT1KnowingLayer * layerProbabilities[ g ] ) /
                     probabilityOfT1;
                outputVolume( voxel, outputSizeT + g ) = probabilities[ g ];

              }
              maximumIndexFilter.filter( probabilities, clusterIndex );
              outputVolume( voxel, outputSizeT + optimalGaussianCount ) =
                                                               clusterIndex + 1;
              maximumIndexFilter.filter( probabilities, clusterIndex );
              sumFilter.filter( probabilities, sumOfProbabilities );
              outputVolume( voxel, outputSizeT + optimalGaussianCount + 1 ) =
                                                             sumOfProbabilities;
              outputVolume( voxel, outputSizeT + optimalGaussianCount + 2 ) =
                ( float )this->getClosestT1Class(
                                              t1Value( 0 ),
                                              sortedOptimalGaussianParameters );

            }

          }

        }

      }

      if ( verbose )
      {

        std::cout << std::endl;

      }

    }
    

    ///////////////////////////////////////////////////////////////////////////
    // writing output volume
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing results : " << std::flush;

    }

    gkg::Writer::getInstance().write( fileNameOut, outputVolume );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::RelaxometryT1CorticalLaminationMapperCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameTimes, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameOut, "
             "const std::string& fileNameFittedData, "
             "const std::string& fileNameHistogram, "
             "const std::string& optimizerType, "
             "std::vector< double >& optimizerParameters, "
             "int32_t histogramLevelCount, "
             "float parzenWindowWidth, "
             "std::vector< double >& outlierValues, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


double 
gkg::RelaxometryT1CorticalLaminationMapperCommand::
                                                getBayesianInformationCriterion(
                                  const gkg::Vector& measuredHistogramOrdinates,
                                  const gkg::Vector& fittedHistogramOrdinates,
                                  int32_t gaussianCount ) const
{

  try
  {

    int32_t measurementCount = measuredHistogramOrdinates.getSize();

    double value = 0.0;
    double errorVariance = 0.0;
    int32_t m = 0;
    for ( m = 0; m < measurementCount; m++ )
    {

      value = measuredHistogramOrdinates( m ) - fittedHistogramOrdinates( m );
      errorVariance += value * value;

    }
    errorVariance /= ( double )measurementCount;

    return std::log( measurementCount ) * gaussianCount * 3 +
           measurementCount * std::log( errorVariance ); 

  }
  GKG_CATCH( "double "
             "gkg::RelaxometryT1CorticalLaminationMapperCommand:: "
             "getBayesianInformationCriterion( "
             "const gkg::Vector& measuredHistogramOrdinates, "
             "const gkg::Vector& fittedHistogramOrdinates, "
             "int32_t gaussianCount ) const" );

}


int32_t 
gkg::RelaxometryT1CorticalLaminationMapperCommand::getClosestT1Class(
                     double t1Value,
                     const  gkg::Vector& sortedOptimalGaussianParameters ) const
{

  try
  {

    int32_t optimalGaussianCount = sortedOptimalGaussianParameters.getSize() /
                                   3;
    double bestDistance = 1e38;
    int32_t bestClass = 1;

    double distance = 0.0;
    int32_t g = 0;
    for ( g = 0; g < optimalGaussianCount; g++ )
    {

      distance = std::fabs( t1Value - 
                            sortedOptimalGaussianParameters( 3 * g + 1 ) );
      if ( distance < bestDistance )
      {

        bestDistance = distance;
        bestClass = g + 1;

      }

    }
    return bestClass;

  }
  GKG_CATCH( "int32_t "
             "gkg::RelaxometryT1CorticalLaminationMapperCommand::"
             "getClosestT1Class( "
             "double t1Value, "
             "const  gkg::Vector& sortedOptimalGaussianParameters ) const" );

}


RegisterCommandCreator(
             RelaxometryT1CorticalLaminationMapperCommand,
    	     DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    	     DECLARE_STRING_PARAMETER_HELP( fileNameTimes ) +
    	     DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    	     DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    	     DECLARE_STRING_PARAMETER_HELP( fileNameFittedData ) +
    	     DECLARE_STRING_PARAMETER_HELP( fileNameHistogram ) +
    	     DECLARE_INTEGER_PARAMETER_HELP( componentCount ) +
    	     DECLARE_STRING_PARAMETER_HELP( optimizerType ) +
    	     DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( optimizerParameters ) +
    	     DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( lowerBoundaries ) +
    	     DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( upperBoundaries ) +
    	     DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( histogramParameters ) +
    	     DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    	     DECLARE_STRING_PARAMETER_HELP( format ) +
    	     DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );

