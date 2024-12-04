#include <gkg-qmri-plugin-functors/RelaxometryMapper/RelaxometryMapperCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/LevenbergMarquardtFunction.h>
#include <gkg-processing-numericalanalysis/NelderMeadSimplexFunction.h>
#include <gkg-processing-algobase/AverageFilter_i.h>
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
#define TURBO_SPIN_ECHO_OFFSET 2048


///////////////////////////////////////////////////////////////////////////////
// case of Spin Echo / Gradient Echo EPI pulse sequence with variable TE
//   - Spin Echo -> rho/T2 mapping
//   - Gradient Echo -> rho/T2* mapping
///////////////////////////////////////////////////////////////////////////////

//
// class RhoT2MagnetizationFunction
//

class RhoT2MagnetizationFunction : public gkg::LevenbergMarquardtFunction,
                                   public gkg::NelderMeadSimplexFunction
{

  public:

    RhoT2MagnetizationFunction( const gkg::Vector& parameters );
    ~RhoT2MagnetizationFunction();

    double getValueWithParametersAt( const gkg::Vector& x,
                                     const gkg::Vector& p ) const;
    gkg::Vector getDerivativeWithParametersAt( const gkg::Vector& x,
                                               const gkg::Vector& p ) const;

    void setMeasurements( const gkg::Vector& measurements );
    void setEchoTimes( const gkg::Vector& echoTimes );
    double getValueAt( const gkg::Vector& parameters ) const;

  protected:

    gkg::Vector _measurements;
    gkg::Vector _echoTimes;

};


RhoT2MagnetizationFunction::RhoT2MagnetizationFunction(
                                               const gkg::Vector& parameters )
                           : gkg::LevenbergMarquardtFunction( parameters ),
                             gkg::NelderMeadSimplexFunction( 2 )
{
}


RhoT2MagnetizationFunction::~RhoT2MagnetizationFunction()
{
}


double RhoT2MagnetizationFunction::getValueWithParametersAt(
                                                    const gkg::Vector& x,
                                                    const gkg::Vector& p ) const
{

  try
  {

    // p(0) -> rho
    // p(1) -> T2 or T2*

    return p( 0 ) * std::exp( - x( 0 ) / p( 1 ) );

  }
  GKG_CATCH( "double RhoT2MagnetizationFunction::getValueWithParametersAt( "
             "const gkg::Vector& x, "
             "const gkg::Vector& p ) const" );

}


gkg::Vector RhoT2MagnetizationFunction::getDerivativeWithParametersAt(
                                                    const gkg::Vector& x,
                                                    const gkg::Vector& p ) const
{

  try
  {

    gkg::Vector derivative( 2 );

    derivative( 0 ) = std::exp( - x( 0 ) / p( 1 ) );
    derivative( 1 ) = p( 0 ) * ( x( 0 ) / ( p( 1 ) * p( 1 ) ) ) *
                      std::exp( - x( 0 ) / p( 1 ) );

    return derivative;

  }
  GKG_CATCH( "gkg::Vector RhoT2MagnetizationFunction::"
             "getDerivativeWithParametersAt( "
             "const gkg::Vector& x, "
             "const gkg::Vector& p ) const" );

}


void RhoT2MagnetizationFunction::setMeasurements(
                                               const gkg::Vector& measurements )
{

  try
  {

    _measurements = measurements;

  }
  GKG_CATCH( "void RhoT2MagnetizationFunction::setMeasurements( "
             "const gkg::Vector& measurements )" );

}


void RhoT2MagnetizationFunction::setEchoTimes( const gkg::Vector& echoTimes )
{

  try
  {

    _echoTimes = echoTimes;

  }
  GKG_CATCH( "void RhoT2MagnetizationFunction::setEchoTimes( "
             "const gkg::Vector& echoTimes )" );

}


double RhoT2MagnetizationFunction::getValueAt(
                                           const gkg::Vector& parameters ) const
{

  try
  {

    int32_t trialCount = _measurements.getSize();
    if ( _echoTimes.getSize() != trialCount )
    {

      throw std::runtime_error(
                         "incompatible measurement and echo time vector size" );

    }
    int32_t t = 0;
    double squareError = 0.0;
    double error = 0.0;
    for ( t = 0; t < trialCount; t++ )
    {

      error = _measurements( t ) - 
              ( parameters( 0 ) * std::exp( - _echoTimes( 0 ) /
                                              parameters( 1 ) ) );
      squareError += error * error; 

    }
    return squareError;

  }
  GKG_CATCH( "double RhoT2MagnetizationFunction::getValueAt( "
             "const gkg::Vector& parameters ) const" );

}


///////////////////////////////////////////////////////////////////////////////
// case of Inversion Recovery Spin Echo pulse sequence with variable TI
//   -> rho/T1 mapping
// As magnetization may be negative with inversion recovery pulses:
// the EPI sequence takes the absolute value
// the turbo spin echo sequence adds an offset
///////////////////////////////////////////////////////////////////////////////

//
// class RhoT1MagnetizationFunction
//

class RhoT1MagnetizationFunction : public gkg::LevenbergMarquardtFunction,
                                   public gkg::NelderMeadSimplexFunction
{

  public:

    enum Type
    {

      EchoPlanar,
      TurboSpinEcho

    };

    RhoT1MagnetizationFunction( const gkg::Vector& parameters,
                                Type type );
    ~RhoT1MagnetizationFunction();

    double getValueWithParametersAt( const gkg::Vector& x,
                                     const gkg::Vector& p ) const;
    gkg::Vector getDerivativeWithParametersAt( const gkg::Vector& x,
                                               const gkg::Vector& p ) const;

    void setMeasurements( const gkg::Vector& measurements );
    void setInversionTimes( const gkg::Vector& echoTimes );
    double getValueAt( const gkg::Vector& parameters ) const;

  protected:

    Type _type;
    gkg::Vector _measurements;
    gkg::Vector _inversionTimes;

};


RhoT1MagnetizationFunction::RhoT1MagnetizationFunction(
                                         const gkg::Vector& parameters,
                                         RhoT1MagnetizationFunction::Type type )
                           : gkg::LevenbergMarquardtFunction( parameters ),
                             gkg::NelderMeadSimplexFunction( 2 ),
                             _type( type )
{
}


RhoT1MagnetizationFunction::~RhoT1MagnetizationFunction()
{
}


double RhoT1MagnetizationFunction::getValueWithParametersAt(
                                                    const gkg::Vector& x,
                                                    const gkg::Vector& p ) const
{

  try
  {

    double value = 0.0;
    switch ( _type )
    {

      // p(0) -> rho
      // p(1) -> T1
      
      case RhoT1MagnetizationFunction::EchoPlanar:

        if ( p( 1 ) < ( x( 0 ) / std::log( 2 ) ) )
        {

          value =  p( 0 ) * ( 1.0 - 2.0 * std::exp( - x( 0 ) / p( 1 ) ) );

        }
        else
        {

          value =  p( 0 ) * ( 2.0 * std::exp( - x( 0 ) / p( 1 ) ) - 1.0 );

        }
        break;

      case RhoT1MagnetizationFunction::TurboSpinEcho:

        value =  p( 0 ) * ( 1.0 - 2.0 * std::exp( - x( 0 ) / p( 1 ) ) ) +
                 TURBO_SPIN_ECHO_OFFSET;
        break;

    }
    
    return value;

  }
  GKG_CATCH( "double RhoT1MagnetizationFunction::getValueWithParametersAt( "
             "const gkg::Vector& x, "
             "const gkg::Vector& p ) const" );

}


gkg::Vector RhoT1MagnetizationFunction::getDerivativeWithParametersAt(
                                                    const gkg::Vector& x,
                                                    const gkg::Vector& p ) const
{

  try
  {

    gkg::Vector derivative( 2 );
    switch ( _type )
    {

      case RhoT1MagnetizationFunction::EchoPlanar:

        if ( p( 1 ) < ( x( 0 ) / std::log( 2 ) ) )
        {

          derivative( 0 ) = 1.0 - 2.0 * std::exp( - x( 0 ) / p( 1 ) );
          derivative( 1 ) = - 2.0 * p( 0 ) * ( x( 0 ) / ( p( 1 ) * p( 1 ) ) ) *
                            std::exp( - x( 0 ) / p( 1 ) );

        }
        else
        {

          derivative( 0 ) = 2.0 * std::exp( - x( 0 ) / p( 1 ) ) - 1.0;
          derivative( 1 ) = 2.0 * p( 0 ) * ( x( 0 ) / ( p( 1 ) * p( 1 ) ) ) *
                            std::exp( - x( 0 ) / p( 1 ) );

        }
        break;

      case RhoT1MagnetizationFunction::TurboSpinEcho:

        derivative( 0 ) = 1.0 - 2.0 * std::exp( - x( 0 ) / p( 1 ) );
        derivative( 1 ) = - 2.0 * p( 0 ) * ( x( 0 ) / ( p( 1 ) * p( 1 ) ) ) *
                          std::exp( - x( 0 ) / p( 1 ) );
        break;

    }
    return derivative;

  }
  GKG_CATCH( "gkg::Vector RhoT1MagnetizationFunction::"
             "getDerivativeWithParametersAt( "
             "const gkg::Vector& x, "
             "const gkg::Vector& p ) const" );

}


void RhoT1MagnetizationFunction::setMeasurements(
                                               const gkg::Vector& measurements )
{

  try
  {

    _measurements = measurements;

  }
  GKG_CATCH( "void RhoT1MagnetizationFunction::setMeasurements( "
             "const gkg::Vector& measurements )" );

}


void RhoT1MagnetizationFunction::setInversionTimes(
                                             const gkg::Vector& inversionTimes )
{

  try
  {

    _inversionTimes = inversionTimes;

  }
  GKG_CATCH( "void RhoT1MagnetizationFunction::setInversionTimes( "
             "const gkg::Vector& inversionTimes )" );

}


double RhoT1MagnetizationFunction::getValueAt(
                                           const gkg::Vector& parameters ) const
{

  try
  {

    int32_t trialCount = _measurements.getSize();
    if ( _inversionTimes.getSize() != trialCount )
    {

      throw std::runtime_error(
                         "incompatible measurement and echo time vector size" );

    }
    int32_t t = 0;
    double value = 0.0;
    double squareError = 0.0;
    double error = 0.0;


    switch ( _type )
    {

      case RhoT1MagnetizationFunction::EchoPlanar:

        for ( t = 0; t < trialCount; t++ )
        {

          value =  std::fabs( parameters( 0 ) * ( 1.0 - 2.0 * 
                   std::exp( - _inversionTimes( t ) / parameters( 1 ) ) ) );
          error = _measurements( t ) - value;
          squareError += error * error; 

        }
        break;

      case RhoT1MagnetizationFunction::TurboSpinEcho:

        for ( t = 0; t < trialCount; t++ )
        {

          value =  TURBO_SPIN_ECHO_OFFSET + parameters( 0 ) * ( 1.0 - 2.0 * 
                   std::exp( - _inversionTimes( t ) / parameters( 1 ) ) );
          error = _measurements( t ) - value;
          squareError += error * error; 

        }
        break;

    }
    return squareError;

  }
  GKG_CATCH( "double RhoT1MagnetizationFunction::getValueAt( "
             "const gkg::Vector& parameters ) const" );

}


///////////////////////////////////////////////////////////////////////////////
// file time(s) reading
///////////////////////////////////////////////////////////////////////////////


gkg::Vector readTimeFile( const std::string& fileName, int32_t trialCount )
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
//  class RelaxometryMapperCommand
//


gkg::RelaxometryMapperCommand::RelaxometryMapperCommand( int32_t argc,
                                                         char* argv[], 
                                                         bool loadPlugin,
                                                         bool removeFirst )
                             : gkg::Command( argc, argv,
                                             loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::RelaxometryMapperCommand::RelaxometryMapperCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::RelaxometryMapperCommand::RelaxometryMapperCommand(
                               const std::string& fileNameIn, 
                               const std::string& fileNameTimes,
                               const std::string& fileNameMask,
                               const std::string& fileNameOut,
                               const std::string& fileNameFittedData,
                               const std::string& relaxometryType,
                               const std::string& optimizerType,
                               std::vector< double >& optimizerParameters,
                               int32_t concatenations,
                               double repetitionTimePerSlice,
                               std::vector< double >& outlierValues,
                               bool ascii,
                               const std::string& format,
                               bool verbose )
                             : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameTimes, fileNameMask, fileNameOut,
             fileNameFittedData, relaxometryType, optimizerType,
             optimizerParameters, concatenations, repetitionTimePerSlice,
             outlierValues, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::RelaxometryMapperCommand::RelaxometryMapperCommand( "
             "const std::string& fileNameIn, const std::string& fileNameTimes, "
             "const std::string& fileNameMask, const std::string& fileNameOut, "
             "const std::string& fileNameFittedData, "
             "const std::string& relaxometryType, "
             "const std::string& optimizerType, "
             "const std::vector< double >& optimizerParameters, "
             "int32_t concatenations, double repetitionTimePerSlice, "
             "std::vector< double >& outlierValues, bool ascii, "
             "const std::string& format, bool verbose )" );

}


gkg::RelaxometryMapperCommand::RelaxometryMapperCommand(
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
    DECLARE_STRING_PARAMETER( parameters, std::string, relaxometryType );
    DECLARE_STRING_PARAMETER( parameters, std::string, optimizerType );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           optimizerParameters );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, concatenations );
    DECLARE_FLOATING_PARAMETER( parameters, double, repetitionTimePerSlice );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           outlierValues );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameTimes, fileNameMask, fileNameOut,
             fileNameFittedData, relaxometryType, optimizerType,
             optimizerParameters, concatenations, repetitionTimePerSlice,
             outlierValues, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::RelaxometryMapperCommand::RelaxometryMapperCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::RelaxometryMapperCommand::~RelaxometryMapperCommand()
{
}


std::string gkg::RelaxometryMapperCommand::getStaticName()
{

  try
  {

    return "RelaxometryMapper";

  }
  GKG_CATCH( "std::string gkg::RelaxometryMapperCommand::getStaticName()" );

}


void gkg::RelaxometryMapperCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameTimes;
    std::string fileNameMask;
    std::string fileNameRelaxometry;
    std::string fileNameFittedData;
    std::string relaxometryType;
    std::string optimizerType = "levenberg-marquardt";
    std::vector< double > optimizerParameters;
    int concatenations = 1;
    double repetitionTimePerSlice = 0.0;
    std::vector< double > outlierValues;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Relaxometry mapper relying on: \n"
                                  " - Spin Echo EPI with variable TE "
                                  "-> rho/T2 mapping\n"
                                  " - Gradient Echo EPI with variable TE "
                                  "-> rho/T2* mapping\n"
                                  " - Inversion Recovery Spin Echo EPI with "
                                  " variable TI -> rho/T1 mapping",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-t",
                                 "Input TE or TI times file name: \n"
                                 " - rho/T2 mapping -> TE times\n"
                                 " - rho/T2* mapping -> TE times\n"
                                 " - rho/T1 mapping -> TI times",
                                 fileNameTimes );
    application.addSingleOption( "-m",
                                 "Mask file name",
                                 fileNameMask );
    application.addSingleOption( "-o",
                                 "Output relaxometry file name",
                                 fileNameRelaxometry );
    application.addSingleOption( "-fittedData",
                                 "Output volume file name of the fitted data",
                                 fileNameFittedData,
                                 true );
    application.addSingleOption( "-relaxometryType",
                                 "Relaxometry type:\n"
                                 " - rhot2   : Rho/T2 mapping,\n"
                                 " - rhot2star : Rho/T2* mapping,\n"
                                 " - rhot1-epi   : Rho/T1 mapping using EPI,\n"
                                 " - rhot1-tse   : Rho/T1 mapping using TSE",
                                 relaxometryType );
    application.addSingleOption( "-optimizerType",
                                 "Optimizer type:\n"
                                 " - levenberg-marquardt\n"
                                 " - nelder-mead-simplex\n"
                                 " - least-square",
                                 optimizerType,
                                 true );
    application.addSeriesOption( "-optimizerParameters",
                                 "Parameters used during optimization and "
                                 "depending on the optmizer type:\n"
                                 "- in case of levenberg-marquardt,\n"
                                 "   <P1>: initial Rho value\n"
                                 "   <P2>: initial T2 / T2* / T1 value\n"
                                 "   <P3>: maximum iteration count\n"
                                 "   <P4>: absolute error\n"
                                 "   <P5>: relative error\n"
                                 "   (default= 1.0 / 1.0 / 500 /"
                                 " 1e-4 / 1e-4)\n\n"
                                 "- in case of nelder-mead-simplex,\n"
                                 "   <P1>: initial Rho value\n"
                                 "   <P2>: initial T2 / T2* / T1 value\n"
                                 "   <P3>: delta Rho value\n"
                                 "   <P4>: delta T2 / T2* / T1 value\n"
                                 "   <P5>: maximum iteration count\n"
                                 "   <P6>: test size\n"
                                 "   (default= 1.0 / 1.0 / 1.0 / 1.0 / 500 /"
                                 " 1e-2)\n\n"
                                 "- in case of least-square,\n"
                                 "   N/A\n",
                                 optimizerParameters,
                                 0, 6 );
    application.addSingleOption( "-concatenations",
                                 "Number of concatenations for the TI "
                                 "correction by slice"
                                 "(default=1.0)",
                                 concatenations,
                                 true );
    application.addSingleOption( "-repetitionTimePerSlice",
                                 "Repetition time for the acquisition of a "
                                 "single slice (default=0.0)",
                                 repetitionTimePerSlice,
                                 true );
    application.addSeriesOption( "-outlierValues",
                                 "Outlier values to discard bad fits"
                                 "(default=10000.0, 10000.0 )",
                                 outlierValues,
                                 0, 2 );
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

    execute( fileNameIn, fileNameTimes, fileNameMask, fileNameRelaxometry,
             fileNameFittedData, relaxometryType, optimizerType,
             optimizerParameters, concatenations, repetitionTimePerSlice,
             outlierValues, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::RelaxometryMapperCommand::parse()" );

}


void gkg::RelaxometryMapperCommand::execute(
                               const std::string& fileNameIn, 
                               const std::string& fileNameTimes,
                               const std::string& fileNameMask,
                               const std::string& fileNameOut,
                               const std::string& fileNameFittedData,
                               const std::string& relaxometryType,
                               const std::string& optimizerType,
                               std::vector< double >& optimizerParameters,
                               int32_t concatenations,
                               double repetitionTimePerSlice,
                               std::vector< double >& outlierValues,
                               bool /* ascii */,
                               const std::string& /* format */,
                               bool verbose )
{

  try
  {

    ///////////////////////////////////////////////////////////////////////////
    // sanity checks
    ///////////////////////////////////////////////////////////////////////////
    if ( ( relaxometryType != "rhot2" ) &&
         ( relaxometryType != "rhot2star" ) &&
         ( relaxometryType != "rhot1-epi" ) &&
         ( relaxometryType != "rhot1-tse" ) )
    {

      throw std::runtime_error( "bad relaxometry type" );

    }

    if ( optimizerType == "levenberg-marquardt" )
    {

      if ( optimizerParameters.empty() )
      {

        optimizerParameters.resize( 5U );
        optimizerParameters[ 0 ] = 1.0;
        optimizerParameters[ 1 ] = 1.0;
        optimizerParameters[ 2 ] = 500.0;
        optimizerParameters[ 3 ] = 1e-4;
        optimizerParameters[ 4 ] = 1e-4;

      }
      else if ( optimizerParameters.size() != 5U )
      {

        throw std::runtime_error(
                   "bad optimizer parameter count for levenberg-marquardt" );

      }

    }
    else if ( optimizerType == "nelder-mead-simplex" )
    {

      if ( optimizerParameters.empty() )
      {

        optimizerParameters.resize( 6U );
        optimizerParameters[ 0 ] = 1.0;
        optimizerParameters[ 1 ] = 1.0;
        optimizerParameters[ 2 ] = 1.0;
        optimizerParameters[ 3 ] = 1.0;
        optimizerParameters[ 4 ] = 500.0;
        optimizerParameters[ 5 ] = 1e-2;

      }
      else if ( optimizerParameters.size() != 6U )
      {

        throw std::runtime_error(
                   "bad optimizer parameter count for nelder-mead-simplex" );

      }

    }
    else if ( optimizerType == "least-square" )
    {

      if ( !optimizerParameters.empty() )
      {

        throw std::runtime_error(
                   "bad optimizer parameter count for least-square" );

      }

    }

    if ( verbose &&
         ( ( optimizerType == "levenberg-marquardt" ) || 
           ( optimizerType == "nelder-mead-simplex" ) ) )
    {

      if ( relaxometryType == "rhot2" )
      {

        std::cout << "initial rho : " << optimizerParameters[ 0 ] << std::endl;
        std::cout << "initial T2 : " << optimizerParameters[ 1 ] << std::endl;

      }
      else if ( relaxometryType == "rhot2star" )
      {

        std::cout << "initial rho : " << optimizerParameters[ 0 ] << std::endl;
        std::cout << "initial T2* : " << optimizerParameters[ 1 ] << std::endl;

      }
      else if ( ( relaxometryType == "rhot1-epi" ) || 
                ( relaxometryType == "rhot1-tse" ) )
      {

        std::cout << "initial rho : " << optimizerParameters[ 0 ] << std::endl;
        std::cout << "initial T1 : " << optimizerParameters[ 1 ] << std::endl;

      }

    }

    if ( outlierValues.empty() )
    {

      outlierValues.resize( 2U );
      outlierValues[ 0 ] = 10000.0;
      outlierValues[ 1 ] = 10000.0;

    }

    ///////////////////////////////////////////////////////////////////////////
    // reading input data
    ///////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "reading input data : " << std::flush;

    }
    gkg::Volume< float > data;
    gkg::TypedVolumeReaderProcess< float > volumeReaderProcess( data );
    volumeReaderProcess.execute( fileNameIn );

    int32_t sizeX = data.getSizeX();
    int32_t sizeY = data.getSizeY();
    int32_t sizeZ = data.getSizeZ();
    gkg::Vector3d< double > resolution;
    double resolutionT = 1.0;
    data.getResolution( resolution, resolutionT );
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
    int32_t trialCount = data.getSizeT();
    gkg::Vector T = readTimeFile( fileNameTimes, trialCount );
    gkg::Vector Tcorrected = T;
    int32_t t;

    if ( verbose )
    {

      if ( ( relaxometryType == "rhot2" ) ||
           ( relaxometryType == "rhot2star" ) )
      {

        std::cout << "TE -> ";

      }
      else if ( ( relaxometryType == "rhot1-epi" ) ||
                ( relaxometryType == "rhot1-tse" ) )
      {

        std::cout << "TI -> ";

      }
      for ( t = 0; t < trialCount; t++ )
      {

        std::cout << T( t ) << "ms ";

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
    gkg::Reader::getInstance().read( fileNameMask, mask );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ///////////////////////////////////////////////////////////////////////////
    // getting numerical analysis implementation factory
    ///////////////////////////////////////////////////////////////////////////

    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();


    ///////////////////////////////////////////////////////////////////////////
    // allocating output result
    ///////////////////////////////////////////////////////////////////////////
    int32_t outputSizeT = 2;
    if ( optimizerType == "levenberg-marquardt" )
    {

      // we also save the standard deviations, the chi square per degree
      // of freedom and the failed mask
      outputSizeT += 4;

    }
    else if ( optimizerType == "nelder-mead-simplex" )
    {

      // we also save the test size and the failed mask
      outputSizeT += 2;

    }
    else if ( optimizerType == "least-square" )
    {

      // we also save the failed mask
      outputSizeT += 1;

    }

    gkg::Volume< float > relaxometry( sizeX, sizeY, sizeZ, outputSizeT );
    relaxometry.fill( 0.0f );
    relaxometry.getHeader().addAttribute( "resolutionX", resolution.x );
    relaxometry.getHeader().addAttribute( "resolutionY", resolution.y );
    relaxometry.getHeader().addAttribute( "resolutionZ", resolution.z );


    ///////////////////////////////////////////////////////////////////////////
    // fitting data
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      if ( relaxometryType == "rhot2" )
      {

        std::cout << "fitting Rho/T2 : " << std::flush;

      }
      else if ( relaxometryType == "rhot2star" )
      {

        std::cout << "fitting Rho/T2* : " << std::flush;

      }
      else if ( ( relaxometryType == "rhot1-epi" ) ||
                ( relaxometryType == "rhot1-tse" ) )
      {

        std::cout << "fitting Rho/T1 : " << std::flush;

      }

    }

    int32_t x, y, z, j;
    for ( z = 0; z < sizeZ; z++ )
    {

      if ( verbose )
      {

        if ( z != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << "slice[ " << std::setw( 4 ) << z + 1 
                  << " / " << std::setw( 4 ) << sizeZ
                  << " ] " << std::flush;

      }


      //
      //  Correct the list of TI for each slice
      //
      if ( ( ( relaxometryType == "rhot1-epi" ) ||
             ( relaxometryType == "rhot1-tse" ) ) &&
           ( repetitionTimePerSlice != 0.0 ) )
      {

        for ( t = 0; t < trialCount; t++ )
        {

          for ( j = 0; j < ( sizeZ / concatenations ); j++ )
          {

            if ( ( z > ( sizeZ - ( j + 1 ) * concatenations - 1 ) ) &&
                 ( z < ( sizeZ - j * concatenations ) ) )
            {

              Tcorrected( t ) = T( t ) + j * repetitionTimePerSlice;

            }

          }

        }

      }
      for ( y = 0; y < sizeY; y++ )
      {

        if ( verbose )
        {

          if ( y != 0 )
          {

            std::cout << gkg::Eraser( 20 );

          }
          std::cout << "line[ " << std::setw( 4 ) << y + 1 
                    << " / " << std::setw( 4 ) << sizeY
                    << " ] " << std::flush;

        }
        for ( x = 0; x < sizeX; x++ )
        {

          if ( mask( x, y, z ) )
          {


            //////////////////////// LEVENBERG-MARQUADT ///////////////////////
            if ( optimizerType == "levenberg-marquardt" )
            {

              // collecting measurements
              gkg::Vector Y( trialCount );
              gkg::Vector W( trialCount );
              for ( t = 0; t < trialCount; t++ )
              {

                Y( t ) = ( double )data( x, y, z, t );
                W( t ) = 1.0;

              }

              // preparing Levenberg-Marquardt parameters
              gkg::Vector initialParameters( 2 );
              initialParameters( 0 ) = optimizerParameters[ 0 ];
              initialParameters( 1 ) = optimizerParameters[ 1 ];
              int32_t iterationCount = ( int32_t )( optimizerParameters[ 2 ] +
                                                    0.5 );
              double absoluteError = optimizerParameters[ 3 ];
              double relativeError = optimizerParameters[ 4 ];

              // allocating magnetization function
              gkg::LevenbergMarquardtFunction*
                magnetizationFunction = 0;
              if ( ( relaxometryType == "rhot2" ) ||
                   ( relaxometryType == "rhot2star" )  )
              {

                magnetizationFunction = new RhoT2MagnetizationFunction(
                                                          initialParameters );

              }
              else if ( relaxometryType == "rhot1-epi" )
              {

                magnetizationFunction = new RhoT1MagnetizationFunction(
                                       initialParameters,
                                       RhoT1MagnetizationFunction::EchoPlanar );

              }
              else if ( relaxometryType == "rhot1-tse" )
              {

                magnetizationFunction = new RhoT1MagnetizationFunction(
                                    initialParameters,
                                    RhoT1MagnetizationFunction::TurboSpinEcho );

              }

              gkg::Matrix covariance;
              double chiSquarePerDegreeOfFreedom = 0.0;
              factory->getLevenbergMarquardtFit( Tcorrected.toMatrix(), Y, W,
                                                 *magnetizationFunction,
                                                 covariance,
                                                 chiSquarePerDegreeOfFreedom,
                                                 iterationCount,
                                                 absoluteError,
                                                 relativeError );

              relaxometry( x, y, z, 0 ) = 
                          ( float )magnetizationFunction->getParameters()( 0 );
              relaxometry( x, y, z, 1 ) = 
                          ( float )magnetizationFunction->getParameters()( 1 );
              relaxometry( x, y, z, 2 ) = 
                          ( float )std::sqrt( covariance( 0, 0 ) );
              relaxometry( x, y, z, 3 ) = 
                          ( float )std::sqrt( covariance( 1, 1 ) );
              relaxometry( x, y, z, 4 ) =
                          ( float )chiSquarePerDegreeOfFreedom;

              // deleting magnetization function
              delete magnetizationFunction;

              // making sanity checks
              if ( ( relaxometry( x, y, z, 0 ) < 0.0 ) ||
                   ( relaxometry( x, y, z, 0 ) > outlierValues[ 0 ] ) ||
                   ( relaxometry( x, y, z, 1 ) < 0.0 ) ||
                   ( relaxometry( x, y, z, 1 ) > outlierValues[ 1 ] ) )
              {

                relaxometry( x, y, z, 0 ) = 0.0;
                relaxometry( x, y, z, 1 ) = 0.0;
                relaxometry( x, y, z, 2 ) = 0.0;
                relaxometry( x, y, z, 3 ) = 0.0;
                relaxometry( x, y, z, 4 ) = 0.0;
                relaxometry( x, y, z, 5 ) = ( float )UNCORRECTED_LABEL;

              }

            }
            /////////////////////// NELDER-MEAD SIMPLEX ///////////////////////
            else if ( optimizerType == "nelder-mead-simplex" )
            {

              // collecting measurements
              gkg::Vector Y( trialCount );
              for ( t = 0; t < trialCount; t++ )
              {

                Y( t ) = ( double )data( x, y, z, t );

              }

              // preparing Nelder-Mead parameters
              gkg::Vector parameters( 2 );
              parameters( 0 ) = optimizerParameters[ 0 ];
              parameters( 1 ) = optimizerParameters[ 1 ];
              gkg::Vector deltaParameters( 2 );
              deltaParameters( 0 ) = optimizerParameters[ 2 ];
              deltaParameters( 1 ) = optimizerParameters[ 3 ];
              int32_t maximumIterationCount =
                                 ( int32_t )( optimizerParameters[ 4 ] + 0.5 );
              double maximumTestSize = optimizerParameters[ 5 ];

              // allocating magnetization function
              gkg::NelderMeadSimplexFunction*
                magnetizationFunction = 0;
              if ( ( relaxometryType == "rhot2" ) ||
                   ( relaxometryType == "rhot2star" )  )
              {

                RhoT2MagnetizationFunction* rhoT2MagnetizationFunction =
                           new RhoT2MagnetizationFunction( parameters );
                rhoT2MagnetizationFunction->setMeasurements( Y );
                rhoT2MagnetizationFunction->setEchoTimes( Tcorrected );
                magnetizationFunction = rhoT2MagnetizationFunction;

              }
              else if ( relaxometryType == "rhot1-epi" )
              {

                RhoT1MagnetizationFunction* rhoT1MagnetizationFunction =
                           new RhoT1MagnetizationFunction( parameters,
                                       RhoT1MagnetizationFunction::EchoPlanar );
                rhoT1MagnetizationFunction->setMeasurements( Y );
                rhoT1MagnetizationFunction->setInversionTimes( Tcorrected );
                magnetizationFunction = rhoT1MagnetizationFunction;

              }
              else if ( relaxometryType == "rhot1-tse" )
              {

                RhoT1MagnetizationFunction* rhoT1MagnetizationFunction =
                           new RhoT1MagnetizationFunction( parameters,
                                    RhoT1MagnetizationFunction::TurboSpinEcho );
                rhoT1MagnetizationFunction->setMeasurements( Y );
                rhoT1MagnetizationFunction->setInversionTimes( Tcorrected );
                magnetizationFunction = rhoT1MagnetizationFunction;

              }

              double testSize = 0.0;
              factory->getNelderMeadSimplexOptimumParameters( 
                                           parameters,
                                           deltaParameters,
                                           testSize,
                                           *magnetizationFunction,
                                           maximumIterationCount,
                                           maximumTestSize,
                                           false );

              relaxometry( x, y, z, 0 ) = ( float )parameters( 0 );
              relaxometry( x, y, z, 1 ) = ( float )parameters( 1 );
              relaxometry( x, y, z, 2 ) = ( float )testSize;

              // deleting magnetization function
              delete magnetizationFunction;

              // making sanity checks
              if ( ( relaxometry( x, y, z, 0 ) < 0.0 ) ||
                   ( relaxometry( x, y, z, 0 ) > outlierValues[ 0 ] ) ||
                   ( relaxometry( x, y, z, 1 ) < 0.0 ) ||
                   ( relaxometry( x, y, z, 1 ) > outlierValues[ 1 ] ) )
              {

                relaxometry( x, y, z, 0 ) = 0.0;
                relaxometry( x, y, z, 1 ) = 0.0;
                relaxometry( x, y, z, 2 ) = 0.0;
                relaxometry( x, y, z, 3 ) = ( float )UNCORRECTED_LABEL;

              }

            }
            /////////////////////////// LEAST-SQUARE //////////////////////////
            else if ( optimizerType == "least-square" )
            {

              gkg::Vector Y( trialCount );
              gkg::Matrix M( trialCount, 2 );

              for ( t = 0; t < trialCount; t++ )
              {

                if ( data( x, y, z, t ) <= 0 )
                {

                  Y( t ) = std::log( ( double )1e-38 );

                }
                else
                {

                  Y( t ) = std::log( ( double )data( x, y, z, t ) );

                }
                M( t, 0 ) = 1.0;
                M( t, 1 ) = -Tcorrected( t );

              }
              gkg::Matrix Mplus;
              factory->getMoorePenrosePseudoInverse( M, Mplus );
              gkg::Vector P = Mplus.getComposition( Y );

              relaxometry( x, y, z, 0 ) = ( float )std::exp( P( 0 ) );
              relaxometry( x, y, z, 1 ) = 1.0 / ( float )P( 1 );

              // making sanity checks
              if ( ( relaxometry( x, y, z, 0 ) < 0.0 ) ||
                   ( relaxometry( x, y, z, 0 ) > outlierValues[ 0 ] ) ||
                   ( relaxometry( x, y, z, 1 ) < 0.0 ) ||
                   ( relaxometry( x, y, z, 1 ) > outlierValues[ 1 ] ) )
              {

                relaxometry( x, y, z, 0 ) = 0.0;
                relaxometry( x, y, z, 1 ) = 0.0;
                relaxometry( x, y, z, 2 ) = ( float )UNCORRECTED_LABEL;

              }

            }

          }

        }

      }
      if ( verbose )
      {

        std::cout << gkg::Eraser( 20 );

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 );

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ///////////////////////////////////////////////////////////////////////////
    // correcting points where fitting failed
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      if ( relaxometryType == "rhot2" )
      {

        std::cout << "correcting Rho/T2 mapping : " << std::flush;

      }
      else if ( relaxometryType == "rhot2star" )
      {

        std::cout << "correcting Rho/T2* mapping : " << std::flush;

      }
      else if ( ( relaxometryType == "rhot1-epi" ) ||
                ( relaxometryType == "rhot1-tse" ) )
      {

        std::cout << "correcting Rho/T1 mapping : " << std::flush;

      }

    }
    std::list< gkg::Vector3d< int32_t > >
      offsets = gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                              gkg::Neighborhood3d::Neighborhood3d_26 );
    gkg::BoundingBox< int32_t > boundingBox( data );
    gkg::Vector3d< int32_t > site, neighbor;
    gkg::AverageFilter< std::vector< float >, float > averageFilter;
    int32_t p = 0;
    for ( z = 0; z < sizeZ; z++ )
    {

      if ( verbose )
      {

        if ( z != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << "slice[ " << std::setw( 4 ) << z + 1 
                  << " / " << std::setw( 4 ) << sizeZ
                  << " ] " << std::flush;

      }

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          bool atLeastOneValidNeighborFound = false;
          if ( ( mask( x, y, z ) ) &&
               ( relaxometry( x, y, z, outputSizeT - 1  ) > 0 ) )
          {

            site = gkg::Vector3d< int32_t >( x, y, z );
            std::vector< std::vector< float > > values( outputSizeT ); 
            gkg::Vector3d< int32_t > neighbor;
            std::list< gkg::Vector3d< int32_t > >::const_iterator
              o = offsets.begin(),
              oe = offsets.end();
            while ( o != oe )
            {

              neighbor = site + *o;
              if ( boundingBox.contains( neighbor ) )
              {

                if ( mask( neighbor ) )
                {

                  if ( ( int32_t )relaxometry( neighbor,
                                               outputSizeT - 1 ) == 0 )
                  {

                    atLeastOneValidNeighborFound = true;
                    for ( p = 0; p < outputSizeT; p++ )
                    {

                      values[ p ].push_back( relaxometry( neighbor, p ) );

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

                averageFilter.filter( values[ p ], relaxometry( site, p ) );

              }
              relaxometry( site,
                           outputSizeT - 1 ) = ( float )CORRECTED_LABEL;

            }

          }

        }

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 );

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ///////////////////////////////////////////////////////////////////////////
    // writing Rho/T2 data
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      if ( relaxometryType == "rhot2" )
      {

        std::cout << "writing Rho/T2 mapping : " << std::flush;

      }
      else if ( relaxometryType == "rhot2star" )
      {

        std::cout << "writing Rho/T2* mapping : " << std::flush;

      }
      else if ( ( relaxometryType == "rhot1-epi" ) ||
                ( relaxometryType == "rhot1-tse" ) )
      {

        std::cout << "writing Rho/T1 mapping : " << std::flush;

      }

    }
    gkg::Writer::getInstance().write( fileNameOut, relaxometry );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

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
      gkg::Volume< float > fittedData( sizeX, sizeY, sizeZ, trialCount );
      fittedData.fill( 0.0f );
      fittedData.getHeader().addAttribute( "resolutionX", resolution.x );
      fittedData.getHeader().addAttribute( "resolutionY", resolution.y );
      fittedData.getHeader().addAttribute( "resolutionZ", resolution.z );
      fittedData.getHeader().addAttribute( "resolutionT", resolutionT );

      for ( z = 0; z < sizeZ; z++ )
      {

        if ( verbose )
        {

          if ( z != 0 )
          {

            std::cout << gkg::Eraser( 21 );

          }
          std::cout << "slice[ " << std::setw( 4 ) << z + 1 
                    << " / " << std::setw( 4 ) << sizeZ
                    << " ] " << std::flush;

        }

        //
        //  Correct the list of TI for each slice
        //
        if ( ( ( relaxometryType == "rhot1-epi" ) ||
               ( relaxometryType == "rhot1-tse" ) ) 
             && ( repetitionTimePerSlice != 0.0 ) )
        {

          for ( t = 0; t < trialCount; t++ )
          {

            for ( j = 0; j < ( sizeZ / concatenations ); j++ )
            {

              if ( ( z > ( sizeZ - ( j + 1 ) * concatenations - 1 ) ) &&
                   ( z < ( sizeZ - j * concatenations ) ) )
              {

                Tcorrected( t ) = T( t ) + j * repetitionTimePerSlice ;

              }

            }

          }

        }

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            if ( ( mask( x, y, z ) ) && 
                 ( relaxometry( x, y, z, outputSizeT - 1 ) !=
                   UNCORRECTED_LABEL ) )
            {

              gkg::LevenbergMarquardtFunction*
                magnetizationFunction = 0;
              gkg::Vector initialParameters( 2 );
              initialParameters( 0 ) = relaxometry( x, y, z, 0 );
              initialParameters( 1 ) = relaxometry( x, y, z, 1 );
              if ( ( relaxometryType == "rhot2" ) ||
                   ( relaxometryType == "rhot2star" ) )
              {

                magnetizationFunction = new RhoT2MagnetizationFunction(
                                                          initialParameters );

              }
              else if ( relaxometryType == "rhot1-epi" )
              {

                magnetizationFunction = new RhoT1MagnetizationFunction(
                                                          initialParameters,
                                       RhoT1MagnetizationFunction::EchoPlanar);
              }
              else if ( relaxometryType == "rhot1-tse" )
              {

                magnetizationFunction = new RhoT1MagnetizationFunction(
                                                          initialParameters,
                                     RhoT1MagnetizationFunction::TurboSpinEcho);
              }
              for ( t = 0; t < trialCount; t++ )
              {

                fittedData( x, y, z, t ) =
                  ( float )magnetizationFunction->getValueWithParametersAt(
                        Tcorrected.toMatrix().getRow( t ), initialParameters );

              }
              delete magnetizationFunction;

            }

          }

        }

      }

      if ( verbose )
      {

        std::cout << gkg::Eraser( 21 );

      }
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      if ( verbose )
      {

        std::cout << "writing fitted data : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameFittedData, fittedData );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "void gkg::RelaxometryMapperCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameTimes, "
             "const std::string& fileNameMask, const std::string& fileNameOut, "
             "const std::string& fileNameFittedData, "
             "const std::string& relaxometryType, "
             "const std::string& optimizerType, "
             "const std::vector< double >& optimizerParameters, "
             "int32_t concatenations, double repetitionTimePerSlice, "
             "std::vector< double >& outlierValues, bool ascii, "
             "const std::string& format, bool verbose )" );

}


RegisterCommandCreator(
             RelaxometryMapperCommand,
    	     DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    	     DECLARE_STRING_PARAMETER_HELP( fileNameTimes ) +
    	     DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    	     DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    	     DECLARE_STRING_PARAMETER_HELP( fileNameFittedData ) +
    	     DECLARE_STRING_PARAMETER_HELP( relaxometryType ) +
    	     DECLARE_STRING_PARAMETER_HELP( optimizerType ) +
    	     DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( optimizerParameters ) +
    	     DECLARE_INTEGER_PARAMETER_HELP( concatenations ) +
    	     DECLARE_FLOATING_PARAMETER_HELP( repetitionTimePerSlice ) +
    	     DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( outlierValues ) +
    	     DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    	     DECLARE_STRING_PARAMETER_HELP( format ) +
    	     DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
