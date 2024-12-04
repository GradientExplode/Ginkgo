#include <gkg-processing-plugin-functors/GetMask/GetMaskCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-mask/MorphoFilledMask.h>
#include <gkg-processing-mask/ClosedMask.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


//
// class MorphoFilledMaskProcess
//

class MorphoFilledMaskProcess : public gkg::Process
{

  public:

    MorphoFilledMaskProcess( const std::string& fileNameOut,
                             const std::string& outType,
                             const float& radius,
                             const double& noisePosition,
                             const double& noiseRatio,
                             const int32_t& thrown,
                             const int32_t& kept,
                             const double& foreground,
                             const double& background,
                             const bool& ascii,
                             const std::string& format,
                             const bool& verbose );

    const std::string& getFileNameOut() const;
    const std::string& getOutType() const;
    const float& getRadius() const;
    const double& getNoisePosition() const;
    const double& getNoiseRatio() const;
    const int32_t& getThrown() const;
    const int32_t& getKept() const;
    const double& getForeground() const;
    const double& getBackground() const;
    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void mask( gkg::Process& process,
                      const std::string& fileNameIn,
                      const gkg::AnalyzedObject&,
                      const std::string& );

    const std::string& _fileNameOut;
    const std::string& _outType;
    const float& _radius;
    const double& _noisePosition;
    const double& _noiseRatio;
    const int32_t& _thrown;
    const int32_t& _kept;
    const double& _foreground;
    const double& _background;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


MorphoFilledMaskProcess::MorphoFilledMaskProcess(
                                                 const std::string& fileNameOut,
                                                 const std::string& outType,
                                                 const float& radius,
                                                 const double& noisePosition,
                                                 const double& noiseRatio,
                                                 const int32_t& thrown,
                                                 const int32_t& kept,
                                                 const double& foreground,
                                                 const double& background,
                                                 const bool& ascii,
                                                 const std::string& format,
                                                 const bool& verbose )
                        : gkg::Process( "Volume" ),
                          _fileNameOut( fileNameOut ),
                          _outType( outType ),
                          _radius( radius ),
                          _noisePosition( noisePosition ),
                          _noiseRatio( noiseRatio ),
                          _thrown( thrown ),
                          _kept( kept ),
                          _foreground( foreground ),
                          _background( background ),
                          _ascii( ascii ),
                          _format( format ),
                          _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &MorphoFilledMaskProcess::mask< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &MorphoFilledMaskProcess::mask< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &MorphoFilledMaskProcess::mask< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &MorphoFilledMaskProcess::mask< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &MorphoFilledMaskProcess::mask< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &MorphoFilledMaskProcess::mask< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &MorphoFilledMaskProcess::mask< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &MorphoFilledMaskProcess::mask< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &MorphoFilledMaskProcess::mask< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &MorphoFilledMaskProcess::mask< double > );

}


const std::string& MorphoFilledMaskProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& MorphoFilledMaskProcess::getOutType() const
{

  return _outType;

}


const float& MorphoFilledMaskProcess::getRadius() const
{

  return _radius;

}


const double& MorphoFilledMaskProcess::getNoisePosition() const
{

  return _noisePosition;

}


const double& MorphoFilledMaskProcess::getNoiseRatio() const
{

  return _noiseRatio;

}


const int32_t& MorphoFilledMaskProcess::getThrown() const
{

  return _thrown;

}


const int32_t& MorphoFilledMaskProcess::getKept() const
{

  return _kept;

}


const double& MorphoFilledMaskProcess::getForeground() const
{

  return _foreground;

}


const double& MorphoFilledMaskProcess::getBackground() const
{

  return _background;

}


const bool& MorphoFilledMaskProcess::getAscii() const
{

  return _ascii;

}


const std::string& MorphoFilledMaskProcess::getFormat() const
{

  return _format;

}


const bool& MorphoFilledMaskProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void MorphoFilledMaskProcess::mask( gkg::Process& process,
                                    const std::string& fileNameIn,
                                    const gkg::AnalyzedObject&,
                                    const std::string& )
{

  try
  {

    MorphoFilledMaskProcess&
      maskProcess = static_cast< MorphoFilledMaskProcess& >( process );

    // reading data
    if ( maskProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    if ( maskProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    float radius = maskProcess.getRadius();
    if ( radius <= 0 )
    {

      if ( volumeIn.getHeader().hasAttribute( "resolutionX" ) &&
           volumeIn.getHeader().hasAttribute( "resolutionY" ) &&
           volumeIn.getHeader().hasAttribute( "resolutionZ" ) )
      {

        double resolutionX = 0.0;
        double resolutionY = 0.0;
        double resolutionZ = 0.0;
        volumeIn.getHeader().getAttribute( "resolutionX",
                                           resolutionX );
        volumeIn.getHeader().getAttribute( "resolutionY",
                                           resolutionY );
        volumeIn.getHeader().getAttribute( "resolutionZ",
                                           resolutionZ );
        radius = 2.0 * std::min( resolutionX,
                                 std::min( resolutionY, resolutionZ ) );

      }
      else
      {

        radius = 1.0;

      }

    }

    if ( maskProcess.getVerbose() )
    {

      std::cout << "radius of closing : " << radius << " mm" << std::endl;
      std::cout << "noise position : " << maskProcess.getNoisePosition()
                << std::endl;
      std::cout << "noise ratio : " << maskProcess.getNoiseRatio()
                << std::endl;
      std::cout << "thrown maximum : " << maskProcess.getThrown()
                << std::endl;
      std::cout << "kept maximum : " << maskProcess.getKept()
                << std::endl;

    }


    if ( maskProcess.getVerbose() )
    {

      std::cout << "processing mask : " << std::flush;

    }


    std::string outType = maskProcess.getOutType();
    if ( outType.empty() )
    {

      outType = gkg::TypeOf< int16_t >::getName();

    }

    if ( gkg::TypeOf< uint8_t >::isSameType( outType ) )
    {

      gkg::Volume< uint8_t > volumeOut;

      gkg::MorphoFilledMask< T, uint8_t >
        morphoFilledMask( radius,
                          maskProcess.getNoisePosition(),
                          maskProcess.getNoiseRatio(),
                          maskProcess.getThrown(),
                          maskProcess.getKept(),
                          ( uint8_t )maskProcess.getForeground(),
                          ( uint8_t )maskProcess.getBackground() );

      morphoFilledMask.getMask( volumeIn, volumeOut, maskProcess.getVerbose() );

      if ( maskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( maskProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << maskProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( maskProcess.getFileNameOut(),
                                        volumeOut,
                                        maskProcess.getAscii(),
                                        maskProcess.getFormat() );
      if ( maskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int16_t >::isSameType( outType ) )
    {

      gkg::Volume< int16_t > volumeOut;

      gkg::MorphoFilledMask< T, int16_t >
        morphoFilledMask( radius,
                          maskProcess.getNoisePosition(),
                          maskProcess.getNoiseRatio(),
                          maskProcess.getThrown(),
                          maskProcess.getKept(),
                          ( int16_t )maskProcess.getForeground(),
                          ( int16_t )maskProcess.getBackground() );

      morphoFilledMask.getMask( volumeIn, volumeOut, maskProcess.getVerbose() );

      if ( maskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( maskProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << maskProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( maskProcess.getFileNameOut(),
                                        volumeOut,
                                        maskProcess.getAscii(),
                                        maskProcess.getFormat() );
      if ( maskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< float >::isSameType( outType ) )
    {

      gkg::Volume< float > volumeOut;

      gkg::MorphoFilledMask< T, float >
        morphoFilledMask( radius,
                          maskProcess.getNoisePosition(),
                          maskProcess.getNoiseRatio(),
                          maskProcess.getThrown(),
                          maskProcess.getKept(),
                          ( float )maskProcess.getForeground(),
                          ( float )maskProcess.getBackground() );

      morphoFilledMask.getMask( volumeIn, volumeOut, maskProcess.getVerbose() );

      if ( maskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( maskProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << maskProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( maskProcess.getFileNameOut(),
                                        volumeOut,
                                        maskProcess.getAscii(),
                                        maskProcess.getFormat() );
      if ( maskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< double >::isSameType( outType ) )
    {

      gkg::Volume< double > volumeOut;

      gkg::MorphoFilledMask< T, double >
        morphoFilledMask( radius,
                          maskProcess.getNoisePosition(),
                          maskProcess.getNoiseRatio(),
                          maskProcess.getThrown(),
                          maskProcess.getKept(),
                          ( double )maskProcess.getForeground(),
                          ( double )maskProcess.getBackground() );

      morphoFilledMask.getMask( volumeIn, volumeOut, maskProcess.getVerbose() );

      if ( maskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( maskProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << maskProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( maskProcess.getFileNameOut(),
                                        volumeOut,
                                        maskProcess.getAscii(),
                                        maskProcess.getFormat() );
      if ( maskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void MorphoFilledMaskProcess::operate( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class ClosedMaskProcess
//

class ClosedMaskProcess : public gkg::Process
{

  public:

    ClosedMaskProcess( const std::string& fileNameOut,
                       const std::string& outType,
                       const float& radius,
                       const double& thresholdRatio,
                       const double& foreground,
                       const double& background,
                       const bool& ascii,
                       const std::string& format,
                       const bool& verbose );

    const std::string& getFileNameOut() const;
    const std::string& getOutType() const;
    const float& getRadius() const;
    const double& getThresholdRatio() const;
    const double& getForeground() const;
    const double& getBackground() const;
    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void mask( gkg::Process& process,
                      const std::string& fileNameIn,
                      const gkg::AnalyzedObject&,
                      const std::string& );

    const std::string& _fileNameOut;
    const std::string& _outType;
    const float& _radius;
    const double& _thresholdRatio;
    const double& _foreground;
    const double& _background;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


ClosedMaskProcess::ClosedMaskProcess( const std::string& fileNameOut,
                                      const std::string& outType,
                                      const float& radius,
                                      const double& thresholdRatio,
                                      const double& foreground,
                                      const double& background,
                                      const bool& ascii,
                                      const std::string& format,
                                      const bool& verbose )
                  : gkg::Process( "Volume" ),
                    _fileNameOut( fileNameOut ),
                    _outType( outType ),
                    _radius( radius ),
                    _thresholdRatio( thresholdRatio ),
                    _foreground( foreground ),
                    _background( background ),
                    _ascii( ascii ),
                    _format( format ),
                    _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &ClosedMaskProcess::mask< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &ClosedMaskProcess::mask< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &ClosedMaskProcess::mask< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &ClosedMaskProcess::mask< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &ClosedMaskProcess::mask< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &ClosedMaskProcess::mask< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &ClosedMaskProcess::mask< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &ClosedMaskProcess::mask< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &ClosedMaskProcess::mask< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &ClosedMaskProcess::mask< double > );

}


const std::string& ClosedMaskProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& ClosedMaskProcess::getOutType() const
{

  return _outType;

}


const float& ClosedMaskProcess::getRadius() const
{

  return _radius;

}


const double& ClosedMaskProcess::getThresholdRatio() const
{

  return _thresholdRatio;

}


const double& ClosedMaskProcess::getForeground() const
{

  return _foreground;

}


const double& ClosedMaskProcess::getBackground() const
{

  return _background;

}


const bool& ClosedMaskProcess::getAscii() const
{

  return _ascii;

}


const std::string& ClosedMaskProcess::getFormat() const
{

  return _format;

}


const bool& ClosedMaskProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void ClosedMaskProcess::mask( gkg::Process& process,
                              const std::string& fileNameIn,
                              const gkg::AnalyzedObject&,
                              const std::string& )
{

  try
  {

    ClosedMaskProcess&
      maskProcess = static_cast< ClosedMaskProcess& >( process );

    // reading data
    if ( maskProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    if ( maskProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    float radius = maskProcess.getRadius();
    if ( radius <= 0 )
    {

      if ( volumeIn.getHeader().hasAttribute( "resolutionX" ) &&
           volumeIn.getHeader().hasAttribute( "resolutionY" ) &&
           volumeIn.getHeader().hasAttribute( "resolutionZ" ) )
      {

        double resolutionX = 0.0;
        double resolutionY = 0.0;
        double resolutionZ = 0.0;
        volumeIn.getHeader().getAttribute( "resolutionX",
                                           resolutionX );
        volumeIn.getHeader().getAttribute( "resolutionY",
                                           resolutionY );
        volumeIn.getHeader().getAttribute( "resolutionZ",
                                           resolutionZ );
        radius = 2.0 * std::min( resolutionX,
                                 std::min( resolutionY, resolutionZ ) );

      }
      else
      {

        radius = 1.0;

      }

    }

    if ( maskProcess.getVerbose() )
    {

      std::cout << "radius of closing : " << radius << " mm" << std::endl;
      std::cout << "threshold ratio   : " << maskProcess.getThresholdRatio()
                                          << std::endl;

    }


    if ( maskProcess.getVerbose() )
    {

      std::cout << "processing mask : " << std::flush;

    }


    std::string outType = maskProcess.getOutType();
    if ( outType.empty() )
    {

      outType = gkg::TypeOf< int16_t >::getName();

    }

    if ( gkg::TypeOf< uint8_t >::isSameType( outType ) )
    {

      gkg::Volume< uint8_t > volumeOut;

      gkg::ClosedMask< T, uint8_t >
        closedMask( radius,
                    maskProcess.getThresholdRatio(),
                    ( uint8_t )maskProcess.getForeground(),
                    ( uint8_t )maskProcess.getBackground() );

      closedMask.getMask( volumeIn, volumeOut, maskProcess.getVerbose() );

      if ( maskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( maskProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << maskProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( maskProcess.getFileNameOut(),
                                        volumeOut,
                                        maskProcess.getAscii(),
                                        maskProcess.getFormat() );
      if ( maskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int16_t >::isSameType( outType ) )
    {

      gkg::Volume< int16_t > volumeOut;

      gkg::ClosedMask< T, int16_t >
        closedMask( radius,
                    maskProcess.getThresholdRatio(),
                    ( int16_t )maskProcess.getForeground(),
                    ( int16_t )maskProcess.getBackground() );

      closedMask.getMask( volumeIn, volumeOut, maskProcess.getVerbose() );

      if ( maskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( maskProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << maskProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( maskProcess.getFileNameOut(),
                                        volumeOut,
                                        maskProcess.getAscii(),
                                        maskProcess.getFormat() );
      if ( maskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< float >::isSameType( outType ) )
    {

      gkg::Volume< float > volumeOut;

      gkg::ClosedMask< T, float >
        closedMask( radius,
                    maskProcess.getThresholdRatio(),
                    ( float )maskProcess.getForeground(),
                    ( float )maskProcess.getBackground() );

      closedMask.getMask( volumeIn, volumeOut, maskProcess.getVerbose() );

      if ( maskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( maskProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << maskProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( maskProcess.getFileNameOut(),
                                        volumeOut,
                                        maskProcess.getAscii(),
                                        maskProcess.getFormat() );
      if ( maskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< double >::isSameType( outType ) )
    {

      gkg::Volume< double > volumeOut;

      gkg::ClosedMask< T, double >
        closedMask( radius,
                    maskProcess.getThresholdRatio(),
                    ( double )maskProcess.getForeground(),
                    ( double )maskProcess.getBackground() );

      closedMask.getMask( volumeIn, volumeOut, maskProcess.getVerbose() );

      if ( maskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( maskProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << maskProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( maskProcess.getFileNameOut(),
                                        volumeOut,
                                        maskProcess.getAscii(),
                                        maskProcess.getFormat() );
      if ( maskProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void ClosedMaskProcess::operate( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   GetMaskCommand
//


gkg::GetMaskCommand::GetMaskCommand( int32_t argc,
                                     char* argv[],
                                     bool loadPlugin,
                                     bool removeFirst )
                   : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::GetMaskCommand::GetMaskCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::GetMaskCommand::GetMaskCommand( const std::string& fileNameIn,
                                     const std::string& fileNameOut,
                    		     int32_t algorithmType,
                    		     const std::string& outType,
    		    		     float radius,
    		    		     double noisePosition,
    		    		     double noiseRatio,
    		    		     int32_t thrown,
    		    		     int32_t kept,
    		    		     double thresholdRatio,
    		    		     double foreground,
    		    		     double background,
    		    		     bool ascii,
    		    		     const std::string& format,
    		    		     bool verbose )
                   : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, algorithmType, outType, radius,
    	     noisePosition, noiseRatio, thrown, kept, thresholdRatio,
    	     foreground, background, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::GetMaskCommand::GetMaskCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "int32_t algorithmType, const std::string& outType, "
    	     "float radius, double noisePosition, double noiseRatio, "
    	     "int32_t thrown, int32_t kept, double thresholdRatio, "
    	     "double foreground, double background, bool ascii, "
    	     "const std::string& format, bool verbose )" );

}


gkg::GetMaskCommand::GetMaskCommand( const gkg::Dictionary& parameters )
                   : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, algorithmType );
    DECLARE_STRING_PARAMETER( parameters, std::string, outType );
    DECLARE_FLOATING_PARAMETER( parameters, float, radius );
    DECLARE_FLOATING_PARAMETER( parameters, double, noisePosition );
    DECLARE_FLOATING_PARAMETER( parameters, double, noiseRatio );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, thrown );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, kept );
    DECLARE_FLOATING_PARAMETER( parameters, double, thresholdRatio );
    DECLARE_FLOATING_PARAMETER( parameters, double, foreground );
    DECLARE_FLOATING_PARAMETER( parameters, double, background );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, algorithmType, outType, radius,
    	     noisePosition, noiseRatio, thrown, kept, thresholdRatio,
    	     foreground, background, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::GetMaskCommand::GetMaskCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::GetMaskCommand::~GetMaskCommand()
{
}


std::string gkg::GetMaskCommand::getStaticName()
{

  try
  {

    return "GetMask";

  }
  GKG_CATCH( "std::string gkg::GetMaskCommand::getStaticName()" );

}


void gkg::GetMaskCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    int32_t algorithmType = 1;
    std::string outType = "";
    float radius = -1.0;
    double noisePosition = 0.9;
    double noiseRatio = 0.01;
    int32_t thrown = 20;
    int32_t kept = 100;
    double thresholdRatio = 0.04;
    double foreground = 1.0;
    double background = 0.0;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Automatic mask processing",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-a",
                                 "Algorithm type\n"
                                 "1-> morpho filled mask (default)\n"
                                 "2-> closed mask",
                                 algorithmType,
                                 true );
    application.addSingleOption( "-t",
                                 "Output type unsigned char/short/float/double "
                                 "(default=short)",
                                 outType,
                                 true );
    application.addSingleOption( "-r",
                                 "Radius of morphological operation "
                                 "(default=2*min. res.)",
                                 radius,
                                 true );
    application.addSingleOption( "-noisePosition",
                                 "Noise position along x axis (default=0.9)",
                                 noisePosition,
                                 true );
    application.addSingleOption( "-noiseRatio",
                                 "Noise ratio (default=0.01)",
                                 noiseRatio,
                                 true );
    application.addSingleOption( "-thrown",
                                 "Maximum thrown count (default=20)",
                                 thrown,
                                 true );
    application.addSingleOption( "-kept",
                                 "Maximum kept (default=100)",
                                 kept,
                                 true );
    application.addSingleOption( "-thresholdRatio",
                                 "Threshold ratio (default=0.04)",
                                 thresholdRatio,
                                 true );
    application.addSingleOption( "-f",
                                 "Foreground value (default=1)",
                                 foreground,
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

    execute( fileNameIn, fileNameOut, algorithmType, outType, radius,
    	     noisePosition, noiseRatio, thrown, kept, thresholdRatio,
    	     foreground, background, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::GetMaskCommand::parse()" );

}


void gkg::GetMaskCommand::execute( const std::string& fileNameIn,
                    		   const std::string& fileNameOut,
                    		   int32_t algorithmType,
                    		   const std::string& outType,
    		    		   float radius,
    		    		   double noisePosition,
    		    		   double noiseRatio,
    		    		   int32_t thrown,
    		    		   int32_t kept,
    		    		   double thresholdRatio,
    		    		   double foreground,
    		    		   double background,
    		    		   bool ascii,
    		    		   const std::string& format,
    		    		   bool verbose )
{

  try
  {

    if ( !outType.empty() &&
         ( ! gkg::TypeOf< uint8_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< int16_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< float >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< double >::isSameType( outType ) ) )
    {

      throw std::runtime_error( "invalid output type" );

    }

    if ( ( algorithmType != 1 ) && ( algorithmType != 2 ) )
    {

      throw std::runtime_error( "invalid algorithm type" );

    }

    //
    // launching process
    //
    if ( algorithmType == 1 )
    {

      MorphoFilledMaskProcess maskProcess( fileNameOut,
                                           outType,
                                           radius,
                                           noisePosition,
                                           noiseRatio,
                                           thrown,
                                           kept,
                                           foreground,
                                           background,
                                           ascii,
                                           format,
                                           verbose );
      maskProcess.execute( fileNameIn );

    }
    else if ( algorithmType == 2 )
    {

      ClosedMaskProcess maskProcess( fileNameOut,
                                     outType,
                                     radius,
                                     thresholdRatio,
                                     foreground,
                                     background,
                                     ascii,
                                     format,
                                     verbose );
      maskProcess.execute( fileNameIn );

    }

  }
  GKG_CATCH( "void gkg::GetMaskCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "int32_t algorithmType, const std::string& outType, "
    	     "float radius, double noisePosition, double noiseRatio, "
    	     "int32_t thrown, int32_t kept, double thresholdRatio, "
    	     "double foreground, double background, bool ascii, "
    	     "const std::string& format, bool verbose )" );

}


RegisterCommandCreator( GetMaskCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_INTEGER_PARAMETER_HELP( algorithmType ) +
    			DECLARE_STRING_PARAMETER_HELP( outType ) +
    			DECLARE_FLOATING_PARAMETER_HELP( radius ) +
    			DECLARE_FLOATING_PARAMETER_HELP( noisePosition ) +
    			DECLARE_FLOATING_PARAMETER_HELP( noiseRatio ) +
    			DECLARE_INTEGER_PARAMETER_HELP( thrown ) +
    			DECLARE_INTEGER_PARAMETER_HELP( kept ) +
    			DECLARE_FLOATING_PARAMETER_HELP( thresholdRatio ) +
    			DECLARE_FLOATING_PARAMETER_HELP( foreground ) +
    			DECLARE_FLOATING_PARAMETER_HELP( background ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
