#include <gkg-processing-plugin-functors/VolumeAverager/VolumeAveragerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-algobase/AverageFilter_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


class VolumeAveragerProcess : public gkg::Process
{

  public:

    VolumeAveragerProcess( const std::string& fileNameOut,
                           const std::string& axis,
                           const std::vector< int32_t >& range,
                           const bool& ascii,
                           const std::string& format,
                           const bool& verbose );

    const std::string& getFileNameOut() const;
    const std::string& getAxis() const;
    const std::vector< int32_t >& getRange() const;
    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void average( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

    const std::string& _fileNameOut;
    const std::string& _axis;
    const std::vector< int32_t >& _range;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


VolumeAveragerProcess::VolumeAveragerProcess( const std::string& fileNameOut,
                                            const std::string& axis,
                                            const std::vector< int32_t >& range,
                                            const bool& ascii,
                                            const std::string& format,
                                            const bool& verbose )
                     : gkg::Process( "Volume" ),
                       _fileNameOut( fileNameOut ),
                       _axis( axis ),
                       _range( range ),
                       _ascii( ascii ),
                       _format( format ),
                       _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &VolumeAveragerProcess::average< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &VolumeAveragerProcess::average< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &VolumeAveragerProcess::average< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &VolumeAveragerProcess::average< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &VolumeAveragerProcess::average< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &VolumeAveragerProcess::average< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &VolumeAveragerProcess::average< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &VolumeAveragerProcess::average< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &VolumeAveragerProcess::average< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &VolumeAveragerProcess::average< double > );

}


const std::string& VolumeAveragerProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& VolumeAveragerProcess::getAxis() const
{

  return _axis;

}


const std::vector< int32_t >& VolumeAveragerProcess::getRange() const
{

  return _range;

}


const bool& VolumeAveragerProcess::getAscii() const
{

  return _ascii;

}


const std::string& VolumeAveragerProcess::getFormat() const
{

  return _format;

}


const bool& VolumeAveragerProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void VolumeAveragerProcess::average( gkg::Process& process,
                                     const std::string& fileNameIn,
                                     const gkg::AnalyzedObject&,
                                     const std::string& )
{

  try
  {

    VolumeAveragerProcess&
      volumeAveragerProcess = static_cast< VolumeAveragerProcess& >( process );

    // sanity checks
    if ( ( volumeAveragerProcess.getAxis() != "x" ) &&
         ( volumeAveragerProcess.getAxis() != "y" ) &&
         ( volumeAveragerProcess.getAxis() != "z" ) &&
         ( volumeAveragerProcess.getAxis() != "t" ) )
    {

      throw std::runtime_error( "not a valid axis" );

    }

    if ( !volumeAveragerProcess.getRange().empty() &&
         ( volumeAveragerProcess.getRange().size() != 2U ) )
    {

      throw std::runtime_error( "two limits required or no limit" );

    }

    // reading input
    if ( volumeAveragerProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > inputVolume;
    gkg::Reader::getInstance().read( fileNameIn, inputVolume );
    int32_t sizeX = inputVolume.getSizeX();
    int32_t sizeY = inputVolume.getSizeY();
    int32_t sizeZ = inputVolume.getSizeZ();
    int32_t sizeT = inputVolume.getSizeT();
    gkg::Vector3d< double > resolution;
    double resolutionT = 1.0;
    inputVolume.getResolution( resolution, resolutionT );

    if ( volumeAveragerProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    
    // reading input
    if ( volumeAveragerProcess.getVerbose() )
    {

      std::cout << "averaging along " << volumeAveragerProcess.getAxis()
                << " : " << std::flush;

    }

    gkg::Volume< T > outputVolume;
    gkg::AverageFilter< std::vector< T >, T > averageFilter;

    if ( volumeAveragerProcess.getAxis() == "x" )
    {

      int32_t lowerX = 0;
      int32_t upperX = sizeX - 1;
      if ( !volumeAveragerProcess.getRange().empty() )
      {

        lowerX = volumeAveragerProcess.getRange()[ 0 ];
        upperX = volumeAveragerProcess.getRange()[ 1 ];

      }

      outputVolume.reallocate( 1, sizeY, sizeZ, sizeT );
      outputVolume.setResolution( resolution, resolutionT );
      std::vector< T > line( upperX - lowerX + 1 );
      int32_t x, y, z, t;
      for ( t = 0; t < sizeT; t++ )
      {

        for ( z = 0; z < sizeZ; z++ )
        {

          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = lowerX; x <= upperX; x++ )
            {

              line[ x - lowerX ] = inputVolume( x, y, z, t );

            }
            T value = 0;
            averageFilter.filter( line, value );
            outputVolume( 0, y, z, t ) = value;

          }

        }

      }
      
    }
    else if ( volumeAveragerProcess.getAxis() == "y" )
    {

      int32_t lowerY = 0;
      int32_t upperY = sizeY - 1;
      if ( !volumeAveragerProcess.getRange().empty() )
      {

        lowerY = volumeAveragerProcess.getRange()[ 0 ];
        upperY = volumeAveragerProcess.getRange()[ 1 ];

      }

      outputVolume.reallocate( sizeX, 1, sizeZ, sizeT );
      outputVolume.setResolution( resolution, resolutionT );
      std::vector< T > line( upperY - lowerY + 1 );
      int32_t x, y, z, t;
      for ( t = 0; t < sizeT; t++ )
      {

        for ( z = 0; z < sizeZ; z++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            for ( y = lowerY; y <= upperY; y++ )
            {

              line[ y - lowerY ] = inputVolume( x, y, z, t );

            }
            T value = 0;
            averageFilter.filter( line, value );
            outputVolume( x, 0, z, t ) = value;

          }

        }

      }
      
    }
    else if ( volumeAveragerProcess.getAxis() == "z" )
    {

      int32_t lowerZ = 0;
      int32_t upperZ = sizeZ - 1;
      if ( !volumeAveragerProcess.getRange().empty() )
      {

        lowerZ = volumeAveragerProcess.getRange()[ 0 ];
        upperZ = volumeAveragerProcess.getRange()[ 1 ];

      }

      outputVolume.reallocate( sizeX, sizeY, 1, sizeT );
      outputVolume.setResolution( resolution, resolutionT );
      std::vector< T > line( upperZ - lowerZ + 1 );
      int32_t x, y, z, t;
      for ( t = 0; t < sizeT; t++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            for ( z = lowerZ; z <= upperZ; z++ )
            {

              line[ z - lowerZ ] = inputVolume( x, y, z, t );

            }
            T value = 0;
            averageFilter.filter( line, value );
            outputVolume( x, y, 0, t ) = value;

          }

        }

      }
      
    }
    else if ( volumeAveragerProcess.getAxis() == "t" )
    {

      int32_t lowerT = 0;
      int32_t upperT = sizeT - 1;
      if ( !volumeAveragerProcess.getRange().empty() )
      {

        lowerT = volumeAveragerProcess.getRange()[ 0 ];
        upperT = volumeAveragerProcess.getRange()[ 1 ];

      }

      outputVolume.reallocate( sizeX, sizeY, sizeZ, 1 );
      outputVolume.setResolution( resolution, resolutionT );
      std::vector< T > line( upperT - lowerT + 1 );
      int32_t x, y, z, t;
      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            for ( t = lowerT; t <= upperT; t++ )
            {

              line[ t - lowerT ] = inputVolume( x, y, z, t );

            }
            T value = 0;
            averageFilter.filter( line, value );
            outputVolume( x, y, z, 0 ) = value;

          }

        }

      }
      
    }

    if ( volumeAveragerProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    // writing result
    if ( volumeAveragerProcess.getVerbose() )
    {

      std::cout << "writing '" << volumeAveragerProcess.getFileNameOut()
                << "' : " << std::flush;

    }

    gkg::Writer::getInstance().write( volumeAveragerProcess.getFileNameOut(),
                                      outputVolume,
                                      volumeAveragerProcess.getAscii(),
                                      volumeAveragerProcess.getFormat() );

    if ( volumeAveragerProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }
  
  }
  GKG_CATCH( "template < class T > "
             "void VolumeAveragerProcess::average( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   VolumeAveragerCommand
//


gkg::VolumeAveragerCommand::VolumeAveragerCommand( int32_t argc,
                                 		   char* argv[],
                                 		   bool loadPlugin,
                                 		   bool removeFirst )
                          : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::VolumeAveragerCommand::VolumeAveragerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::VolumeAveragerCommand::VolumeAveragerCommand(
                           		    const std::string& fileNameIn,
                           		    const std::string& fileNameOut,
                           		    const std::string& axis,
                           		    const std::vector< int32_t >& range,
                           		    bool ascii,
                           		    const std::string& format,
                           		    bool verbose )
                          : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, axis, range, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::VolumeAveragerCommand::VolumeAveragerCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& axis, "
             "const std::vector< int32_t >& range, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::VolumeAveragerCommand::VolumeAveragerCommand(
                                             const gkg::Dictionary& parameters )
                          : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, axis );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters,
                                          std::vector< int32_t >,
                                          range );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, axis, range, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::VolumeAveragerCommand::VolumeAveragerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::VolumeAveragerCommand::~VolumeAveragerCommand()
{
}


std::string gkg::VolumeAveragerCommand::getStaticName()
{

  try
  {

    return "VolumeAverager";

  }
  GKG_CATCH( "std::string gkg::VolumeAveragerCommand::getStaticName()" );

}


void gkg::VolumeAveragerCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string axis = "t";
    std::vector< int32_t > range;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Tool for averaging volume along a given "
                                  "direction (x, y, z or t)",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-axis", "Axis x, y, z or t (default=t)",
                                 axis, true );
    application.addSeriesOption( "-r", "Range of data to be average along the"
                                 " given axis (default=0:size - 1)",
                                 range );
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

    execute( fileNameIn, fileNameOut, axis, range, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::VolumeAveragerCommand::parse()" );

}


void gkg::VolumeAveragerCommand::execute( const std::string& fileNameIn,
                           		  const std::string& fileNameOut,
                           		  const std::string& axis,
                           		  const std::vector< int32_t >& range,
                           		  bool ascii,
                           		  const std::string& format,
                           		  bool verbose )
{

  try
  {

    //
    // launching sub-volume process
    //
    VolumeAveragerProcess volumeAveragerProcess( fileNameOut,
                                                 axis,
                                                 range,
                                                 ascii,
                                                 format,
                                                 verbose );
    volumeAveragerProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::VolumeAveragerCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& axis, "
             "const std::vector< int32_t >& range, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator( VolumeAveragerCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_STRING_PARAMETER_HELP( axis ) +
    			DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( range ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
