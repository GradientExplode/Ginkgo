#include <gkg-processing-plugin-functors/Convolution3d/Convolution3dCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/Convolution3d_i.h>
#include <gkg-processing-algobase/SeparableConvolution3d_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


namespace std
{

std::ostream& operator << ( ostream& os, const std::vector< float >& thing )
{

  for ( int32_t s = 0; s < ( int32_t )thing.size() - 1; s++ )
  {

    os << thing[ s ] << " ";

  }
  os << thing[ ( int32_t )thing.size() - 1 ];
  return os;

}

}


class Convolution3dProcess : public gkg::Process
{

  public:

    Convolution3dProcess( const std::string& fileNameOut,
                          const std::string& outType,
                          const std::vector< float >& kernelX,
                          const std::vector< float >& kernelY,
                          const std::vector< float >& kernelZ,
                          const std::vector< float >& kernel3d,
                          const std::vector< int32_t >& kernel3dSize,
                          const bool& ascii,
                          const std::string& format,
                          const bool& verbose );

    const std::string& getFileNameOut() const;
    const std::string& getOutType() const;
    const std::vector< float >& getKernelX() const;
    const std::vector< float >& getKernelY() const;
    const std::vector< float >& getKernelZ() const;
    const std::vector< float >& getKernel3d() const;
    const std::vector< int32_t >& getKernel3dSize() const;
    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void convolve( gkg::Process& process,
                          const std::string& fileNameIn,
                          const gkg::AnalyzedObject&,
                          const std::string& );

    const std::string& _fileNameOut;
    const std::string& _outType;
    const std::vector< float >& _kernelX;
    const std::vector< float >& _kernelY;
    const std::vector< float >& _kernelZ;
    const std::vector< float >& _kernel3d;
    const std::vector< int32_t >& _kernel3dSize;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


Convolution3dProcess::Convolution3dProcess( 
                                     const std::string& fileNameOut,
                                     const std::string& outType,
                                     const std::vector< float >& kernelX,
                                     const std::vector< float >& kernelY,
                                     const std::vector< float >& kernelZ,
                                     const std::vector< float >& kernel3d,
                                     const std::vector< int32_t >& kernel3dSize,
                                     const bool& ascii,
                                     const std::string& format,
                                     const bool& verbose)
                     : gkg::Process(),
                       _fileNameOut( fileNameOut ),
                       _outType( outType ),
                       _kernelX( kernelX ),
                       _kernelY( kernelY ),
                       _kernelZ( kernelZ ),
                       _kernel3d( kernel3d ),
                       _kernel3dSize( kernel3dSize ),
                       _ascii( ascii ),
                       _format( format ),
                       _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &Convolution3dProcess::convolve< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &Convolution3dProcess::convolve< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &Convolution3dProcess::convolve< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &Convolution3dProcess::convolve< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &Convolution3dProcess::convolve< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &Convolution3dProcess::convolve< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &Convolution3dProcess::convolve< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &Convolution3dProcess::convolve< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &Convolution3dProcess::convolve< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &Convolution3dProcess::convolve< double > );

}


const std::string& Convolution3dProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& Convolution3dProcess::getOutType() const
{

  return _outType;

}


const std::vector< float >& Convolution3dProcess::getKernelX() const
{

  return _kernelX;

}


const std::vector< float >& Convolution3dProcess::getKernelY() const
{

  return _kernelY;

}


const std::vector< float >& Convolution3dProcess::getKernelZ() const
{

  return _kernelZ;

}


const std::vector< float >& Convolution3dProcess::getKernel3d() const
{

  return _kernel3d;

}


const std::vector< int32_t >& Convolution3dProcess::getKernel3dSize() const
{

  return _kernel3dSize;

}


const bool& Convolution3dProcess::getAscii() const
{

  return _ascii;

}


const std::string& Convolution3dProcess::getFormat() const
{

  return _format;

}


const bool& Convolution3dProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void Convolution3dProcess::convolve( gkg::Process& process,
                                     const std::string& fileNameIn,
                                     const gkg::AnalyzedObject&,
                                     const std::string& )
{

  try
  {

    Convolution3dProcess&
      convolution3dProcess = static_cast< Convolution3dProcess& >( process );

    // reading data
    if ( convolution3dProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    if ( convolution3dProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }


    bool separable = false;
    gkg::Volume< float > kernel;
    if ( !convolution3dProcess.getKernelX().empty() ||
         !convolution3dProcess.getKernelY().empty() ||
         !convolution3dProcess.getKernelZ().empty() )
    {

      separable = true;
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "X kernel : " << convolution3dProcess.getKernelX()
                  << std::endl;
        std::cout << "Y kernel : " << convolution3dProcess.getKernelY()
                  << std::endl;
        std::cout << "Z kernel : " << convolution3dProcess.getKernelZ()
                  << std::endl;

      }

    }
    else
    {

      int32_t kernelSizeX = convolution3dProcess.getKernel3dSize()[ 0 ];
      int32_t kernelSizeY = convolution3dProcess.getKernel3dSize()[ 1 ];
      int32_t kernelSizeZ = convolution3dProcess.getKernel3dSize()[ 2 ];

      kernel = gkg::Volume< float >( kernelSizeX,
                                     kernelSizeY,
                                     kernelSizeZ );
      int32_t x, y, z, count = 0;
      for ( z = 0; z < kernelSizeZ; z++ )
      {

        for ( y = 0; y < kernelSizeY; y++ )
        {

          for ( x = 0; x < kernelSizeX; x++ )
          {

            kernel( x, y, z ) =  convolution3dProcess.getKernel3d()[ count ];
            ++ count;

          }

        }

      }

    }

    if ( convolution3dProcess.getVerbose() )
    {

      std::cout << "convolving : " << std::flush;

    }

    if ( convolution3dProcess.getOutType().empty() )
    {

      gkg::Volume< T > volumeOut;
      if ( separable )
      {

        gkg::SeparableConvolution3d< T, float, T > convolution;
        convolution.convolve( volumeIn,
                              convolution3dProcess.getKernelX(),
                              convolution3dProcess.getKernelY(),
                              convolution3dProcess.getKernelZ(),
                              volumeOut );

      }
      else
      {

        gkg::Convolution3d< T, float, T > convolution;
        convolution.convolve( volumeIn, kernel, volumeOut );

      }
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << convolution3dProcess.getFileNameOut() << "' : "
                  << std::flush;

      }
      gkg::Writer::getInstance().write( convolution3dProcess.getFileNameOut(),
                                        volumeOut,
                                        convolution3dProcess.getAscii(),
                                        convolution3dProcess.getFormat() );
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    if ( gkg::TypeOf< int8_t >::isSameType(
                                           convolution3dProcess.getOutType() ) )
    {

      gkg::Volume< int8_t > volumeOut;
      if ( separable )
      {

        gkg::SeparableConvolution3d< T, float, int8_t > convolution;
        convolution.convolve( volumeIn,
                              convolution3dProcess.getKernelX(),
                              convolution3dProcess.getKernelY(),
                              convolution3dProcess.getKernelZ(),
                              volumeOut );

      }
      else
      {

        gkg::Convolution3d< T, float, int8_t > convolution;
        convolution.convolve( volumeIn, kernel, volumeOut );

      }
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << convolution3dProcess.getFileNameOut() << "' : "
                  << std::flush;

      }
      gkg::Writer::getInstance().write( convolution3dProcess.getFileNameOut(),
                                        volumeOut,
                                        convolution3dProcess.getAscii(),
                                        convolution3dProcess.getFormat() );
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint8_t >::isSameType( 
                                           convolution3dProcess.getOutType() ) )
    {

      gkg::Volume< uint8_t > volumeOut;
      if ( separable )
      {

        gkg::SeparableConvolution3d< T, float, uint8_t > convolution;
        convolution.convolve( volumeIn,
                              convolution3dProcess.getKernelX(),
                              convolution3dProcess.getKernelY(),
                              convolution3dProcess.getKernelZ(),
                              volumeOut );

      }
      else
      {

        gkg::Convolution3d< T, float, uint8_t > convolution;
        convolution.convolve( volumeIn, kernel, volumeOut );

      }
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << convolution3dProcess.getFileNameOut() << "' : "
                  << std::flush;

      }
      gkg::Writer::getInstance().write( convolution3dProcess.getFileNameOut(),
                                        volumeOut,
                                        convolution3dProcess.getAscii(),
                                        convolution3dProcess.getFormat() );
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int16_t >::isSameType( 
                                           convolution3dProcess.getOutType() ) )
    {

      gkg::Volume< int16_t > volumeOut;
      if ( separable )
      {

        gkg::SeparableConvolution3d< T, float, int16_t > convolution;
        convolution.convolve( volumeIn,
                              convolution3dProcess.getKernelX(),
                              convolution3dProcess.getKernelY(),
                              convolution3dProcess.getKernelZ(),
                              volumeOut );

      }
      else
      {

        gkg::Convolution3d< T, float, int16_t > convolution;
        convolution.convolve( volumeIn, kernel, volumeOut );

      }
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << convolution3dProcess.getFileNameOut() << "' : "
                  << std::flush;

      }
      gkg::Writer::getInstance().write( convolution3dProcess.getFileNameOut(),
                                        volumeOut,
                                        convolution3dProcess.getAscii(),
                                        convolution3dProcess.getFormat() );
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint16_t >::isSameType( 
                                           convolution3dProcess.getOutType() ) )
    {

      gkg::Volume< uint16_t > volumeOut;
      if ( separable )
      {

        gkg::SeparableConvolution3d< T, float, uint16_t > convolution;
        convolution.convolve( volumeIn,
                              convolution3dProcess.getKernelX(),
                              convolution3dProcess.getKernelY(),
                              convolution3dProcess.getKernelZ(),
                              volumeOut );

      }
      else
      {

        gkg::Convolution3d< T, float, uint16_t > convolution;
        convolution.convolve( volumeIn, kernel, volumeOut );

      }
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << convolution3dProcess.getFileNameOut() << "' : "
                  << std::flush;

      }
      gkg::Writer::getInstance().write( convolution3dProcess.getFileNameOut(),
                                        volumeOut,
                                        convolution3dProcess.getAscii(),
                                        convolution3dProcess.getFormat() );
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int32_t >::isSameType(
                                           convolution3dProcess.getOutType() ) )
    {

      gkg::Volume< int32_t > volumeOut;
      if ( separable )
      {

        gkg::SeparableConvolution3d< T, float, int32_t > convolution;
        convolution.convolve( volumeIn,
                              convolution3dProcess.getKernelX(),
                              convolution3dProcess.getKernelY(),
                              convolution3dProcess.getKernelZ(),
                              volumeOut );

      }
      else
      {

        gkg::Convolution3d< T, float, int32_t > convolution;
        convolution.convolve( volumeIn, kernel, volumeOut );

      }
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << convolution3dProcess.getFileNameOut() << "' : "
                  << std::flush;

      }
      gkg::Writer::getInstance().write( convolution3dProcess.getFileNameOut(),
                                        volumeOut,
                                        convolution3dProcess.getAscii(),
                                        convolution3dProcess.getFormat() );
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint32_t >::isSameType(
                                           convolution3dProcess.getOutType() ) )
    {

      gkg::Volume< uint32_t > volumeOut;
      if ( separable )
      {

        gkg::SeparableConvolution3d< T, float, uint32_t > convolution;
        convolution.convolve( volumeIn,
                              convolution3dProcess.getKernelX(),
                              convolution3dProcess.getKernelY(),
                              convolution3dProcess.getKernelZ(),
                              volumeOut );

      }
      else
      {

        gkg::Convolution3d< T, float, uint32_t > convolution;
        convolution.convolve( volumeIn, kernel, volumeOut );

      }
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << convolution3dProcess.getFileNameOut() << "' : " 
                  << std::flush;

      }
      gkg::Writer::getInstance().write( convolution3dProcess.getFileNameOut(),
                                        volumeOut,
                                        convolution3dProcess.getAscii(),
                                        convolution3dProcess.getFormat() );
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int64_t >::isSameType( 
                                           convolution3dProcess.getOutType() ) )
    {

      gkg::Volume< int64_t > volumeOut;
      if ( separable )
      {

        gkg::SeparableConvolution3d< T, float, int64_t > convolution;
        convolution.convolve( volumeIn,
                              convolution3dProcess.getKernelX(),
                              convolution3dProcess.getKernelY(),
                              convolution3dProcess.getKernelZ(),
                              volumeOut );

      }
      else
      {

        gkg::Convolution3d< T, float, int64_t > convolution;
        convolution.convolve( volumeIn, kernel, volumeOut );

      }
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << convolution3dProcess.getFileNameOut() << "' : " 
                  << std::flush;

      }
      gkg::Writer::getInstance().write( convolution3dProcess.getFileNameOut(),
                                        volumeOut,
                                        convolution3dProcess.getAscii(),
                                        convolution3dProcess.getFormat() );
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint64_t >::isSameType( 
                                           convolution3dProcess.getOutType() ) )
    {

      gkg::Volume< uint64_t > volumeOut;
      if ( separable )
      {

        gkg::SeparableConvolution3d< T, float, uint64_t > convolution;
        convolution.convolve( volumeIn,
                              convolution3dProcess.getKernelX(),
                              convolution3dProcess.getKernelY(),
                              convolution3dProcess.getKernelZ(),
                              volumeOut );

      }
      else
      {

        gkg::Convolution3d< T, float, uint64_t > convolution;
        convolution.convolve( volumeIn, kernel, volumeOut );

      }
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << convolution3dProcess.getFileNameOut() << "' : "
                  << std::flush;

      }
      gkg::Writer::getInstance().write( convolution3dProcess.getFileNameOut(),
                                        volumeOut,
                                        convolution3dProcess.getAscii(),
                                        convolution3dProcess.getFormat() );
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< float >::isSameType( 
                                           convolution3dProcess.getOutType() ) )
    {

      gkg::Volume< float > volumeOut;
      if ( separable )
      {

        gkg::SeparableConvolution3d< T, float, float > convolution;
        convolution.convolve( volumeIn,
                              convolution3dProcess.getKernelX(),
                              convolution3dProcess.getKernelY(),
                              convolution3dProcess.getKernelZ(),
                              volumeOut );

      }
      else
      {

        gkg::Convolution3d< T, float, float > convolution;
        convolution.convolve( volumeIn, kernel, volumeOut );

      }
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << convolution3dProcess.getFileNameOut() << "' : "
                  << std::flush;

      }
      gkg::Writer::getInstance().write( convolution3dProcess.getFileNameOut(),
                                        volumeOut,
                                        convolution3dProcess.getAscii(),
                                        convolution3dProcess.getFormat() );
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< double >::isSameType(
                                           convolution3dProcess.getOutType() ) )
    {

      gkg::Volume< double > volumeOut;
      if ( separable )
      {

        gkg::SeparableConvolution3d< T, float, double > convolution;
        convolution.convolve( volumeIn,
                              convolution3dProcess.getKernelX(),
                              convolution3dProcess.getKernelY(),
                              convolution3dProcess.getKernelZ(),
                              volumeOut );

      }
      else
      {

        gkg::Convolution3d< T, float, double > convolution;
        convolution.convolve( volumeIn, kernel, volumeOut );

      }
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << convolution3dProcess.getFileNameOut() << "' : "
                  << std::flush;

      }
      gkg::Writer::getInstance().write( convolution3dProcess.getFileNameOut(),
                                        volumeOut,
                                        convolution3dProcess.getAscii(),
                                        convolution3dProcess.getFormat() );
      if ( convolution3dProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void Convolution3dProcess::binarize( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class Convolution3dCommand
//


gkg::Convolution3dCommand::Convolution3dCommand( int32_t argc,
                                 		 char* argv[],
                                 		 bool loadPlugin,
                                 		 bool removeFirst )
                         : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::Convolution3dCommand::Convolution3dCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::Convolution3dCommand::Convolution3dCommand(
                          	     const std::string& fileNameIn,
                          	     const std::string& fileNameOut,
                          	     const std::string& outType,
                          	     std::vector< float >& kernelX,
                          	     std::vector< float >& kernelY,
                          	     std::vector< float >& kernelZ,
                          	     const std::vector< float >& kernel3d,
                          	     const std::vector< int32_t >& kernel3dSize,
                          	     bool ascii,
                          	     const std::string& format,
                          	     bool verbose )
{

  try
  {

    execute( fileNameIn, fileNameOut, outType, kernelX, kernelY, kernelZ,
             kernel3d, kernel3dSize, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::Convolution3dCommand::Convolution3dCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& outType, "
             "std::vector< float >& kernelX, "
             "std::vector< float >& kernelY, "
             "std::vector< float >& kernelZ, "
             "const std::vector< float >& kernel3d, "
             "const std::vector< int32_t >& kernel3dSize, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::Convolution3dCommand::Convolution3dCommand(
                                             const gkg::Dictionary& parameters )
                         : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, outType );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< float >,
                                           kernelX );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< float >, 
                                           kernelY );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, 
                                           std::vector< float >, 
                                           kernelZ );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, 
                                           std::vector< float >, 
                                           kernel3d );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters, 
                                          std::vector< int32_t >, 
                                          kernel3dSize );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, outType, kernelX, kernelY, kernelZ,
             kernel3d, kernel3dSize, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::Convolution3dCommand::Convolution3dCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::Convolution3dCommand::~Convolution3dCommand()
{
}


std::string gkg::Convolution3dCommand::getStaticName()
{

  try
  {

    return "Convolution3d";

  }
  GKG_CATCH( "std::string gkg::Convolution3dCommand::getStaticName()" );

}


void gkg::Convolution3dCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string outType = "";
    std::vector< float > kernelX;
    std::vector< float > kernelY;
    std::vector< float > kernelZ;
    std::vector< float > kernel3d;
    std::vector< int32_t > kernel3dSize;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "3D Convolution of a volume with a kernel\n"
                                  "(either use (kx, ky, kz) options or "
                                  "(k, s) options",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-t",
                                 "Output type (default=input type)",
                                 outType,
                                 true );
    application.addSeriesOption( "-kx",
                                 "X separable convolution kernel, "
                                 "i.e. list of floating values",
                                 kernelX );
    application.addSeriesOption( "-ky",
                                 "Y separable convolution kernel, "
                                 "i.e. list of floating values",
                                 kernelY );
    application.addSeriesOption( "-kz",
                                 "Z separable convolution kernel, "
                                 "i.e. list of floating values",
                                 kernelZ );
    application.addSeriesOption( "-k",
                                 "3D convolution kernel; size is provided by -s"
                                 " option",
                                 kernel3d );
    application.addSeriesOption( "-s",
                                 "Size of the 3D convolution kernel",
                                 kernel3dSize );
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

    execute( fileNameIn, fileNameOut, outType, kernelX, kernelY, kernelZ,
             kernel3d, kernel3dSize, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::Convolution3dCommand::parse()" );

}


void gkg::Convolution3dCommand::execute(
                  	             const std::string& fileNameIn,
                          	     const std::string& fileNameOut,
                          	     const std::string& outType,
                          	     std::vector< float >& kernelX,
                          	     std::vector< float >& kernelY,
                          	     std::vector< float >& kernelZ,
                          	     const std::vector< float >& kernel3d,
                          	     const std::vector< int32_t >& kernel3dSize,
                          	     bool ascii,
                          	     const std::string& format,
                          	     bool verbose )
{

  try
  {

    // sanity checks
    if ( kernel3d.empty() && kernel3dSize.empty() )
    {

      if ( kernelX.empty() &&
           kernelY.empty() &&
           kernelZ.empty() )
      {

        throw std::runtime_error(
                                "at least one X/Y/Z kernel must be specified" );

      }
      if ( kernelX.empty() )
      {

        kernelX = std::vector< float >( 1 );
        kernelX[ 0 ] = 1.0;

      }
      if ( kernelY.empty() )
      {

        kernelY = std::vector< float >( 1 );
        kernelY[ 0 ] = 1.0;

      }
      if ( kernelZ.empty() )
      {

        kernelZ = std::vector< float >( 1 );
        kernelZ[ 0 ] = 1.0;

      }

    }
    else
    {

      if ( kernel3d.empty() )
      {

        throw std::runtime_error( "unspecified 3D kernel value(s)" );

      }
      if ( kernel3dSize.empty() )
      {

        throw std::runtime_error( "unspecified 3D kernel size(s)" );

      }

      if ( kernel3dSize.size() != 3 )
      {

        throw std::runtime_error( "3D kernel size(s) should be equal to 3" );

      }
      if ( ( int32_t )kernel3d.size() != ( kernel3dSize[ 0 ] *
                                           kernel3dSize[ 1 ] *
                                           kernel3dSize[ 2 ] ) )
      {

        throw std::runtime_error(
                           "inconsistent 3D kernel size(s) and element count" );

      }

    }
         
    if ( !outType.empty() &&
         ( ! gkg::TypeOf< int8_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< uint8_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< int16_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< uint16_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< int32_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< uint32_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< int64_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< uint64_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< float >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< double >::isSameType( outType ) ) )
    {

      throw std::runtime_error( "invalid output type" );

    }

    //
    // launching convolution process
    //
    Convolution3dProcess convolution3dProcess( fileNameOut,
                                               outType,
                                               kernelX,
                                               kernelY,
                                               kernelZ,
                                               kernel3d,
                                               kernel3dSize,
                                               ascii,
                                               format,
                                               verbose );

    convolution3dProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::Convolution3dCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& outType, "
             "std::vector< float >& kernelX, "
             "std::vector< float >& kernelY, "
             "std::vector< float >& kernelZ, "
             "const std::vector< float >& kernel3d, "
             "const std::vector< int32_t >& kernel3dSize, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator(
                     Convolution3dCommand,
                     DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                     DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                     DECLARE_STRING_PARAMETER_HELP( outType ) +
                     DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( kernelX ) +
                     DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( kernelY ) +
                     DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( kernelZ ) +
                     DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( kernel3d ) +
                     DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( kernel3dSize ) +
                     DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
                     DECLARE_STRING_PARAMETER_HELP( format );
                     DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
