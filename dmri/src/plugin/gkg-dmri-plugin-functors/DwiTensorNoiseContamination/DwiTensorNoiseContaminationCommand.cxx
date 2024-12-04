#include <gkg-dmri-plugin-functors/DwiTensorNoiseContamination/DwiTensorNoiseContaminationCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-coordinates/OrientationSet_i.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>


//
// void readTensorInformationFile()
//

static void readTensorInformationFile( const std::string& name,
                                       gkg::Matrix& directTensor )
{

  try
  {

    std::ifstream is( name.c_str() );

    if ( !is )
    {

      throw std::runtime_error( std::string( "cannot open '" ) +
                                name + "' file" );

    }

    double angle = 0.0;

    is >> angle;

    angle *= M_PI / 180.0;

    gkg::Matrix Rz( 3, 3 );
    Rz( 0, 0 ) = std::cos( angle );
    Rz( 0, 1 ) = -std::sin( angle );
    Rz( 0, 2 ) = 0.0;
    Rz( 1, 0 ) = std::sin( angle );
    Rz( 1, 1 ) = std::cos( angle );
    Rz( 1, 2 ) = 0.0;
    Rz( 2, 0 ) = 0.0;
    Rz( 2, 1 ) = 0.0;
    Rz( 2, 2 ) = 1.0;

    directTensor.reallocate( 3, 3 );
    is >> directTensor( 0, 0 )
       >> directTensor( 0, 1 )
       >> directTensor( 0, 2 )
       >> directTensor( 1, 1 )
       >> directTensor( 1, 2 )
       >> directTensor( 2, 2 );

    directTensor( 1, 0 ) = directTensor( 0, 1 );
    directTensor( 2, 0 ) = directTensor( 0, 2 );
    directTensor( 2, 1 ) = directTensor( 1, 2 );

    directTensor = Rz.getTransposition().getComposition(
                                          directTensor ).getComposition( Rz );

    std::cout << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "D=" << std::endl << directTensor << std::endl;
    std::cout << "--------------------------------------------" << std::endl;

    is.close();

  }
  GKG_CATCH( "void readTensorInformationFile( "
             "const std::string& name, "
             "gkg::Matrix& directTensor )" );

}


//
// class DwiTensorNoiseContaminationCommand
//

gkg::DwiTensorNoiseContaminationCommand::DwiTensorNoiseContaminationCommand(
                                                              int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                        : gkg::Command( argc, argv, loadPlugin,
                                                        removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiTensorNoiseContaminationCommand::"
             "DwiTensorNoiseContaminationCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiTensorNoiseContaminationCommand::DwiTensorNoiseContaminationCommand(
                                              const std::string& fileNameIn,
                                              double bValue,
                                              const std::string& fileNameTensor,
                                              double minimumSNR,
                                              double maximumSNR,
                                              int32_t stepCount,
                                              int32_t orientationCount,
                                              bool verbose )
                                        : gkg::Command()
{

  try
  {

    execute( fileNameIn,
             bValue,
             fileNameTensor,
             minimumSNR,
             maximumSNR,
             stepCount,
             orientationCount,
             verbose );

  }
  GKG_CATCH( "gkg::DwiTensorNoiseContaminationCommand::"
             "DwiTensorNoiseContaminationCommand( "
             "const std::string& fileNameIn, "
             "double bValue, "
             "const std::string& fileNameTensor, "
             "double minimumSNR, "
             "double maximumSNR, "
             "int32_t stepCount, "
             "int32_t orientationCount, "
             "bool verbose )" );

}


gkg::DwiTensorNoiseContaminationCommand::DwiTensorNoiseContaminationCommand(
                                             const gkg::Dictionary& parameters )
                                        : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_FLOATING_PARAMETER( parameters, double, bValue );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameTensor );
    DECLARE_FLOATING_PARAMETER( parameters, double, minimumSNR );
    DECLARE_FLOATING_PARAMETER( parameters, double, maximumSNR );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, stepCount );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, orientationCount );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    execute( fileNameIn,
             bValue,
             fileNameTensor,
             minimumSNR,
             maximumSNR,
             stepCount,
             orientationCount,
             verbose );

  }
  GKG_CATCH( "gkg::DwiTensorNoiseContaminationCommand::"
             "DwiTensorNoiseContaminationCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiTensorNoiseContaminationCommand::~DwiTensorNoiseContaminationCommand()
{
}


std::string gkg::DwiTensorNoiseContaminationCommand::getStaticName()
{

  try
  {

    return "DwiTensorNoiseContamination";

  }
  GKG_CATCH( "std::string gkg::DwiTensorNoiseContaminationCommand::getStaticName()" );

}


void gkg::DwiTensorNoiseContaminationCommand::parse()
{

  try
  {

    std::string fileNameIn;
    double bValue = 0;
    std::string fileNameTensor;
    double minimumSNR = 2.0;
    double maximumSNR = 100.0;
    int32_t stepCount = 20;
    int32_t orientationCount = 500;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "DWI tensor model noise contamination",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input text tensor information file name.\n"
                                 "Diffusion coefficients are given in mm2/s.\n"
                                 "Must follow the convention:\n"
                                 "<rotation_angle>\n"
                                 "D(xx) D(xy) D(xz) D(yy) D(yz) D(zz)\n",
                                 fileNameIn );
    application.addSingleOption( "-b",
                                 "B-value in s/mm2",
                                 bValue );
    application.addSingleOption( "-minimumSnr",
                                 "Minimum SNR (default=1.0)",
                                 minimumSNR,
                                 true );
    application.addSingleOption( "-maximumSnr",
                                 "Maximum SNR (default=100.0)",
                                 maximumSNR,
                                 true );
    application.addSingleOption( "-stepCount",
                                 "SNR step count (default=20)",
                                 stepCount,
                                 true );
    application.addSingleOption( "-orientationCount",
                                 "Orientation count (default=500)",
                                 orientationCount,
                                 true );
    application.addSingleOption( "-o",
                                 "Output tensor file name",
                                 fileNameTensor );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameIn,
             bValue,
             fileNameTensor,
             minimumSNR,
             maximumSNR,
             stepCount,
             orientationCount,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiTensorNoiseContaminationCommand::parse()" );

}


void gkg::DwiTensorNoiseContaminationCommand::execute(
                                              const std::string& fileNameIn,
                                              double bValue,
                                              const std::string& fileNameTensor,
                                              double minimumSNR,
                                              double maximumSNR,
                                              int32_t stepCount,
                                              int32_t orientationCount,
                                              bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( orientationCount <= 0 )
    {

      throw std::runtime_error( "orientation count must be strictly positive" );

    }
    if ( bValue < 0 )
    {

      throw std::runtime_error( "b-value must be positive" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading tensor information
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading tensor information : " << std::flush;

    }
    gkg::Matrix directTensor;
    readTensorInformationFile( fileNameIn,
                               directTensor );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building orientation set
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "building orientation set : " << std::flush;

    }
    gkg::ElectrostaticOrientationSet orientationSet( orientationCount );
    orientationCount = orientationSet.getCount();
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building SVD decomposition
    ////////////////////////////////////////////////////////////////////////////


    if ( verbose )
    {

      std::cout << "building SVD decomposition : " << std::flush;

    }
    gkg::Matrix U( orientationCount, 6 );
    gkg::Vector S( 6 );
    gkg::Matrix V( 6, 6 );
    int32_t o;
    for ( o = 0; o < U.getSize1(); o++ )
    {

      gkg::Vector3d< float >
        orientation = orientationSet.getOrientation( o );

      U( o, 0 ) = bValue * orientation.x * orientation.x;
      U( o, 1 ) = 2 * bValue * orientation.x * orientation.y;
      U( o, 2 ) = 2 * bValue * orientation.x * orientation.z;
      U( o, 3 ) = bValue * orientation.y * orientation.y;
      U( o, 4 ) = 2 * bValue * orientation.y * orientation.z;
      U( o, 5 ) = bValue * orientation.z * orientation.z;

    }


    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // performing SVD decomposition
    factory->getSingularValueDecomposition( U, V, S );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // performing noise contamination study on tensor estimation
    ////////////////////////////////////////////////////////////////////////////

    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

    std::ofstream os( fileNameTensor.c_str() );

    std::cout << "minimumSNR=" << minimumSNR << std::endl;
    std::cout << "maximumSNR=" << maximumSNR << std::endl;

    int32_t step;
    double dwi, noise;
    gkg::Vector normalizedSignal( orientationCount );
    gkg::Vector coefficients( 6 );
    for ( step = 0; step < stepCount; step++ )
    {

      double snr = minimumSNR +
                   step * ( ( maximumSNR - minimumSNR ) / ( stepCount - 1 ) );

      std::cout << "================ SNR = " << snr << "=================="
                << std::endl;


      for ( o = 0; o < orientationCount; o++ )
      {

        gkg::Vector u( 3 );
        u( 0 ) = ( double )orientationSet.getOrientation( o ).x;
        u( 1 ) = ( double )orientationSet.getOrientation( o ).y;
        u( 2 ) = ( double )orientationSet.getOrientation( o ).z;
                       
        dwi = std::exp( -bValue *
                        u.getComposition( directTensor.getComposition( u ) ) );
        noise = factory->getGaussianRandomNumber(
                                                randomGenerator,
                                                0.0,
                                                std::sqrt( 2.0 / M_PI ) / snr );
        std::cout << "dwi=" << dwi << " noise=" << noise << std::endl;
        normalizedSignal( o ) = std::log( 1.0 /
                                       std::sqrt( dwi * dwi + noise * noise ) );


      }

      // fitting coefficients
      factory->getSingularValueSolution( U, S, V,
                                         normalizedSignal,
                                         coefficients );

      gkg::Matrix D( 3, 3 );

      D( 0, 0 ) = coefficients( 0 );
      D( 0, 1 ) = coefficients( 1 );
      D( 0, 2 ) = coefficients( 2 );
      D( 1, 0 ) = coefficients( 1 );
      D( 1, 1 ) = coefficients( 3 );
      D( 1, 2 ) = coefficients( 4 );
      D( 2, 0 ) = coefficients( 2 );
      D( 2, 1 ) = coefficients( 4 );
      D( 2, 2 ) = coefficients( 5 );

      gkg::Vector eigenValues( 3 );
      gkg::Matrix eigenVectors( 3, 3 );

      factory->getEigenSystem( D, eigenValues, eigenVectors );
      factory->sortEigenSystem( eigenValues, eigenVectors, 
              gkg::NumericalAnalysisImplementationFactory::AbsoluteDescending );

      os << snr
         << "        "
         << eigenValues
         << std::endl;
         
    }

    os.close();

  }
  GKG_CATCH( "void gkg::DwiTensorNoiseContaminationCommand::execute( "
             "const std::string& fileNameIn, "
             "double bValue, "
             "const std::string& fileNameTensor, "
             "double minimumSNR, "
             "double maximumSNR, "
             "int32_t stepCount, "
             "int32_t orientationCount, "
             "bool verbose )" );

}


RegisterCommandCreator(
    DwiTensorNoiseContaminationCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_FLOATING_PARAMETER_HELP( bValue ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameTensor ) +
    DECLARE_FLOATING_PARAMETER_HELP( minimumSNR ) +
    DECLARE_FLOATING_PARAMETER_HELP( maximumSNR ) +
    DECLARE_INTEGER_PARAMETER_HELP( stepCount ) +
    DECLARE_INTEGER_PARAMETER_HELP( orientationCount ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
