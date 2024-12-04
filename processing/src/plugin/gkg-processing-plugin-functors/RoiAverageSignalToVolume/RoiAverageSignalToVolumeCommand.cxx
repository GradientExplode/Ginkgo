#include <gkg-processing-plugin-functors/RoiAverageSignalToVolume/RoiAverageSignalToVolumeCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineWoShearingTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-algobase/UpdatableMeanStandardDeviationMinMax.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <vector>
#include <map>


class RoiAverageSignalToVolumeProcess : public gkg::Process
{

  public:

    RoiAverageSignalToVolumeProcess(
                          const std::string& theFileNameLabel,
                          const std::string& theFileNameLabelToInputTransform3d,
                          const std::string& theFileNameOut,
                          const bool& theAscii,
                          const std::string& theFormat,
                          const bool& theVerbose );

    const std::string& fileNameLabel;
    const std::string& fileNameLabelToInputTransform3d;
    const std::string& fileNameOut;
    const bool& ascii;
    const std::string& format;
    const bool& verbose;

  private:

    template < class T >
    static void average( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

};


RoiAverageSignalToVolumeProcess::RoiAverageSignalToVolumeProcess(
                          const std::string& theFileNameLabel,
                          const std::string& theFileNameLabelToInputTransform3d,
                          const std::string& theFileNameOut,
                          const bool& theAscii,
                          const std::string& theFormat,
                          const bool& theVerbose )
                                : gkg::Process( "Volume" ),
                                  fileNameLabel( theFileNameLabel ),
                                  fileNameLabelToInputTransform3d(
                                           theFileNameLabelToInputTransform3d ),
                                  fileNameOut( theFileNameOut ),
                                  ascii( theAscii ),
                                  format( theFormat ),
                                  verbose( theVerbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &RoiAverageSignalToVolumeProcess::average< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &RoiAverageSignalToVolumeProcess::average< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &RoiAverageSignalToVolumeProcess::average< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &RoiAverageSignalToVolumeProcess::average< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &RoiAverageSignalToVolumeProcess::average< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &RoiAverageSignalToVolumeProcess::average< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &RoiAverageSignalToVolumeProcess::average< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &RoiAverageSignalToVolumeProcess::average< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &RoiAverageSignalToVolumeProcess::average< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &RoiAverageSignalToVolumeProcess::average< double > );

  }
  GKG_CATCH( "RoiAverageSignalToVolumeProcess::"
             "RoiAverageSignalToVolumeProcess( "
             "const std::string& theFileNameLabel, "
             "const std::string& theFileNameLabelToInputTransform3d, "
             "const std::string& theFileNameOut, "
             "const bool& theAscii, "
             "const std::string& theFormat, "
             "const bool& theVerbose )" );

}


template < class T >
void RoiAverageSignalToVolumeProcess::average( gkg::Process& process,
                                               const std::string& fileNameIn,
                                               const gkg::AnalyzedObject&,
                                               const std::string& )
{

  try
  {

    RoiAverageSignalToVolumeProcess&
      roiAverageSignalToVolumeProcess = static_cast<
                                  RoiAverageSignalToVolumeProcess& >( process );

    const std::string& 
     fileNameLabel = roiAverageSignalToVolumeProcess.fileNameLabel;
    const std::string&
     fileNameLabelToInputTransform3d =
                roiAverageSignalToVolumeProcess.fileNameLabelToInputTransform3d;
    const std::string& fileNameOut =
                                    roiAverageSignalToVolumeProcess.fileNameOut;
    const bool& ascii = roiAverageSignalToVolumeProcess.ascii;
    const std::string& format = roiAverageSignalToVolumeProcess.format;
    const bool& verbose = roiAverageSignalToVolumeProcess.verbose;


    ////////////////////////////////////////////////////////////////////////////
    // reading input volume
    ////////////////////////////////////////////////////////////////////////////
    
    if ( verbose )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }

    gkg::Volume< T > inputVolume;
    gkg::Reader::getInstance().read( fileNameIn, inputVolume );
    int32_t inputSizeX = inputVolume.getSizeX();
    int32_t inputSizeY = inputVolume.getSizeY();
    int32_t inputSizeZ = inputVolume.getSizeZ();
    gkg::Vector3d< double > inputResolution;
    inputVolume.getResolution( inputResolution );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading label volume
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::Volume< int16_t > > labelVolumeInt16;
    gkg::RCPointer< gkg::Volume< uint8_t > > labelVolumeUint8;

    if ( verbose )
    {

      std::cout << "reading '" << fileNameLabel << "' : " << std::flush;

    }


    try
    {

      gkg::Volume< int16_t >* 
       pLabelVolumeInt16 = new gkg::Volume< int16_t >;
      gkg::Reader::getInstance().read( fileNameLabel,
                                       *pLabelVolumeInt16 );
      labelVolumeInt16.reset( pLabelVolumeInt16 );

    }
    catch( std::exception& )
    {

      try
      {

        gkg::Volume< uint8_t >* 
          pLabelVolumeUint8 = new gkg::Volume< uint8_t >;
        gkg::Reader::getInstance().read( fileNameLabel,
                                         *pLabelVolumeUint8 );

        labelVolumeUint8.reset( pLabelVolumeUint8 );

      }
      catch( std::exception& )
      {

        throw std::runtime_error( "unable to read label volume" );

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading label to input volume 3D transform
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::Transform3d< float > > labelToInputTransform3d;

    if ( fileNameLabelToInputTransform3d.empty() )
    {

      if ( verbose )
      {

        std::cout << "setting 3D transform to identity : " << std::flush;

      }
      labelToInputTransform3d.reset( new gkg::IdentityTransform3d< float > );

    }
    else
    {

      if ( verbose )
      {

        std::cout << "reading 3D transform '"
                  << fileNameLabelToInputTransform3d
                  << "' : " << std::flush;

      }
      try
      {

        gkg::RigidTransform3d< float >*
          rigidTransform3d = new gkg::RigidTransform3d< float >;
        std::ifstream is( fileNameLabelToInputTransform3d.c_str() );
        rigidTransform3d->readTrm( is );
        is.close();
        labelToInputTransform3d.reset( rigidTransform3d );

      }
      catch( std::exception& )
      {

        try
        {

          gkg::AffineWoShearingTransform3d< float >*
            affineWoShearingTransform3d =
              new gkg::AffineWoShearingTransform3d< float >;
          std::ifstream is( fileNameLabelToInputTransform3d.c_str() );
          affineWoShearingTransform3d->readTrm( is );
          is.close();
          labelToInputTransform3d.reset( affineWoShearingTransform3d );

        }
        catch( std::exception& )
        {

          try
          {

            gkg::AffineTransform3d< float >*
              affineTransform3d =
                new gkg::AffineTransform3d< float >;
            std::ifstream is( fileNameLabelToInputTransform3d.c_str() );
            affineTransform3d->readTrm( is );
            is.close();
            labelToInputTransform3d.reset( affineTransform3d );

          }
          catch( std::exception& )
          {

            throw std::runtime_error( "unable to read 3D transform" );

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // analyzing volumes of interest
    ////////////////////////////////////////////////////////////////////////////


    if ( verbose )
    {

      std::cout << "analyzing..." << std::endl;

    }

    int32_t inputSizeT = inputVolume.getSizeT();
    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    int32_t t = 0;
    std::map< int32_t, std::vector< T > > mapOfAverages;

    for ( t = 0; t < inputSizeT ; t++ )
    {
      
      // parsing input volumes and collecting information
      std::vector< gkg::UpdatableMeanStandardDeviationMinMax > analysis;

      if ( !labelVolumeInt16.isNull() )
      {

        // resizing the analysis according to the int16_t type
        analysis.resize( 32767 );

        // getting point to the nearest neighbor resampler for int16_t type
        gkg::Resampler< int16_t >* resampler =
          gkg::ResamplerFactory< int16_t >::getInstance().getResampler( 0 );

        // looping over voxels
        int16_t interpolatedValue = 0;
        gkg::Vector3d< float > inputSite;

        for ( z = 0; z < inputSizeZ; z++ )
        {

          inputSite.z = ( float )z * ( float )inputResolution.z;

          for ( y = 0; y < inputSizeY; y++ )
          {

            inputSite.y = ( float )y * ( float )inputResolution.y;
            for ( x = 0; x < inputSizeX; x++ )
            {

              inputSite.x = ( float )x * ( float )inputResolution.x;
              resampler->resample( *labelVolumeInt16,
                                   *labelToInputTransform3d,
                                   ( int16_t )0,
                                   inputSite,
                                   interpolatedValue );

              if ( interpolatedValue > 0 )
              {

                analysis[ ( int32_t )interpolatedValue ].add(
                                          ( double )inputVolume( x, y, z, t ) );

              }


            }

          }

        }

      }
      else if ( !labelVolumeUint8.isNull() )
      {

        // resizing the analysis according to the uint8_t type
        analysis.resize( 255 );

        // getting point to the nearest neighbor resampler for uint8_t type
        gkg::Resampler< uint8_t >* resampler =
          gkg::ResamplerFactory< uint8_t >::getInstance().getResampler( 0 );

        // looping over voxels
        uint8_t interpolatedValue = 0;
        gkg::Vector3d< float > inputSite;

        for ( z = 0; z < inputSizeZ; z++ )
        {

          inputSite.z = ( float )z * ( float )inputResolution.z;

          for ( y = 0; y < inputSizeY; y++ )
          {

            inputSite.y = ( float )y * ( float )inputResolution.y;
            for ( x = 0; x < inputSizeX; x++ )
            {

              inputSite.x = ( float )x * ( float )inputResolution.x;
              resampler->resample( *labelVolumeUint8,
                                   *labelToInputTransform3d,
                                   ( uint8_t )0,
                                   inputSite,
                                   interpolatedValue );

              if ( interpolatedValue > 0 )
              {

                analysis[ ( int32_t )interpolatedValue ].add(
                                          ( double )inputVolume( x, y, z, t ) );

              }


            }

          }

        }

      }


      // collecting mean signal in each label
      std::vector<
            gkg::UpdatableMeanStandardDeviationMinMax >::const_iterator
        u = analysis.begin(),
        ue = analysis.end();
      int32_t label = 0;

      while ( u != ue )
      {

        if ( u->getCount() )
        {

          if ( mapOfAverages.find( label ) == mapOfAverages.end() )
          {

            mapOfAverages.insert( std::pair< int32_t, std::vector< T > >(
                                             label,
                                             std::vector< T >( inputSizeT ) ) );

          }
          mapOfAverages[ label ][ t ] = ( T )u->getMean();

        }
        ++ u;
        ++ label;

      }

      if ( verbose )
      {

        std::cout << "rank " << t << ": done" << std::endl;

      }


    }

    ////////////////////////////////////////////////////////////////////////////
    // creating output volumes
    ////////////////////////////////////////////////////////////////////////////

    typename std::map< int32_t, std::vector< T > >::const_iterator
      a = mapOfAverages.begin(),
      ae = mapOfAverages.end();
    while ( a != ae )
    {

      const int32_t& label = a->first;
      if ( verbose )
      {

        std::cout << "creating output volume for label " << label << " : "
                  << std::flush;

      }

      gkg::Volume< T > outputVolume( 1, 1, 1, inputSizeT );
      outputVolume.getHeader() = inputVolume.getHeader();
      outputVolume.getHeader()[ "sizeX" ] = 1;
      outputVolume.getHeader()[ "sizeY" ] = 1;
      outputVolume.getHeader()[ "sizeZ" ] = 1;
      const std::vector< T >& averages = a->second;
      for ( t = 0; t < inputSizeT ; t++ )
      {
      
        outputVolume( 0, 0, 0, t ) = averages[ t ];

      }

      std::string fileNameOutWithLabel = 
          std::string( "label-" ) +
          gkg::StringConverter::toString( label ) + 
          "-" + fileNameOut;

      gkg::Writer::getInstance().write( fileNameOutWithLabel, outputVolume,
                                        ascii, format );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      ++ a;

    }

  }
  GKG_CATCH( "template < class T > "
             "void RoiAverageSignalToVolumeProcess::average( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//
//


gkg::RoiAverageSignalToVolumeCommand::RoiAverageSignalToVolumeCommand(
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
  GKG_CATCH( "gkg::RoiAverageSignalToVolumeCommand::RoiAverageSignalToVolumeCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::RoiAverageSignalToVolumeCommand::RoiAverageSignalToVolumeCommand(
                             const std::string& fileNameIn,
                             const std::string& fileNameLabel,
                             const std::string& fileNameLabelToInputTransform3d,
                             const std::string& fileNameOut,
                             bool ascii,
                             const std::string& format,
                             bool verbose )
                                    : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameLabel, fileNameLabelToInputTransform3d,
             fileNameOut, ascii, format, verbose );

  }
  GKG_CATCH( 
       "gkg::RoiAverageSignalToVolumeCommand::RoiAverageSignalToVolumeCommand( "
       "const std::string& fileNameIn, const std::string& fileNameLabel, "
       "const std::string& fileNameLabelToInputTransform3d, "
       "const std::string& fileNameOut, bool ascii, const std::string& format, "
       "bool verbose )" );

}


gkg::RoiAverageSignalToVolumeCommand::RoiAverageSignalToVolumeCommand(
                                             const gkg::Dictionary& parameters )
                                    : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameLabel );
    DECLARE_STRING_PARAMETER( parameters, 
                              std::string,
                              fileNameLabelToInputTransform3d );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameLabel, fileNameLabelToInputTransform3d,
             fileNameOut, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::RoiAverageSignalToVolumeCommand::"
             "RoiAverageSignalToVolumeCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::RoiAverageSignalToVolumeCommand::~RoiAverageSignalToVolumeCommand()
{
}


std::string gkg::RoiAverageSignalToVolumeCommand::getStaticName()
{

  try
  {

    return "RoiAverageSignalToVolume";

  }
  GKG_CATCH( "std::string gkg::RoiAverageSignalToVolumeCommand::getStaticName()" );

}


void gkg::RoiAverageSignalToVolumeCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameLabel;
    std::string fileNameLabelToInputTransform3d;
    std::string fileNameOut;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Tool for averaging signal in ROIs and "
                                  " creating a volume per ROI",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input volume file name",
                                 fileNameIn );
    application.addSingleOption( "-l",
                                 "Input label int16_t/uint8_t volume file name",
                                 fileNameLabel );
    application.addSingleOption( "-t",
                                 "label to input volume 3D transform file name"
                                 " (default=id)",
                                 fileNameLabelToInputTransform3d,
                                 true );
    application.addSingleOption( "-o",
                                 "Output volume file name",
                                 fileNameOut );
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

    execute( fileNameIn, fileNameLabel, fileNameLabelToInputTransform3d,
             fileNameOut, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::RoiAverageSignalToVolumeCommand::parse()" );

}


void gkg::RoiAverageSignalToVolumeCommand::execute(
                  	     const std::string& fileNameIn,
                             const std::string& fileNameLabel,
                             const std::string& fileNameLabelToInputTransform3d,
                             const std::string& fileNameOut,
                             bool ascii,
                             const std::string& format,
                             bool verbose )
{

  try
  {

    RoiAverageSignalToVolumeProcess roiAverageSignalToVolumeProcess(
                                        	fileNameLabel,
                                        	fileNameLabelToInputTransform3d,
                                        	fileNameOut,
                                        	ascii,
                                        	format,
                                        	verbose );
    roiAverageSignalToVolumeProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::RoiAverageSignalToVolumeCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameLabel, "
             "const std::string& fileNameLabelToInputTransform3d, "
             "const std::string& fileNameOut, bool ascii, "
             "const std::string& format, bool verbose )" );

}


RegisterCommandCreator(
              RoiAverageSignalToVolumeCommand,
    	      DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    	      DECLARE_STRING_PARAMETER_HELP( fileNameLabel ) +
    	      DECLARE_STRING_PARAMETER_HELP( fileNameLabelToInputTransform3d ) +
    	      DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    	      DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    	      DECLARE_STRING_PARAMETER_HELP( format ) +
    	      DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
