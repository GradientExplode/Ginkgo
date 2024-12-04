#include <gkg-dmri-plugin-functors/DwiDirectionFieldDiscrepancy/DwiDirectionFieldDiscrepancyCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <vector>
#include <list>


gkg::DwiDirectionFieldDiscrepancyCommand::DwiDirectionFieldDiscrepancyCommand(
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
  GKG_CATCH( "gkg::DwiDirectionFieldDiscrepancyCommand::"
             "DwiDirectionFieldDiscrepancyCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiDirectionFieldDiscrepancyCommand::DwiDirectionFieldDiscrepancyCommand(
                                    const std::string& fileNameDirectionField1,
                                    const std::string& fileNameDirectionField2,
                                    const std::string& fileNameAngleDiscrepancy,
                                    bool degree,
                                    const std::string format,
                                    bool ascii,
                                    bool verbose )
                                         : gkg::Command()
{

  try
  {

    execute( fileNameDirectionField1,
             fileNameDirectionField2,
             fileNameAngleDiscrepancy,
             degree,
             format,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiDirectionFieldDiscrepancyCommand::"
             "DwiDirectionFieldDiscrepancyCommand( "
             "const std::string& fileNameDirectionField1, "
             "const std::string& fileNameDirectionField2, "
             "const std::string& fileNameAngleDiscrepancy, "
             "bool degree, "
             "const std::string format, "
             "bool ascii, "
             "bool verbose )" );

}


gkg::DwiDirectionFieldDiscrepancyCommand::DwiDirectionFieldDiscrepancyCommand(
                                             const gkg::Dictionary& parameters )
                                         : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameDirectionField1 );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameDirectionField2 );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameAngleDiscrepancy );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, degree );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameDirectionField1,
             fileNameDirectionField2,
             fileNameAngleDiscrepancy,
             degree,
             format,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiDirectionFieldDiscrepancyCommand::"
             "DwiDirectionFieldDiscrepancyCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiDirectionFieldDiscrepancyCommand::~DwiDirectionFieldDiscrepancyCommand()
{
}


std::string gkg::DwiDirectionFieldDiscrepancyCommand::getStaticName()
{

  try
  {

    return "DwiDirectionFieldDiscrepancy";

  }
  GKG_CATCH( "std::string gkg::DwiDirectionFieldDiscrepancyCommand::"
             "getStaticName()" );

}


void gkg::DwiDirectionFieldDiscrepancyCommand::parse()
{

  try
  {

    std::string fileNameDirectionField1;
    std::string fileNameDirectionField2;
    std::string fileNameAngleDiscrepancy;
    bool degree = false;
    std::string format = "";
    bool ascii = false;
    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv,
                                  "DWI direction field angular discrepancy",
                                  _loadPlugin );
    application.addSingleOption( "-i1",
                                 "First input direction field",
                                 fileNameDirectionField1 );
    application.addSingleOption( "-i2",
                                 "Second input direction field",
                                 fileNameDirectionField2 );
    application.addSingleOption( "-o",
                                 "Output angular discrepancy volume file name",
                                 fileNameAngleDiscrepancy );
    application.addSingleOption( "-degree",
                                 "Save ouput volume in degree rather than in"
                                 " radians",
                                 degree,
                                 true );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=GIS)",
                                 format,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameDirectionField1,
             fileNameDirectionField2,
             fileNameAngleDiscrepancy,
             degree,
             format,
             ascii,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiDirectionFieldDiscrepancyCommand::parse()" );

}


void gkg::DwiDirectionFieldDiscrepancyCommand::execute(
                                    const std::string& fileNameDirectionField1,
                                    const std::string& fileNameDirectionField2,
                                    const std::string& fileNameAngleDiscrepancy,
                                    bool degree,
                                    const std::string format,
                                    bool ascii,
                                    bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // reading first direction field volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameDirectionField1
               << "' : " << std::flush;

    }
    gkg::Volume< gkg::Vector3d< float > > directionField1;
    gkg::Reader::getInstance().read( fileNameDirectionField1, directionField1 );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading second direction field volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameDirectionField2
               << "' : " << std::flush;

    }
    gkg::Volume< gkg::Vector3d< float > > directionField2;
    gkg::Reader::getInstance().read( fileNameDirectionField2, directionField2 );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////
    
    gkg::Vector3d< int32_t > sizeOfDirectionField1;
    int32_t sizeTOfDirectionField1;
    gkg::Vector3d< double > resolutionOfDirectionField1;

    gkg::Vector3d< int32_t > sizeOfDirectionField2;
    int32_t sizeTOfDirectionField2;
    gkg::Vector3d< double > resolutionOfDirectionField2;

    directionField1.getSize( sizeOfDirectionField1, sizeTOfDirectionField1 );
    directionField1.getResolution( resolutionOfDirectionField1 );

    directionField2.getSize( sizeOfDirectionField2, sizeTOfDirectionField2 );
    directionField2.getResolution( resolutionOfDirectionField2 );


    if ( ( sizeOfDirectionField1.x != sizeOfDirectionField2.x ) ||
         ( sizeOfDirectionField1.y != sizeOfDirectionField2.y ) ||
         ( sizeOfDirectionField1.z != sizeOfDirectionField2.z ) ||
         ( sizeTOfDirectionField1 != sizeTOfDirectionField2 ) ||
         ( resolutionOfDirectionField1.x != resolutionOfDirectionField2.x ) ||
         ( resolutionOfDirectionField1.y != resolutionOfDirectionField2.y ) ||
         ( resolutionOfDirectionField1.z != resolutionOfDirectionField2.z ) )
    {

      throw std::runtime_error( "incompatible direction field(s)!" );

    }
         
         

    ////////////////////////////////////////////////////////////////////////////
    // computing angular discrepancy
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "computing angular discrepancy : " << std::flush;

    }

    gkg::Volume< float > angularDiscrepancy( sizeOfDirectionField1,
                                             sizeTOfDirectionField1 );
    
    angularDiscrepancy.setResolution( resolutionOfDirectionField1 );

    angularDiscrepancy.fill( 0.0 );

    gkg::Volume< gkg::Vector3d< float > >::const_iterator
      d1 = directionField1.begin(),
      d1e = directionField1.end();
    gkg::Volume< gkg::Vector3d< float > >::const_iterator
      d2 = directionField2.begin();
    gkg::Volume< float >::iterator a = angularDiscrepancy.begin();

    while ( d1 != d1e )
    {

      *a = gkg::getLineAngles( *d1, *d2 );
      if ( degree )
      {

        *a *= 180.0 / M_PI;

      }
      ++ d1;
      ++ d2;
      ++ a;

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing result
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "writing '" << fileNameAngleDiscrepancy
               << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameAngleDiscrepancy,
                                      angularDiscrepancy,
                                      ascii,
                                      format );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DwiDirectionFieldDiscrepancyCommand::execute( "
             "const std::string& fileNameDirectionField1, "
             "const std::string& fileNameDirectionField2, "
             "const std::string& fileNameAngleDiscrepancy, "
             "bool degree, "
             "const std::string format, "
             "bool ascii, "
             "bool verbose )" );

}


RegisterCommandCreator(
    DwiDirectionFieldDiscrepancyCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameDirectionField1 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameDirectionField2 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameAngleDiscrepancy ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( degree ) +
    DECLARE_STRING_PARAMETER_HELP( format ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
