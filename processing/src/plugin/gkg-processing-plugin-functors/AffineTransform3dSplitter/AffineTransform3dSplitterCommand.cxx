#include <gkg-processing-plugin-functors/AffineTransform3dSplitter/AffineTransform3dSplitterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-core-exception/Exception.h>

#include <fstream>
#include <iostream>


gkg::AffineTransform3dSplitterCommand::AffineTransform3dSplitterCommand(
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
  GKG_CATCH( "gkg::AffineTransform3dSplitterCommand::"
             "AffineTransform3dSplitterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::AffineTransform3dSplitterCommand::AffineTransform3dSplitterCommand(
                               const std::string& transformNameIn,
                               const std::string& transformNameOut,
                               const std::vector< std::string >& keptParameters,
                               bool verbose )
{

  try
  {

    execute( transformNameIn, transformNameOut, keptParameters, verbose );

  }
  GKG_CATCH( "gkg::AffineTransform3dSplitterCommand::"
             "AffineTransform3dSplitterCommand( "
             "const std::string& transformNameIn, "
             "const std::string& transformNameOut, "
             "const std::vector< std::string >& keptParameters, "
             "bool verbose )" );

}


gkg::AffineTransform3dSplitterCommand::AffineTransform3dSplitterCommand(
                                             const gkg::Dictionary& parameters )
                                     : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, transformNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, transformNameOut );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, 
                                         std::vector< std::string >,
                                         keptParameters );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( transformNameIn, transformNameOut, keptParameters, verbose );

  }
  GKG_CATCH( "gkg::AffineTransform3dSplitterCommand::"
             "AffineTransform3dSplitterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::AffineTransform3dSplitterCommand::~AffineTransform3dSplitterCommand()
{
}


std::string gkg::AffineTransform3dSplitterCommand::getStaticName()
{

  try
  {

    return "AffineTransform3dSplitter";

  }
  GKG_CATCH( "std::string "
             "gkg::AffineTransform3dSplitterCommand::getStaticName()" );

}


void gkg::AffineTransform3dSplitterCommand::parse()
{

  try
  {

    std::string transformNameIn;
    std::string transformNameOut;
    std::vector< std::string > keptParameters;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Affine 3D transform splitter",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input affine transform .trm file name",
                                 transformNameIn );
    application.addSingleOption( "-o",
                                 "Output affine transform .trm file name",
                                 transformNameOut );
    application.addSeriesOption( "-kept",
                                 "List of parameters to keep in the output "
                                 "transformation (default: all)\n"
                                 "- translation\n"
                                 "- rotation\n"
                                 "- scalings\n"
                                 "- shearings\n",
                                 keptParameters,
                                 0, 4 );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( transformNameIn, transformNameOut, keptParameters, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::AffineTransform3dSplitterCommand::parse()" );

}


void gkg::AffineTransform3dSplitterCommand::execute(
                  	           const std::string& transformNameIn,
                               const std::string& transformNameOut,
                               const std::vector< std::string >& keptParameters,
                               bool verbose )
{

  try
  {

    std::ifstream ifs( transformNameIn.c_str() );

    if ( ifs.good() )
    {

      gkg::AffineTransform3d< float > transform3dIn;
      gkg::AffineTransform3d< float > transform3dOut;

      if ( verbose )
      {

        std::cout << "Reading affine transformation : " << std::flush;

      }

      transform3dIn.readTrm( ifs );
      ifs.close();

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      if ( keptParameters.size() )
      {

        gkg::Vector3d< float > scalings;
        gkg::Vector3d< float > shearings;
        gkg::Vector3d< float > angles;
        gkg::Vector3d< float > translation;
        gkg::Vector3d< float > outScalings( 1.0f, 1.0f, 1.0f );
        gkg::Vector3d< float > outShearings( 0.0f, 0.0f, 0.0f );
        gkg::Vector3d< float > outAngles( 0.0f, 0.0f, 0.0f );
        gkg::Vector3d< float > outTranslation( 0.0f, 0.0f, 0.0f );
        std::vector< std::string >::const_iterator
          t = keptParameters.begin(),
          te = keptParameters.end();

        transform3dIn.getDirectParameters( scalings, shearings,
                                           angles, translation, true );

        if ( verbose )
        {

          std::cout << "Keep : " << std::flush;

        }

        while ( t != te )
        {

          bool ok = true;

          if ( *t == "translation" )
          {

            outTranslation = translation;

          }
          else if ( *t == "rotation" )
          {

            outAngles = angles;

          }
          else if ( *t == "scalings" )
          {

            outScalings = scalings;

          }
          else if ( *t == "shearings" )
          {

            outShearings = shearings;

          }
          else
          {

            ok = false;

            if ( verbose ) 
            {

              std::cout << std::endl
                        << "Bad parameter : "
                        << *t 
                        << std::endl;
            }

          }

          if ( ok && verbose )
          {

            std::cout << *t << " " << std::flush;

          }

          ++t;

        }

        transform3dOut.setDirectParameters( outScalings,
                                            outShearings,
                                            outAngles,
                                            outTranslation,
                                            true );

        if ( verbose )
        {

          std::cout << std::endl;

        }

      }
      else
      {

        if ( verbose )
        {

          std::cout << "Keep : translation rotation scaling shearing" 
                    << std::endl;

        }

        transform3dOut = transform3dIn;

      }

      if ( verbose )
      {

        std::cout << "Writing affine transformation : " << std::flush;

      }

      std::ofstream ofs( transformNameOut.c_str() );

      if ( ofs.good() )
      {

        transform3dOut.writeTrm( ofs );
        ofs.close();

        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

      }

    }

  }
  GKG_CATCH( "void gkg::AffineTransform3dSplitterCommand::execute( "
             "const std::string& transformNameIn, "
             "const std::string& transformNameOut, "
             "const std::vector< std::string >& keptParameters, "
             "bool verbose )" );

}


RegisterCommandCreator(
           AffineTransform3dSplitterCommand,
           DECLARE_STRING_PARAMETER_HELP( transformNameIn ) +
           DECLARE_STRING_PARAMETER_HELP( transformNameOut ) +
           DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( keptParameters ) +
           DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );

