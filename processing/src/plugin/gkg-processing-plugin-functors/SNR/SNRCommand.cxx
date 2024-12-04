#include <gkg-processing-plugin-functors/SNR/SNRCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <cmath>


//
//   SNRProcess
//


class SNRProcess : public gkg::Process
{

  public:

    SNRProcess( const std::string& fileNameMask,
                const std::vector< int32_t >& signalBoundingBox,
                const std::vector< int32_t >& noiseBoundingBox,
                const int32_t& time,
                const std::string& fileNameOut,
                const bool& verbose );

    const std::string& getFileNameMask() const;
    const std::vector< int32_t >& getSignalBoundingBox() const;
    const std::vector< int32_t >& getNoiseBoundingBox() const;
    const int32_t& getTime() const;
    const std::string& getFileNameOut() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void snr( gkg::Process& process,
                     const std::string& fileNameIn,
                     const gkg::AnalyzedObject&,
                     const std::string& );

    const std::string& _fileNameMask;
    const std::vector< int32_t >& _signalBoundingBox;
    const std::vector< int32_t >& _noiseBoundingBox;
    const int32_t& _time;
    const std::string& _fileNameOut;
    const bool& _verbose;

};


SNRProcess::SNRProcess( const std::string& fileNameMask,
                        const std::vector< int32_t >& signalBoundingBox,
                        const std::vector< int32_t >& noiseBoundingBox,
                        const int32_t& time,
                        const std::string& fileNameOut,
                        const bool& verbose )
           : gkg::Process(),
             _fileNameMask( fileNameMask ),
             _signalBoundingBox( signalBoundingBox ),
             _noiseBoundingBox( noiseBoundingBox ),
             _time( time ),
             _fileNameOut( fileNameOut ),
             _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &SNRProcess::snr< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &SNRProcess::snr< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &SNRProcess::snr< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &SNRProcess::snr< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &SNRProcess::snr< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &SNRProcess::snr< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &SNRProcess::snr< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &SNRProcess::snr< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &SNRProcess::snr< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &SNRProcess::snr< double > );

}


const std::string& SNRProcess::getFileNameMask() const
{

  return _fileNameMask;

}


const std::vector< int32_t >& SNRProcess::getSignalBoundingBox() const
{

  return _signalBoundingBox;

}


const std::vector< int32_t >& SNRProcess::getNoiseBoundingBox() const
{

  return _noiseBoundingBox;

}


const int32_t& SNRProcess::getTime() const
{

  return _time;

}


const std::string& SNRProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const bool& SNRProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void SNRProcess::snr( gkg::Process& process,
                      const std::string& fileNameIn,
                      const gkg::AnalyzedObject&,
                      const std::string& )
{

  try
  {

    SNRProcess&
      snrProcess = static_cast< SNRProcess& >( process );


    double signalMean = 0.0;
    double noiseMean = 0.0;
    double noiseStdDev = 0.0;

    if ( snrProcess.getFileNameMask().empty() )
    {

      // reading signal sub-volume
      if ( snrProcess.getVerbose() )
      {

        std::cout << "reading signal bounding box : " << std::flush;

      }
      int32_t signalSizeX = snrProcess.getSignalBoundingBox()[ 3 ] -
                            snrProcess.getSignalBoundingBox()[ 0 ] + 1;
      int32_t signalSizeY = snrProcess.getSignalBoundingBox()[ 4 ] -
                            snrProcess.getSignalBoundingBox()[ 1 ] + 1;
      int32_t signalSizeZ = snrProcess.getSignalBoundingBox()[ 5 ] -
                            snrProcess.getSignalBoundingBox()[ 2 ] + 1;

      gkg::BoundingBox< int32_t > signalBox( 
                                         snrProcess.getSignalBoundingBox()[ 0 ],
                                         snrProcess.getSignalBoundingBox()[ 3 ],
                                         snrProcess.getSignalBoundingBox()[ 1 ],
                                         snrProcess.getSignalBoundingBox()[ 4 ],
                                         snrProcess.getSignalBoundingBox()[ 2 ],
                                         snrProcess.getSignalBoundingBox()[ 5 ],
                                         snrProcess.getTime(),
                                         snrProcess.getTime() );
      gkg::VolumeProxy< T > signalProxy;
      gkg::Reader::getInstance().template preparePartialBinaryRead<
                                              gkg::Volume< T > >( fileNameIn,
                                                                  signalProxy );
      gkg::Volume< T > signal( signalSizeX, signalSizeY, signalSizeZ );
      gkg::Reader::getInstance().template partialBinaryRead< 
                                                gkg::Volume< T > >( signalProxy,
                                                                    signal,
                                                                    signalBox );
      if ( snrProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // reading noise sub-volume
      if ( snrProcess.getVerbose() )
      {

        std::cout << "reading noise bounding box : " << std::flush;

      }
      int32_t noiseSizeX = snrProcess.getNoiseBoundingBox()[ 3 ] -
                           snrProcess.getNoiseBoundingBox()[ 0 ] + 1;
      int32_t noiseSizeY = snrProcess.getNoiseBoundingBox()[ 4 ] -
                           snrProcess.getNoiseBoundingBox()[ 1 ] + 1;
      int32_t noiseSizeZ = snrProcess.getNoiseBoundingBox()[ 5 ] -
                           snrProcess.getNoiseBoundingBox()[ 2 ] + 1;

      gkg::BoundingBox< int32_t > noiseBox( 
                                        snrProcess.getNoiseBoundingBox()[ 0 ],
                                        snrProcess.getNoiseBoundingBox()[ 3 ],
                                        snrProcess.getNoiseBoundingBox()[ 1 ],
                                        snrProcess.getNoiseBoundingBox()[ 4 ],
                                        snrProcess.getNoiseBoundingBox()[ 2 ],
                                        snrProcess.getNoiseBoundingBox()[ 5 ],
                                        snrProcess.getTime(),
                                        snrProcess.getTime() );
      gkg::VolumeProxy< T > noiseProxy;
      gkg::Reader::getInstance().template preparePartialBinaryRead<
                                               gkg::Volume< T > >( fileNameIn,
                                                                   noiseProxy );
      gkg::Volume< T > noise( noiseSizeX, noiseSizeY, noiseSizeZ );
      gkg::Reader::getInstance().template partialBinaryRead< 
                                                 gkg::Volume< T > >( noiseProxy,
                                                                     noise,
                                                                     noiseBox );
      if ( snrProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // processing signal mean
      if ( snrProcess.getVerbose() )
      {

        std::cout << "processing signal mean : " << std::flush;

      }
      typename gkg::Volume< T >::const_iterator s = signal.begin(),
                                                se = signal.end();
      signalMean = 0.0;
      while ( s != se )
      {

        signalMean += ( double )*s;
        ++ s;

      }
      signalMean /= signal.getSizeXYZT();
      if ( snrProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // processing noise mean
      if ( snrProcess.getVerbose() )
      {

        std::cout << "processing noise mean : " << std::flush;

      }
      typename gkg::Volume< T >::const_iterator n = noise.begin(),
                                                ne = noise.end();
      noiseMean = 0.0;
      while ( n != ne )
      {

        noiseMean += ( double )*n;
        ++ n;

      }
      noiseMean /= noise.getSizeXYZT();
      if ( snrProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }


      // processing noise standard deviation
      if ( snrProcess.getVerbose() )
      {

        std::cout << "processing noise standard deviation : " << std::flush;

      }
      noiseStdDev = 0.0;
      n = noise.begin();
      while ( n != ne )
      {

        noiseStdDev += ( ( double )*n - noiseMean ) *
                       ( ( double )*n - noiseMean );
        ++ n;

      }
      noiseStdDev /= ( noise.getSizeXYZT() - 1 );
      noiseStdDev = std::sqrt( noiseStdDev );
      if ( snrProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      // reading mask of signal
      if ( snrProcess.getVerbose() )
      {

        std::cout << "reading signal mask : " << std::flush;

      }
      gkg::Volume< int16_t > mask;
      gkg::Reader::getInstance().read( snrProcess.getFileNameMask(), mask );
      if ( snrProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // reading signal volume
      if ( snrProcess.getVerbose() )
      {

        std::cout << "reading input volume : " << std::flush;

      }
      gkg::VolumeProxy< T > volumeProxy;
      gkg::Reader::getInstance().template preparePartialBinaryRead<
                                              gkg::Volume< T > >( fileNameIn,
                                                                  volumeProxy );
      int32_t sizeX = 0;
      int32_t sizeY = 0;
      int32_t sizeZ = 0;
      volumeProxy.getHeader().getAttribute( "sizeX", sizeX );
      volumeProxy.getHeader().getAttribute( "sizeY", sizeY );
      volumeProxy.getHeader().getAttribute( "sizeZ", sizeZ );

      if ( ( mask.getSizeX() != sizeX ) ||
           ( mask.getSizeY() != sizeY ) ||
           ( mask.getSizeZ() != sizeZ ) )
      {

        throw std::runtime_error( "inconsistent mask and input size" );

      }
      gkg::BoundingBox< int32_t > volumeBox( 0, sizeX - 1,
                                             0, sizeY - 1,
                                             0, sizeZ - 1,
                                             snrProcess.getTime(),
                                             snrProcess.getTime() );

      gkg::Volume< T > volume( sizeX, sizeY, sizeZ );
      gkg::Reader::getInstance().template partialBinaryRead<
                                                gkg::Volume< T > >( volumeProxy,
                                                                    volume,
                                                                    volumeBox );
      if ( snrProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // processing signal and noise mean
      if ( snrProcess.getVerbose() )
      {

        std::cout << "processing signal and noise mean : " << std::flush;

      }
      typename gkg::Volume< T >::const_iterator v = volume.begin(),
                                                ve = volume.end();
      gkg::Volume< int16_t >::const_iterator m = mask.begin();
      int32_t signalCount = 0;
      int32_t noiseCount = 0;
      signalMean = 0.0;
      noiseMean = 0.0;
      while ( v != ve )
      {

        if ( *m )
        {

          signalMean += ( double )*v;
          ++ signalCount;

        }
        else
        {

          noiseMean += ( double )*v;
          ++ noiseCount;

        }
        ++ v;
        ++ m;

      }
      signalMean /= signalCount;
      noiseMean /= noiseCount;
      if ( snrProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // processing noise standard deviation
      if ( snrProcess.getVerbose() )
      {

        std::cout << "processing noise standard deviation : " << std::flush;

      }
      v = volume.begin();
      m = mask.begin();
      noiseStdDev = 0.0;
      while ( v != ve )
      {

        if ( !*m )
        {

          noiseStdDev += ( ( double )*v - noiseMean ) *
                         ( ( double )*v - noiseMean ) ;

        }
        ++ v;
        ++ m;

      }
      noiseStdDev /= ( noiseCount - 1 );
      noiseStdDev = std::sqrt( noiseStdDev );
      if ( snrProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

    if ( snrProcess.getFileNameOut().empty() )
    {

      std::cout << "signal mean : " << signalMean << std::endl;
      std::cout << "noise mean : " << noiseMean << std::endl;
      std::cout << "noise std dev : " << noiseStdDev << std::endl;
      std::cout << "SNR : " << signalMean / noiseStdDev << std::endl;
      std::cout << "SNR(dB) : " << 20.0 * std::log10( signalMean / noiseStdDev )
                << std::endl;

    }
    else
    {

      if ( snrProcess.getVerbose() )
      {

        std::cout << "saving snr file : " << std::flush;

      }
      std::ofstream os( snrProcess.getFileNameOut().c_str() );
      os << signalMean << std::endl;
      os << noiseMean << std::endl;
      os << noiseStdDev << std::endl;
      os << signalMean / noiseStdDev << std::endl;
      os << 20.0 * std::log10( signalMean / noiseStdDev ) << std::endl;
      os.close();
      if ( snrProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void SNRProcess::snr( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   SNRCommand
//


gkg::SNRCommand::SNRCommand( int32_t argc,
                             char* argv[],
                             bool loadPlugin,
                             bool removeFirst )
               : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::SNRCommand::SNRCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::SNRCommand::SNRCommand( const std::string& fileNameIn,
                	     const std::string& fileNameMask,
                	     std::vector< int32_t >& signalBoundingBox,
                	     std::vector< int32_t >& noiseBoundingBox,
                	     int32_t time,
                	     const std::string& fileNameOut,
                	     bool verbose )
               : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameMask, signalBoundingBox, noiseBoundingBox,
             time, fileNameOut, verbose );

  }
  GKG_CATCH( "gkg::SNRCommand::SNRCommand( "
             "const std::string& fileNameIn, const std::string& fileNameMask, "
             "std::vector< int32_t >& signalBoundingBox, "
             "std::vector< int32_t >& noiseBoundingBox,  int32_t time, "
             "const std::string& fileNameOut, bool verbose )" );

}


gkg::SNRCommand::SNRCommand( const gkg::Dictionary& parameters )
               : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters,
                                          std::vector< int32_t >,
                                          signalBoundingBox );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters,
                                          std::vector< int32_t >,
                                          noiseBoundingBox );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, time );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameMask, signalBoundingBox, noiseBoundingBox,
             time, fileNameOut, verbose );

  }
  GKG_CATCH( "gkg::SNRCommand::SNRCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::SNRCommand::~SNRCommand()
{
}


std::string gkg::SNRCommand::getStaticName()
{

  try
  {

    return "SNR";

  }
  GKG_CATCH( "std::string gkg::SNRCommand::getStaticName()" );

}


void gkg::SNRCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameMask;
    std::vector< int32_t > signalBoundingBox;
    std::vector< int32_t > noiseBoundingBox;
    int32_t time = 0;
    std::string fileNameOut;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "SNR processing in volumes",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-m",
                                 "Mask name",
                                 fileNameMask,
                                 true );
    application.addSeriesOption( "-s",
                                 "Signal ROI defined by a bounding box"
                                 " ( x1, y1, z1, x2, y2, z2 )",
                                 signalBoundingBox,
                                 0U, 6U );
    application.addSeriesOption( "-n",
                                 "Noise ROI defined by a bounding box"
                                 " ( x1, y1, z1, x2, y2, z2 )",
                                 noiseBoundingBox,
                                 0U, 6U );
    application.addSingleOption( "-t",
                                 "Rank time (default=0)",
                                 time,
                                 true );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn, fileNameMask, signalBoundingBox, noiseBoundingBox,
             time, fileNameOut, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::SNRCommand::parse()" );

}


void gkg::SNRCommand::execute( const std::string& fileNameIn,
                	       const std::string& fileNameMask,
                	       std::vector< int32_t >& signalBoundingBox,
                	       std::vector< int32_t >& noiseBoundingBox,
                	       int32_t time,
                	       const std::string& fileNameOut,
                	       bool verbose )
{

  try
  {

    if ( !signalBoundingBox.empty() &&
         !noiseBoundingBox.empty() &&
         !fileNameMask.empty() )
    {

      throw std::runtime_error( "cannot use -m option woth -n,-s options" ); 

    }

    if ( ( !signalBoundingBox.empty() && noiseBoundingBox.empty() ) ||
         ( signalBoundingBox.empty() && !noiseBoundingBox.empty() ) )
    {

      throw std::runtime_error( "both signal and noise bounding box are "
                                "required" );

    }

    if ( !signalBoundingBox.empty() &&
         ( signalBoundingBox.size() != 6U ) )
    {

      throw std::runtime_error( "not a valid signal bounding box" );

    }

    if ( !noiseBoundingBox.empty() &&
         ( noiseBoundingBox.size() != 6U ) )
    {

      throw std::runtime_error( "not a valid noise bounding box" );

    }

    // reordering coordinates
    if ( fileNameMask.empty() )
    {

      if ( signalBoundingBox[ 0 ] > signalBoundingBox[ 3 ] )
      {

        std::swap( signalBoundingBox[ 0 ], signalBoundingBox[ 3 ] );

      }
      if ( signalBoundingBox[ 1 ] > signalBoundingBox[ 4 ] )
      {

        std::swap( signalBoundingBox[ 1 ], signalBoundingBox[ 4 ] );

      }
      if ( signalBoundingBox[ 2 ] > signalBoundingBox[ 5 ] )
      {

        std::swap( signalBoundingBox[ 2 ], signalBoundingBox[ 5 ] );

      }
      if ( noiseBoundingBox[ 0 ] > noiseBoundingBox[ 3 ] )
      {

        std::swap( noiseBoundingBox[ 0 ], noiseBoundingBox[ 3 ] );

      }
      if ( noiseBoundingBox[ 1 ] > noiseBoundingBox[ 4 ] )
      {

        std::swap( noiseBoundingBox[ 1 ], noiseBoundingBox[ 4 ] );

      }
      if ( noiseBoundingBox[ 2 ] > noiseBoundingBox[ 5 ] )
      {

        std::swap( noiseBoundingBox[ 2 ], noiseBoundingBox[ 5 ] );

      }

    }

    //
    // launching process
    //
    SNRProcess snrProcess( fileNameMask,
                           signalBoundingBox,
                           noiseBoundingBox,
                           time,
                           fileNameOut,
                           verbose );
    snrProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::SNRCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameMask, "
             "std::vector< int32_t >& signalBoundingBox, "
             "std::vector< int32_t >& noiseBoundingBox,  int32_t time, "
             "const std::string& fileNameOut, bool verbose )" );

}


RegisterCommandCreator(
                SNRCommand,
    		DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    		DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    		DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( signalBoundingBox ) +
    		DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( noiseBoundingBox ) +
    		DECLARE_INTEGER_PARAMETER_HELP( time ) +
    		DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    		DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
