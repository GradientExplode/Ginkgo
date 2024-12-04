#include <gkg-processing-plugin-functors/BrainVISAVectorFieldConverter/BrainVISAVectorFieldConverterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>




//
// class BrainVISAVectorFieldConverterCommand
//


gkg::BrainVISAVectorFieldConverterCommand::BrainVISAVectorFieldConverterCommand( int32_t argc,
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
  GKG_CATCH( "gkg::BrainVISAVectorFieldConverterCommand::BrainVISAVectorFieldConverterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::BrainVISAVectorFieldConverterCommand::BrainVISAVectorFieldConverterCommand(
                             const std::string& fileNameInputDirectVectorField,
                             const std::string& fileNameInputInverseVectorField,
                             const std::string& fileNameOutputVectorField,
                             const std::string& fileNameOutputDistanceField,
                             bool ascii,
                             const std::string& format,
                             bool verbose )
{

  try
  {

    execute( fileNameInputDirectVectorField,
             fileNameInputInverseVectorField,
             fileNameOutputVectorField,
             fileNameOutputDistanceField,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::BrainVISAVectorFieldConverterCommand::"
             "BrainVISAVectorFieldConverterCommand( "
             "const std::string& fileNameInputDirectVectorField, "
             "const std::string& fileNameInputInverseVectorField, "
             "const std::string& fileNameOutputVectorField, "
             "const std::string& fileNameOutputDistanceField, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


gkg::BrainVISAVectorFieldConverterCommand::BrainVISAVectorFieldConverterCommand(
                                             const gkg::Dictionary& parameters )
                                          : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameInputDirectVectorField );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameInputInverseVectorField );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameOutputVectorField );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameOutputDistanceField );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameInputDirectVectorField,
             fileNameInputInverseVectorField,
             fileNameOutputVectorField,
             fileNameOutputDistanceField,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::BrainVISAVectorFieldConverterCommand::"
             "BrainVISAVectorFieldConverterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::BrainVISAVectorFieldConverterCommand::
                                         ~BrainVISAVectorFieldConverterCommand()
{
}


std::string gkg::BrainVISAVectorFieldConverterCommand::getStaticName()
{

  try
  {

    return "BrainVISAVectorFieldConverter";

  }
  GKG_CATCH( "std::string gkg::BrainVISAVectorFieldConverterCommand::"
             "getStaticName()" );

}


void gkg::BrainVISAVectorFieldConverterCommand::parse()
{

  try
  {

    std::string fileNameInputDirectVectorField;
    std::string fileNameInputInverseVectorField;
    std::string fileNameOutputVectorField;
    std::string fileNameOutputDistanceField;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "BrainVISA to GKG vector field converter",
                                  _loadPlugin );
    application.addSingleOption( "-d",
                                 "Input BrainVISA direct POINT3DF "
                                 "vector field volume",
                                 fileNameInputDirectVectorField );
    application.addSingleOption( "-i",
                                 "Input BrainVISA inverse POINT3DF "
                                 "vector field volume",
                                 fileNameInputInverseVectorField );
    application.addSingleOption( "-o",
                                 "Output GKG direct and inverse vector field",
                                 fileNameOutputVectorField );
    application.addSingleOption( "-D",
                                 "Output GKG direct and inverse distance field",
                                 fileNameOutputDistanceField );
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

    execute( fileNameInputDirectVectorField,
             fileNameInputInverseVectorField,
             fileNameOutputVectorField,
             fileNameOutputDistanceField,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void "
                     "gkg::BrainVISAVectorFieldConverterCommand::parse()" );

}


void gkg::BrainVISAVectorFieldConverterCommand::execute(
                             const std::string& fileNameInputDirectVectorField,
                             const std::string& fileNameInputInverseVectorField,
                             const std::string& fileNameOutputVectorField,
                             const std::string& fileNameOutputDistanceField,
                             bool ascii,
                             const std::string& format,
                             bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // reading input BrainVISA direct vector field volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameInputDirectVectorField
                << "' : " << std::flush;

    }
    gkg::Volume< gkg::Vector3d< float > > directVectorFieldVolume;
    gkg::Reader::getInstance().read( fileNameInputDirectVectorField,
                                     directVectorFieldVolume );

    gkg::Vector3d< int32_t > size;
    directVectorFieldVolume.getSize( size );

    gkg::Vector3d< double > resolution;
    directVectorFieldVolume.getResolution( resolution );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading input BrainVISA direct vector field volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameInputInverseVectorField
                << "' : " << std::flush;

    }
    gkg::Volume< gkg::Vector3d< float > > inverseVectorFieldVolume;
    gkg::Reader::getInstance().read( fileNameInputInverseVectorField,
                                     inverseVectorFieldVolume );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////


    if ( !inverseVectorFieldVolume.hasSameSize( directVectorFieldVolume,
                                                true ) ||
         !inverseVectorFieldVolume.hasSameResolution( directVectorFieldVolume,
                                                      true ) )
    {

      throw std::runtime_error( "direct and inverse vector fields "
                                "don't have the same size or resolution" );


    }
         

    ////////////////////////////////////////////////////////////////////////////
    // computing output GKG direct/inverse vector field volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "computing GKG direct/inverse vector field : "
                << std::flush;

    }

    gkg::Volume< float >
      directAndInverseVectorFieldVolume( size, 6 );
    directAndInverseVectorFieldVolume.setResolution( resolution );

    gkg::Vector3d< int32_t > voxel;
    for ( voxel.z = 0; voxel.z < size.z; voxel.z++ )
    {

      for ( voxel.y = 0; voxel.y < size.y; voxel.y++ )
      {

        for ( voxel.x = 0; voxel.x < size.x; voxel.x++ )
        {

          directAndInverseVectorFieldVolume( voxel, 0 ) =
            directVectorFieldVolume( voxel ).x;
          directAndInverseVectorFieldVolume( voxel, 1 ) =
            directVectorFieldVolume( voxel ).y;
          directAndInverseVectorFieldVolume( voxel, 2 ) =
            directVectorFieldVolume( voxel ).z;

          directAndInverseVectorFieldVolume( voxel, 3 ) =
            inverseVectorFieldVolume( voxel ).x;
          directAndInverseVectorFieldVolume( voxel, 4 ) =
            inverseVectorFieldVolume( voxel ).y;
          directAndInverseVectorFieldVolume( voxel, 5 ) =
            inverseVectorFieldVolume( voxel ).z;


        }

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing output GKG direct/inverse vector field volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing '" << fileNameOutputVectorField
                << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOutputVectorField,
                                      directAndInverseVectorFieldVolume,
                                      ascii,
                                      format );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing and saving output GKG direct/inverse distance field volume
    ////////////////////////////////////////////////////////////////////////////

    if ( !fileNameOutputDistanceField.empty() )
    {
  
      // computing output GKG direct/inverse distance field volume

      if ( verbose )
      {

        std::cout << "computing GKG direct/inverse distance field : "
                  << std::flush;

      }

      gkg::Volume< float >
        directAndInverseDistanceFieldVolume( size, 2 );
      directAndInverseDistanceFieldVolume.setResolution( resolution );

      for ( voxel.z = 0; voxel.z < size.z; voxel.z++ )
      {

        for ( voxel.y = 0; voxel.y < size.y; voxel.y++ )
        {

          for ( voxel.x = 0; voxel.x < size.x; voxel.x++ )
          {

            directAndInverseDistanceFieldVolume( voxel, 0 ) =
              std::sqrt( directAndInverseVectorFieldVolume( voxel, 0 ) *
                         directAndInverseVectorFieldVolume( voxel, 0 ) +
                         directAndInverseVectorFieldVolume( voxel, 1 ) *
                         directAndInverseVectorFieldVolume( voxel, 1 ) +
                         directAndInverseVectorFieldVolume( voxel, 2 ) *
                         directAndInverseVectorFieldVolume( voxel, 2 ) );

            directAndInverseDistanceFieldVolume( voxel, 1 ) =
              std::sqrt( directAndInverseVectorFieldVolume( voxel, 3 ) *
                         directAndInverseVectorFieldVolume( voxel, 3 ) +
                         directAndInverseVectorFieldVolume( voxel, 4 ) *
                         directAndInverseVectorFieldVolume( voxel, 4 ) +
                         directAndInverseVectorFieldVolume( voxel, 5 ) *
                         directAndInverseVectorFieldVolume( voxel, 5 ) );


          }

        }

      }

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing output GKG direct/inverse distance field volume

      if ( verbose )
      {

        std::cout << "writing '" << fileNameOutputDistanceField
                  << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOutputDistanceField,
                                        directAndInverseDistanceFieldVolume,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "void gkg::BrainVISAVectorFieldConverterCommand::execute( "
             "const std::string& fileNameInputDirectVectorField, "
             "const std::string& fileNameInputInverseVectorField, "
             "const std::string& fileNameOutputVectorField, "
             "const std::string& fileNameOutputDistanceField, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator( BrainVISAVectorFieldConverterCommand,
                        DECLARE_STRING_PARAMETER_HELP( 
                                              fileNameInputDirectVectorField ) +
                        DECLARE_STRING_PARAMETER_HELP(
                                             fileNameInputInverseVectorField ) +
                        DECLARE_STRING_PARAMETER_HELP(
                                                   fileNameOutputVectorField ) +
                        DECLARE_STRING_PARAMETER_HELP(
                                                 fileNameOutputDistanceField ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
                        DECLARE_STRING_PARAMETER_HELP( format ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
