#include <gkg-processing-plugin-functors/CheckFloatingVolume/CheckFloatingVolumeCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <cmath>


class CheckFloatingVolumeProcess : public gkg::Process
{

  public:

    CheckFloatingVolumeProcess( const bool& verbose );

    const bool& _verbose;

  private:

    template < class T >
    static void check( gkg::Process& process,
                       const std::string& fileNameIn,
                       const gkg::AnalyzedObject&,
                       const std::string& );

};


CheckFloatingVolumeProcess::CheckFloatingVolumeProcess( const bool& verbose )
                          : gkg::Process( "Volume" ),
                            _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &CheckFloatingVolumeProcess::check< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &CheckFloatingVolumeProcess::check< double > );

}


template < class T >
void CheckFloatingVolumeProcess::check( gkg::Process& process,
                                        const std::string& fileNameIn,
                                        const gkg::AnalyzedObject&,
                                        const std::string& )
{

  try
  {

    CheckFloatingVolumeProcess&
           checkProcess = static_cast< CheckFloatingVolumeProcess& >( process );

    // reading data
    if ( checkProcess._verbose )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    if ( checkProcess._verbose )
    {

      std::cout << "done" << std::endl;

    }

    int32_t nanCount = 0;
    int32_t infCount = 0;
    int32_t minusInfCount = 0;
    int32_t sizeX = volumeIn.getSizeX();
    int32_t sizeY = volumeIn.getSizeY();
    int32_t sizeZ = volumeIn.getSizeZ();
    int32_t sizeT = volumeIn.getSizeT();
    int32_t x, y, z, t;
    T value;

    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            value = volumeIn( x, y, z, t );

            if ( std::isnan( value ) )
            {

              nanCount++;

              if ( checkProcess._verbose )
              {

                std::cout << "NaN value at : (" 
          		  << x << "," << y << "," << z << "," << t <<  ")"
                          << std::endl;

              }

            }

            int32_t test = std::isinf( value );
            if ( test > 0 )
            {

              infCount++;

              if ( checkProcess._verbose )
              {

                std::cout << "+Inf value at : (" 
          		  << x << "," << y << "," << z << "," << t <<  ")"
                          << std::endl;

              }

            }
            else if ( test < 0 )
            {

              minusInfCount++;

              if ( checkProcess._verbose )
              {

                std::cout << "-Inf value at : (" 
          		  << x << "," << y << "," << z << "," << t <<  ")"
                          << std::endl;

              }

            }

          }

        }

      }

    }

    int32_t total = nanCount + infCount + minusInfCount;
    if ( !total )
    {

      std::cout << "All floating values are valid" << std::endl;

    }
    else
    {

      std::cout << "Found " << total 
                << " invalid floating value(s)" << std::endl;

      if ( nanCount )
      {

        std::cout << "  #NaN value(s)  : " << nanCount << std::endl;

      }
      if ( infCount )
      {

        std::cout << "  #+Inf value(s) : " << infCount << std::endl;

      }
      if ( minusInfCount )
      {

        std::cout << "  #-Inf value(s) : " << minusInfCount << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void CheckFloatingVolumeProcess::check( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class CheckFloatingVolumeCommand
//


gkg::CheckFloatingVolumeCommand::CheckFloatingVolumeCommand( int32_t argc,
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
  GKG_CATCH( "gkg::CheckFloatingVolumeCommand::CheckFloatingVolumeCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::CheckFloatingVolumeCommand::CheckFloatingVolumeCommand(
                                                 const std::string& fileNameIn,
                      		         	 bool verbose )
{

  try
  {

    execute( fileNameIn, verbose );

  }
  GKG_CATCH( "gkg::CheckFloatingVolumeCommand::CheckFloatingVolumeCommand( "
             "const std::string& fileNameIn, bool verbose )" );

}


gkg::CheckFloatingVolumeCommand::CheckFloatingVolumeCommand(
                                             const gkg::Dictionary& parameters )
                               : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, verbose );

  }
  GKG_CATCH( "gkg::CheckFloatingVolumeCommand::CheckFloatingVolumeCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::CheckFloatingVolumeCommand::~CheckFloatingVolumeCommand()
{
}


std::string gkg::CheckFloatingVolumeCommand::getStaticName()
{

  try
  {

    return "CheckFloatingVolume";

  }
  GKG_CATCH( "std::string gkg::CheckFloatingVolumeCommand::getStaticName()" );

}


void gkg::CheckFloatingVolumeCommand::parse()
{

  try
  {

    std::string fileNameIn;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Check validity of floating values in a "
                                  "volume of float or double",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameIn, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::CheckFloatingVolumeCommand::parse()" );

}


void gkg::CheckFloatingVolumeCommand::execute( const std::string& fileNameIn,
                                               bool verbose )
{

  try
  {         

    //
    // launching check process
    //
    CheckFloatingVolumeProcess checkProcess( verbose );

    checkProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::CheckFloatingVolumeCommand::execute( "
             "const std::string& fileNameIn, bool verbose )" );

}


RegisterCommandCreator( CheckFloatingVolumeCommand,
                        DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
