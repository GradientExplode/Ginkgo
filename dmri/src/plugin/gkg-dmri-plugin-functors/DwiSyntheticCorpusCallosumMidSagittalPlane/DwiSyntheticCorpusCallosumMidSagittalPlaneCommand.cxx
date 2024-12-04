#include <gkg-dmri-plugin-functors/DwiSyntheticCorpusCallosumMidSagittalPlane/DwiSyntheticCorpusCallosumMidSagittalPlaneCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-processing-io/SparseMatrixItemIOFactory.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <fstream>
#include <vector>


gkg::DwiSyntheticCorpusCallosumMidSagittalPlaneCommand::
                              DwiSyntheticCorpusCallosumMidSagittalPlaneCommand(
                                                   int32_t argc,
                                                   char* argv[],
                                                   bool loadPlugin,
                                                   bool removeFirst )
                                                       : gkg::Command(
                                                                   argc,
                                                                   argv,
                                                                   loadPlugin,
                                                                   removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiSyntheticCorpusCallosumMidSagittalPlaneCommand::"
             "DwiSyntheticCorpusCallosumMidSagittalPlaneCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiSyntheticCorpusCallosumMidSagittalPlaneCommand::
                              DwiSyntheticCorpusCallosumMidSagittalPlaneCommand(
                                      const std::string& fileNameMask,
                                      const std::string& fileNameTrainingMatrix,
                                      int32_t trainingSampleCount,
                                      const std::string& fileNameOut,
                                      bool verbose )
                           : gkg::Command()
{

  try
  {

    execute( fileNameMask,
             fileNameTrainingMatrix,
             trainingSampleCount,
             fileNameOut,
             verbose );

  }
  GKG_CATCH( "gkg::DwiSyntheticCorpusCallosumMidSagittalPlaneCommand::"
             "DwiSyntheticCorpusCallosumMidSagittalPlaneCommand( "
             "const std::string& fileNameMask, "
             "const std::string& fileNameTrainingMatrix, "
             "const std::string& trainingSampleCount, "
             "const std::list< std::string >& fileNameOut, "
             "bool verbose )" );

}


gkg::DwiSyntheticCorpusCallosumMidSagittalPlaneCommand::
                              DwiSyntheticCorpusCallosumMidSagittalPlaneCommand(
                                             const gkg::Dictionary& parameters )
                           : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameTrainingMatrix );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, trainingSampleCount );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameMask,
             fileNameTrainingMatrix,
             trainingSampleCount,
             fileNameOut,
             verbose );

  }
  GKG_CATCH( "gkg::DwiSyntheticCorpusCallosumMidSagittalPlaneCommand::"
             "DwiSyntheticCorpusCallosumMidSagittalPlaneCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiSyntheticCorpusCallosumMidSagittalPlaneCommand::
                            ~DwiSyntheticCorpusCallosumMidSagittalPlaneCommand()
{
}


std::string 
gkg::DwiSyntheticCorpusCallosumMidSagittalPlaneCommand::getStaticName()
{

  try
  {

    return "DwiSyntheticCorpusCallosumMidSagittalPlane";

  }
  GKG_CATCH( "std::string "
             "gkg::DwiSyntheticCorpusCallosumMidSagittalPlaneCommand::"
             "getStaticName()" );

}


void gkg::DwiSyntheticCorpusCallosumMidSagittalPlaneCommand::parse()
{

  try
  {

    std::string fileNameMask;
    std::string fileNameTrainingMatrix;
    int32_t trainingSampleCount;
    std::string fileNameOut;
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv,
                                  "Corpus callosum mid-sagittal plane phantom "
                                  "synthetizer",
                                  _loadPlugin );
    application.addSingleOption( "-m",
                                 "Input mask file name",
                                 fileNameMask );
    application.addSingleOption( "-t",
                                 "Input training matrix file name",
                                 fileNameTrainingMatrix );
    application.addSingleOption( "-s",
                                 "Input sample count per geometrical "
                                 "configuration",
                                 trainingSampleCount );
    application.addSingleOption( "-o",
                                 "Output corpus callosum volume file name",
                                 fileNameOut );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameMask,
             fileNameTrainingMatrix,
             trainingSampleCount,
             fileNameOut,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void "
                     "gkg::DwiSyntheticCorpusCallosumMidSagittalPlaneCommand::"
                     "parse()" );

}


void gkg::DwiSyntheticCorpusCallosumMidSagittalPlaneCommand::execute(
                     const std::string& fileNameMask,
                     const std::string& fileNameTrainingMatrix,
                     int32_t trainingSampleCount,
                     const std::string& fileNameOut,
                     bool verbose )
{

  try
  {



    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( trainingSampleCount <= 0 )
    {

      throw std::runtime_error(
                          "training sampling count must be strictly positive" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // getting numerical analysis implementation factory
    ////////////////////////////////////////////////////////////////////////////

    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );


    ////////////////////////////////////////////////////////////////////////////
    // reading mask volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameMask << "' : " << std::flush;

    }
    gkg::Volume< int16_t > mask;
    gkg::Reader::getInstance().read( fileNameMask, mask );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading training sparse matrix
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameTrainingMatrix << "' : "
               << std::flush;

    }
    std::ifstream is;
    try
    {

      is.open( fileNameTrainingMatrix.c_str() );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( "cannot open training sparse matrix" );

    }

    gkg::SparseMatrix trainingMatrix;
    gkg::ItemReader< gkg::SparseMatrix >& sparseMatrixR =
    gkg::SparseMatrixItemIOFactory::getInstance().getReader( true, false );
    sparseMatrixR.read( is, trainingMatrix );

    is.close();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // collecting output volume information and allocating output volume
    ////////////////////////////////////////////////////////////////////////////


    if ( verbose )
    {

     std::cout << "creating synthetic corpus callosum : " << std::flush;

    }

    std::vector< int32_t > lut( 14, 0 );
    lut[  2 ] = 3;
    lut[  3 ] = 6;
    lut[  5 ] = 14;
    lut[  7 ] = 27;
    lut[  8 ] = 14;
    lut[ 10 ] = 5;
    lut[ 11 ] = 9;
    lut[ 13 ] = 26;

    gkg::Vector3d< int32_t > outputSize( mask.getSizeX(),
                                         mask.getSizeY(),
                                         mask.getSizeZ() );
    int32_t outputSizeT = trainingMatrix.getSize2();

    gkg::Vector3d< double > outputResolution;
    mask.getResolution( outputResolution );

    gkg::Volume< float > outputVolume( outputSize, outputSizeT );
    outputVolume.setResolution( outputResolution );
    outputVolume.fill( 0.0 );

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    int32_t t = 0;
    int32_t value = 0;
    int32_t label = 0;
    int32_t line = 0;

    for ( z = 0; z < outputSize.z; z++ )
    {

      for ( y = 0; y < outputSize.y; y++ )
      {

        for ( x = 0; x < outputSize.x; x++ )
        {

          value = ( int32_t )mask( x, y, z );
          if ( value > 0 )
          {

            label = lut[ value ];
            line = label * trainingSampleCount +
                   ( int32_t )factory->getUniformRandomUInt32( randomGenerator,
                                                               999U );
            for ( t = 0; t < outputSizeT; t++ )
            {

              outputVolume( x, y, z, t ) = trainingMatrix( line, t );

            }

          }

        }

      }

    }
                                         
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving output volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "saving '" << fileNameOut << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOut, outputVolume );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DwiSyntheticCorpusCallosumMidSagittalPlaneCommand::"
             "execute( "
             "const std::string& fileNameMask, "
             "const std::string& fileNameTrainingMatrix, "
             "int32_t trainingSampleCount, "
             "const std::string& fileNameOut, "
             "bool verbose )" );

}


RegisterCommandCreator(
    DwiSyntheticCorpusCallosumMidSagittalPlaneCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameTrainingMatrix ) +
    DECLARE_INTEGER_PARAMETER_HELP( trainingSampleCount ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
