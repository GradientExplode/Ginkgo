#include <gkg-processing-plugin-functors/MannWhitneyWilcoxonUTest/MannWhitneyWilcoxonUTestCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-statistics/MannWhitneyWilcoxonUTest.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <list>
#include <fstream>


gkg::Vector getSample( const std::string& fileNameSample, bool verbose )
{

  try
  {

    if ( verbose )
    {

      std::cout << "reading sample '" << fileNameSample << "' : " << std::flush;

    }
    std::ifstream is( fileNameSample.c_str() );

    std::list< double > values;
    double value = 0.0;

    is >> value;
    while ( !is.eof() )
    {

      values.push_back( value );
      is >> value;

    }

    is.close();

    gkg::Vector sample( values.size() );
    int32_t s = 0;
    std::list< double >::const_iterator v = values.begin(),
                                        ve = values.end();
    while ( v != ve )
    {

      sample( s ) = *v;
      ++ v;
      ++ s;

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    return sample;

  }
  GKG_CATCH( "gkg::Vector getSample( const std::string& fileNameSample )" );

}


gkg::MannWhitneyWilcoxonUTestCommand::MannWhitneyWilcoxonUTestCommand(
                                 			      int32_t argc,
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
  GKG_CATCH( "gkg::MannWhitneyWilcoxonUTestCommand::MannWhitneyWilcoxonUTestCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::MannWhitneyWilcoxonUTestCommand::MannWhitneyWilcoxonUTestCommand(
                                     	     const std::string& fileNameSample1,
                                     	     const std::string& fileNameSample2,
                                     	     const std::string& fileNameOut,
                                     	     bool verbose )
                                    : gkg::Command()
{

  try
  {

    execute( fileNameSample1, fileNameSample2, fileNameOut, verbose );

  }
  GKG_CATCH( "gkg::MannWhitneyWilcoxonUTestCommand::MannWhitneyWilcoxonUTestCommand( "
             "const std::string& fileNameSample1, "
             "const std::string& fileNameSample2, "
             "const std::string& fileNameOut, bool verbose )" );

}


gkg::MannWhitneyWilcoxonUTestCommand::MannWhitneyWilcoxonUTestCommand(
                                             const gkg::Dictionary& parameters )
                                    : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameSample1 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameSample2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameSample1, fileNameSample2, fileNameOut, verbose );

  }
  GKG_CATCH( "gkg::MannWhitneyWilcoxonUTestCommand::"
             "MannWhitneyWilcoxonUTestCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::MannWhitneyWilcoxonUTestCommand::~MannWhitneyWilcoxonUTestCommand()
{
}


std::string gkg::MannWhitneyWilcoxonUTestCommand::getStaticName()
{

  try
  {

    return "MannWhitneyWilcoxonUTest";

  }
  GKG_CATCH( "std::string gkg::MannWhitneyWilcoxonUTestCommand::getStaticName()" );

}


void gkg::MannWhitneyWilcoxonUTestCommand::parse()
{

  try
  {

    std::string fileNameSample1;
    std::string fileNameSample2;
    std::string fileNameOut;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Perform a Mann-Whitney-Wilcoxon U test "
                                  "between two samples",
                                  _loadPlugin );
    application.addSingleOption( "-s1",
                                 "Sample 1 file name (samples are put in a "
                                 "single column)",
                                 fileNameSample1 );
    application.addSingleOption( "-s2",
                                 "Sample 2 file name (samples are put in a "
                                 "single column)",
                                 fileNameSample2 );
    application.addSingleOption( "-o",
                                 "Output result text file",
                                 fileNameOut,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameSample1, fileNameSample2, fileNameOut, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::MannWhitneyWilcoxonUTestCommand::parse()" );

}


void gkg::MannWhitneyWilcoxonUTestCommand::execute(
                                             const std::string& fileNameSample1,
                                             const std::string& fileNameSample2,
                                             const std::string& fileNameOut,
                                             bool verbose )
{

  try
  {

    gkg::Vector sample1 = getSample( fileNameSample1, verbose );
    gkg::Vector sample2 = getSample( fileNameSample2, verbose );

    if ( verbose )
    {

      std::cout << "sample 1 : " << sample1 << std::endl;
      std::cout << "sample 2 : " << sample2 << std::endl;

    }

    if ( verbose )
    {

      std::cout << "performing Mann-Whitney-Wilcoxon test : " << std::flush;

    }

    gkg::MannWhitneyWilcoxonUTest mannWhitneyWilcoxonUTest;

    gkg::MannWhitneyWilcoxonUTest::Result result = 
                              mannWhitneyWilcoxonUTest.test( sample1, sample2 );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    if ( fileNameOut.empty() )
    {

      std::cout << "null hypothesis + same median values : p="
                << result.twoTailedTestPValue << std::endl;
      std::cout << "null hypothesis + the median of the first sample is greater"
                   " than or equal to the median of the second sample : p="
                << result.leftTailedTestPValue << std::endl;
      std::cout << "null hypothesis + the median of the first sample is less"
                   " than or equal to the median of the second sample : p="
                << result.rightTailedTestPValue << std::endl;

    }
    else
    {

      if ( verbose )
      {

        std::cout << "saving results to '" << fileNameOut << "' : "
                  << std::flush;

      }

      std::ofstream os( fileNameOut.c_str() );
      os << "null hypothesis + same median values : p="
         << result.twoTailedTestPValue << std::endl;
      os << "null hypothesis + the median of the first sample is greater"
            " than or equal to the median of the second sample : p="
         << result.leftTailedTestPValue << std::endl;
      os << "null hypothesis + the median of the first sample is less"
            " than or equal to the median of the second sample : p="
         << result.rightTailedTestPValue << std::endl;
      os.close();

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "void gkg::MannWhitneyWilcoxonUTestCommand::execute( "
             "const std::string& fileNameSample1, "
             "const std::string& fileNameSample2, "
             "const std::string& fileNameOut, bool verbose )" );

}


RegisterCommandCreator( MannWhitneyWilcoxonUTestCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameSample1 ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameSample2 ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
