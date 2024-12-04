#include <gkg-processing-plugin-functors/AddBorder2Volume/AddBorder2VolumeCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-transform/Translation3d.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


class BorderProcess : public gkg::Process
{

  public:

    BorderProcess( const std::string& fileNameOut,
                   const int32_t& borderSizeX,
                   const int32_t& borderSizeY,
                   const int32_t& borderSizeZ,
                   const int32_t& borderSizeT,
                   const std::string& fileNameNativeToExtendedTransform3d,
                   const std::string& fileNameExtendedToNativeTransform3d,
                   const double& background,
                   const bool& ascii,
                   const std::string& format,
                   const bool& verbose );

    const std::string& getFileNameOut() const;
    const int32_t& getBorderSizeX() const;
    const int32_t& getBorderSizeY() const;
    const int32_t& getBorderSizeZ() const;
    const int32_t& getBorderSizeT() const;
    const std::string& getFileNameNativeToExtendedTransform3d() const;
    const std::string& getFileNameExtendedToNativeTransform3d() const;
    const double& getBackground() const;
    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void addBorder( gkg::Process& process,
                           const std::string& fileNameIn,
                           const gkg::AnalyzedObject&,
                           const std::string& );

    const std::string& _fileNameOut;
    const int32_t& _borderSizeX;
    const int32_t& _borderSizeY;
    const int32_t& _borderSizeZ;
    const int32_t& _borderSizeT;
    const std::string& _fileNameNativeToExtendedTransform3d;
    const std::string& _fileNameExtendedToNativeTransform3d;
    const double& _background;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


BorderProcess::BorderProcess(
                         const std::string& fileNameOut,
                         const int32_t& borderSizeX,
                         const int32_t& borderSizeY,
                         const int32_t& borderSizeZ,
                         const int32_t& borderSizeT,
                         const std::string& fileNameNativeToExtendedTransform3d,
                         const std::string& fileNameExtendedToNativeTransform3d,
                         const double& background,
                         const bool& ascii,
                         const std::string& format,
                         const bool& verbose)
              : gkg::Process( "Volume" ),
                _fileNameOut( fileNameOut ),
                _borderSizeX( borderSizeX ),
                _borderSizeY( borderSizeY ),
                _borderSizeZ( borderSizeZ ),
                _borderSizeT( borderSizeT ),
                _fileNameNativeToExtendedTransform3d( 
                                          fileNameNativeToExtendedTransform3d ),
                _fileNameExtendedToNativeTransform3d( 
                                          fileNameExtendedToNativeTransform3d ),
                _background( background ),
                _ascii( ascii ),
                _format( format ),
                _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &BorderProcess::addBorder< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &BorderProcess::addBorder< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &BorderProcess::addBorder< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &BorderProcess::addBorder< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &BorderProcess::addBorder< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &BorderProcess::addBorder< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &BorderProcess::addBorder< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &BorderProcess::addBorder< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &BorderProcess::addBorder< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &BorderProcess::addBorder< double > );

}


const std::string& BorderProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const int32_t& BorderProcess::getBorderSizeX() const
{

  return _borderSizeX;

}


const int32_t& BorderProcess::getBorderSizeY() const
{

  return _borderSizeY;

}


const int32_t& BorderProcess::getBorderSizeZ() const
{

  return _borderSizeZ;

}


const int32_t& BorderProcess::getBorderSizeT() const
{

  return _borderSizeT;

}


const std::string& BorderProcess::getFileNameNativeToExtendedTransform3d() const
{

  return _fileNameNativeToExtendedTransform3d;

}


const std::string& BorderProcess::getFileNameExtendedToNativeTransform3d() const
{

  return _fileNameExtendedToNativeTransform3d;

}


const double& BorderProcess::getBackground() const
{

  return _background;

}


const bool& BorderProcess::getAscii() const
{

  return _ascii;

}


const std::string& BorderProcess::getFormat() const
{

  return _format;

}


const bool& BorderProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void BorderProcess::addBorder( gkg::Process& process,
                               const std::string& fileNameIn,
                               const gkg::AnalyzedObject&,
                               const std::string& )
{

  try
  {

    BorderProcess&
      borderProcess = static_cast< BorderProcess& >( process );

    // reading data
    if ( borderProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    if ( borderProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    //
    if ( borderProcess.getVerbose() )
    {

      std::cout << "adding border : " << std::flush;

    }
    int32_t sizeX = volumeIn.getSizeX();
    int32_t sizeY = volumeIn.getSizeY();
    int32_t sizeZ = volumeIn.getSizeZ();
    int32_t sizeT = volumeIn.getSizeT();
    int32_t borderSizeX = borderProcess.getBorderSizeX();
    int32_t borderSizeY = borderProcess.getBorderSizeY();
    int32_t borderSizeZ = borderProcess.getBorderSizeZ();
    int32_t borderSizeT = borderProcess.getBorderSizeT();
    gkg::Volume< T > volumeOut = volumeIn;
    volumeOut.reallocate( sizeX + 2 * borderSizeX,
                          sizeY + 2 * borderSizeY,
                          sizeZ + 2 * borderSizeZ,
                          sizeT + 2 * borderSizeT );
    volumeOut.fill( borderProcess.getBackground() );

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    int32_t t = 0;

    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            volumeOut( x + borderSizeX,
                       y + borderSizeY,
                       z + borderSizeZ,
                       t + borderSizeT ) = volumeIn( x, y, z, t );

          }

        }

      }

    }

    if ( borderProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    // writing data
    if ( borderProcess.getVerbose() )
    {

      std::cout << "writing '"
                << borderProcess.getFileNameOut() << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( borderProcess.getFileNameOut(),
                                      volumeOut,
                                      borderProcess.getAscii(),
                                      borderProcess.getFormat() );
    if ( borderProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    // writing direct native to extended 3D transform
    if ( !borderProcess.getFileNameNativeToExtendedTransform3d().empty() )
    {

      if ( borderProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << borderProcess.getFileNameNativeToExtendedTransform3d()
                  << "' : " << std::flush;

      }

      gkg::Vector3d< double > resolution;
      volumeIn.getResolution( resolution );

      gkg::Translation3d< float >
        nativeToExtendedTransform3d( borderSizeX * resolution.x,
                                     borderSizeY * resolution.y,
                                     borderSizeZ * resolution.z );
      std::ofstream os(
               borderProcess.getFileNameNativeToExtendedTransform3d().c_str() );
      nativeToExtendedTransform3d.writeTrm( os );
      os.close();

      if ( borderProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

    // writing inverse extended to native 3D transform
    if ( !borderProcess.getFileNameExtendedToNativeTransform3d().empty() )
    {

      if ( borderProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << borderProcess.getFileNameExtendedToNativeTransform3d()
                  << "' : " << std::flush;

      }

      gkg::Vector3d< double > resolution;
      volumeIn.getResolution( resolution );

      gkg::Translation3d< float >
        extendedToNativeTransform3d( -borderSizeX * resolution.x,
                                     -borderSizeY * resolution.y,
                                     -borderSizeZ * resolution.z );
      std::ofstream os(
               borderProcess.getFileNameExtendedToNativeTransform3d().c_str() );
      extendedToNativeTransform3d.writeTrm( os );
      os.close();

      if ( borderProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void BorderProcess::addBorder( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//  class AddBorder2VolumeCommand
//


gkg::AddBorder2VolumeCommand::AddBorder2VolumeCommand( int32_t argc,
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
  GKG_CATCH( "gkg::AddBorder2VolumeCommand::AddBorder2VolumeCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::AddBorder2VolumeCommand::AddBorder2VolumeCommand(
                         const std::string& fileNameIn,
                         const std::string& fileNameOut,
                         int32_t borderSizeX,
                         int32_t borderSizeY,
                         int32_t borderSizeZ,
                         int32_t borderSizeT,
                         const std::string& fileNameNativeToExtendedTransform3d,
                         const std::string& fileNameExtendedToNativeTransform3d,
                         double background,
                         bool ascii,
                         const std::string& format,
                         bool verbose )
                            : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, borderSizeX, borderSizeY,
             borderSizeZ, borderSizeT,
             fileNameNativeToExtendedTransform3d,
             fileNameExtendedToNativeTransform3d,
             background, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::AddBorder2VolumeCommand::AddBorder2VolumeCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "int32_t borderSizeX, int32_t borderSizeY, "
             "int32_t borderSizeZ, int32_t borderSizeT, "
             "const std::string& fileNameNativeToExtendedTransform3d, "
             "const std::string& fileNameExtendedToNativeTransform3d, "
             "double background, bool ascii, const std::string& format, "
             "bool verbose )" );

}


gkg::AddBorder2VolumeCommand::AddBorder2VolumeCommand(
                                             const gkg::Dictionary& parameters )
                            : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, borderSizeX );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, borderSizeY );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, borderSizeZ );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, borderSizeT );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameNativeToExtendedTransform3d );
    DECLARE_STRING_PARAMETER( parameters, std::string, 
                              fileNameExtendedToNativeTransform3d );
    DECLARE_FLOATING_PARAMETER( parameters, double, background );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    execute( fileNameIn, fileNameOut, borderSizeX, borderSizeY,
             borderSizeZ, borderSizeT,
             fileNameNativeToExtendedTransform3d,
             fileNameExtendedToNativeTransform3d,
             background, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::AddBorder2VolumeCommand::AddBorder2VolumeCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::AddBorder2VolumeCommand::~AddBorder2VolumeCommand()
{
}


std::string gkg::AddBorder2VolumeCommand::getStaticName()
{

  try
  {

    return "AddBorder2Volume";

  }
  GKG_CATCH( "std::string gkg::AddBorder2VolumeCommand::getStaticName()" );

}


void gkg::AddBorder2VolumeCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    int32_t borderSizeX = 0;
    int32_t borderSizeY = 0;
    int32_t borderSizeZ = 0;
    int32_t borderSizeT = 0;
    std::string fileNameNativeToExtendedTransform3d = "";
    std::string fileNameExtendedToNativeTransform3d = "";
    double background = 0.0;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Tool to add borders to volumes",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-bx",
                                 "X border size (default=0)",
                                 borderSizeX,
                                 true );
    application.addSingleOption( "-by",
                                 "Y border size (default=0)",
                                 borderSizeY,
                                 true );
    application.addSingleOption( "-bz",
                                 "Z border size (default=0)",
                                 borderSizeZ,
                                 true );
    application.addSingleOption( "-bt",
                                 "T border size (default=0)",
                                 borderSizeT,
                                 true );
    application.addSingleOption( "-td",
                                 "Native to extended 3D transform",
                                 fileNameNativeToExtendedTransform3d,
                                 true );
    application.addSingleOption( "-ti",
                                 "Extended to native to 3D transform",
                                 fileNameExtendedToNativeTransform3d,
                                 true );
    application.addSingleOption( "-b",
                                 "Background value (default=0)",
                                 background,
                                 true );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
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

    execute( fileNameIn, fileNameOut, borderSizeX, borderSizeY,
             borderSizeZ, borderSizeT,
             fileNameNativeToExtendedTransform3d,
             fileNameExtendedToNativeTransform3d,
             background, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::AddBorder2VolumeCommand::parse()" );

}


void gkg::AddBorder2VolumeCommand::execute( 
                         const std::string& fileNameIn,
                         const std::string& fileNameOut,
                         int32_t borderSizeX,
                         int32_t borderSizeY,
                         int32_t borderSizeZ,
                         int32_t borderSizeT,
                         const std::string& fileNameNativeToExtendedTransform3d,
                         const std::string& fileNameExtendedToNativeTransform3d,
                         double background,
                         bool ascii,
                         const std::string& format,
                         bool verbose )
{

  try
  {

    BorderProcess borderProcess( fileNameOut,
                                 borderSizeX,
                                 borderSizeY,
                                 borderSizeZ,
                                 borderSizeT,
                                 fileNameNativeToExtendedTransform3d,
                                 fileNameExtendedToNativeTransform3d,
                                 background,
                                 ascii,
                                 format,
                                 verbose );

    borderProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::AddBorder2VolumeCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "int32_t borderSizeX, int32_t borderSizeY, "
             "int32_t borderSizeZ, int32_t borderSizeT, "
             "const std::string& fileNameNativeToExtendedTransform3d, "
             "const std::string& fileNameExtendedToNativeTransform3d, "
             "double background, bool ascii, const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator(
          AddBorder2VolumeCommand,
          DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
          DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
          DECLARE_INTEGER_PARAMETER_HELP( borderSizeX ) +
          DECLARE_INTEGER_PARAMETER_HELP( borderSizeY ) +
          DECLARE_INTEGER_PARAMETER_HELP( borderSizeZ ) +
          DECLARE_INTEGER_PARAMETER_HELP( borderSizeT ) +
          DECLARE_STRING_PARAMETER_HELP( fileNameNativeToExtendedTransform3d ) +
          DECLARE_STRING_PARAMETER_HELP( fileNameExtendedToNativeTransform3d ) +
          DECLARE_FLOATING_PARAMETER_HELP( background ) +
          DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
          DECLARE_STRING_PARAMETER_HELP( format ) +
          DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
