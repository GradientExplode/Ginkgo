#include <gkg-processing-plugin-functors/SiemensDwiOrientationFile/SiemensDwiOrientationFileCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <iomanip>


gkg::SiemensDwiOrientationFileCommand::SiemensDwiOrientationFileCommand(
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
  GKG_CATCH( "gkg::SiemensDwiOrientationFileCommand::SiemensDwiOrientationFileCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::SiemensDwiOrientationFileCommand::SiemensDwiOrientationFileCommand(
                                  const std::vector< int32_t >& directionCounts,
                                  const std::string& fileNameIn,
                                  const std::string& fileNameOut,
                                  bool normalize,
                                  bool verbose )
                                     : gkg::Command()
{

  try
  {

    execute( directionCounts, fileNameIn, fileNameOut, normalize, verbose );

  }
  GKG_CATCH( "gkg::SiemensDwiOrientationFileCommand::SiemensDwiOrientationFileCommand( "
             "const std::vector< int32_t >& directionCounts, "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "bool normalize, bool verbose )" );

}


gkg::SiemensDwiOrientationFileCommand::SiemensDwiOrientationFileCommand(
                                             const gkg::Dictionary& parameters )
                                     : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters,
                                          std::vector< int32_t >,
                                          directionCounts );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, normalize );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( directionCounts, fileNameIn, fileNameOut, normalize, verbose );

  }
  GKG_CATCH( "gkg::SiemensDwiOrientationFileCommand::"
             "SiemensDwiOrientationFileCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::SiemensDwiOrientationFileCommand::~SiemensDwiOrientationFileCommand()
{
}


std::string gkg::SiemensDwiOrientationFileCommand::getStaticName()
{

  try
  {

    return "SiemensDwiOrientationFile";

  }
  GKG_CATCH( "std::string gkg::SiemensDwiOrientationFileCommand::getStaticName()" );

}


void gkg::SiemensDwiOrientationFileCommand::parse()
{

  try
  {

    std::vector< int32_t > directionCounts;
    std::string fileNameIn = "";
    std::string fileNameOut = "DiffusionVectors.txt";
    bool normalize = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Siemens Syngo DWI orientation file "
                                  "generator",
                                  _loadPlugin );
    application.addSeriesOption( "-d",
                                 "Direction counts",
                                 directionCounts );
    application.addSingleOption( "-D",
                                 "File containing the list of directions",
                                 fileNameIn,
                                 true );
    application.addSingleOption( "-o",
                                 "Output DWI orientation file "
                                 "(default=DiffusionVectors.txt)",
                                 fileNameOut,
                                 true );
    application.addSingleOption( "-normalize",
                                 "Normalize by bigger norm (default=false)",
                                 normalize, 
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( directionCounts, fileNameIn, fileNameOut, normalize, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::SiemensDwiOrientationFileCommand::parse()" );

}


void gkg::SiemensDwiOrientationFileCommand::execute(
                  	          const std::vector< int32_t >& directionCounts,
                                  const std::string& fileNameIn,
                                  const std::string& fileNameOut,
                                  bool normalize,
                                  bool verbose )
{

  try
  {

    //
    // sanity checks
    //
    if ( !directionCounts.empty() && !fileNameIn.empty() )
    {

      throw std::runtime_error( "'-d' and '-D' cannot be used together" );

    }
    if ( directionCounts.empty() && fileNameIn.empty() )
    {

      throw std::runtime_error( "'-d' or '-D' must be used" );

    }

    //
    // opening output file
    //
    if ( verbose )
    {

      std::cout << "opening output '"
                << fileNameOut << "' : "
                << std::flush;

    }
    std::ofstream os( fileNameOut.c_str() );
    os << "#----------------------------------------------------------"
       << "---------------------" << std::endl;
    os << "# DWI Siemens orientation file built from GKG package" << std::endl;
    os << "#----------------------------------------------------------"
       << "---------------------" << std::endl;
    os << "# Copyright (C) 2003-2007 CEA" << std::endl;
    os << "# " << std::endl;
    os << "# This software and supporting documentation were developed by"
       << std::endl;
    os << "#        Cyril Poupon & Fabrice Poupon" << std::endl;
    os << "#        CEA/DSV/I2BM/NeuroSpin, Bat. 145" << std::endl;
    os << "#        91191 Gif-sur-Yvette" << std::endl;
    os << "#        France" << std::endl;
    os << "# " << std::endl;
    os << "# For any question or remark, please contact:" << std::endl;
    os << "#   cyril.poupon@cea.fr" << std::endl;
    os << "#   fabrice.poupon@cea.fr" << std::endl;
    os << "#----------------------------------------------------------"
       << "---------------------" << std::endl;
    os << std::endl;
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    //
    // processing orientation set(s)
    //
    if ( verbose )
    {

      std::cout << "processing orientation set(s) : "
                << std::flush;

    }
    if ( !directionCounts.empty() )
    {

      int32_t c;
      for ( c = 0; c < ( int32_t )directionCounts.size(); c++ )
      {

        try
        {

          // check if orientation set exists
          gkg::GkgGetUniformOrientation( directionCounts[ c ], 0, 0 );

          if ( verbose )
          {

            std::cout << directionCounts[ c ] << " " << std::flush;

          }

 
          os << "[directions=" << directionCounts[ c ] << "]" << std::endl;
          os << "CoordinateSystem = xyz" << std::endl;
          os << "Normalisation = none" << std::endl;

          gkg::ElectrostaticOrientationSet
            orientationSet( directionCounts[ c ] );

          int32_t o;
          for ( o = 0; o < orientationSet.getCount(); o++ )
          {

            os << "Vector[" << o << "] = "
               << orientationSet.getOrientation( o )
               << std::endl;

          }
          os << std::endl;


        }
        catch ( std::exception& )
        {
        }

      }

    }
    else
    {

      std::vector< gkg::Vector3d< float > > inputOrientations;
      int32_t inputOrientationCount = 0;
      std::ifstream is( fileNameIn.c_str() );

      is >> inputOrientationCount;
      inputOrientations.resize( inputOrientationCount );

      int32_t o;
      float maximumNorm = 0.0;
      for ( o = 0; o < inputOrientationCount; o++ )
      {

        is >> inputOrientations[ o ].x
           >> inputOrientations[ o ].y
           >> inputOrientations[ o ].z;
        if ( normalize )
        {

          if ( inputOrientations[ o ].getNorm() > maximumNorm )
          {

            maximumNorm = inputOrientations[ o ].getNorm();

          }

        }

      }
      if ( normalize && ( maximumNorm > 0 ) )
      {

        for ( o = 0; o < inputOrientationCount; o++ )
        {

          inputOrientations[ o ] /= maximumNorm;

        }
 
      }
      is.close();


      os << "[directions=" << inputOrientationCount << "]" << std::endl;
      os << "CoordinateSystem = xyz" << std::endl;
      os << "Normalisation = none" << std::endl;
      for ( o = 0; o < inputOrientationCount; o++ )
      {

        os << "Vector[" << o << "] = "
           << inputOrientations[ o ]
           << std::endl;

      }
      os << std::endl;

    }
    if ( verbose )
    {

      std::cout << std::endl;

    }


    //
    // closing output file
    //
    if ( verbose )
    {

      std::cout << "closing output file '"
                << fileNameOut << "' : "
                << std::flush;

    }
    os.close();
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::SiemensDwiOrientationFileCommand::execute( "
             "const std::vector< int32_t >& directionCounts, "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "bool normalize, bool verbose )" );

}


RegisterCommandCreator( 
                  SiemensDwiOrientationFileCommand,
                  DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( directionCounts ) +
                  DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                  DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                  DECLARE_BOOLEAN_PARAMETER_HELP( normalize ) +
                  DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
