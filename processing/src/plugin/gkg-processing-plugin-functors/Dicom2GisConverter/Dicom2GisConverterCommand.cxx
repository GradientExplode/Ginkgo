#include <gkg-processing-plugin-functors/Dicom2GisConverter/Dicom2GisConverterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>
#include <fstream>


//
// class Dicom2GisProcess
//

class Dicom2GisProcess : public gkg::Process
{

  public:

    Dicom2GisProcess( const std::string& fileNameOut,
                      const std::string& outputFormat,
                      bool noFlip,
		      bool noDemosaic,
                      bool verbose );

    const std::string& getFileNameOut() const;
    const std::string& getOutputFormat() const;
    bool getNoFlip() const;
    bool getNoDemosaic() const;
    bool getVerbose() const;

  private:

    template < class T >
    static void read( gkg::Process& process,
                      const std::string& fileNameIn,
                      const gkg::AnalyzedObject&,
                      const std::string& );

    const std::string& _fileNameOut;
    const std::string& _outputFormat;
    bool _noFlip;
    bool _noDemosaic;
    bool _verbose;

};


Dicom2GisProcess::Dicom2GisProcess( const std::string& fileNameOut,
                                    const std::string& outputFormat,
                          	    bool noFlip,
			  	    bool noDemosaic,
                          	    bool verbose )
            	: gkg::Process( "Volume" ),
            	  _fileNameOut( fileNameOut ),
            	  _outputFormat( outputFormat ),
            	  _noFlip( noFlip ),
	    	  _noDemosaic( noDemosaic ),
            	  _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &Dicom2GisProcess::read< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &Dicom2GisProcess::read< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &Dicom2GisProcess::read< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &Dicom2GisProcess::read< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &Dicom2GisProcess::read< float > );

}


const std::string& Dicom2GisProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& Dicom2GisProcess::getOutputFormat() const
{

  return _outputFormat;

}


bool Dicom2GisProcess::getNoFlip() const
{

  return _noFlip;

}


bool Dicom2GisProcess::getNoDemosaic() const
{

  return _noDemosaic;

}


bool Dicom2GisProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void Dicom2GisProcess::read( gkg::Process& process,
                             const std::string& fileNameIn,
                             const gkg::AnalyzedObject&,
                             const std::string& )
{

  try
  {

    Dicom2GisProcess&
      dicom2GisProcess = static_cast< Dicom2GisProcess& >( process );

    // checking that data is readable and collecting sizes
    if ( dicom2GisProcess.getVerbose() )
    {

      std::cout << "reading Dicom file header: "
                << fileNameIn << std::endl;

    }
    std::string format;
    gkg::Volume< T > inputVolumeProxy;

    if ( dicom2GisProcess.getNoFlip() )
    {

      inputVolumeProxy.getHeader().addAttribute( "no_flip", 1 );

    }
    
    if ( dicom2GisProcess.getNoDemosaic() )
    {

      inputVolumeProxy.getHeader().addAttribute( "no_demosaic", 1 );

    }

    format = gkg::Reader::getInstance().template preparePartialBinaryRead<
                                           gkg::Volume< T > >( fileNameIn,
                                                               inputVolumeProxy,
                                                               "dicom" );

    if ( format != "dicom" )
    {

      throw std::runtime_error( "wrong format, should be \'dicom\'");

    }

    int32_t sizeX = inputVolumeProxy.getSizeX();
    int32_t lineCount = inputVolumeProxy.getSizeY();
    int32_t sliceCount = inputVolumeProxy.getSizeZ();
    int32_t timeCount = inputVolumeProxy.getSizeT();

    // collecting resolution(s)
    double resolutionX;
    double resolutionY;
    double resolutionZ;
    double resolutionT;
    inputVolumeProxy.getHeader().getAttribute( "resolutionX", resolutionX );
    inputVolumeProxy.getHeader().getAttribute( "resolutionY", resolutionY );
    inputVolumeProxy.getHeader().getAttribute( "resolutionZ", resolutionZ );
    inputVolumeProxy.getHeader().getAttribute( "resolutionT", resolutionT );


    // preparing a bounding box and a slice data for slice reading
    gkg::BoundingBox< int32_t > inputBoundingBox( 0, sizeX - 1,
                                                  0, lineCount - 1,
                                                  0, sliceCount - 1,
                                                  0, 0 );
    gkg::Volume< T > inputSubVolume( sizeX, lineCount, sliceCount );

    // preparing partial write proxy
    if ( dicom2GisProcess.getVerbose() )
    {

      std::cout << "preparing partial write in \'"
                << dicom2GisProcess.getOutputFormat()
                << "\' format: "
                << std::flush;

    }
    gkg::VolumeProxy< T >
      outputVolumeProxy( sizeX, lineCount, sliceCount, timeCount );
    outputVolumeProxy.getHeader() = inputVolumeProxy.getHeader();
    outputVolumeProxy.getHeader()[ "item_type" ] =
                                gkg::TypeOf< T >::getName();
    outputVolumeProxy.getHeader()[ "sizeX" ] = sizeX;
    outputVolumeProxy.getHeader()[ "sizeY" ] = lineCount;
    outputVolumeProxy.getHeader()[ "sizeZ" ] = sliceCount;
    outputVolumeProxy.getHeader()[ "sizeT" ] = timeCount;
    outputVolumeProxy.getHeader()[ "resolutionX" ] = resolutionX;
    outputVolumeProxy.getHeader()[ "resolutionY" ] = resolutionY;
    outputVolumeProxy.getHeader()[ "resolutionZ" ] = resolutionZ;
    outputVolumeProxy.getHeader()[ "resolutionT" ] = resolutionT;

    if ( outputVolumeProxy.getHeader().hasAttribute( "no_flip" ) )
    {

      outputVolumeProxy.getHeader().removeAttribute( "no_flip" );

    }

    if ( outputVolumeProxy.getHeader().hasAttribute( "no_demosaic" ) )
    {

      outputVolumeProxy.getHeader().removeAttribute( "no_demosaic" );

    }

    gkg::BoundingBox< int32_t > outputBoundingBox( 0, sizeX - 1,
                                                   0, lineCount - 1,
                                                   0, sliceCount - 1,
                                                   0, 0 );

    if ( gkg::Writer::getInstance().template preparePartialBinaryWrite< 
                                              gkg::Volume< T > >(
                                         dicom2GisProcess.getFileNameOut(),
                                         outputVolumeProxy,
                                         dicom2GisProcess.getOutputFormat() ) !=
         dicom2GisProcess.getOutputFormat() )
    {

      throw std::runtime_error(
      std::string( "failed to prepare partial write with format \' " ) +
      dicom2GisProcess.getOutputFormat() + "\'" );

    }
    if ( dicom2GisProcess.getVerbose() )
    {

      std::cout << "done " << std::endl;

    }

    // looping over ranks(s)
    if ( dicom2GisProcess.getVerbose() )
    {

      std::cout << "converting: "
                << std::flush;

    }

    int32_t time;
    for ( time = 0; time < timeCount; time++ )
    {

      if ( dicom2GisProcess.getVerbose() )
      {

        if ( time != 0 )
        {

          std::cout << gkg::Eraser( 20 );

        }
        std::cout << "rank[ " << std::setw( 4 ) << time + 1 
                  << " / " << std::setw( 4 ) << timeCount
                  << " ] " << std::flush;

      }

      // reading reco data from 2dsequ Bruker file
      inputBoundingBox.setLowerT( time );
      inputBoundingBox.setUpperT( time );
      format =
        gkg::Reader::getInstance().template partialBinaryRead<
                                             gkg::Volume< T > >(
                                                         inputVolumeProxy,
                                                         inputSubVolume,
                                                         inputBoundingBox,
                                                         "dicom" );

      if ( format != "dicom" )
      {

        throw std::runtime_error(
                              "wrong format, should be \'dicom\'");

      }

      // saving sub-volume(s) to disk
      outputBoundingBox.setLowerT( time );
      outputBoundingBox.setUpperT( time );
      gkg::Writer::getInstance().template partialBinaryWrite<
                                             gkg::Volume< T > >(
                                           outputVolumeProxy,
                                           inputSubVolume,
                                           outputBoundingBox,
                                           dicom2GisProcess.getOutputFormat() );

    }

    if ( dicom2GisProcess.getVerbose() )
    {

      std::cout << std::endl;

    }

    // Writing .bvec and .bval files for diffusion datasets
    if ( inputVolumeProxy.getHeader().hasAttribute( "b_values" ) &&
         inputVolumeProxy.getHeader().hasAttribute( "diffusion_directions" ) )
    {

      gkg::TypedDiskFormat< gkg::Volume< T > >* diskFormat =
        gkg::DiskFormatFactory< gkg::Volume< T > >::getInstance().getDiskFormat(
                                           dicom2GisProcess.getOutputFormat() );

      if ( diskFormat )
      {

        std::string nameWithoutExtension = diskFormat->getNameWithoutExtension(
                                            dicom2GisProcess.getFileNameOut() );
        std::string bValFileName = nameWithoutExtension + ".bval";
        std::string bVecFileName = nameWithoutExtension + ".bvec";
        std::vector< double > bValues( timeCount, 0.0 );
        std::vector< gkg::Vector3d< double > > bVectors;
        gkg::GenericObjectList golDiffDir;

        inputVolumeProxy.getHeader().getAttribute( "b_values", bValues );
        inputVolumeProxy.getHeader().getAttribute( "diffusion_directions", 
                                                   golDiffDir );

        gkg::GenericObjectList::iterator
          d = golDiffDir.begin(),
          de = golDiffDir.end();

        while ( d != de )
        {

          gkg::GenericObjectList direction = 
                                     (*d)->getValue< gkg::GenericObjectList >();

          gkg::Vector3d< double > dirVec( direction[ 0 ]->getScalar(),
                                          direction[ 1 ]->getScalar(),
                                          direction[ 2 ]->getScalar() );

          bVectors.push_back( dirVec );
          ++d;

        }

        // writing bval
        std::ofstream ofs( bValFileName.c_str() );

        if ( ofs.bad() )
        {
    
          throw std::runtime_error( "Cannot create .bval file" );
    
        }

        if ( dicom2GisProcess.getVerbose() )
        {

          std::cout << "Writing " << bValFileName << std::flush;

        }

        std::vector< double >::const_iterator
          v = bValues.begin(),
          ve = bValues.end();

        while ( v != ve )
        {

          ofs << *v << " ";
          ++v;

        }

        ofs << std::endl;
        ofs.close();

        if ( dicom2GisProcess.getVerbose() )
        {

          std::cout << " done" << std::endl;

        }

        // writing bvec
        ofs.open( bVecFileName.c_str() );

        if ( ofs.bad() )
        {
    
          throw std::runtime_error( "Cannot create .bvec file" );
    
        }

        if ( dicom2GisProcess.getVerbose() )
        {

          std::cout << "Writing " << bVecFileName << std::flush;

        }

        // x
        std::vector< gkg::Vector3d< double > >::const_iterator
          dir = bVectors.begin(),
          dire = bVectors.end();

        while ( dir != dire )
        {

          ofs << dir->x << " ";
          ++dir;

        }

        ofs << std::endl;

        // y
        dir = bVectors.begin();

        while ( dir != dire )
        {

          ofs << dir->y << " ";
          ++dir;

        }

        ofs << std::endl;

        // z
        dir = bVectors.begin();

        while ( dir != dire )
        {

          ofs << dir->z << " ";
          ++dir;

        }

        ofs << std::endl;
        ofs.close();

        if ( dicom2GisProcess.getVerbose() )
        {

          std::cout << " done" << std::endl;

        }

      }

    }
  
  }
  GKG_CATCH( "template < class T > "
             "void Dicom2GisProcess::read( gkg::Process& process, "
             "const std::string& fileNameReference, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


gkg::Dicom2GisConverterCommand::Dicom2GisConverterCommand( int32_t argc,
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
  GKG_CATCH( "gkg::Dicom2GisConverterCommand::Dicom2GisConverterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::Dicom2GisConverterCommand::Dicom2GisConverterCommand(
                                		const std::string& fileNameIn,
                                		const std::string& fileNameOut,
                                		const std::string& outputFormat,
                                		bool noFlip,
                                		bool noDemosaic,
                                		bool verbose )
                              : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, outputFormat,
             noFlip, noDemosaic, verbose );

  }
  GKG_CATCH( "gkg::Dicom2GisConverterCommand::Dicom2GisConverterCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& outputFormat, "
             "bool noFlip, bool noDemosaic, bool verbose )" );

}


gkg::Dicom2GisConverterCommand::Dicom2GisConverterCommand(
                                             const gkg::Dictionary& parameters )
                              : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, noFlip );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, noDemosaic );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, outputFormat,
             noFlip, noDemosaic, verbose );

  }
  GKG_CATCH( "gkg::Dicom2GisConverterCommand::Dicom2GisConverterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::Dicom2GisConverterCommand::~Dicom2GisConverterCommand()
{
}


std::string gkg::Dicom2GisConverterCommand::getStaticName()
{

  try
  {

    return "Dicom2GisConverter";

  }
  GKG_CATCH( "std::string gkg::Dicom2GisConverterCommand::getStaticName()" );

}


void gkg::Dicom2GisConverterCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string outputFormat = "gis";
    bool noFlip = false;
    bool noDemosaic = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "DICOM to GIS disk format converter",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input DICOM directory name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output image file name",
                                 fileNameOut );
    application.addSingleOption( "-format",
                                 "Output format (default=gis)",
                                 outputFormat,
                                 true );
    application.addSingleOption( "-noFlip",
                                 "No automatic flip of dataset (default=false)",
                                 noFlip,
                                 true );
    application.addSingleOption( "-noDemosaic",
                                 "No automatic demosaicing of dataset "
				 "(default=false)",
                                 noDemosaic,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn, fileNameOut, outputFormat,
             noFlip, noDemosaic, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::Dicom2GisConverterCommand::parse()" );

}


void gkg::Dicom2GisConverterCommand::execute( const std::string& fileNameIn,
                                	      const std::string& fileNameOut,
                                	      const std::string& outputFormat,
                                	      bool noFlip,
                                	      bool noDemosaic,
                                	      bool verbose )
{

  try
  {

    Dicom2GisProcess dicom2GisProcess( fileNameOut, 
                             	       outputFormat, 
			     	       noFlip, 
			     	       noDemosaic,
			     	       verbose );
    dicom2GisProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::Dicom2GisConverterCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& outputFormat, "
             "bool noFlip, bool noDemosaic, bool verbose )" );

}


RegisterCommandCreator( Dicom2GisConverterCommand,
                        DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                        DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( noFlip ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( noDemosaic ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
